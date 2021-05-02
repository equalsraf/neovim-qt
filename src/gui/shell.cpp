#include "shell.h"

#include <cmath>
#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QScreen>
#include <QFontDialog>
#include <QKeyEvent>
#include <QMimeData>
#include <QPainter>
#include <QPaintEvent>
#include <QSettings>

#include "compat.h"
#include "helpers.h"
#include "input.h"
#include "konsole_wcwidth.h"
#include "msgpackrequest.h"
#include "util.h"
#include "version.h"

namespace NeovimQt {

static ShellOptions GetShellOptionsFromQSettings() noexcept
{
	ShellOptions opts;
	QSettings settings("nvim-qt", "nvim-qt");

	QVariant ext_linegrid{ settings.value("ext_linegrid", opts.IsLineGridEnabled()) };
	QVariant ext_popupmenu{ settings.value("ext_popupmenu", opts.IsPopupmenuEnabled()) };
	QVariant ext_tabline{ settings.value("ext_tabline", opts.IsTablineEnabled()) };

	if (ext_linegrid.canConvert<bool>())
	{
		opts.SetIsLineGridEnabled(ext_linegrid.toBool());
	}

	if (ext_popupmenu.canConvert<bool>())
	{
		opts.SetIsPopupmenuEnabled(ext_popupmenu.toBool());
	}

	if (ext_tabline.canConvert<bool>())
	{
		opts.SetIsTablineEnabled(ext_tabline.toBool());
	}

	return opts;
}

Shell::Shell(NeovimConnector *nvim, QWidget *parent)
	: ShellWidget{ parent }
	, m_nvim{ nvim }
	, m_options{ GetShellOptionsFromQSettings() }
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

	if (!m_nvim) {
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

/// Set the GUI font, or display the current font
///
/// @param updateOption controls update of the guifont option, which should not
/// be updated when the user calls from 'set guifont=...'.
bool Shell::setGuiFont(const QString& fdesc, bool force, bool updateOption)
{
	// Exit early if the font description does not change, prevent loops.
	if (fdesc.compare(fontDesc(), Qt::CaseInsensitive) == 0) {
		return false;
	}

	bool setShellFontSuccess{ false };
	const bool isGuiDialogRequest{ fdesc == "*" };

	if (isGuiDialogRequest) {
		bool fontDialogSuccess = false;
		QFont font{ QFontDialog::getFont(&fontDialogSuccess, QWidget::font(), this, {},
			QFontDialog::MonospacedFonts) };

		// The font selection dialog was canceled by the user.
		if (!fontDialogSuccess) {
			return false;
		}

		setShellFontSuccess = setShellFont(font, force);
	}
	else {
		QVariant varFont{ TryGetQFontFromDescription(fdesc) };

		if (!ShellWidget::IsValidFont(varFont)) {
			m_nvim->api0()->vim_report_error(
				m_nvim->encode(varFont.toString()));
			return false;
		}

		setShellFontSuccess = setShellFont(qvariant_cast<QFont>(varFont), force);
	}

	// Only update the ShellWidget when font changes.
	if (!setShellFontSuccess || !m_attached) {
		return false;
	}

	// Font changed: trigger resize to update the ShellWidget, and update font variables.
	resizeNeovim(size());

	m_nvim->api0()->vim_set_var("GuiFont", fontDesc());

	// Updating guifont when the user has already called 'set guifont=...' may cause
	// unwanted recursion. Only update this option for ':GuiFont', and dialog calls.
	if (isGuiDialogRequest || updateOption) {
		m_nvim->api0()->vim_set_option("guifont", fontDesc());
	}

	return true;
}

bool Shell::setGuiFontWide(const QString& fdesc) noexcept
{
	// An empty list is valid, use guifont
	if (fdesc.isEmpty())
	{
		m_guifontwidelist = {};
		update();
		return true;
	}

	const QStringList fdescList{ fdesc.split(",") };
	if (fdescList.size() < 1) {
		return false;
	}

	std::vector<QFont> fontList;
	fontList.reserve(fdescList.size());

	for (const auto& strFont : fdescList) {
		QVariant varFont{ TryGetQFontFromDescription(strFont) };

		if (!ShellWidget::IsValidFont(varFont)) {
			m_nvim->api0()->vim_report_error(
				m_nvim->encode(varFont.toString()));
			return false;
		}

		fontList.push_back(qvariant_cast<QFont>(varFont));
	}

	m_guifontwidelist = std::move(fontList);
	update();
	return true;
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

	QRect screenRect = NeovimQt::Compat::screenAvailableGeometry(this);
	int64_t width = screenRect.width()*0.66/cellSize().width();
	int64_t height = screenRect.height()*0.66/cellSize().height();
	QVariantMap options;
	if (m_options.IsTablineEnabled()) {
		options.insert("ext_tabline", true);
	}
	if (m_options.IsPopupmenuEnabled()) {
		options.insert("ext_popupmenu", true);
	}
	if (m_options.IsLineGridEnabled()
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
		m_hg_foreground = QColor{ attrs.value("foreground").toUInt() };
	} else {
		m_hg_foreground = foreground();
	}

	if (attrs.contains("background")) {
		m_hg_background = QColor{ attrs.value("background").toUInt() };
	} else {
		m_hg_background = background();
	}

	if (attrs.contains(("special"))) {
		m_hg_special = QColor{ attrs.value("special").toUInt() };
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
	m_font_strikethrough = attrs.value("strikethrough").toBool();
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
				m_font_underline, m_font_undercurl, m_font_strikethrough);
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
	if (args.size() < 1
		|| !args.at(0).canConvert<int64_t>()) {
		qWarning() << "Unexpected arguments for redraw:scroll" << args;
		return;
	}

	const int64_t count{ args.at(0).toLongLong() };

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
		handleModeChange(opargs);
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
		handleModeInfoSet(opargs);
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
	} else if (name == "hl_group_set") {
		handleHighlightGroupSet(opargs);
	} else if (name == "win_viewport") {
	} else {
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

void Shell::handleModeChange(const QVariantList& opargs)
{
	if (opargs.size() < 2
		|| !opargs.at(0).canConvert<QByteArray>()
		|| !opargs.at(1).canConvert<uint64_t>()) {
		qWarning() << "Unexpected arguments for mode_change:" << opargs;
		return;
	}

	const QString mode{ m_nvim->decode(opargs.at(0).toByteArray()) };
	const uint64_t modeIndex{ opargs.at(1).toULongLong() };

	if (!m_cursor.IsStyleEnabled()) {
		if (mode == "insert") {
			m_cursor.SetColor({});
			m_cursor.SetStyle(Cursor::Shape::Vertical, 25);
			m_cursor.SetTimer(0, 0, 0);
		}
		else if (mode == "replace") {
			m_cursor.SetColor({});
			m_cursor.SetStyle(Cursor::Shape::Horizontal, 20);
			m_cursor.SetTimer(0, 0, 0);
		}
		else {
			m_cursor.SetColor({});
			m_cursor.SetStyle(Cursor::Shape::Block, 100);
			m_cursor.SetTimer(0, 0, 0);
		}

		update(neovimCursorRect());
		return;
	}

	const uint32_t sizeModeInfo{ static_cast<uint32_t>(m_modeInfo.size()) };
	if (modeIndex >= sizeModeInfo) {
		return;
	}

	const QVariantMap modePropertyMap{ m_modeInfo.at(modeIndex).toMap() };

	Cursor::Shape cursorShape{ Cursor::Shape::Block };
	const QVariant cursor_shape{ modePropertyMap.value("cursor_shape") };
	if (!cursor_shape.isNull() && cursor_shape.canConvert<QByteArray>()) {
		const QString cursor_shape{ modePropertyMap.value("cursor_shape").toByteArray() };
		if (cursor_shape == "block") {
			cursorShape = Cursor::Shape::Block;
		}
		else if (cursor_shape == "horizontal") {
			cursorShape = Cursor::Shape::Horizontal;
		}
		else if (cursor_shape == "vertical") {
			cursorShape = Cursor::Shape::Vertical;
		}
	}

	uint64_t cellPercentage{ 100 };
	const QVariant cell_percentage{ modePropertyMap.value("cell_percentage") };
	if (!cell_percentage.isNull() && cell_percentage.canConvert<uint32_t>()) {
		const uint32_t cell_percentage{ modePropertyMap.value("cell_percentage").toUInt() };
		if (cell_percentage > 0 && cell_percentage < 100) {
			cellPercentage = cell_percentage;
		}
	}

	uint64_t blinkWaitTime{ 0 };
	const QVariant blinkwait{ modePropertyMap.value("blinkwait") };
	if (!blinkwait.isNull() && blinkwait.canConvert<uint32_t>()) {
		blinkWaitTime = blinkwait.toUInt();
	}

	uint64_t blinkOffTime{ 0 };
	const QVariant blinkoff{ modePropertyMap.value("blinkoff") };
	if (!blinkoff.isNull() && blinkoff.canConvert<uint32_t>()) {
		blinkOffTime = blinkoff.toUInt();
	}

	uint64_t blinkOnTime{ 0 };
	const QVariant blinkon{ modePropertyMap.value("blinkon") };
	if (!blinkon.isNull() && blinkon.canConvert<uint32_t>()) {
		blinkOnTime = blinkon.toUInt();
	}

	const QVariant attr_id{ modePropertyMap.value("attr_id") };
	HighlightAttribute highlight;
	if (!attr_id.isNull() && attr_id.canConvert<uint32_t>()) {
		highlight = m_highlightMap.value(attr_id.toUInt());
	}

	m_cursor.SetColor(highlight);
	m_cursor.SetStyle(cursorShape, cellPercentage);
	m_cursor.SetTimer(blinkWaitTime, blinkOnTime, blinkOffTime);

	update(neovimCursorRect());
}

void Shell::handleModeInfoSet(const QVariantList& opargs)
{
	if (opargs.size() < 2
		|| !opargs.at(0).canConvert<bool>()
		|| static_cast<QMetaType::Type>(opargs.at(1).type()) != QMetaType::QVariantList) {
		qWarning() << "Unexpected arguments for mode_info_set:" << opargs;
		return;
	}

	const bool cursor_style_enabled{ opargs.at(0).toBool() };
	const QVariantList mode_info = opargs.at(1).toList();

	m_cursor.SetIsStyleEnabled(cursor_style_enabled);
	m_modeInfo = mode_info;
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
	m_cursor.SetIsBusy(busy);

	if (busy != m_neovimBusy) {
		update(neovimCursorRect());
	}

	m_neovimBusy = busy;

	setCursorFromBusyState();
	emit neovimBusy(busy);
}

// FIXME: fix QVariant type conversions
void Shell::handleNeovimNotification(const QByteArray &name, const QVariantList& args)
{
	if (name == "Gui" && args.size() > 0) {
		QString guiEvName = m_nvim->decode(args.at(0).toByteArray());
		if (guiEvName == "Font") {
			handleGuiFontFunction(args);
		} else if (guiEvName == "Foreground" && args.size() == 1) {
			if (isWindow()) {
				setWindowState(windowState() & ~Qt::WindowMinimized);
				show();
				activateWindow();
			} else {
				emit neovimForeground();
			}
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
			handleLineSpace(args.at(1));
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
		} else if (guiEvName == "WindowOpacity" && args.size() == 2) {
			bool ok = false;
			auto val = args.at(1).toDouble(&ok);
			if (ok) {
				emit neovimOpacity(val);
			}
		} else if (guiEvName == "ShowContextMenu") {
			emit neovimShowContextMenu();
		} else if (guiEvName == "AdaptiveColor") {
			handleGuiAdaptiveColor(args);
		} else if (guiEvName == "AdaptiveFont") {
			handleGuiAdaptiveFont(args);
		} else if (guiEvName == "AdaptiveStyle") {
			handleGuiAdaptiveStyle(args);
		} else if (guiEvName == "AdaptiveStyleList") {
			handleGuiAdaptiveStyleList();
		}
		return;
	} else if (name != "redraw") {
		return;
	}

	DispatchRedrawNotifications(this, args);
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
	} else if (name == "RenderLigatures"){
		setLigatureMode(value.toBool());
	} else {
		qDebug() << "Unknown GUI Option" << name << value;
	}
}

void Shell::handleSetOption(const QString& name, const QVariant& value)
{
	if (name == "guifont") {
		setGuiFont(value.toString(), false /*force*/, false /*setOption*/);
	} else if (name == "guifontset") {
	} else if (name == "guifontwide") {
		handleGuiFontWide(value);
	} else if (name == "linespace") {
		handleLineSpace(value);
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

void Shell::handleGuiFontFunction(const QVariantList& args)
{
	if (args.size() < 2
		|| !args.at(1).canConvert<QByteArray>())
	{
		return;
	}

	const QString fdesc{ m_nvim->decode(args.at(1).toByteArray()) };

	bool force{ false };
	if (args.size() >= 3 && args.at(2).canConvert<bool>())
	{
		force = args.at(2).toBool();
	}

	setGuiFont(fdesc, force, true /*setOption*/);
}

void Shell::handleGuiFontWide(const QVariant& value) noexcept
{
	if (!value.canConvert<QByteArray>())
	{
		qWarning() << "Unexpected value for guifontwide:" << value;
		return;
	}

	const QString fdesc{ m_nvim->decode(value.toByteArray()) };

	setGuiFontWide(fdesc);
}

void Shell::handleLineSpace(const QVariant& value) noexcept
{
	bool isValid{ false };
	int linespace{ value.toInt(&isValid) };

	if (!isValid || linespace < 0) {
		qDebug() << "Invalid GuiLinespace value:" << value;
		return;
	}

	setLineSpace(linespace);
	m_nvim->api0()->vim_set_var("GuiLinespace", linespace);
	resizeNeovim(size());
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

	const uint32_t rgb_fg{ opargs.at(0).toUInt() };
	const uint32_t rgb_bg{ opargs.at(1).toUInt() };
	const uint32_t rgb_sp{ opargs.at(2).toUInt() };

	MsgpackRequest* getBackgroundMode{
		m_nvim->api0()->vim_get_option(QString{ "background" }.toLatin1()) };

	connect(getBackgroundMode, &MsgpackRequest::finished, this, &Shell::handleGetBackgroundOption);

	const QColor foregroundColor{ rgb_fg };
	const QColor backgroundColor{ rgb_bg };
	const QColor specialColor{ rgb_sp };

	// Update shellwidget default colors
	setForeground(foregroundColor);
	setBackground(backgroundColor);
	setSpecial(specialColor);

	// Cells drawn with the default colors require a re-paint
	update();
	emit colorsChanged();
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
	const QVariantMap rgb_attr = opargs.at(1).toMap();

	HighlightAttribute hl_attr{ rgb_attr };
	m_highlightMap.insert(id, hl_attr);
}

void Shell::handleHighlightGroupSet(const QVariantList& opargs) noexcept
{
	if (opargs.size() < 2
		|| opargs.at(0).type() != QVariant::Type::ByteArray
		|| !opargs.at(1).canConvert<uint64_t>()) {
		qWarning() << "Unexpected arguments for hl_group_set:" << opargs;
		return;
	}

	const QString name{ m_nvim->decode(opargs.at(0).toByteArray()) };
	const uint64_t hl_id{ opargs.at(1).toULongLong() };

	m_highlightGroupNameMap.insert(name, hl_id);
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
			put(text, row, col_next, hl_attr);
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
		|| !opargs.at(0).canConvert<uint64_t>()
		|| !opargs.at(1).canConvert<uint64_t>()
		|| !opargs.at(2).canConvert<uint64_t>()
		|| !opargs.at(3).canConvert<uint64_t>()
		|| !opargs.at(4).canConvert<uint64_t>()
		|| !opargs.at(5).canConvert<uint64_t>()
		|| !opargs.at(6).canConvert<int64_t>()) {
		qWarning() << "Unexpected arguments for grid_scroll:" << opargs;
		return;
	}

	// Unused parameters:
	//     "grid" = opargs.at(0).toULongLong()
	//     "cols" = opargs.at(6).toULongLong()

	const uint64_t top{ opargs.at(1).toULongLong() };
	const uint64_t bot{ opargs.at(2).toULongLong() };
	const uint64_t left{ opargs.at(3).toULongLong() };
	const uint64_t right{ opargs.at(4).toULongLong() };
	const int64_t rows{ opargs.at(5).toLongLong() };

	m_scroll_region = QRect(QPoint(left, top), QPoint(right, bot));

	// Remove old cursor
	if (m_scroll_region.contains(m_cursor_pos)) {
		update(neovimCursorRect());
	}

	scrollShellRegion(m_scroll_region.top(), m_scroll_region.bottom(),
		m_scroll_region.left(), m_scroll_region.right(), rows);

	// Draw new cursor
	update(neovimCursorRect());
}

void Shell::handleGuiAdaptiveColor(const QVariantList& opargs) noexcept
{
	if (opargs.size() < 2
		|| !opargs.at(1).canConvert<bool>()) {
		qWarning() << "Unexpected arguments for GuiAdaptiveColor:" << opargs;
		return;
	}

	const bool isEnabled{ opargs.at(1).toBool() };

	emit setGuiAdaptiveColorEnabled(isEnabled);
}

void Shell::handleGuiAdaptiveFont(const QVariantList& opargs) noexcept
{
	if (opargs.size() < 2
		|| !opargs.at(1).canConvert<bool>()) {
		qWarning() << "Unexpected arguments for GuiAdaptiveFont:" << opargs;
		return;
	}

	const bool isEnabled{ opargs.at(1).toBool() };

	emit setGuiAdaptiveFontEnabled(isEnabled);
}

void Shell::handleGuiAdaptiveStyle(const QVariantList& opargs) noexcept
{
	if (opargs.size() < 2
		|| !opargs.at(1).canConvert<QByteArray>()) {
		qWarning() << "Unexpected arguments for GuiAdaptiveStyle:" << opargs;
		return;
	}

	const QString styleName { opargs.at(1).toByteArray() };

	emit setGuiAdaptiveStyle(styleName);
}

void Shell::handleGuiAdaptiveStyleList() noexcept
{
	emit showGuiAdaptiveStyleList();
}

void Shell::paintEvent(QPaintEvent *ev)
{
	if (!m_attached) {
		QPainter painter(this);
		painter.fillRect(rect(), palette().window());
		return;
	}

	ShellWidget::paintEvent(ev);
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

	const QString inp{ Input::convertKey(*ev) };

	// Uncomment for key input debugging and unit test writing.
	// qDebug() << "QKeyEvent ev:" << ev;
	// qDebug() << "  " << inp;

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
		} else if (ev->buttons() & Qt::MiddleButton) {
			bt = Qt::MiddleButton;
		} else {
			return;
		}
		inp = Input::convertMouse(bt, ev->type(), ev->modifiers(), pos, 0);
	} else {
		inp = Input::convertMouse(ev->button(), ev->type(), ev->modifiers(), pos,
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
	setCursorFromBusyState();

	QPoint pos(ev->x()/cellSize().width(),
			ev->y()/cellSize().height());
	if (pos != m_mouse_pos) {
		m_mouse_pos = pos;
		mouseClickReset();
		neovimMouseEvent(ev);
	}
}

void Shell::setCursorFromBusyState() noexcept
{
	Qt::CursorShape desiredCursor{};

	if (m_neovimBusy) {
		desiredCursor = Qt::CursorShape::WaitCursor;
	}

	if (cursor().shape() != desiredCursor) {
		setCursor(desiredCursor);
	}
}

void Shell::wheelEvent(QWheelEvent *ev)
{
	if (!m_attached || !m_mouseEnabled) {
		return;
	}

	const QString evString{ GetWheelEventStringAndSetScrollRemainder(
		*ev, m_scrollDeltaRemainder, cellSize()) };

	if (evString.isEmpty()) {
		return;
	}

	m_nvim->api0()->vim_input(evString.toLatin1());
}

/*static*/ QString Shell::GetWheelEventStringAndSetScrollRemainder(
	const QWheelEvent& ev,
	QPoint& scrollRemainderOut,
	QSize cellSize,
	int deltasPerStep) noexcept
{
	int invertConstant{ (ev.inverted()) ? -1 : 1 };

	QPoint scrollRemainderAndEvent { (ev.angleDelta() * invertConstant) + scrollRemainderOut };

	scrollRemainderOut.rx() = scrollRemainderAndEvent.x() % deltasPerStep;
	scrollRemainderOut.ry() = scrollRemainderAndEvent.y() % deltasPerStep;

	const bool isScrollDeltaOverflow{ scrollRemainderAndEvent.x() >= deltasPerStep
		|| scrollRemainderAndEvent.x() <= -deltasPerStep
		|| scrollRemainderAndEvent.y() >= deltasPerStep
		|| scrollRemainderAndEvent.y() <= -deltasPerStep };

	if (!isScrollDeltaOverflow)
	{
		return {};
	}

// TODO Issue#751:  Remove Deprecated code, keep #else below
#if (QT_VERSION < QT_VERSION_CHECK(5, 14, 0))
	const QPoint evPos{ ev.x(), ev.y() };
#else
	const QPoint evPos{ ev.position().toPoint() };
#endif

	QPoint evCellPos{ evPos.x() / cellSize.width(), evPos.y() / cellSize.height() };

	QString wheelEventString;
	if (scrollRemainderAndEvent.y() > 0) {
		wheelEventString += QStringLiteral("<%1ScrollWheelUp><%2,%3>");
	}
	else if (scrollRemainderAndEvent.y() < 0) {
		wheelEventString += QStringLiteral("<%1ScrollWheelDown><%2,%3>");
	}

	if (scrollRemainderAndEvent.x() < 0) {
		wheelEventString += QStringLiteral("<%1ScrollWheelRight><%2,%3>");
	}
	else if (scrollRemainderAndEvent.x() > 0) {
		wheelEventString += QStringLiteral("<%1ScrollWheelLeft><%2,%3>");
	}

	return wheelEventString.arg(Input::GetModifierPrefix(ev.modifiers())).arg(evCellPos.x()).arg(evCellPos.y());
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
		// Issue #329: The <FocusGained> key no longer exists, use autocmd instead.
		m_nvim->api0()->vim_command("if exists('#FocusGained') | doautocmd <nomodeline> FocusGained | endif");
	}
	QWidget::focusInEvent(ev);
}

void Shell::focusOutEvent(QFocusEvent *ev)
{
	if (m_attached) {
		// Issue #591: Option <nomodeline> prevents unwanted interaction, consistent with nvim.
		m_nvim->api0()->vim_command("if exists('#FocusLost') | doautocmd <nomodeline> FocusLost | endif");
	}
	QWidget::focusOutEvent(ev);
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

	m_tooltip->setMinimumWidth(GetHorizontalAdvance(QFontMetrics{ m_tooltip->font() }, text));
	m_tooltip->setMaximumWidth(GetHorizontalAdvance(QFontMetrics{ m_tooltip->font() }, text));
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
	} else if ( query == Qt::ImCursorRectangle ) {
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
