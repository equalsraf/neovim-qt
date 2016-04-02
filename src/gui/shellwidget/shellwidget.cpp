#include <QPainter>
#include <QPaintEvent>
#include <QDebug>
#include "shellwidget.h"
#include "helpers.h"

ShellWidget::ShellWidget(QWidget *parent)
:QWidget(parent), m_contents(0,0), m_bgColor(Qt::white),
	m_fgColor(Qt::black)
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
	w->m_contents.fromFile(path);
	return w;
}

void ShellWidget::setDefaultFont()
{
#if defined(Q_OS_MAC)
	setShellFont("Courier New", 11);
#else
	setShellFont("Monospace", 11);
#endif
}

bool ShellWidget::setShellFont(const QString& family, int ptSize, int weight, bool italic)
{
	QFont f(family, ptSize, weight, italic);
	f.setStyleHint(QFont::TypeWriter, QFont::StyleStrategy(QFont::PreferDefault | QFont::ForceIntegerMetrics));
	f.setFixedPitch(true);
	f.setKerning(false);

	QFontInfo fi(f);
	if (fi.family().compare(f.family(), Qt::CaseInsensitive) != 0 &&
			f.family().compare("Monospace", Qt::CaseInsensitive) != 0) {
		emit fontError(QString("Unknown font: %1").arg(f.family()));
		return false;
	}
	if ( !fi.fixedPitch() ) {
		emit fontError(QString("%1 is not a fixed pitch font").arg(f.family()));
		return false;
	}

	if (isBadMonospace(f)) {
		emit fontError(QString("Warning: Font \"%1\" reports bad fixed pitch metrics").arg(f.family()));
	}

	setCellSize(f);
	setFont(f);
	emit shellFontChanged();
	return true;
}

/// Don't used this, use setShellFont instead;
void ShellWidget::setFont(const QFont& f)
{
	QWidget::setFont(f);
}

/// Changed the cell size based on font metrics:
/// - Height is either the line spacing or the font
///   height, the leading may be negative and we want the
///   larger value
/// - Width is the width of the "W" character
void ShellWidget::setCellSize(const QFont& f)
{
	QFontMetrics fm(f);
	m_ascent = fm.ascent();
	m_cellSize = QSize(fm.width('W'),
			qMax(fm.lineSpacing(), fm.height()));
	setSizeIncrement(m_cellSize);
}
QSize ShellWidget::cellSize() const
{
	return m_cellSize;
}

void ShellWidget::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);
	foreach(QRect rect, ev->region().rects()) {
		int start_row = rect.top() / m_cellSize.height();
		int end_row = rect.bottom() / m_cellSize.height();
		int start_col = rect.left() / m_cellSize.width();
		int end_col = rect.right() / m_cellSize.width();

		// Paint margins
		if (end_col > m_contents.columns()) {
			end_col = m_contents.columns();
		}
		if (end_row > m_contents.rows()) {
			end_col = m_contents.columns();
		}

		// end_col/row is inclusive
		for (int i=start_row; i<=end_row && i < m_contents.rows(); i++) {
			for (int j=start_col; j<=end_col && j < m_contents.columns();
					j++) {

				const Cell& cell = m_contents.constValue(i,j);
				int chars = cell.doubleWidth ? 2 : 1;
				QRect r = absoluteShellRect(i, j, 1, chars);

				if (j <= 0 || !contents().constValue(i, j-1).doubleWidth) {
					// Only paint bg/fg if this is not the second cell
					// of a wide char
					if (cell.backgroundColor.isValid()) {
						p.fillRect(r, cell.backgroundColor);
					} else {
						p.fillRect(r, m_bgColor);
					}

					if (cell.c == ' ') {
						continue;
					}

					if (cell.foregroundColor.isValid()) {
						p.setPen(cell.foregroundColor);
					} else {
						p.setPen(m_fgColor);
					}

					if (cell.bold || cell.italic || cell.underline) {
						QFont f = p.font();
						f.setBold(cell.bold);
						f.setItalic(cell.italic);
						f.setUnderline(cell.underline);
						p.setFont(f);
					} else {
						p.setFont(font());
					}

					// Draw chars at the baseline
					QPoint pos(r.left(), r.top()+m_ascent);
					p.drawText(pos, QString(cell.c));
				}

				// Draw "undercurl" at the bottom of the cell
				if (cell.undercurl) {
					// FIXME: use correct highlight color instead of red
					// TODO: draw a proper undercurl
					p.setPen(QPen(Qt::red, 1, Qt::DashDotDotLine));
					QPoint start = r.bottomLeft();
					QPoint end = r.bottomRight();
					start.ry()--; end.ry()--;
					p.drawLine(start, end);
				}
			}
		}
	}

	QRect shellArea = absoluteShellRect(0, 0,
				m_contents.rows(), m_contents.columns());
	QRegion margins = QRegion(rect()).subtracted(shellArea);
	foreach(QRect margin, margins.intersected(ev->region()).rects()) {
		p.fillRect(margin, m_bgColor);
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

void ShellWidget::setBackground(const QColor& color)
{
	m_bgColor = color;
}

QColor ShellWidget::background() const
{
	return m_bgColor;
}

void ShellWidget::setForeground(const QColor& color)
{
	m_fgColor = color;
}

QColor ShellWidget::foreground() const
{
	return m_fgColor;
}

const ShellContents& ShellWidget::contents() const
{
	return m_contents;
}

/// Put text in position, returns the amount of colums used
int ShellWidget::put(const QString& text, int row, int column,
		QColor fg, QColor bg, bool bold, bool italic,
		bool underline, bool undercurl)
{
	if (!fg.isValid()) {
		fg = m_fgColor;
	}
	if (!bg.isValid()) {
		bg = m_bgColor;
	}
	int cols_changed = m_contents.put(text, row, column, fg, bg,
				bold, italic, underline, undercurl);
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
int ShellWidget::fontSize() const
{
	return font().pointSize();
}

int ShellWidget::rows() const
{
	return m_contents.rows();
}
int ShellWidget::columns() const
{
	return m_contents.columns();
}
