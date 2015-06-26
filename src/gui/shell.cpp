#include "shell.h"

#include <QPainter>
#include <QPaintEvent>
#include <QDebug>
#include <QDesktopWidget>
#include <QApplication>
#include <QKeyEvent>
#include "input.h"
#include "konsole_wcwidth.h"

namespace NeovimQt {

Shell::Shell(NeovimConnector *nvim, QWidget *parent)
:QWidget(parent), m_attached(false), m_nvim(nvim), m_rows(1), m_cols(1),
	m_font_bold(false), m_font_italic(false), m_font_underline(false), m_fm(NULL),
	m_foreground(Qt::black), m_background(Qt::white),
	m_hg_foreground(Qt::black), m_hg_background(Qt::white),
	m_cursor_color(Qt::white), m_cursor_pos(0,0), m_insertMode(false),
	m_resizing(false), m_logo(QPixmap(":/neovim.png")),
	m_neovimBusy(false)
{
	QFont f;
	f.setStyleStrategy(QFont::StyleStrategy(QFont::PreferDefault | QFont::ForceIntegerMetrics) );
	f.setStyleHint(QFont::TypeWriter);
	f.setFamily("DejaVu Sans Mono");
	f.setFixedPitch(true);
	f.setPointSize(11);
	f.setKerning(false);
	f.setFixedPitch(true);

	m_font = f;
	m_fm = new QFontMetrics(m_font);

	m_image = QImage(neovimSize(), QImage::Format_ARGB32_Premultiplied);

	setAttribute(Qt::WA_KeyCompression, false);
	setAttribute(Qt::WA_OpaquePaintEvent, true);

	setFocusPolicy(Qt::StrongFocus);

	// IM Tooltip
	setAttribute(Qt::WA_InputMethodEnabled, true);
	m_tooltip = new QLabel(this);
	m_tooltip->setVisible(false);
	m_tooltip->setTextFormat(Qt::PlainText);
	m_tooltip->setTextInteractionFlags(Qt::NoTextInteraction);
	m_tooltip->setAutoFillBackground(true);

	if (m_nvim == NULL) {
		qWarning() << "Received NULL as Neovim Connector";
		return;
	}

	connect(m_nvim, &NeovimConnector::ready,
			this, &Shell::neovimIsReady);
	connect(m_nvim, &NeovimConnector::error,
			this, &Shell::neovimError);
	connect(m_nvim, &NeovimConnector::processExited,
			this, &Shell::neovimExited);

	if (m_nvim->isReady()) {
		neovimIsReady();
	}
}

Shell::~Shell()
{
	if (m_nvim && m_attached) {
		m_nvim->detachUi();
	}
}

void Shell::setAttached(bool attached)
{
	setAttribute(Qt::WA_StaticContents, attached);
	m_attached = attached;
	update();
}

/** Neovim shell width in pixels (does not include extra margin) */
int Shell::neovimWidth() const
{
	return m_cols*neovimCellWidth();
}

/** Neovim shell height in pixels (does not include extra margin) */
int Shell::neovimHeight() const
{
	return m_rows*neovimRowHeight();
}

/** Height of a row (in pixels)*/
quint64 Shell::neovimRowHeight() const
{
	// The leading may be negative making the linespacing
	// smaller than height
	return qMax(m_fm->lineSpacing(), m_fm->height());
}

/** Width of a char (in pixels)*/
quint64 Shell::neovimCellWidth() const
{
	return m_fm->width('W');
}

/** Pixel size for a char cell */
QSize Shell::neovimCharSize() const
{
	return QSize(neovimCellWidth(), neovimRowHeight());
}

QSize Shell::sizeIncrement() const
{
	return neovimCharSize();
}

QSize Shell::sizeHint() const
{
	if (m_attached) {
		return neovimSize();
	} else {
		return QWidget::sizeHint();
	}
}

/** Pixel size of the Neovim shell */
QSize Shell::neovimSize() const
{
	return QSize(neovimWidth(), neovimHeight());
}

/** The top left corner position (pixel) for the cursor */
QPoint Shell::neovimCursorTopLeft() const
{
	return QPoint(m_cursor_pos.x()*neovimCellWidth(), m_cursor_pos.y()*neovimRowHeight());
}

void Shell::neovimIsReady()
{
	if (!m_nvim || !m_nvim->neovimObject()) {
		return;
	}
	// FIXME: Don't set this here, wait for return from ui_attach instead
	setAttached(true);

	connect(m_nvim->neovimObject(), &Neovim::neovimNotification,
			this, &Shell::handleNeovimNotification);
	// FIXME: this API will change
	QRect screenRect = QApplication::desktop()->availableGeometry(this);
	m_nvim->attachUi(screenRect.width()*0.66/neovimCellWidth(), screenRect.height()*0.66/neovimRowHeight());

	connect(m_nvim->neovimObject(), &Neovim::on_ui_try_resize,
			this, &Shell::neovimResizeFinished);
}

void Shell::neovimError(NeovimConnector::NeovimError err)
{
	if (m_attached) {
		setAttached(false);
		update();
	}
}

/** The Neovim process has exited */
void Shell::neovimExited(int status)
{
	setAttached(false);
	if (status == 0 && m_nvim->errorCause() == NeovimConnector::NoError) {
		close();
	}
}

/**
 * Neovim requested a resize
 *
 * - update cols/rows
 * - reset the cursor, scroll_region
 */
void Shell::handleResize(uint64_t cols, uint64_t rows)
{
	// TODO: figure out how to handle cases when Neovim wants one
	// size but the user is resizing to another
	bool needs_resize = (rows != m_rows || cols != m_cols);

	m_rows = rows;
	m_cols = cols;
	m_cursor_pos = QPoint(0,0);
	m_scroll_region = QRect(QPoint(0,0), neovimSize());

	if (needs_resize) {
		QImage new_image = QImage(neovimSize(), QImage::Format_ARGB32_Premultiplied);
		{
			// copy the old contents into the new canvas
			QPainter painter(&new_image);
			painter.drawImage(QPoint(0,0), m_image);
		}
		m_image.swap(new_image);
		updateGeometry();
		emit neovimResized(neovimSize());
	}
}

void Shell::handleHighlightSet(const QVariantMap& attrs, QPainter& painter)
{
	if (attrs.contains("foreground")) {
		// TODO: When does Neovim send -1
		m_hg_foreground = color(attrs.value("foreground").toLongLong(), m_foreground);
	} else {
		m_hg_foreground = m_foreground;
	}

	if (attrs.contains("background")) {
		m_hg_background = color(attrs.value("background").toLongLong(), m_background);
	} else {
		m_hg_background = m_background;
	}

	// TODO: undercurl
	m_font_bold = attrs.value("bold").toBool();
	m_font_italic = attrs.value("italic").toBool();
	m_font_underline = attrs.value("undercurl").toBool();
	setupPainter(painter);
}

/**
 * Paint a character and advance the cursor by one
 */
void Shell::handlePut(const QVariantList& args, QPainter& painter)
{
	if (args.size() != 1 || (QMetaType::Type)args.at(0).type() != QMetaType::QByteArray) {
		qWarning() << "Unexpected arguments for redraw:put" << args;
		return;
	}

	QString text = m_nvim->decode(args.at(0).toByteArray());
	QRect updateRect(neovimCursorTopLeft(),
			QSize(neovimCellWidth(), neovimRowHeight()));

	if (!text.isEmpty()) {
		painter.save();

		const QChar& c = text.at(0);
		// fullwidth chars take up two columns
		int charWidth = konsole_wcwidth(c.unicode());
		QRect clipRect(neovimCursorTopLeft(),
				QSize(neovimCellWidth()*charWidth, neovimRowHeight()));
		painter.setClipRect(clipRect);

		// Draw text at the baseline
		QPoint pos(m_cursor_pos.x()*neovimCellWidth(), m_cursor_pos.y()*neovimRowHeight()+m_fm->ascent());
		painter.drawText(pos, text.at(0));

		painter.restore();
	}

	update(updateRect);
	// Move cursor ahead
	m_cursor_pos.setX(m_cursor_pos.x() + 1);
	update(QRect(neovimCursorTopLeft(), neovimCharSize()));
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
void Shell::handleScroll(const QVariantList& args, QPainter& painter)
{
	if (!args.at(0).canConvert<qint64>()) {
		qWarning() << "Unexpected arguments for redraw:scroll" << args;
		return;
	}

	qint64 count = args.at(0).toULongLong();
	QRect exposed;	// Area exposed after the scroll, that needs repainting
	QRect rect;	// Area to be moved
	QPoint pos;	// Position where the image will be drawn
	if (count == 0) {
		return;
	} else if (count > 0) {
		// Scroll lines to the top
		exposed = QRect(QPoint(m_scroll_region.left(), m_scroll_region.bottom()-count*neovimRowHeight()+1),
				QSize(m_scroll_region.width(), count*neovimRowHeight()));
		rect = QRect(QPoint(m_scroll_region.left(), m_scroll_region.top()+count*neovimRowHeight()),
				QPoint(m_scroll_region.right(), m_scroll_region.bottom()));
		pos = m_scroll_region.topLeft();
	} else {
		count = -count;
		// Scroll lines to the bottom
		exposed = QRect(m_scroll_region.topLeft(),
				QSize(m_scroll_region.width(), count*neovimRowHeight()));
		rect = QRect(m_scroll_region.topLeft(),
				QPoint(m_scroll_region.right(), m_scroll_region.bottom()-count*neovimRowHeight()));
		pos = m_scroll_region.topLeft();
		pos.setY(pos.y()+count*neovimRowHeight());
	}

	QImage copy = m_image.copy(rect);
	painter.drawImage(pos, copy);
	// Scroll always uses the background color, not the highlight
	painter.fillRect(exposed, m_background);
	update(m_scroll_region);
}

/** Ready a painter with Neovim settings */
void Shell::setupPainter(QPainter& painter)
{
	painter.setPen(m_hg_foreground);
	painter.setBackground(m_hg_background);
	QFont f(m_font);
	f.setBold(m_font_bold);
	f.setItalic(m_font_italic);
	f.setUnderline(m_font_underline);
	painter.setFont(f);
}

void Shell::handleSetScrollRegion(const QVariantList& opargs)
{
	if (opargs.size() != 4) {
		qWarning() << "Unexpected arguments for redraw:set_scroll_region" << opargs;
		return;
	}

	qint64 top, bot, left, right;
	top = opargs.at(0).toULongLong();
	bot = opargs.at(1).toULongLong();
	left = opargs.at(2).toULongLong();
	right = opargs.at(3).toULongLong();

	m_scroll_region = QRect(QPoint(left*neovimCellWidth(), top*neovimRowHeight()),
				QPoint((right+1)*neovimCellWidth(), (bot+1)*neovimRowHeight()-1));
}

void Shell::handleRedraw(const QByteArray& name, const QVariantList& opargs, QPainter& painter)
{
	if (name == "update_fg") {
		if (opargs.size() != 1 || !opargs.at(0).canConvert<quint64>()) {
			qWarning() << "Unexpected arguments for redraw:" << name << opargs;
			return;
		}
		m_foreground = color(opargs.at(0).toLongLong(), m_foreground);
		m_hg_foreground = m_foreground;
		painter.setPen(m_hg_foreground);
	} else if (name == "update_bg") {
		if (opargs.size() != 1 || !opargs.at(0).canConvert<quint64>()) {
			qWarning() << "Unexpected arguments for redraw:" << name << opargs;
			return;
		}
		m_background = color(opargs.at(0).toLongLong(), m_background);
		m_hg_background = m_background;
		painter.setBackground(m_hg_background);
		update();
	} else if (name == "resize") {
		if (opargs.size() != 2 || !opargs.at(0).canConvert<quint64>() ||
				!opargs.at(1).canConvert<quint64>()) {
			qWarning() << "Unexpected arguments for redraw:" << name << opargs;
			return;
		}

		painter.end();
		handleResize(opargs.at(0).toULongLong(), opargs.at(1).toULongLong());
		painter.begin(&m_image);
		setupPainter(painter);
	} else if (name == "clear") {
		painter.fillRect(rect(), m_background);
		update();
	} else if (name == "bell"){
		QApplication::beep();
	} else if (name == "eol_clear") {
		QPoint tl = neovimCursorTopLeft();
		QPoint br(neovimWidth()-1, tl.y()+neovimRowHeight()-1);
		QRect clearRect = QRect(tl, br);
		painter.fillRect(clearRect, m_background);
		update(clearRect);
	} else if (name == "cursor_goto"){
		if (opargs.size() != 2 || !opargs.at(0).canConvert<quint64>() ||
				!opargs.at(1).canConvert<quint64>()) {
			qWarning() << "Unexpected arguments for redraw:" << name << opargs;
			return;
		}
		QRect cursorRect(neovimCursorTopLeft(), neovimCharSize());
		setNeovimCursor(opargs.at(0).toULongLong(), opargs.at(1).toULongLong());
	} else if (name == "highlight_set") {
		if (opargs.size() != 1 && (QMetaType::Type)opargs.at(0).type() != QMetaType::QVariantMap) {
			qWarning() << "Unexpected argument for redraw:" << name << opargs;
			return;
		}
		handleHighlightSet(opargs.at(0).toMap(), painter);
	} else if (name == "put") {
		handlePut(opargs, painter);
	} else if (name == "scroll"){
		handleScroll(opargs, painter);
	} else if (name == "set_scroll_region"){
		handleSetScrollRegion(opargs);
	} else if (name == "mouse_on"){
		this->unsetCursor();
	} else if (name == "mouse_off"){
		this->setCursor(Qt::BlankCursor);
	} else if (name == "normal_mode"){
		handleNormalMode(painter);
	} else if (name == "insert_mode"){
		handleInsertMode(painter);
	} else if (name == "cursor_on"){
	} else if (name == "set_title"){
		handleSetTitle(opargs);
	} else if (name == "cursor_off"){
	} else if (name == "busy_start"){
		handleBusy(true);
	} else if (name == "busy_stop"){
		handleBusy(false);
	} else {
		qDebug() << "Received unknown redraw notification" << name << opargs;
	}

}

void Shell::setNeovimCursor(quint64 row, quint64 col)
{
	update(QRect(neovimCursorTopLeft(), neovimCharSize()));
	m_cursor_pos = QPoint(col, row);
	update(QRect(neovimCursorTopLeft(), neovimCharSize()));
}

void Shell::handleNormalMode(QPainter& painter)
{
	m_insertMode = false;
}

void Shell::handleInsertMode(QPainter& painter)
{
	m_insertMode = true;
}

void Shell::handleSetTitle(const QVariantList& opargs)
{
	if (opargs.size() != 1 || !opargs.at(0).canConvert<QByteArray>()) {
		qWarning() << "Unexpected arguments for set_title:" << opargs;
		return;
	}
	QString title = m_nvim->decode(opargs.at(0).toByteArray());
	emit neovimTitleChanged(title);
}

void Shell::handleBusy(bool busy)
{
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
	if (name != "redraw") {
		return;
	}

	QPainter painter(&m_image);
	setupPainter(painter);

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

		if (name == "put") {
			// A redraw:put does three things
			// 1. Paints the cell background
			// 2. Draws a char
			// 3. Advance the cursor by one
			//
			// We draw the background here and leave 2/3 to handlePut
			quint64 cells = update_args.size();
			QRect bgRect(neovimCursorTopLeft(),
					QSize(cells*neovimCellWidth(), neovimRowHeight())
				);
			painter.eraseRect(bgRect);
		}

		foreach (const QVariant& opargs_var, update_args) {
			if ((QMetaType::Type)opargs_var.type() != QMetaType::QVariantList) {
				qWarning() << "Received unexpected redraw arguments, expecting list" << opargs_var;
				continue;
			}

			const QVariantList& opargs = opargs_var.toList();
			handleRedraw(name, opargs, painter);
		}
	}
#if 0
	// Dump all paint events as jpg files for debugging
	static quint64 count = 0;
	qDebug() << "Redraw:" << count;
	m_image.save(QString("debug-paint-%1.jpg").arg(count++));
#endif

}

/**
 * Draws the Neovim logo at the center of the widget.
 * If the is too small do nothing.
 */
void Shell::paintLogo(QPainter& p)
{
	if (size().width() > m_logo.width() &&
		size().height() > m_logo.height() ) {
		int x = size().width()/2 - m_logo.width()/2;
		int y = size().height()/2 - m_logo.height()/2;
		p.drawPixmap(QPoint(x,y), m_logo);
	}
}

void Shell::paintEvent(QPaintEvent *ev)
{
	QPainter painter(this);
	if (!m_attached) {
		painter.fillRect(rect(), Qt::white);
		paintLogo(painter);
		return;
	}

	QRegion imageReg(QRect(QPoint(0,0),neovimSize()));
	QRegion intersection = imageReg.intersected(ev->region());
	QRegion diff = ev->region().subtracted(imageReg);

	foreach(QRect rect, intersection.rects()) {
		painter.drawImage(rect, m_image, rect);
	}

	// Paint margins
	foreach(QRect rect, diff.rects()) {
		painter.fillRect( rect, m_background);
	}

	// paint cursor - we are not actually using Neovim colors yet,
	// just invert the shell colors by painting white with XoR
	if (ev->region().contains(neovimCursorTopLeft())) {
		QRect cursorRect(neovimCursorTopLeft(), neovimCharSize());

		if (m_insertMode) {
			cursorRect.setWidth(2);
		}
		painter.setCompositionMode(QPainter::RasterOp_SourceXorDestination);
		painter.fillRect(cursorRect, m_cursor_color);
	}
}

void Shell::keyPressEvent(QKeyEvent *ev)
{
	if (!m_nvim || !m_attached) {
		QWidget::keyPressEvent(ev);
		return;
	}

	// FIXME mousehide - conceal mouse pointer when typing

	QString inp = Input.convertKey(ev->text(), ev->key(), ev->modifiers());
	if (inp.isEmpty()) {
		QWidget::keyPressEvent(ev);
		return;
	}

	m_nvim->neovimObject()->vim_input(m_nvim->encode(inp));
	// FIXME: bytes might not be written, and need to be buffered
}

void Shell::resizeNeovim(const QSize& newSize)
{
	uint64_t cols = newSize.width()/neovimCellWidth();
	uint64_t rows = newSize.height()/neovimRowHeight();

	// Neovim will ignore simultaneous calls to ui_try_resize
	if (!m_resizing && m_nvim && m_attached &&
			(cols != m_cols || rows != m_rows) ) {
		m_nvim->neovimObject()->ui_try_resize(cols, rows);
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
}

void Shell::changeEvent( QEvent *ev)
{
//	if (ev->type() == QEvent::WindowStateChange && isWindow()) {
//		if ( windowState() & Qt::WindowFullScreen ) {
//			// TODO: implement fullscreen support - center QImage in widget
//			// update();
//		} else {
//		}
//	}
	QWidget::changeEvent(ev);
}

void Shell::closeEvent(QCloseEvent *ev)
{
	if (m_attached) {
		ev->ignore();
		m_nvim->neovimObject()->vim_command("qa");
	} else {
		QWidget::closeEvent(ev);
	}
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
		m_tooltip->setMinimumHeight(neovimRowHeight());
		m_tooltip->move(neovimCursorTopLeft() );
		m_tooltip->show();
	}

	m_tooltip->setMinimumWidth( QFontMetrics(m_tooltip->font()).width(text) );
	m_tooltip->setMaximumWidth( QFontMetrics(m_tooltip->font()).width(text) );
	m_tooltip->update();
}

void Shell::inputMethodEvent(QInputMethodEvent *ev)
{
	if ( !ev->commitString().isEmpty() ) {
		QByteArray s = m_nvim->encode(ev->commitString());
		m_nvim->neovimObject()->vim_input(s);
		tooltip("");
	} else {
		tooltip(ev->preeditString());
	}
}

QVariant Shell::inputMethodQuery(Qt::InputMethodQuery query) const
{
	if ( query == Qt::ImFont) {
		return font();
	} else if ( query == Qt::ImMicroFocus ) {
		return QRect(neovimCursorTopLeft(), QSize(0, neovimRowHeight()));
	}

	return QVariant();
}

bool Shell::neovimBusy() const
{
	return m_neovimBusy;
}

} // Namespace
