#include <QPainter>
#include <QPaintEvent>
#include <QDebug>
#include "shellwidget.h"
#include "helpers.h"

ShellWidget::ShellWidget(QWidget *parent)
:QWidget(parent), m_contents(0,0), m_bgColor(Qt::white),
	m_fgColor(Qt::black), m_spColor(QColor()), m_lineSpace(0),
	m_ligatureMode(false)
{
	setAttribute(Qt::WA_OpaquePaintEvent);
	setAttribute(Qt::WA_KeyCompression, false);
	setFocusPolicy(Qt::StrongFocus);
	setSizePolicy(QSizePolicy::Expanding,
			QSizePolicy::Expanding);
	setMouseTracking(true);

	setDefaultFont();
}

ShellWidget* ShellWidget::fromFile(const QString& path)
{
	ShellWidget *w = new ShellWidget();
	if (w->m_contents.fromFile(path)) {
		return w;
	} else {
		return NULL;
	}
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

		QChar *rowStr = nullptr;
		if(m_ligatureMode){
			start_col = 0;
			end_col = m_contents.columns() - 1;
			rowStr = new QChar[m_contents.columns()];
		}

		// end_row is inclusive
		for (int i=start_row; i<=end_row && i < m_contents.rows(); i++) {
			for (int j=end_col; j>=start_col && j >= 0;) {
				const Cell& firstCell = m_contents.constValue(i,j);
				const QColor &currentForeground = firstCell.foregroundColor.isValid()?firstCell.foregroundColor:m_fgColor;
				const QColor &currentBackground = firstCell.backgroundColor.isValid()?firstCell.backgroundColor:m_bgColor;
				QColor nextForeground, nextBackground;
				int startIndex = j;
				const Cell* cell = &firstCell;
				do {
					QRect r = absoluteShellRect(i,j,1,cell->doubleWidth?2:1);
					p.setClipRect(r);
					p.fillRect(r,currentBackground);

					// Draw "undercurl" at the bottom of the cell
					if (cell->underline || cell->undercurl) {
						if (cell->undercurl) {
							if (cell->specialColor.isValid()) {
								p.setPen(cell->specialColor);
							} else if (m_spColor.isValid()) {
								p.setPen(m_spColor);
							} else if (cell->foregroundColor.isValid()) {
								p.setPen(cell->foregroundColor);
							} else {
								p.setPen(foreground());
							}
						} else if (cell->underline) {
							if (cell->foregroundColor.isValid()) {
								p.setPen(cell->foregroundColor);
							} else {
								p.setPen(foreground());
							}
						}

						QPoint start = r.bottomLeft();
						QPoint end = r.bottomRight();
						start.ry()--; end.ry()--;
						if (cell->underline) {
							p.drawLine(start, end);
						} else if (cell->undercurl) {
							static const int val[8] = {1, 0, 0, 1, 1, 2, 2, 2};
							QPainterPath path(start);
							for (int i = start.x() + 1; i <= end.x(); i++) {
								int offset = val[i % 8];
								path.lineTo(QPoint(i, start.y() - offset));
							}
							p.drawPath(path);
						}
					}
					if(m_ligatureMode){
						rowStr[j]=cell->c;
					}
					j--;
					cell = &m_contents.constValue(i,j);
					nextForeground = cell->foregroundColor.isValid()?cell->foregroundColor:foreground();
					nextBackground = cell->backgroundColor.isValid()?cell->backgroundColor:background();
				} while ( m_ligatureMode &&
						!(cell->bold!=firstCell.bold || cell->italic!=firstCell.italic ||
						currentForeground != nextForeground ||
						currentBackground != nextBackground ||
						m_contents.constValue(i,j).doubleWidth ||
						m_contents.constValue(i,j).c>0x7f ||
						j == 0));
				QRect r = absoluteShellRect(i, j+1, 1, startIndex-j);
				QRect ovflw = absoluteShellRect(i, j+1, 1, startIndex-j+1);
				p.setClipRect(ovflw);

				p.setPen(currentForeground);
				QFont f = p.font();
				f.setBold(firstCell.bold);
				f.setItalic(firstCell.italic);
				p.setFont(f);
				QPoint pos(r.left(), r.top()+m_ascent+(m_lineSpace / 2));
				if(m_ligatureMode){
					p.drawText(pos, QString(rowStr+j+1,startIndex-j));
				} else {
					p.drawText(pos, QString(m_contents.constValue(i,j+1).c));
				}
			}
		}
		if(rowStr){
			delete[] rowStr;
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
int ShellWidget::put(const QString& text, int row, int column,
		QColor fg, QColor bg, QColor sp, bool bold, bool italic,
		bool underline, bool undercurl)
{
	int cols_changed = m_contents.put(text, row, column, fg, bg, sp,
				bold, italic, underline, undercurl);
	if (cols_changed > 0) {
	int rectWidth = m_ligatureMode?m_contents.columns():cols_changed;
	int rectStart = m_ligatureMode?0:column;
		QRect rect = absoluteShellRect(row, rectStart, 1, rectWidth);
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

void ShellWidget::setLigatureMode(bool ligatureMode)
{
	m_ligatureMode = ligatureMode;
	update();
}

bool ShellWidget::ligatureMode() const
{
	return m_ligatureMode;
}
