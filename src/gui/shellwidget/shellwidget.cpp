#include <QPainter>
#include <QPainterPath>
#include <QPaintEvent>
#include <QDebug>
#include "shellwidget.h"
#include "helpers.h"

ShellWidget::ShellWidget(QWidget* parent)
	: QWidget(parent)
{
	setAttribute(Qt::WA_OpaquePaintEvent);
	setAttribute(Qt::WA_KeyCompression, false);
	setFocusPolicy(Qt::StrongFocus);
	setSizePolicy(QSizePolicy::Expanding,
			QSizePolicy::Expanding);
	setMouseTracking(true);

	setDefaultFont();

	// Blinking Cursor Timer
	connect(&m_cursor, &Cursor::CursorChanged, this, &ShellWidget::handleCursorChanged);
}

ShellWidget* ShellWidget::fromFile(const QString& path)
{
	ShellWidget *w = new ShellWidget();
	w->m_contents.fromFile(path);
	return w;
}

void ShellWidget::setDefaultFont()
{
#if defined(Q_OS_MAC)
#  define DEFAULT_FONT "Courier New"
#elif defined(Q_OS_WIN)
#  define DEFAULT_FONT "Consolas"
#else
#  define DEFAULT_FONT "Monospace"
#endif
	setShellFont(DEFAULT_FONT, 11, -1, false, true);
}

bool ShellWidget::setShellFont(const QString& family, qreal ptSize, int weight, bool italic, bool force)
{
	QFont f(family, -1, weight, italic);
	// Issue #575: Clear style name. The KDE/Plasma theme plugin may set this
	// but we want to match the family name with the bold/italic attributes.
	f.setStyleName(QStringLiteral(""));

	f.setPointSizeF(ptSize);
	f.setStyleHint(QFont::TypeWriter, QFont::StyleStrategy(QFont::PreferDefault | QFont::ForceIntegerMetrics));
	f.setFixedPitch(true);
	f.setKerning(false);

	// Issue #585 Error message "Unknown font:" for Neovim 0.4.2+.
	// This case has always been hit, but results in user visible error messages for recent
	// releases. It is safe to ignore this case, which occurs at startup time.
	if (f.family().isEmpty()) {
		return false;
	}

	QFontInfo fi(f);
	if (fi.family().compare(f.family(), Qt::CaseInsensitive) != 0 &&
			f.family().compare("Monospace", Qt::CaseInsensitive) != 0) {
		emit fontError(QString("Unknown font: %1").arg(f.family()));
		return false;
	}

	if (!force) {
		if (!fi.fixedPitch()) {
			emit fontError(QString("%1 is not a fixed pitch font").arg(f.family()));
			return false;
		}

		if (isBadMonospace(f)) {
			emit fontError(QString("Warning: Font \"%1\" reports bad fixed pitch metrics").arg(f.family()));
		}
	}

	setFont(f);
	setCellSize();
	emit shellFontChanged();
	return true;
}

/// Don't used this, use setShellFont instead;
void ShellWidget::setFont(const QFont& f)
{
	QWidget::setFont(f);
}

void ShellWidget::setLineSpace(int height)
{
	if (height != m_lineSpace) {
		m_lineSpace = height;
		setCellSize();
		emit shellFontChanged();
	}
}

/// Changed the cell size based on font metrics:
/// - Height is either the line spacing or the font
///   height, the leading may be negative and we want the
///   larger value
/// - Width is the width of the "W" character
void ShellWidget::setCellSize()
{
	QFontMetrics fm(font());
	m_ascent = fm.ascent();
	m_cellSize = QSize(fm.width('W'),
			qMax(fm.lineSpacing(), fm.height()) + m_lineSpace);
	setSizeIncrement(m_cellSize);
}
QSize ShellWidget::cellSize() const
{
	return m_cellSize;
}

QRect ShellWidget::getNeovimCursorRect(QRect cellRect) noexcept
{
	QRect cursorRect{ cellRect };
	switch (m_cursor.GetShape())
	{
		case Cursor::Shape::Block:
			break;

		case Cursor::Shape::Horizontal:
		{
			const int height{ cursorRect.height() * m_cursor.GetPercentage() / 100 };
			const int verticalOffset{ cursorRect.height() - height };
			cursorRect.adjust(0, verticalOffset, 0, verticalOffset);
			cursorRect.setHeight(height);
		}
		break;

		case Cursor::Shape::Vertical:
		{
			cursorRect.setWidth(cursorRect.width() * m_cursor.GetPercentage() / 100);
		}
		break;
	}

	return cursorRect;
}

void ShellWidget::paintNeovimCursorBackground(QPainter& p, QRect cellRect) noexcept
{
	const QRect cursorRect{ getNeovimCursorRect(cellRect) };

	QColor cursorBackgroundColor{ m_cursor.GetBackgroundColor() };
	if (!cursorBackgroundColor.isValid()) {
		// Neovim can send QColor::Invalid, indicating the default colors with
		// an inverted foreground/background.
		cursorBackgroundColor = foreground();
	}

	// If the window does not have focus, draw an outline around the cursor cell.
	if (!hasFocus()) {
		QRect noFocusCursorRect{ cellRect };
		noFocusCursorRect.adjust(-1, -1, -1, -1);

		QPen pen{ cursorBackgroundColor };
		pen.setWidth(2);

		p.setPen(pen);
		p.drawRect(cellRect);
		return;
	}

	p.fillRect(cursorRect, cursorBackgroundColor);
}

void ShellWidget::paintNeovimCursorForeground(
	QPainter& p,
	QRect cellRect,
	QPoint pos,
	QChar character) noexcept
{
	// No focus: cursor is outline with default foreground color.
	if (!hasFocus()) {
		return;
	}

	const QRect cursorRect{ getNeovimCursorRect(cellRect) };

	QColor cursorForegroundColor{ m_cursor.GetForegroundColor() };
	if (!cursorForegroundColor.isValid()) {
		// Neovim can send QColor::Invalid, indicating the default colors with
		// an inverted foreground/background.
		cursorForegroundColor = background();
	}

	// Painting the cursor requires setting the clipping region. This is used
	// to paint only the part of text within the cursor region. Save the active
	// clipping settings, and restore them after the paint operation.
	const QRegion oldClippingRegion{ p.clipRegion() };
	const bool oldClippingSetting{ p.hasClipping() };

	p.setClipping(true);
	p.setClipRect(cursorRect) ;

	p.setPen(cursorForegroundColor);
	p.drawText(pos, character);

	p.setClipRegion(oldClippingRegion);
	p.setClipping(oldClippingSetting);
}

void ShellWidget::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	p.setClipping(true);

	foreach(QRect rect, ev->region().rects()) {
		int start_row = rect.top() / m_cellSize.height();
		int end_row = rect.bottom() / m_cellSize.height();
		int start_col = rect.left() / m_cellSize.width();
		int end_col = rect.right() / m_cellSize.width();

		// Paint margins
		if (end_col >= m_contents.columns()) {
			end_col = m_contents.columns() - 1;
		}
		if (end_row >= m_contents.rows()) {
			end_row = m_contents.rows() - 1;
		}

		// end_col/row is inclusive
		for (int i=start_row; i<=end_row; i++) {
			for (int j=end_col; j>=start_col; j--) {

				const Cell& cell = m_contents.constValue(i,j);
				int chars = cell.IsDoubleWidth() ? 2 : 1;
				QRect r = absoluteShellRect(i, j, 1, chars);
				QRect ovflw = absoluteShellRect(i, j, 1, chars + 1);

				p.setClipRegion(ovflw);

				if (j <= 0 || !contents().constValue(i, j-1).IsDoubleWidth()) {
					// Only paint bg/fg if this is not the second cell
					// of a wide char
					QColor bgColor{ cell.GetBackgroundColor() };
					if (!bgColor.isValid()) {
						bgColor = (cell.IsReverse()) ? foreground() : background();
					}
					p.fillRect(r, bgColor);

					const QPoint curPos{ j, i };
					const bool isCursorVisibleAtCell{ m_cursor.IsVisible() && m_cursor_pos == curPos };

					if (isCursorVisibleAtCell) {
						paintNeovimCursorBackground(p, r);
					}

					if (cell.GetCharacter() != ' ') {
						QColor fgColor{ cell.GetForegroundColor() };
						if (!fgColor.isValid()) {
							fgColor = (cell.IsReverse()) ? background() : foreground();
						}
						p.setPen(fgColor);

						if (cell.IsBold() || cell.IsItalic()) {
							QFont f = p.font();
							f.setBold(cell.IsBold());
							f.setItalic(cell.IsItalic());
							p.setFont(f);
						} else {
							p.setFont(font());
						}

						// Draw chars at the baseline
						const int cellTextOffset{ m_ascent + (m_lineSpace / 2) };
						const QPoint pos{ r.left(), r.top() + cellTextOffset};
						const uint character{ cell.GetCharacter() };

						p.drawText(pos, QString::fromUcs4(&character, 1));

						if (isCursorVisibleAtCell) {
							paintNeovimCursorForeground(p, r, pos, character);
						}
					}
				}

				// Draw "undercurl" at the bottom of the cell
				if (cell.IsUnderline()|| cell.IsUndercurl()) {
					QPen pen = QPen();
					if (cell.IsUndercurl()) {
						if (cell.GetSpecialColor().isValid()) {
							pen.setColor(cell.GetSpecialColor());
						} else if (special().isValid()) {
							pen.setColor(special());
						} else if (cell.GetForegroundColor().isValid()) {
							pen.setColor(cell.GetForegroundColor());
						} else {
							pen.setColor(foreground());
						}
					} else if (cell.IsUnderline()) {
						if (cell.GetForegroundColor().isValid()) {
							pen.setColor(cell.GetForegroundColor());
						} else {
							pen.setColor(foreground());
						}
					}

					p.setPen(pen);
					QPoint start = r.bottomLeft();
					QPoint end = r.bottomRight();
					start.ry()--; end.ry()--;
					if (cell.IsUnderline()) {
						p.drawLine(start, end);
					} else if (cell.IsUndercurl()) {
						static const int val[8] = {1, 0, 0, 1, 1, 2, 2, 2};
						QPainterPath path(start);
						for (int i = start.x() + 1; i <= end.x(); i++) {
							int offset = val[i % 8];
							path.lineTo(QPoint(i, start.y() - offset));
						}
						p.drawPath(path);
					}
				}
			}
		}
	}

	p.setClipping(false);

	QRect shellArea = absoluteShellRect(0, 0,
				m_contents.rows(), m_contents.columns());
	QRegion margins = QRegion(rect()).subtracted(shellArea);
	foreach(QRect margin, margins.intersected(ev->region()).rects()) {
		p.fillRect(margin, background());
	}

#if 0
	// Draw DEBUG rulers
	for (int i=m_cellSize.width(); i<width(); i+=m_cellSize.width()) {
		p.setPen(QPen(Qt::red, 1,  Qt::DashLine));
		p.drawLine(i, 0, i, height());
	}
	for (int i=m_cellSize.height(); i<width(); i+=m_cellSize.height()) {
		p.setPen(QPen(Qt::red, 1,  Qt::DashLine));
		p.drawLine(0, i, width(), i);
	}
#endif
}

void ShellWidget::resizeEvent(QResizeEvent *ev)
{
	int cols = ev->size().width() / m_cellSize.width();
	int rows = ev->size().height() / m_cellSize.height();
	resizeShell(rows, cols);
	QWidget::resizeEvent(ev);
}

QSize ShellWidget::sizeHint() const
{
	return QSize(m_cellSize.width()*m_contents.columns(),
				m_cellSize.height()*m_contents.rows());
}

void ShellWidget::resizeShell(int n_rows, int n_columns)
{
	if (n_rows != rows() || n_columns != columns()) {
		m_contents.resize(n_rows, n_columns);
		updateGeometry();
	}
}

void ShellWidget::setSpecial(const QColor& color)
{
	m_spColor = color;
}

QColor ShellWidget::special() const
{
	return m_spColor;
}

void ShellWidget::setBackground(const QColor& color)
{
	m_bgColor = color;
}

QColor ShellWidget::background() const
{
	// See 'default_colors_set' in Neovim ':help ui-linegrid'.
	// QColor::Invalid indicates the default color (-1), which should be
	// rendered as white or black based on Neovim 'background'.
	if (!m_bgColor.isValid())
	{
		switch (m_background)
		{
			case Background::Light:
				return Qt::white;

			case Background::Dark:
				return Qt::black;

			default:
				return Qt::black;
		}
	}

	return m_bgColor;
}

void ShellWidget::setForeground(const QColor& color)
{
	m_fgColor = color;
}

QColor ShellWidget::foreground() const
{
	// See ShellWidget::background() for more details.
	if (!m_bgColor.isValid())
	{
		switch (m_background)
		{
			case Background::Light:
				return Qt::black;

			case Background::Dark:
				return Qt::white;

			default:
				return Qt::white;
		}
	}

	return m_fgColor;
}

const ShellContents& ShellWidget::contents() const
{
	return m_contents;
}

/// Put text in position, returns the amount of colums used
int ShellWidget::put(
	const QString& text,
	int row,
	int column,
	QColor fg,
	QColor bg,
	QColor sp,
	bool bold,
	bool italic,
	bool underline,
	bool undercurl,
	bool reverse)
{
	HighlightAttribute hl_attr = { fg, bg, sp,
		reverse, italic, bold, underline, undercurl };
	return put(text, row, column, hl_attr);
}

int ShellWidget::put(
	const QString& text,
	int row,
	int column,
	const HighlightAttribute& hl_attr)
{
	int cols_changed = m_contents.put(text, row, column, hl_attr);
	if (cols_changed > 0) {
		QRect rect = absoluteShellRect(row, column, 1, cols_changed);
		update(rect);
	}
	return cols_changed;
}

void ShellWidget::clearRow(int row)
{
	m_contents.clearRow(row);
	QRect rect = absoluteShellRect(row, 0, 1, m_contents.columns());
	update(rect);
}
void ShellWidget::clearShell(QColor bg)
{
	m_contents.clearAll(bg);
	update();
}

/// Clear region (row0, col0) to - but not including (row1, col1)
void ShellWidget::clearRegion(int row0, int col0, int row1, int col1)
{
	m_contents.clearRegion(row0, col0, row1, col1);
	// FIXME: check offset error
	update(absoluteShellRect(row0, col0, row1-row0, col1-col0));
}

/// Scroll count rows (positive numbers move content up)
void ShellWidget::scrollShell(int rows)
{
	if (rows != 0) {
		m_contents.scroll(rows);
		// Qt's delta uses positive numbers to move down
		scroll(0, -rows*m_cellSize.height());
	}
}
/// Scroll an area, count rows (positive numbers move content up)
void ShellWidget::scrollShellRegion(int row0, int row1, int col0,
			int col1, int rows)
{
	if (rows != 0) {
		m_contents.scrollRegion(row0, row1, col0, col1, rows);
		// Qt's delta uses positive numbers to move down
		QRect r = absoluteShellRect(row0, col0, row1-row0, col1-col0);
		scroll(0, -rows*m_cellSize.height(), r);
	}
}

/// Convert Area in row/col coordinates into pixel coordinates
///
/// (row0, col0) is the start position and rowcount/colcount the size
QRect ShellWidget::absoluteShellRect(int row0, int col0, int rowcount, int colcount)
{
	return QRect(col0*m_cellSize.width(), row0*m_cellSize.height(),
			colcount*m_cellSize.width(), rowcount*m_cellSize.height());
}

QString ShellWidget::fontFamily() const
{
	return QFontInfo(font()).family();
}
qreal ShellWidget::fontSize() const
{
	return font().pointSizeF();
}

int ShellWidget::rows() const
{
	return m_contents.rows();
}

int ShellWidget::columns() const
{
	return m_contents.columns();
}

void ShellWidget::setNeovimCursor(uint64_t row, uint64_t col) noexcept
{
	// Clear the stale cursor
	update(neovimCursorRect());

	// Update cursor position, draw at new location
	m_cursor_pos = QPoint(col, row);
	m_cursor.ResetTimer();
	update(neovimCursorRect());
}

/// The top left corner position (pixel) for the cursor
QPoint ShellWidget::neovimCursorTopLeft() const noexcept
{
	const QSize cSize{ cellSize() };
	const int xPixels{ m_cursor_pos.x() * cSize.width() };
	const int yPixels{ m_cursor_pos.y() * cSize.height() };

	return { xPixels, yPixels };
}

/// Get the area filled by the cursor
QRect ShellWidget::neovimCursorRect() const noexcept
{
	QRect cursor{ neovimCursorTopLeft(), cellSize() };

	const Cell& cell{ contents().constValue(m_cursor_pos.y(), m_cursor_pos.x()) };
	if (cell.IsDoubleWidth()) {
		cursor.setWidth(cursor.width() * 2);
	}

	return cursor;
}

void ShellWidget::handleCursorChanged()
{
	update(neovimCursorRect());
}


