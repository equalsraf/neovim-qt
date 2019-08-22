#include "shell.h"

#include <QPainter>
#include <QPaintEvent>
#include <QDebug>
#include <QDesktopWidget>
#include <QApplication>
#include <QKeyEvent>
#include <QMimeData>
#include <QClipboard>
#include "msgpackrequest.h"
#include "input.h"
#include "konsole_wcwidth.h"
#include "util.h"
#include "version.h"

namespace NeovimQt {

Shell::Shell(NeovimConnector *nvim, ShellOptions opts, QWidget *parent)
:ShellWidget(parent), m_attached(false), m_nvim(nvim),
	m_font_bold(false), m_font_italic(false), m_font_underline(false), m_font_undercurl(false),
	m_mouseHide(true),
	m_hg_foreground(Qt::black), m_hg_background(Qt::white), m_hg_special(QColor()),
	m_cursor_color(Qt::white), m_cursor_pos(0,0), m_insertMode(false),
	m_resizing(false),
	m_mouse_wheel_delta_fraction(0, 0),
	m_neovimBusy(false),
	m_options(opts),
	m_mouseEnabled(true)
{
	setAttribute(Qt::WA_KeyCompression, false);

	setAcceptDrops(true);
	setMouseTracking(true);
	m_mouseclick_timer.setInterval(QApplication::doubleClickInterval());
	m_mouseclick_timer.setSingleShot(true);
	connect(&m_mouseclick_timer, &QTimer::timeout,
			this, &Shell::mouseClickReset);

	// IM Tooltip
	setAttribute(Qt::WA_InputMethodEnabled, true);
	m_tooltip = new QLabel(this);
	m_tooltip->setVisible(false);
	m_tooltip->setTextFormat(Qt::PlainText);
	m_tooltip->setTextInteractionFlags(Qt::NoTextInteraction);
	m_tooltip->setAutoFillBackground(true);

	// PUM
	m_pum.setParent(this);
	m_pum.hide();

	if (m_nvim == NULL) {
		qWarning() << "Received NULL as Neovim Connector";
		return;
	}

	connect(m_nvim, &NeovimConnector::ready,
			this, &Shell::init);
	connect(m_nvim, &NeovimConnector::error,
			this, &Shell::neovimError);
	connect(m_nvim, &NeovimConnector::processExited,
			this, &Shell::neovimExited);
	connect(this, &ShellWidget::shellFontChanged,
			this, &Shell::fontChanged);
	connect(this, &ShellWidget::fontError,
			this, &Shell::fontError);

	m_nvim->setRequestHandler(new ShellRequestHandler(this));

	if (m_nvim->isReady()) {
		init();
	}
}

void Shell::fontError(const QString& msg)
{
	if (m_attached) {
		m_nvim->api0()->vim_report_error(m_nvim->encode(msg));
	}
}

QString Shell::fontDesc()
{
	QString fdesc = QString("%1:h%2").arg(fontFamily()).arg(fontSize());
	if (font().bold()) {
		fdesc += ":b";
	} else if (font().weight() == QFont::Light) {
		fdesc += ":l";
	}
	if (font().italic()) {
		fdesc += ":i";
	}
	return fdesc;
}

/**
 * Set the GUI font, or display the current font
 */
bool Shell::setGuiFont(const QString& fdesc, bool force)
{
	QStringList attrs = fdesc.split(':');
	if (attrs.size() < 1) {
		m_nvim->api0()->vim_report_error("Invalid font");
		return false;
	}

	qreal pointSize = font().pointSizeF();
	int weight = -1;
	bool italic = false;
	foreach(QString attr, attrs) {
		if (attr.size() >= 2 && attr[0] == 'h') {
			bool ok = false;
			qreal height = attr.mid(1).toFloat(&ok);
			if (!ok) {
				m_nvim->api0()->vim_report_error("Invalid font height");
				return false;
			}
			pointSize = height;
		} else if (attr == "b") {
			weight = QFont::Bold;
		} else if (attr == "l") {
			weight = QFont::Light;
		} else if (attr == "i") {
			italic = true;
		}
	}
	bool ok = setShellFont(attrs.at(0), pointSize, weight, italic, force);
	if (ok && m_attached) {
		resizeNeovim(size());
		m_nvim->api0()->vim_set_var("GuiFont", fontDesc());
	}

	return ok;
}

Shell::~Shell()
{
	if (m_nvim && m_attached) {
		m_nvim->api0()->ui_detach();
	}
}

void Shell::setAttached(bool attached)
{
	m_attached = attached;
	if (attached) {
		updateWindowId();
		m_nvim->api0()->vim_set_var("GuiFont", fontDesc());
		if (isWindow()) {
			updateGuiWindowState(windowState());
		}

		updateClientInfo();

		auto req_shim = m_nvim->api0()->vim_command("runtime plugin/nvim_gui_shim.vim");
		connect(req_shim, &MsgpackRequest::error, this, &Shell::handleShimError);
		auto gviminit = qgetenv("GVIMINIT");
		if (gviminit.isEmpty()) {
			auto req_ginit = m_nvim->api0()->vim_command("runtime! ginit.vim");
			connect(req_ginit, &MsgpackRequest::error, this, &Shell::handleGinitError);
		} else {
			m_nvim->api0()->vim_command(gviminit);
		}

		// Noevim was not able to open urls till now. Check if we have any to open.
		if(!m_deferredOpen.isEmpty()){
			openFiles(m_deferredOpen);
			m_deferredOpen.clear();    //Neovim may change state. Clear to prevent reopening.
		}

	}
	emit neovimAttached(attached);
	update();
}

/// The top left corner position (pixel) for the cursor
QPoint Shell::neovimCursorTopLeft() const
{
	return QPoint(m_cursor_pos.x()*cellSize().width(),
			m_cursor_pos.y()*cellSize().height());
}

/// Get the area filled by the cursor
QRect Shell::neovimCursorRect() const
{
	return neovimCursorRect(m_cursor_pos);
}

/// Get the area filled by the cursor at an arbitrary
/// position
QRect Shell::neovimCursorRect(QPoint at) const
{
	const Cell& c = contents().constValue(at.y(), at.x());
	bool wide = c.IsDoubleWidth();
	QRect r(neovimCursorTopLeft(), cellSize());
	if (wide) {
		r.setWidth(r.width()*2);
	}
	return r;
}

void Shell::init()
{
	// Make sure the connector provides us with an api object
	if (!m_nvim || !m_nvim->api0()) {
		emit neovimIsUnsupported();
		return;
	}

	connect(m_nvim->api0(), &NeovimApi0::neovimNotification,
			this, &Shell::handleNeovimNotification);
	connect(m_nvim->api0(), &NeovimApi0::on_ui_try_resize,
			this, &Shell::neovimResizeFinished);

	QRect screenRect = QApplication::desktop()->availableGeometry(this);
	int64_t width = screenRect.width()*0.66/cellSize().width();
	int64_t height = screenRect.height()*0.66/cellSize().height();
	QVariantMap options;
	if (m_options.enable_ext_tabline) {
		options.insert("ext_tabline", true);
	}
	if (m_options.enable_ext_popupmenu) {
		options.insert("ext_popupmenu", true);
	}
	if (m_options.enable_ext_linegrid
		&& m_nvim->hasUIOption("ext_linegrid")) {
		// Modern Grid UI API is optionally enabled via cmdline
		options.insert("ext_linegrid", true);
	}
	options.insert("rgb", true);

	MsgpackRequest *req;
	if (m_nvim->api2()) {
		req = m_nvim->api2()->nvim_ui_attach(width, height, options);
	} else {
		req = m_nvim->api0()->ui_attach(width, height, true);
	}
	connect(req, &MsgpackRequest::timeout,
			m_nvim, &NeovimConnector::fatalTimeout);
	// FIXME grab timeout from connector
	req->setTimeout(10000);

	connect(req, &MsgpackRequest::finished,
			this, &Shell::setAttached);

	// Subscribe to GUI events
	m_nvim->api0()->vim_subscribe("Gui");
}

void Shell::neovimError(NeovimConnector::NeovimError err)
{
	setAttached(false);
}

/** The Neovim process has exited */
void Shell::neovimExited(int status)
{
	setAttached(false);
	if (status == 0 && m_nvim->errorCause() == NeovimConnector::NoError) {
		close();
	}
}

/// Neovim requested a resize
///
/// - update cols/rows
/// - reset the cursor, scroll_region
void Shell::handleResize(uint64_t n_cols, uint64_t n_rows)
{
	m_cursor_pos = QPoint(0,0);
	resizeShell(n_rows, n_cols);
	m_scroll_region = QRect(QPoint(0,0), QPoint(n_cols, n_rows));
	if (isWindow()) {
		// Never call resize on a maximized window
		// QTBUG-45806
		resizeNeovim(size());
	}
	emit neovimResized(rows(), columns());
}

void Shell::handleHighlightSet(const QVariantMap& attrs)
{
	if (attrs.contains("foreground")) {
		// TODO: When does Neovim send -1
		m_hg_foreground = color(attrs.value("foreground").toLongLong(), foreground());
	} else {
		m_hg_foreground = foreground();
	}

	if (attrs.contains("background")) {
		m_hg_background = color(attrs.value("background").toLongLong(), background());
	} else {
		m_hg_background = background();
	}

	if (attrs.contains(("special"))) {
		m_hg_special = color(attrs.value("special").toLongLong(), special());
	} else {
		m_hg_special = special();
	}

	if (attrs.contains("reverse")) {
		auto tmp = m_hg_background;
		m_hg_background = m_hg_foreground;
		m_hg_foreground = tmp;
	}

	m_font_bold = attrs.value("bold").toBool();
	m_font_italic = attrs.value("italic").toBool();
	m_font_undercurl = attrs.value("undercurl").toBool();
	m_font_underline = attrs.value("underline").toBool();
}

/// Paint a character and advance the cursor
void Shell::handlePut(const QVariantList& args)
{
	if (args.size() != 1 || (QMetaType::Type)args.at(0).type() != QMetaType::QByteArray) {
		qWarning() << "Unexpected arguments for redraw:put" << args;
		return;
	}

	QString text = m_nvim->decode(args.at(0).toByteArray());
	if (text.isEmpty() && m_cursor_pos.x() > 0 &&
	    contents().constValue(m_cursor_pos.y(), m_cursor_pos.x() - 1).IsDoubleWidth()) {
		// nvim will seek to the second cell of a wide char and put "",
		// expecting the cursor position and cell style to be updated properly.
		// Handle this case.
		text = ' ';
	}

	if (!text.isEmpty()) {
		int cols = put(text, m_cursor_pos.y(), m_cursor_pos.x(),
				m_hg_foreground, m_hg_background, m_hg_special,
				m_font_bold, m_font_italic,
				m_font_underline, m_font_undercurl);
		// Move cursor ahead
		setNeovimCursor(m_cursor_pos.y(), m_cursor_pos.x()+cols);
	}
}

/**
 * Scroll shell contents by *count* lines, a positive count scrolls
 * lines to the top, a negative number scrolls lines to the bottom.
 *
 * - After scrolling the exposed area at the top/bottom is repainted
 *   with the background color.
 * - The scrolled area can be the entire shell, or a region defined
 *   by the set_scroll_region notification
 */
void Shell::handleScroll(const QVariantList& args)
{
	if (!args.at(0).canConvert<qint64>()) {
		qWarning() << "Unexpected arguments for redraw:scroll" << args;
		return;
	}
	qint64 count = args.at(0).toULongLong();

	if (m_scroll_region.contains(m_cursor_pos)) {
		// Schedule cursor region to be repainted
		update(neovimCursorRect());
	}

	scrollShellRegion(m_scroll_region.top(), m_scroll_region.bottom(),
			m_scroll_region.left(), m_scroll_region.right(),
			count);
}

void Shell::handleSetScrollRegion(const QVariantList& opargs)
{
	if (opargs.size() < 4) {
		qWarning() << "Unexpected arguments for redraw:set_scroll_region" << opargs;
		return;
	}

	qint64 top, bot, left, right;
	top = opargs.at(0).toULongLong();
	bot = opargs.at(1).toULongLong();
	left = opargs.at(2).toULongLong();
	right = opargs.at(3).toULongLong();

	m_scroll_region = QRect(QPoint(left, top),
				QPoint(right+1, bot+1));
}

void Shell::handleRedraw(const QByteArray& name, const QVariantList& opargs)
{
	if (name == "update_fg") {
		if (opargs.size() < 1 || !opargs.at(0).canConvert<quint64>()) {
			qWarning() << "Unexpected arguments for redraw:" << name << opargs;
			return;
		}
		qint64 val = opargs.at(0).toLongLong();
		if (val != -1) {
			setForeground(QRgb(val));
		}
		m_hg_foreground = foreground();
	} else if (name == "update_bg") {
		if (opargs.size() < 1 || !opargs.at(0).canConvert<quint64>()) {
			qWarning() << "Unexpected arguments for redraw:" << name << opargs;
			return;
		}
		qint64 val = opargs.at(0).toLongLong();
		if (val != -1) {
			setBackground(QRgb(val));
		}
		m_hg_background = background();
		update();
	} else if (name == "update_sp") {
		if (opargs.size() < 1 || !opargs.at(0).canConvert<quint64>()) {
			qWarning() << "Unexpected arguments for redraw:" << name << opargs;
			return;
		}
		qint64 val = opargs.at(0).toLongLong();
		if (val != -1) {
			setSpecial(QRgb(val));
		}
		m_hg_special = special();
	} else if (name == "resize") {
		if (opargs.size() < 2 || !opargs.at(0).canConvert<quint64>() ||
				!opargs.at(1).canConvert<quint64>()) {
			qWarning() << "Unexpected arguments for redraw:" << name << opargs;
			return;
		}
		handleResize(opargs.at(0).toULongLong(), opargs.at(1).toULongLong());
	} else if (name == "clear") {
		clearShell(m_hg_background);
	} else if (name == "bell"){
		QApplication::beep();
	} else if (name == "eol_clear") {
		clearRegion(m_cursor_pos.y(), m_cursor_pos.x(),
				m_cursor_pos.y()+1, columns());
	} else if (name == "cursor_goto"){
		if (opargs.size() < 2 || !opargs.at(0).canConvert<quint64>() ||
				!opargs.at(1).canConvert<quint64>()) {
			qWarning() << "Unexpected arguments for redraw:" << name << opargs;
			return;
		}
		setNeovimCursor(opargs.at(0).toULongLong(), opargs.at(1).toULongLong());
		// @zhmars: On my system, call update(Qt::ImCursorRectangle) in function
		// setNeovimCursor will cause typing lags
		qApp->inputMethod()->update(Qt::ImCursorRectangle);
	} else if (name == "highlight_set") {
		if (opargs.size() < 1 && (QMetaType::Type)opargs.at(0).type() != QMetaType::QVariantMap) {
			qWarning() << "Unexpected argument for redraw:" << name << opargs;
			return;
		}
		handleHighlightSet(opargs.at(0).toMap());
	} else if (name == "put") {
		handlePut(opargs );
	} else if (name == "scroll"){
		handleScroll(opargs);
	} else if (name == "set_scroll_region"){
		handleSetScrollRegion(opargs);
	} else if (name == "mouse_on"){
		handleMouse(true);
	} else if (name == "mouse_off"){
		handleMouse(false);
	} else if (name == "mode_change"){
		if (opargs.size() < 1 || !opargs.at(0).canConvert<QByteArray>()) {
			qWarning() << "Unexpected argument for change_mode:" << opargs;
			return;
		}
		QString mode = m_nvim->decode(opargs.at(0).toByteArray());
		handleModeChange(mode);
	} else if (name == "cursor_on"){
	} else if (name == "set_title"){
		handleSetTitle(opargs);
	} else if (name == "cursor_off"){
	} else if (name == "busy_start"){
		handleBusy(true);
	} else if (name == "busy_stop"){
		handleBusy(false);
	} else if (name == "set_icon") {
	} else if (name == "tabline_update") {
		if (opargs.size() < 2 || !opargs.at(0).canConvert<int64_t>()) {
			qWarning() << "Unexpected argument for tabline_update:" << opargs;
			return;
		}
		int64_t curtab = opargs.at(0).toInt();
		QList<Tab> tabs;
		foreach(const QVariant& tabv, opargs.at(1).toList()) {
			QVariantMap tab = tabv.toMap();

			if (!tab.contains("tab") || !tab.contains("name")) {
				qWarning() << "Unexpected tab value in tabline_update:" << tab;
			}

			int64_t num = tab.value("tab").toInt();
			QString name = tab.value("name").toString();
			tabs.append(Tab(num, name));
		}

		emit neovimTablineUpdate(curtab, tabs);
	} else if (name == "option_set") {
		if (2 <= opargs.size()) {
			handleSetOption(opargs.at(0).toString(), opargs.at(1));
		}
	} else if (name == "suspend") {
		if (isWindow()) {
			setWindowState(windowState() | Qt::WindowMinimized);
		} else {
			emit neovimSuspend();
		}
	} else if (name == "popupmenu_show") {
		handlePopupMenuShow(opargs);
	} else if (name == "popupmenu_select") {
		handlePopupMenuSelect(opargs);
	} else if (name == "popupmenu_hide") {
		m_pum.hide();
	} else if (name == "mode_info_set") {
		// TODO
	} else if (name == "flush") {
		// Do Nothing, a notification that nvim is done redrawing.
	} else if (name == "grid_resize") {
		handleGridResize(opargs);
	} else if (name == "default_colors_set") {
		handleDefaultColorsSet(opargs);
	} else if (name == "hl_attr_define") {
		handleHighlightAttributeDefine(opargs);
	} else if (name == "grid_line") {
		handleGridLine(opargs);
	} else if (name == "grid_clear") {
		clearShell();
	} else if (name == "grid_destroy") {
		qDebug() << "Not implemented grid_destroy:" << opargs;
	} else if (name == "grid_cursor_goto") {
		handleGridCursorGoto(opargs);
	} else if (name == "grid_scroll") {
		handleGridScroll(opargs);
	}
	else {
		qDebug() << "Received unknown redraw notification" << name << opargs;
	}

}

void Shell::handlePopupMenuShow(const QVariantList& opargs)
{
	// The 'popupmenu_show' API is not consistent across NeoVim versions!
	// A 5th argument was introduced in neovim/neovim@16c3337
	if (opargs.size() < 4
		|| static_cast<QMetaType::Type>(opargs.at(0).type()) != QMetaType::QVariantList
		|| !opargs.at(1).canConvert<int64_t>()
		|| !opargs.at(2).canConvert<int64_t>()
		|| !opargs.at(3).canConvert<int64_t>()) {
		qWarning() << "Unexpected arguments for popupmenu_show:" << opargs;
		return;
	}
	else if (opargs.size() >= 5 && !opargs.at(4).canConvert<int64_t>()) {
		qWarning() << "Unexpected 5th argument for popupmenu_show:" << opargs.at(4);
		return;
	}

	const QVariantList items = opargs.at(0).toList();
	const int64_t selected = opargs.at(1).toULongLong();
	const int64_t row = opargs.at(2).toULongLong();
	const int64_t col = opargs.at(3).toULongLong();
	//const int64_t grid = (opargs.size() < 5) ? 0 : opargs.at(4).toULongLong();

	QList<PopupMenuItem> model;
	for (const auto& v : items) {
		QVariantList item = v.toList();
		// Item is (text, kind, extra, info)
		QString text = item.value(0).toString();
		if (item.size() < 4
			|| item.isEmpty()
			|| item.value(0).toString().isEmpty()) {

			// Usually faster/smaller to init strings with {} instead of ""
			model.append({ QString{}, QString{}, QString{}, QString{} });
			continue;
		}

		model.append({
			item.value(0).toString(),
			item.value(1).toString(),
			item.value(2).toString(),
			item.value(3).toString() });
	}

	m_pum.setModel(new PopupMenuModel(model));

	m_pum.setSelectedIndex(selected);

	m_pum.setAnchor(row, col);
	m_pum.updateGeometry();
	m_pum.show();
}

void Shell::handlePopupMenuSelect(const QVariantList& opargs)
{
	if (opargs.size() < 1
		|| !opargs.at(0).canConvert<int64_t>()) {
		qWarning() << "Unexpected arguments for popupmenu_select:" << opargs;
		return;
	}

	// Neovim and Qt both use -1 for 'no selection'.
	m_pum.setSelectedIndex(opargs.at(0).toLongLong());
}

void Shell::setNeovimCursor(quint64 row, quint64 col)
{
	update(neovimCursorRect());
	m_cursor_pos = QPoint(col, row);
	update(neovimCursorRect());
}

void Shell::handleModeChange(const QString& mode)
{
	auto old = m_insertMode;

	// TODO: Implement visual aids for other modes
	if (mode == "insert") {
		m_insertMode = true;
	} else {
		m_insertMode = false;
	}

	// redraw the cursor
	if (old != m_insertMode) {
		update(neovimCursorRect());
	}
}

void Shell::handleSetTitle(const QVariantList& opargs)
{
	if (opargs.size() < 1 || !opargs.at(0).canConvert<QByteArray>()) {
		qWarning() << "Unexpected arguments for set_title:" << opargs;
		return;
	}
	QString title = m_nvim->decode(opargs.at(0).toByteArray());
	emit neovimTitleChanged(title);
}

void Shell::handleBusy(bool busy)
{
	if (busy != m_neovimBusy) {
		update(neovimCursorRect());
	}
	m_neovimBusy = busy;
	if (busy) {
		this->setCursor(Qt::WaitCursor);
	} else {
		this->unsetCursor();
	}
	emit neovimBusy(busy);
}

// FIXME: fix QVariant type conversions
void Shell::handleNeovimNotification(const QByteArray &name, const QVariantList& args)
{
	if (name == "Gui" && args.size() > 0) {
		QString guiEvName = m_nvim->decode(args.at(0).toByteArray());
		if (guiEvName == "Font") {
			if (args.size() == 2) {
				QString fdesc = m_nvim->decode(args.at(1).toByteArray());
				setGuiFont(fdesc);
			} else if (args.size() == 3) {
				QString fdesc = m_nvim->decode(args.at(1).toByteArray());
				setGuiFont(fdesc, args.at(2) == 1);
			}
		} else if (guiEvName == "Foreground" && args.size() == 1) {
			activateWindow();
			raise();
		} else if (guiEvName == "WindowMaximized" && args.size() == 2) {
			if (isWindow()) {
				setWindowState(variant_not_zero(args.at(1)) ?
					windowState() | Qt::WindowMaximized :
					windowState() & ~Qt::WindowMaximized);
			} else {
				emit neovimMaximized(variant_not_zero(args.at(1)));
			}
		} else if (guiEvName == "WindowFullScreen" && args.size() == 2) {
			if (isWindow()) {
				setWindowState(variant_not_zero(args.at(1)) ?
					windowState() | Qt::WindowFullScreen :
					windowState() & ~Qt::WindowFullScreen);
			} else {
				emit neovimFullScreen(variant_not_zero(args.at(1)));
			}
		} else if (guiEvName == "Linespace" && args.size() == 2) {
			// The conversion to string and then to int happens because of http://doc.qt.io/qt-5/qvariant.html#toUInt
			// toUint() fails to detect an overflow i.e. it converts to ulonglong and then returns a MAX UINT
			auto val = args.at(1).toString().toInt();
			setLineSpace(val);
			m_nvim->api0()->vim_set_var("GuiLinespace", val);
			resizeNeovim(size());
		} else if (guiEvName == "Mousehide" && args.size() == 2) {
			m_mouseHide = variant_not_zero(args.at(1));
			int val = m_mouseHide ? 1 : 0;
			m_nvim->api0()->vim_set_var("GuiMousehide", val);
		} else if (guiEvName == "Close" && args.size() == 1) {
			qDebug() << "Neovim requested a GUI close";
			emit neovimGuiCloseRequest();
		} else if (guiEvName == "Option" && args.size() >= 3) {
			QString option = m_nvim->decode(args.at(1).toByteArray());
			handleExtGuiOption(option, args.at(2));
		} else if (guiEvName == "SetClipboard" && args.size() >= 4) {
			QStringList lines = args.at(1).toStringList();
			QString type = args.at(2).toString();
			QString reg_name = args.at(3).toString();

			if (reg_name != "*" && reg_name != "+") {
				m_nvim->api0()->vim_report_error(m_nvim->encode("Cannot set register via GUI"));
				return;
			}

			// FIXME proper newline char
			QString data = lines.join("\n");

			QByteArray payload;
			QDataStream serialize(&payload, QIODevice::WriteOnly);
			serialize << type;

			// Store the selection type in the clipboard
			QMimeData *clipData = new QMimeData();
			clipData->setText(data);
			clipData->setData("application/x-nvim-selection-type", payload);

			auto clipboard = QClipboard::Clipboard;
			if (reg_name == "*") {
#if defined(Q_OS_MAC) || defined(Q_OS_WIN32)
				clipboard = QClipboard::Clipboard;
#else
				clipboard = QClipboard::Selection;
#endif
			}

			qDebug() << "Neovim changed clipboard" << data << type << reg_name << clipboard;
			QGuiApplication::clipboard()->setMimeData(clipData, clipboard);
		} else if (guiEvName == "ShowContextMenu") {
			emit neovimShowContextMenu();
		}
		return;
	} else if (name != "redraw") {
		return;
	}

	foreach(const QVariant& update_item, args) {
		if ((QMetaType::Type)update_item.type() != QMetaType::QVariantList) {
			qWarning() << "Received unexpected redraw operation" << update_item;
			continue;
		}

		const QVariantList& redrawupdate = update_item.toList();
		if (redrawupdate.size() < 2) {
			qWarning() << "Received unexpected redraw operation" << update_item;
			continue;
		}

		const QByteArray& name = redrawupdate.at(0).toByteArray();
		const QVariantList& update_args = redrawupdate.mid(1);

		foreach (const QVariant& opargs_var, update_args) {
			if ((QMetaType::Type)opargs_var.type() != QMetaType::QVariantList) {
				qWarning() << "Received unexpected redraw arguments, expecting list" << opargs_var;
				continue;
			}

			const QVariantList& opargs = opargs_var.toList();
			handleRedraw(name, opargs);
		}
	}
}

void Shell::handleExtGuiOption(const QString& name, const QVariant& value)
{
	if (!m_nvim->api2()) return;
	if (name == "Tabline") {
		m_nvim->api2()->nvim_ui_set_option("ext_tabline", value.toBool());
	} else if (name == "Popupmenu") {
		m_nvim->api2()->nvim_ui_set_option("ext_popupmenu", value.toBool());
	} else if (name == "Cmdline") {
	} else if (name == "Wildmenu") {
	} else {
		qDebug() << "Unknown GUI Option" << name << value;
	}
}

void Shell::handleSetOption(const QString& name, const QVariant& value)
{
	if (name == "guifont") {
		setGuiFont(value.toString());
	} else if (name == "guifontset") {
	} else if (name == "guifontwide") {
	} else if (name == "linespace") {
		// The conversion to string and then to int happens because of http://doc.qt.io/qt-5/qvariant.html#toUInt
		// toUint() fails to detect an overflow i.e. it converts to ulonglong and then returns a MAX UINT
		setLineSpace(value.toString().toInt());
	} else if (name == "showtabline") {
		emit neovimShowtablineSet(value.toString().toInt());
	} else if (name == "ext_tabline") {
		emit neovimExtTablineSet(value.toBool());
	} else if (name == "ext_popupmenu") {
		emit neovimExtPopupmenuSet(value.toBool());
	// TODO
	} else if (name == "arabicshape") {
	} else if (name == "ambiwidth") {
	} else if (name == "emoji") {
	} else if (name == "termguicolors") {
	} else if (name == "ext_cmdline") {
	} else if (name == "ext_wildmenu") {
	} else if (name == "ext_linegrid") {
	} else {
		qDebug() << "Received unknown option" << name << value;
	}
}

// Enable/Disable mouse support. See mouse_off/mouse_on in :h ui-global.
void Shell::handleMouse(bool enabled)
{
	m_mouseEnabled = enabled;
}

void Shell::handleGridResize(const QVariantList& opargs)
{
	if (opargs.size() < 3
		|| !opargs.at(0).canConvert<qint64>()
		|| !opargs.at(1).canConvert<qint64>()
		|| !opargs.at(2).canConvert<qint64>()) {
		qWarning() << "Unexpected arguments for grid_resize:" << opargs;
		return;
	}

	// Unused parameters:
	//     "grid" = opargs.at(0).toULongLong()

	const uint64_t width = opargs.at(1).toULongLong();
	const uint64_t height = opargs.at(2).toULongLong();

	handleResize(width, height);
}

void Shell::handleDefaultColorsSet(const QVariantList& opargs)
{
	if (opargs.size() < 5
		|| !opargs.at(0).canConvert<qint64>()
		|| !opargs.at(1).canConvert<qint64>()
		|| !opargs.at(2).canConvert<qint64>()
		|| !opargs.at(3).canConvert<qint64>()
		|| !opargs.at(4).canConvert<qint64>()) {
		qWarning() << "Unexpected arguments for default_colors_set:" << opargs;
		return;
	}

	// Unused parameters:
	//     "cterm_fg" = opargs.at(3).toULongLong()
	//     "cterm_bg" = opargs.at(4).toULongLong()

	const uint64_t rgb_fg = opargs.at(0).toULongLong();
	const uint64_t rgb_bg = opargs.at(1).toULongLong();
	const uint64_t rgb_sp = opargs.at(2).toULongLong();

	MsgpackRequest* getBackgroundMode{
		m_nvim->api0()->vim_get_option(QString{ "background" }.toLatin1()) };

	connect(getBackgroundMode, &MsgpackRequest::finished, this, &Shell::handleGetBackgroundOption);

	const QColor foregroundColor = color(rgb_fg, QColor::Invalid);
	const QColor backgroundColor = color(rgb_bg, QColor::Invalid);
	const QColor specialColor = color(rgb_sp, QColor::Invalid);

	// Update shellwidget default colors
	setForeground(foregroundColor);
	setBackground(backgroundColor);
	setSpecial(specialColor);

	// Cells drawn with the default colors require a re-paint
	update();
}

void Shell::handleHighlightAttributeDefine(const QVariantList& opargs)
{
	if (opargs.size() < 4
		|| !opargs.at(0).canConvert<qint64>()
		|| static_cast<QMetaType::Type>(opargs.at(1).type()) != QMetaType::QVariantMap
		|| static_cast<QMetaType::Type>(opargs.at(2).type()) != QMetaType::QVariantMap) {
		qWarning() << "Unexpected arguments for hl_attr_define:" << opargs;
		return;
	}

	// Unused parameters:
	//     "cterm_attr" = opargs.at(2).toMap()

	const uint64_t id = opargs.at(0).toULongLong();
	const auto& rgb_attr = opargs.at(1).toMap();

	const QColor foregroundColor = (rgb_attr.contains("foreground")) ?
		color(rgb_attr.value("foreground").toLongLong(), QColor::Invalid) :
		QColor::Invalid;

	const QColor backgroundColor = (rgb_attr.contains("background")) ?
		color(rgb_attr.value("background").toLongLong(), QColor::Invalid) :
		QColor::Invalid;

	const QColor specialColor = rgb_attr.contains(("special")) ?
		color(rgb_attr.value("special").toLongLong(), QColor::Invalid) :
		QColor::Invalid;

	HighlightAttribute hl_attr {
		foregroundColor,
		backgroundColor,
		specialColor,
		rgb_attr.contains("reverse"),
		rgb_attr.contains("italic"),
		rgb_attr.contains("bold"),
		rgb_attr.contains("underline"),
		rgb_attr.contains("undercurl") };

	m_highlightMap.insert(id, hl_attr);
}


void Shell::handleGridLine(const QVariantList& opargs)
{
	if (opargs.size() < 4
		|| !opargs.at(0).canConvert<qint64>()
		|| !opargs.at(1).canConvert<qint64>()
		|| !opargs.at(2).canConvert<qint64>()
		|| static_cast<QMetaType::Type>(opargs.at(3).type()) != QMetaType::QVariantList) {
		qWarning() << "Unexpected arguments for grid_line:" << opargs;
		return;
	}

	// Unused parameters:
	//     "grid" = opargs.at(0).toULongLong()

	const uint64_t row = opargs.at(1).toULongLong();
	const uint64_t col_start = opargs.at(2).toULongLong();
	const QVariantList& cells = opargs.at(3).toList();

	// Last used hl_attr, default ctor triggers default highlight/style.
	HighlightAttribute hl_attr;

	uint64_t col_next = col_start;
	for (const auto& cell : cells) {
		const QVariantList& cellPropertyList = cell.toList();

		QString text = m_nvim->decode(cellPropertyList[0].toByteArray());

		// Optional highlight style, default is the last hl_attr.
		if (cellPropertyList.size() > 1) {
			const uint64_t hl_id = cellPropertyList[1].toULongLong();

			// Entry for 'hl_id == 0' is intentionally absent.
			// Any unknown key triggers the default highlight/style.
			hl_attr = m_highlightMap.value(hl_id);
		}

		// Optional repeat count, default is 1.
		uint64_t repeat = 1;
		if (cellPropertyList.size() > 2) {
			repeat = cellPropertyList[2].toULongLong();
		}

		// Send GUI updates to 'ShellWidget'.
		for (uint64_t i=0;i<repeat;i++)
		{
			put(text, row, col_next,
				hl_attr.GetForegroundColor(),
				hl_attr.GetBackgroundColor(),
				hl_attr.GetSpecialColor(),
				hl_attr.IsBold(),
				hl_attr.IsItalic(),
				hl_attr.IsUnderline(),
				hl_attr.IsUndercurl());

			col_next++;
		}
	}
}

void Shell::handleGridCursorGoto(const QVariantList& opargs)
{
	if (opargs.size() < 3
		|| !opargs.at(1).canConvert<quint64>()
		|| !opargs.at(2).canConvert<quint64>()) {
		qWarning() << "Unexpected arguments for grid_cursor_goto:" << opargs;
		return;
	}

	// Unused parameters:
	//     "grid" = opargs.at(0).toULongLong()

	const uint64_t row = opargs.at(1).toULongLong();
	const uint64_t column = opargs.at(2).toULongLong();

	setNeovimCursor(row, column);
	qApp->inputMethod()->update(Qt::ImCursorRectangle);
}

void Shell::handleGridScroll(const QVariantList& opargs)
{
	if (opargs.size() < 7
		|| !opargs.at(0).canConvert<quint64>()
		|| !opargs.at(1).canConvert<quint64>()
		|| !opargs.at(2).canConvert<quint64>()
		|| !opargs.at(3).canConvert<quint64>()
		|| !opargs.at(4).canConvert<quint64>()
		|| !opargs.at(5).canConvert<quint64>()
		|| !opargs.at(6).canConvert<quint64>()) {
		qWarning() << "Unexpected arguments for grid_scroll:" << opargs;
		return;
	}

	// Unused parameters:
	//     "grid" = opargs.at(0).toULongLong()
	//     "cols" = opargs.at(6).toULongLong()

	const uint64_t top = opargs.at(1).toULongLong();
	const uint64_t bot = opargs.at(2).toULongLong();
	const uint64_t left = opargs.at(3).toULongLong();
	const uint64_t right = opargs.at(4).toULongLong();
	const uint64_t rows = opargs.at(5).toULongLong();

	m_scroll_region = QRect(QPoint(left, top), QPoint(right, bot));

	if (m_scroll_region.contains(m_cursor_pos)) {
		// Schedule cursor region to be repainted
		update(neovimCursorRect());
	}

	scrollShellRegion(m_scroll_region.top(), m_scroll_region.bottom(),
		m_scroll_region.left(), m_scroll_region.right(), rows);
}

void Shell::paintEvent(QPaintEvent *ev)
{
	if (!m_attached) {
		QPainter painter(this);
		painter.fillRect(rect(), palette().window());
		return;
	}

	ShellWidget::paintEvent(ev);

	// paint cursor - we are not actually using Neovim colors yet,
	// just invert the shell colors by painting white with XoR
	if (!m_neovimBusy && ev->region().contains(neovimCursorTopLeft())) {
		bool wide = contents().constValue(m_cursor_pos.y(),
						m_cursor_pos.x()).IsDoubleWidth();
		QRect cursorRect(neovimCursorTopLeft(), cellSize());

		if (m_insertMode) {
			cursorRect.setWidth(2);
		} else if (wide) {
			cursorRect.setWidth(cursorRect.width()*2);
		}
		QPainter painter(this);
		painter.setPen(m_cursor_color);
		painter.setCompositionMode(QPainter::RasterOp_SourceXorDestination);
		if (hasFocus()) {
			painter.fillRect(cursorRect, m_cursor_color);
		} else {
			painter.drawRect(cursorRect);
		}
	}
}

void Shell::keyPressEvent(QKeyEvent *ev)
{
	if (!m_nvim || !m_attached) {
		QWidget::keyPressEvent(ev);
		return;
	}

	// conceal mouse pointer when typing
	if (m_mouseHide) {
		this->setCursor(Qt::BlankCursor);
	}

	QString inp = Input.convertKey(ev->text(), ev->key(), ev->modifiers());
	if (inp.isEmpty()) {
		QWidget::keyPressEvent(ev);
		return;
	}

	m_nvim->api0()->vim_input(m_nvim->encode(inp));
	// FIXME: bytes might not be written, and need to be buffered
}

void Shell::neovimMouseEvent(QMouseEvent *ev)
{
	if (!m_attached || !m_mouseEnabled) {
		return;
	}

	QPoint pos(ev->x()/cellSize().width(),
			ev->y()/cellSize().height());
	QString inp;
	if (ev->type() == QEvent::MouseMove) {
		Qt::MouseButton bt;
		if (ev->buttons() & Qt::LeftButton) {
			bt = Qt::LeftButton;
		} else if (ev->buttons() & Qt::RightButton) {
			bt = Qt::RightButton;
		} else if (ev->buttons() & Qt::MidButton) {
			bt = Qt::MidButton;
		} else {
			return;
		}
		inp = Input.convertMouse(bt, ev->type(), ev->modifiers(), pos, 0);
	} else {
		inp = Input.convertMouse(ev->button(), ev->type(), ev->modifiers(), pos,
						m_mouseclick_count);
	}
	if (inp.isEmpty()) {
		return;
	}
	m_nvim->api0()->vim_input(inp.toLatin1());
}
void Shell::mousePressEvent(QMouseEvent *ev)
{
	m_mouseclick_timer.start();
	mouseClickIncrement(ev->button());
	neovimMouseEvent(ev);
}
/** Reset state for mouse N-click tracking */
void Shell::mouseClickReset()
{
	m_mouseclick_count = 0;
	m_mouseclick_pending = Qt::NoButton;
	m_mouseclick_timer.stop();
}
/**
 * Increment consecutive mouse click count
 *
 * Since Vim only supports up to 4-click events the counter
 * rotates after 4 clicks.
 */
void Shell::mouseClickIncrement(Qt::MouseButton bt)
{
	if (m_mouseclick_pending != Qt::NoButton && bt != m_mouseclick_pending) {
		mouseClickReset();
	}

	m_mouseclick_pending = bt;
	if (m_mouseclick_count > 3) {
		m_mouseclick_count = 1;
	} else {
		m_mouseclick_count += 1;
	}
}
void Shell::mouseReleaseEvent(QMouseEvent *ev)
{
	neovimMouseEvent(ev);
}
void Shell::mouseMoveEvent(QMouseEvent *ev)
{
	unsetCursor();
	QPoint pos(ev->x()/cellSize().width(),
			ev->y()/cellSize().height());
	if (pos != m_mouse_pos) {
		m_mouse_pos = pos;
		mouseClickReset();
		neovimMouseEvent(ev);
	}
}

void Shell::wheelEvent(QWheelEvent *ev)
{
	if (!m_attached || !m_mouseEnabled) {
		return;
	}
#ifdef Q_OS_MAC
	// For some reason <ScrollWheel*> scrolls multiple lines at once
	// we have to account for it, to make sure that pixelDelta() is used correctly.
	const int scroll_columns = 6;
	const int scroll_rows = 3;
	// Minimal scroll step
	int scroll_step_x = cellSize().width() * scroll_columns;
	int scroll_step_y = cellSize().height() * scroll_rows;
	// Total scroll delta considering previous events
	QPoint total_delta = m_mouse_wheel_delta_fraction + ev->pixelDelta();
	// Delta rounded to a minimal scroll step
	QPoint cell_delta(total_delta.x() / scroll_step_x, total_delta.y() / scroll_step_y);
	// Save remainder for future events
	m_mouse_wheel_delta_fraction = total_delta - QPoint(cell_delta.x() * scroll_step_x, cell_delta.y() * scroll_step_y);

	int horiz = cell_delta.x();
	int vert = cell_delta.y();
#else
	int horiz, vert;
	horiz = ev->angleDelta().x();
	vert = ev->angleDelta().y();
#endif

	if (horiz == 0 && vert == 0) {
		return;
	}

	QPoint pos(ev->x()/cellSize().width(),
			ev->y()/cellSize().height());

	QString inp;
	if (vert != 0) {
		inp += QString("<%1ScrollWheel%2><%3,%4>")
			.arg(Input.modPrefix(ev->modifiers()))
			.arg(vert > 0 ? "Up" : "Down")
			.arg(pos.x()).arg(pos.y());
	}
	if (horiz != 0) {
		inp += QString("<%1ScrollWheel%2><%3,%4>")
			.arg(Input.modPrefix(ev->modifiers()))
			.arg(horiz > 0 ? "Left" : "Right")
			.arg(pos.x()).arg(pos.y());
	}
	m_nvim->api0()->vim_input(inp.toLatin1());
}

void Shell::updateWindowId()
{
	if (m_attached &&
		m_nvim->connectionType() == NeovimConnector::SpawnedConnection) {
		WId window_id = effectiveWinId();
		m_nvim->api0()->vim_set_var("GuiWindowId", QVariant(window_id));
		m_nvim->api0()->vim_command(QString("let v:windowid = %1").arg(window_id).toLatin1());
		updateClientInfo();
	}
}

void Shell::updateClientInfo()
{
	if (m_attached) {
		auto api4 = m_nvim->api4();
		if (api4) {
			WId window_id = effectiveWinId();
			auto version = QVariantMap();
			version.insert("major", PROJECT_VERSION_MAJOR);
			version.insert("minor", PROJECT_VERSION_MINOR);
			version.insert("patch", PROJECT_VERSION_PATCH);
			QVariantMap attrs;
			attrs.insert("windowid", window_id);
			attrs.insert("gui-clipboard", true);
			api4->nvim_set_client_info("nvim-qt", version, "ui", QVariantMap(), attrs);
		}
	}
}

bool Shell::event(QEvent *event)
{
	if (event->type() == QEvent::KeyPress) {
		QKeyEvent *ke = (QKeyEvent *)(event);
		if (ke->key() == Qt::Key_Tab || ke->key() == Qt::Key_Backtab) {
			keyPressEvent(ke);
			return true;
		}
	}
	if (event->type() == QEvent::WinIdChange){
		updateWindowId();
	}

	return QWidget::event(event);
}

/// Resize remote Neovim (pixel coordinates)
///
/// The given size is rounded down to the nearest
/// row/column count.
void Shell::resizeNeovim(const QSize& newSize)
{
	int n_cols = newSize.width()/cellSize().width();
	int n_rows = newSize.height()/cellSize().height();
	resizeNeovim(n_cols, n_rows);
}

/// Resize remote Neovim.
///
/// Neovim ignores concurrent resizes. If you call this method while
/// a resize is already in progress, the later call is delayed until
/// the previous one is finished.
void Shell::resizeNeovim(int n_cols, int n_rows)
{
	if (!m_nvim || (n_cols == columns() && n_rows == rows())) {
		return;
	}
	if (m_resizing) {
		m_resize_neovim_pending = QSize(n_cols, n_rows);
	} else {
		m_nvim->api0()->ui_try_resize(n_cols, n_rows);
		m_resizing = true;
	}
}

void Shell::resizeEvent(QResizeEvent *ev)
{
	if (!m_attached) {
		QWidget::resizeEvent(ev);
		return;
	}

	resizeNeovim(ev->size());
	QWidget::resizeEvent(ev);
}

/**
 * Finished call to ui_try_resize
 */
void Shell::neovimResizeFinished()
{
	m_resizing = false;
	if (m_resize_neovim_pending.isValid()) {
		resizeNeovim(m_resize_neovim_pending.width(),
				m_resize_neovim_pending.height());
		m_resize_neovim_pending = QSize();
	}
}

void Shell::changeEvent( QEvent *ev)
{
	if (ev->type() == QEvent::WindowStateChange && isWindow()) {
		updateGuiWindowState(windowState());
	}
	QWidget::changeEvent(ev);
}

/// Call this when the GUI window state has changed. The relevant
/// g:Gui* variables will be set in Neovim
void Shell::updateGuiWindowState(Qt::WindowStates state)
{
	if (!m_attached) {
		return;
	}
	if (state & Qt::WindowMaximized) {
		m_nvim->api0()->vim_set_var("GuiWindowMaximized", 1);
	} else {
		m_nvim->api0()->vim_set_var("GuiWindowMaximized", 0);
	}
	if (state & Qt::WindowFullScreen) {
		m_nvim->api0()->vim_set_var("GuiWindowFullScreen", 1);
	} else {
		m_nvim->api0()->vim_set_var("GuiWindowFullScreen", 0);
	}
}

void Shell::closeEvent(QCloseEvent *ev)
{
	if (m_attached &&
		m_nvim->connectionType() == NeovimConnector::SpawnedConnection) {
		// If attached to a spawned Neovim process, ignore the event
		// and try to close Neovim as :qa
		ev->ignore();
		bailoutIfinputBlocking();
		m_nvim->api0()->vim_command("confirm qa");
	} else {
		QWidget::closeEvent(ev);
	}
}

void Shell::focusInEvent(QFocusEvent *ev)
{
	if (m_attached) {
		// See neovim-qt/issues/329 the FocusGained key no longer exists, use autocmd instead
		m_nvim->api0()->vim_command("if exists('#FocusGained') | doautocmd FocusGained | endif");
	}
	QWidget::focusInEvent(ev);
}

void Shell::focusOutEvent(QFocusEvent *ev)
{
	if (m_attached) {
		m_nvim->api0()->vim_command("if exists('#FocusLost') | doautocmd FocusLost | endif");
	}
	QWidget::focusOutEvent(ev);
}

QColor Shell::color(qint64 color, const QColor& fallback)
{
	if (color == -1) {
		return fallback;
	}
	return QRgb(color);
}

/*
 * Display a tooltip over the shell, covering underlying shell content.
 * The tooltip is placed at the current shell cursor position.
 *
 * When the given string is empty the tooltip is concealed.
 *
 * FIXME: Colors could use improving
 */
void Shell::tooltip(const QString& text)
{
	m_tooltip->setText(text);
	if ( text.isEmpty() ) {
		m_tooltip->hide();
		return;
	}

	if ( !m_tooltip->isVisible() ) {
		m_tooltip->setMinimumHeight(cellSize().height());
		m_tooltip->move(neovimCursorTopLeft() );
		m_tooltip->show();
	}

	m_tooltip->setMinimumWidth( QFontMetrics(m_tooltip->font()).width(text) );
	m_tooltip->setMaximumWidth( QFontMetrics(m_tooltip->font()).width(text) );
	m_tooltip->update();
}

void Shell::inputMethodEvent(QInputMethodEvent *ev)
{
	if (!m_attached) {
		return;
	}
	if ( !ev->commitString().isEmpty() ) {
		QByteArray s = m_nvim->encode(ev->commitString());
		m_nvim->api0()->vim_input(s);
		tooltip("");
	} else {
		tooltip(ev->preeditString());
	}
}

QVariant Shell::inputMethodQuery(Qt::InputMethodQuery query) const
{
	if ( query == Qt::ImFont) {
		return font();
	} else if ( query == Qt::ImMicroFocus || query == Qt::ImCursorRectangle ) {
		return QRect(neovimCursorTopLeft(), cellSize());
	}

	return QVariant();
}

bool Shell::neovimBusy() const
{
	return m_neovimBusy;
}

bool Shell::neovimAttached() const
{
	return m_attached;
}

/**
 * Check if a font can be safely used as a fixed pitch font
 *
 * This function is not perfect and some broken fonts may still return false,
 * or font substitution may cause good fonts to fail. The font max/average
 * metrics are compared with the italic/bold double width variants.
 */
bool Shell::isBadMonospace(const QFont& f)
{
	QFont fi(f);
	fi.setItalic(true);
	QFont fb(f);
	fb.setBold(true);
	QFont fbi(fb);
	fbi.setItalic(true);

	QFontMetrics fm_normal(f);
	QFontMetrics fm_italic(fi);
	QFontMetrics fm_boldit(fbi);
	QFontMetrics fm_bold(fb);

	// Regular
	if ( fm_normal.averageCharWidth() != fm_normal.maxWidth() ) {
		QFontInfo info(f);
		qDebug() << f.family()
			<< "Average and Maximum font width mismatch for Regular font; QFont::exactMatch() is" << f.exactMatch()
			<< "Real font is " << info.family() << info.pointSize();
		return true;
	}

	// Italic
	if ( fm_italic.averageCharWidth() != fm_italic.maxWidth() ||
			fm_italic.maxWidth()*2 != fm_italic.width("MM") ) {
		QFontInfo info(fi);
		qDebug() << fi.family() << "Average and Maximum font width mismatch for Italic font; QFont::exactMatch() is" << fi.exactMatch()
			<< "Real font is " << info.family() << info.pointSize();
		return true;
	}

	// Bold
	if ( fm_bold.averageCharWidth() != fm_bold.maxWidth() ||
			fm_bold.maxWidth()*2 != fm_bold.width("MM") ) {
		QFontInfo info(fb);
		qDebug() << fb.family() << "Average and Maximum font width mismatch for Bold font; QFont::exactMatch() is" << fb.exactMatch()
			<< "Real font is " << info.family() << info.pointSize();
		return true;
	}

	// Bold+Italic
	if ( fm_boldit.averageCharWidth() != fm_boldit.maxWidth() ||
			fm_boldit.maxWidth()*2 != fm_boldit.width("MM") ) {
		QFontInfo info(fbi);
		qDebug() << fbi.family() << "Average and Maximum font width mismatch for Bold+Italic font; QFont::exactMatch() is" << fbi.exactMatch()
			<< "Real font is " << info.family() << info.pointSize();
		return true;
	}

	if ( fm_normal.maxWidth() != fm_italic.maxWidth() ||
		fm_normal.maxWidth() != fm_boldit.maxWidth() ||
		fm_normal.maxWidth() != fm_bold.maxWidth()) {
		qDebug() << f.family() << "Average and Maximum font width mismatch between font types";
		return true;
	}

	return false;
}

void Shell::dragEnterEvent(QDragEnterEvent *ev)
{
	if (!m_attached) {
		return;
	}

	if (ev->mimeData()->hasFormat("text/uri-list")) {
		ev->acceptProposedAction();
	}
}

void Shell::dropEvent(QDropEvent *ev)
{
	if (!m_attached) {
		return;
	}

	if ( ev->mimeData()->hasFormat("text/uri-list") ) {
		QList<QUrl> urls = ev->mimeData()->urls();
		if ( urls.size() == 0 ) {
			return;
		}

		openFiles(urls);
	}
	ev->acceptProposedAction();
}

/// Open multiple URLs in Neovim
void Shell::openFiles(QList<QUrl> urls)
{
	if (m_nvim && m_attached) {
		QVariantList args;
		foreach(QUrl u, urls) {
			if ( u.scheme() == "file" ) {
				args.append(u.toLocalFile());
			} else {
				args.append(u.toString());
			}
		}
		m_nvim->api0()->vim_call_function("GuiDrop", args);
	} else {
		// Neovim cannot open urls now. Store them to open later.
		m_deferredOpen.append(urls);
	}
}

// If neovim is blocked waiting for input, attempt to bailout from
// whatever neovim is doing by pressing Ctrl-C.
void Shell::bailoutIfinputBlocking()
{
	auto api = m_nvim->api2();
	if (api) {
		auto req = api->nvim_get_mode();

		connect(req, &MsgpackRequest::finished, [api](quint32 msgid, quint64 f, const QVariant& r) {
				auto map = r.toMap();
				if (map.value("blocking", false) == true) {
					api->nvim_input("<C-c>");
				}
		});
	}
}

ShellRequestHandler::ShellRequestHandler(Shell *parent)
:QObject(parent)
{
}

const char SELECTION_MIME_TYPE[] = "application/x-nvim-selection-type";
void ShellRequestHandler::handleRequest(MsgpackIODevice* dev, quint32 msgid, const QByteArray& method, const QVariantList& args)
{
	if (method == "Gui" && args.size() > 0) {
		QString ctx = args.at(0).toString();
		if (ctx == "GetClipboard" && args.size() > 1) {
			QVariant reg = args.at(1);
			QString reg_name = reg.toString();

			if (reg_name != "*" && reg_name != "+") {
				dev->sendResponse(msgid, QString("Unknown register"), QVariant());
				return;
			}

			// + by default
			auto mode = QClipboard::Clipboard;
			if (reg_name == "*") {
#if defined(Q_OS_MAC) || defined(Q_OS_WIN32)
				mode = QClipboard::Clipboard;
#else
				mode = QClipboard::Selection;
#endif
			}

			// Check nvim, ops.c/get_clipboard() - Expected to return a list with two items
			// [register data, selection type]. The type can be ommited.
			QVariantList result;

			auto clipboard_data = QGuiApplication::clipboard()->mimeData(mode);
			auto data = clipboard_data->text();
			qDebug() << data << "<<<<< clipboard text";
			// The register data is either a string with a single line,
			// or a list of strings for multiple lines.
			if (data.contains("\n")) {
				result.append(data.split("\n"));
			} else {
				result.append(QStringList() << data);
			}

			// If available, deserialize the motion type from the clipboard
			if (clipboard_data->hasFormat(SELECTION_MIME_TYPE)) {
				QString type;
				QDataStream serialize(clipboard_data->data(SELECTION_MIME_TYPE));
				serialize >> type;
				result.append(type);
			} else {
				result.append("");
			}

			qDebug() << "Neovim requested clipboard contents" << args << mode << "->" << result;
			dev->sendResponse(msgid, QVariant(), result);
			return;
		}
	}
	// be sure to return early or this message will be sent
	dev->sendResponse(msgid, QString("Unknown method"), QVariant());
}

/**
 * Convert neovim error response into an error message string. If this fails
 * serialize the entire error response as with QDebug.
 */
QString Shell::neovimErrorToString(const QVariant& err)
{
	auto lst = err.toList();
	if (1 < lst.size()) {
		return lst.at(1).toByteArray();
	} else {
		QString payload;
		QDebug dbg(&payload);
		dbg << err;
		return payload;
	}
}

void Shell::handleGinitError(quint32 msgid, quint64 fun, const QVariant& err)
{
	qDebug() << "ginit.vim error " << err;
	auto msg = neovimErrorToString(err);
	m_nvim->api0()->vim_report_error("ginit.vim error: " + msg.toUtf8());
}

void Shell::handleShimError(quint32 msgid, quint64 fun, const QVariant& err)
{
	qDebug() << "GUI shim error " << err;
}

void Shell::handleGetBackgroundOption(quint32 msgid, quint64 fun, const QVariant& val)
{
	const QString mode{ val.toString() };

	if (mode == "dark" && getBackgroundType() != Background::Dark) {
		setBackgroundType(Background::Dark);
		update();
	}
	else if (mode == "light" && getBackgroundType() != Background::Light) {
		setBackgroundType(Background::Light);
		update();
	}
}

} // namespace NeovimQt
