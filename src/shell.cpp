#include "shell.h"

#include <QPainter>
#include <QPaintEvent>
#include <QDebug>
#include <QDesktopWidget>
#include <QApplication>
#include <QKeyEvent>

namespace NeovimQt {

Shell::Shell(NeovimConnector *nvim, QWidget *parent)
:QWidget(parent), m_attached(false), m_nvim(nvim), m_rows(1), m_cols(1), m_fm(NULL),
	m_foreground(Qt::black), m_background(Qt::white),
	m_hg_foreground(Qt::black), m_hg_background(Qt::white),
	m_cursor_color(Qt::white), m_cursor_pos(0,0), m_cursor(false)
{
	QFont f;
	f.setStyleStrategy(QFont::StyleStrategy(QFont::PreferDefault | QFont::ForceIntegerMetrics) );
	f.setStyleHint(QFont::TypeWriter);
	f.setFamily("Monospace");
	f.setFixedPitch(true);
	f.setPointSize(10);
	f.setKerning(false);
	f.setFixedPitch(true);

	m_font = f;
	m_fm = new QFontMetrics(m_font);

	m_image = QImage(neovimSize(), QImage::Format_ARGB32_Premultiplied);

	setAttribute(Qt::WA_OpaquePaintEvent, true);
	setAttribute(Qt::WA_StaticContents, true);

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
}

Shell::~Shell()
{
	if (m_nvim && m_attached) {
		m_nvim->detachUi();
	}
}

/** Neovim shell width in pixels (does not include extra margin) */
quint64 Shell::neovimWidth() const
{
	return m_cols*neovimCellWidth();
}

/** Neovim shell height in pixels (does not include extra margin) */
quint64 Shell::neovimHeight() const
{
	return m_rows*neovimRowHeight();
}

/** Height of a row (in pixels)*/
quint64 Shell::neovimRowHeight() const
{
	return m_fm->lineSpacing();
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
QPoint Shell::neovimCursorTopLeft()
{
	return QPoint(m_cursor_pos.x()*neovimCellWidth(), m_cursor_pos.y()*neovimRowHeight());
}

void Shell::neovimIsReady()
{
	if (!m_nvim || !m_nvim->neovimObject()) {
		return;
	}
	// FIXME: Don't set this here, wait for return from ui_attach instead
	m_attached = true;

	connect(m_nvim->neovimObject(), &Neovim::neovimNotification,
			this, &Shell::handleNeovimNotification);
	// FIXME: this API will change
	QRect screenRect = QApplication::desktop()->availableGeometry(this);
	m_nvim->attachUi(screenRect.width()/neovimCellWidth(), screenRect.height()/neovimRowHeight());

	// FIXME: connect to neovimObject()->on_ui_try_resize

	m_nvim->neovimObject()->vim_command("redraw!");
}

void Shell::neovimError(NeovimConnector::NeovimError err)
{
	if (m_attached) {
		m_attached = false;
		update();
	}
}

/** The Neovim process has exited */
void Shell::neovimExited(int status)
{
	m_attached = false;
	close();
}

/**
 * Neovim was resized
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
		adjustSize();
	}
}

void Shell::handleHighlightSet(const QVariantMap& attrs, QPainter& painter)
{
	if (attrs.contains("foreground")) {
		m_hg_foreground = QColor::fromRgb(QRgb(attrs.value("foreground").toULongLong()));
		painter.setPen(m_hg_foreground);
	}
	if (attrs.contains("background")) {
		m_hg_background = QColor::fromRgb(QRgb(attrs.value("background").toULongLong()));
		painter.setBackground(m_hg_background);
	}

	QFont f = painter.font();
	f.setBold(attrs.value("bold").toBool());
	f.setItalic(attrs.value("italic").toBool());
	painter.setFont(f);
}

void Shell::handlePut(const QVariantList& args, QPainter& painter)
{
	if (args.size() != 1 || (QMetaType::Type)args.at(0).type() != QMetaType::QByteArray) {
		qWarning() << "Unexpected arguments for redraw:put" << args;
		return;
	}

	painter.save();
	QString text = m_nvim->decode(args.at(0).toByteArray());

	QRect updateRect(neovimCursorTopLeft(),
			QSize(text.size()*neovimCellWidth(), neovimRowHeight())
		);

	// We don't trust fonts to fit inside the cell - clip them
	painter.setClipRect(updateRect);
	painter.eraseRect(updateRect);

	// Draw text at the baseline
	QPoint pos(m_cursor_pos.x()*neovimCellWidth(), m_cursor_pos.y()*neovimRowHeight()+m_fm->ascent());
	painter.drawText(pos, text);
	update(updateRect);

	// Move cursor ahead
	m_cursor_pos.setX(m_cursor_pos.x() + text.size());
	update(QRect(neovimCursorTopLeft(), neovimCharSize()));

	painter.restore();
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
	painter.eraseRect(exposed);
	update(m_scroll_region);
}

/** Ready a painter with Neovim settings */
void Shell::setupPainter(QPainter& painter)
{
	painter.setFont(m_font);
	painter.setPen(m_hg_foreground);
	painter.setBackground(m_hg_background);
	painter.setFont(m_font);
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
		m_foreground = QRgb(opargs.at(0).toULongLong());
		m_hg_foreground = m_foreground;
		painter.setPen(m_hg_foreground);
	} else if (name == "update_bg") {
		if (opargs.size() != 1 || !opargs.at(0).canConvert<quint64>()) {
			qWarning() << "Unexpected arguments for redraw:" << name << opargs;
			return;
		}
		m_background = QRgb(opargs.at(0).toULongLong());
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
		if (opargs.size() != 2 || !opargs.at(0).canConvert<quint64>(),
				!opargs.at(1).canConvert<quint64>()) {
			qWarning() << "Unexpected arguments for redraw:" << name << opargs;
			return;
		}
		QRect cursorRect(neovimCursorTopLeft(), neovimCharSize());
		setCursor(opargs.at(0).toULongLong(), opargs.at(1).toULongLong());
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
	} else if (name == "cursor_on"){
		handleCursorOn(painter);
	} else if (name == "cursor_off"){
		handleCursorOff(painter);
	} else if (name == "mouse_on"){
		QApplication::restoreOverrideCursor();
	} else if (name == "mouse_off"){
		QApplication::setOverrideCursor(Qt::BlankCursor);
	} else if (name == "normal_mode"){
		handleNormalMode(painter);
	} else if (name == "insert_mode"){
		handleInsertMode(painter);
	} else {
		qDebug() << "Received unknown redraw notification" << name << opargs;
	}

}

void Shell::handleCursorOn(QPainter& painter)
{
	// FIXME: what about wide chars?
	m_cursor = true;
	update(QRect(neovimCursorTopLeft(), neovimCharSize()));
}

void Shell::handleCursorOff(QPainter& painter)
{
	// FIXME: what about wide chars?
	m_cursor = false;
	update(QRect(neovimCursorTopLeft(), neovimCharSize()));
}

void Shell::setCursor(quint64 row, quint64 col)
{
	if (m_cursor) {
		update(QRect(neovimCursorTopLeft(), neovimCharSize()));
	}
	m_cursor_pos = QPoint(col, row);
	if (m_cursor) {
		update(QRect(neovimCursorTopLeft(), neovimCharSize()));
	}
}

void Shell::handleNormalMode(QPainter& painter)
{
}

void Shell::handleInsertMode(QPainter& painter)
{
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

//		if (name == "put") {
//			// TODO: merge put() calls into a single call if possible
//			// continue - requires "proper" Monospace fonts
//			QByteArray data;
//			foreach (const QVariant& opargs_var, update_args) {
//				data += opargs_var.toList().at(0).toByteArray();
//			}
//		}

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

void Shell::paintEvent(QPaintEvent *ev)
{
	QPainter painter(this);
	if (!m_attached) {
		painter.eraseRect(rect());
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
	if (m_cursor && ev->region().contains(neovimCursorTopLeft())) {
		QRect cursorRect(neovimCursorTopLeft(), neovimCharSize());
		painter.setCompositionMode(QPainter::RasterOp_SourceXorDestination);
		painter.fillRect(cursorRect, m_cursor_color);
	}
}

void Shell::keyPressEvent(QKeyEvent *ev)
{
	if (!m_nvim || !m_attached) {
		return;
	}
	// TODO: need to figure out how special keys and keyboard modifiers
	// work for Neovim - for now simple keys only

	// FIXME mousehide - conceal mouse pointer when typing
	// FIXME: handle special keys and modifiers
	if (!ev->text().size()) {
		return;
	}

	m_nvim->neovimObject()->vim_input(m_nvim->encode(ev->text()));
	// FIXME: bytes might not be written, and need to be buffered
}

void Shell::resizeEvent(QResizeEvent *ev)
{
	// Call Neovim to resize
	uint64_t cols = ev->size().width()/neovimCellWidth();
	uint64_t rows = ev->size().height()/neovimRowHeight();
	if (m_nvim && m_attached &&
			(cols != m_cols || rows != m_rows) ) {
		qDebug() << "Calling neovim to resize" << ev->size() << cols << rows << m_cols << m_rows;
		// FIXME: Neovim will ignore simultaneous calls to resize - e.g. queue calls
		m_nvim->tryResizeUi(cols, rows);
	}
	QWidget::resizeEvent(ev);
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

} // Namespace
