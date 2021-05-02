#include "shellwidget.h"

#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QPaintEvent>
#include <QTextLayout>

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
	static const QFont font{ getDefaultFontFamily(), 11 /*pointSize*/, -1 /*weight*/, false /*italic*/ };
	setShellFont(font, true /*force*/);
}

/*static*/ QString ShellWidget::getDefaultFontFamily() noexcept
{
#if defined(Q_OS_MAC)
	return QStringLiteral("Courier New");
#elif defined(Q_OS_WIN)
	return QStringLiteral("Consolas");
#else
	return QStringLiteral("Monospace");
#endif
}

bool ShellWidget::setShellFont(const QFont& font, bool force) noexcept
{
	// Issue #585 Error message "Unknown font:" for Neovim 0.4.2+.
	// This case has always been hit, but results in user visible error messages for recent
	// releases. It is safe to ignore this case, which occurs at startup time.
	if (font.family().isEmpty()) {
		return false;
	}

	QFontInfo fi(font);
	if (fi.family().compare(font.family(), Qt::CaseInsensitive) != 0 &&
			font.family().compare("Monospace", Qt::CaseInsensitive) != 0) {
		emit fontError(QString("Unknown font: %1").arg(font.family()));
		return false;
	}

	if (!force) {
		if (!fi.fixedPitch()) {
			emit fontError(QString("%1 is not a fixed pitch font").arg(font.family()));
			return false;
		}

		if (isBadMonospace(font)) {
			emit fontError(QString("Warning: Font \"%1\" reports bad fixed pitch metrics").arg(font.family()));
		}
	}

	setFont(font);
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
	m_cellSize = QSize(GetHorizontalAdvance(fm, 'W'),
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
			// Issue #800: for vertical bar cursor, use single cell width
			// instead of the provided cell rect. Cell rect may have doubled
			// width, for example, when the cell contains a double width char.
			// Always using single cell width prevents vertical bar to be a bit
			// thicker on a double width char.
			cursorRect.setWidth(m_cellSize.width() * m_cursor.GetPercentage() / 100);
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
	const QString& character) noexcept
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

static QLine GetUnderline(QRect cellRect) noexcept
{
	QPoint start{ cellRect.bottomLeft() };
	start.ry()--;

	QPoint end{ cellRect.bottomRight() };
	end.ry()--;

	return { start, end };
}

void ShellWidget::paintUnderline(
	QPainter& p,
	const Cell& cell,
	QRect cellRect) noexcept
{
	if (!cell.IsUnderline()) {
		return;
	}

	p.setPen(getForegroundPen(cell));

	p.drawLine(GetUnderline(cellRect));
}

static QPainterPath GetUndercurlPath(QRect cellRect) noexcept
{
	const QLine underline{ GetUnderline(cellRect) };
	const QPoint& start{ underline.p1() };
	const QPoint& end{ underline.p2() };

	static constexpr int offset[8]{ 1, 0, 0, 1, 1, 2, 2, 2 };

	// Be careful to set correct offset for the starting point of the undercurl
	// so that it doesn't make discontinuity at the cell boundary.
	//
	// Simply using `start` always set the y offset of starting point to zero,
	// but it is incorrect in most cases; the ending point of the undercurl on
	// the previous cell has non-zero offset except
	// offset[previous_undercurl_ending_x % 8] == 0.
	//
	// To make undercurl smoothly connected, we need to set correct offset to
	// the starting point according to the x coordinate of it.
	//
	// See also Pull Request #803 (comment):
	// https://github.com/equalsraf/neovim-qt/pull/803#issuecomment-751166085
	QPoint undercurlStart{ start.x(), start.y() - offset[start.x() % 8] };

	QPainterPath path{ undercurlStart };
	for (int i = start.x() + 1; i <= end.x(); i++) {
		path.lineTo(QPoint(i, start.y() - offset[i%8]));
	}

	return path;
}

void ShellWidget::paintUndercurl(
	QPainter& p,
	const Cell& cell,
	QRect cellRect) noexcept
{
	if (!cell.IsUndercurl()) {
		return;
	}

	QPen pen;
	if (cell.GetSpecialColor().isValid()) {
		pen.setColor(cell.GetSpecialColor());
	} else if (special().isValid()) {
		pen.setColor(special());
	} else if (cell.GetForegroundColor().isValid()) {
		pen.setColor(cell.GetForegroundColor());
	} else {
		pen.setColor(foreground());
	}

	p.setPen(pen);

	p.drawPath(GetUndercurlPath(cellRect));
}

static QLine GetStrikeThrough(QRect cellRect) noexcept
{
	QPoint start{ cellRect.bottomLeft() };
	start.ry() -= (cellRect.bottom() - cellRect.top()) / 2 + 1;

	QPoint end{ cellRect.bottomRight() };
	end.ry() -= (cellRect.bottom() - cellRect.top()) / 2 + 1;

	return { start, end };
}

void ShellWidget::paintStrikeThrough(
	QPainter& p,
	const Cell& cell,
	QRect cellRect) noexcept
{
	if (!cell.IsStrikeThrough()) {
		return;
	}

	p.setPen(getForegroundPen(cell));

	p.drawLine(GetStrikeThrough(cellRect));
}

void ShellWidget::paintBackgroundClearCell(
	QPainter& p,
	const Cell& cell,
	QRect cellRect,
	bool isCursorCell) noexcept
{
	QColor bgColor{ cell.GetBackgroundColor() };
	if (!bgColor.isValid()) {
		bgColor = (cell.IsReverse()) ? foreground() : background();
	}

	p.fillRect(cellRect, bgColor);

	if (isCursorCell) {
		paintNeovimCursorBackground(p, cellRect);
		return;
	}
}

QFont ShellWidget::GetCellFont(const Cell& cell) const noexcept
{
	QFont cellFont{ font() };

	if (cell.IsDoubleWidth()) {

		auto isCharacterInWideFont = [&](const QFont& wideFont) noexcept
		{
			return QFontMetrics(wideFont).inFontUcs4(cell.GetCharacter());
		};

		auto wideFont = std::find_if(m_guifontwidelist.begin(), m_guifontwidelist.end(), isCharacterInWideFont);

		if (wideFont != m_guifontwidelist.end())
		{
			cellFont = *wideFont;
		}
	}

	if (cell.IsBold() || cell.IsItalic()) {
		cellFont.setBold(cell.IsBold());
		cellFont.setItalic(cell.IsItalic());
	}

	// Issue #575: Clear style name. The KDE/Plasma theme plugin may set this
	// but we want to match the family name with the bold/italic attributes.
	cellFont.setStyleName({});

	cellFont.setStyleHint(QFont::TypeWriter, QFont::StyleStrategy(QFont::PreferDefault | QFont::PreferMatch));
	cellFont.setFixedPitch(true);
	cellFont.setKerning(false);

	return cellFont;
}

QPen ShellWidget::getForegroundPen(const Cell& cell) noexcept
{
	QPen pen;
	if (cell.GetForegroundColor().isValid()) {
		pen.setColor(cell.GetForegroundColor());
	} else {
		pen.setColor(foreground());
	}

	return pen;
}

void ShellWidget::paintForegroundCellText(
	QPainter& p,
	const Cell& cell,
	QRect cellRect,
	bool isCursorCell) noexcept
{
	if (cell.GetCharacter() == ' ') {
		return;
	}

	QColor fgColor{ cell.GetForegroundColor() };
	if (!fgColor.isValid()) {
		fgColor = (cell.IsReverse()) ? background() : foreground();
	}
	p.setPen(fgColor);
	p.setFont(GetCellFont(cell));

	// Draw chars at the baseline
	const int cellTextOffset{ m_ascent + (m_lineSpace / 2) };
	const QPoint pos{ cellRect.left(), cellRect.top() + cellTextOffset};
	const uint character{ cell.GetCharacter() };
	const QString text{ QString::fromUcs4(&character, 1) };

	p.drawText(pos, text);

	if (isCursorCell) {
		paintNeovimCursorForeground(p, cellRect, pos, text);
	}
}

static bool AreGlyphPositionsUniform(
	const QVector<QPointF>& glyphPositionList,
	int cellWidth) noexcept
{
	if (glyphPositionList.size() <= 1) {
		return true;
	}

	qreal lastPos{ glyphPositionList[0].x() };
	for (int i=1; i<glyphPositionList.size(); i++) {
		const qreal delta{ lastPos - glyphPositionList[i].x() };
		if (delta != cellWidth) {
			return false;
		}
	}

	return true;
}

static QVector<QPointF> DistributeGlyphPositions(
	QVector<QPointF>&& glyphPositionList,
	int cellWidth) noexcept
{
	if (glyphPositionList.size() > 1) {
		qreal adjustPositionX{ glyphPositionList[0].x() };
		for (auto& glyphPos : glyphPositionList) {
			glyphPos.setX(adjustPositionX);
			adjustPositionX += cellWidth;
		}
	}

	return std::move(glyphPositionList);
}

static QVector<uint32_t> RemoveLigaturesUnderCursor(
	const QGlyphRun& glyphRun,
	const QString& textGlyphRun,
	int cursorGlyphRunPos) noexcept
{
	auto glyphIndexList{ glyphRun.glyphIndexes() };
	auto glyphIndexListNoLigatures{ glyphRun.rawFont().glyphIndexesForString(textGlyphRun) };

	if (cursorGlyphRunPos < 0
		|| cursorGlyphRunPos >= glyphIndexList.size()
		|| cursorGlyphRunPos >= glyphIndexListNoLigatures.size()) {
		qDebug() << "ERROR: Invalid cursorGlyphRunPos!";
		return {};
	}

	if (glyphIndexList.at(cursorGlyphRunPos) != glyphIndexListNoLigatures.at(cursorGlyphRunPos)) {
		for(int i=cursorGlyphRunPos; i>=0; i--) {
			if (glyphIndexList.at(i) == glyphIndexListNoLigatures.at(i)) {
				break;
			}

			glyphIndexList.data()[i] = glyphIndexListNoLigatures[i];
		}

		for(int i=cursorGlyphRunPos+1; i<glyphIndexList.size(); i++) {
			if (glyphIndexList.at(i) == glyphIndexListNoLigatures.at(i)) {
				break;
			}

			glyphIndexList.data()[i] = glyphIndexListNoLigatures[i];
		}

		return glyphIndexList;
	}

	// No glyph changes required
	return {};
}

void ShellWidget::paintForegroundTextBlock(
	QPainter& p,
	const Cell& cell,
	QRect blockRect,
	const QString& text,
	int cursorPos) noexcept
{
	QColor fgColor{ cell.GetForegroundColor() };
	if (!fgColor.isValid()) {
		fgColor = (cell.IsReverse()) ? background() : foreground();
	}

	const QFont blockFont{ GetCellFont(cell) };

	p.setPen(fgColor);
	p.setFont(blockFont);

	const int cellTextOffset{ m_lineSpace / 2 };
	const QPoint pos{ blockRect.left(), blockRect.top() + cellTextOffset };

	QTextLayout textLayout{ text, blockFont, p.device() };
	textLayout.setCacheEnabled(true);
	textLayout.beginLayout();
	QTextLine line = textLayout.createLine();
	if (!line.isValid()) {
		return;
	}
	line.setNumColumns(text.length());
	textLayout.endLayout();

	int glyphsRendered{ 0 };
	for (auto& glyphRun : textLayout.glyphRuns()) {
		auto glyphPositionList{ glyphRun.positions() };
		qsizetype sizeGlyphRun{ glyphPositionList.size() };

		const int cellWidth{ (cell.IsDoubleWidth()) ?
			m_cellSize.width() * 2 : m_cellSize.width() };

		// When characters are rendered as a string, they may not be uniformly
		// distributed. Check for even spacing and redistribute as necessary.
		if (!AreGlyphPositionsUniform(glyphPositionList, cellWidth)) {
			glyphRun.setPositions(
				DistributeGlyphPositions(std::move(glyphPositionList), cellWidth));
		}

		const bool isCursorVisibleInGlyphRun{ m_cursor.IsVisible()
			&& cursorPos >= 0
			&& cursorPos < sizeGlyphRun + glyphsRendered
			&& cursorPos >= glyphsRendered };

		// When the cursor is NOT within the glyph run, render as-is.
		if (!isCursorVisibleInGlyphRun) {
			p.drawGlyphRun(pos, glyphRun);
			glyphsRendered += sizeGlyphRun;
			continue;
		}

		// When the cursor IS within the glyph run, decompose individual characters under the cursor.
		const int cursorGlyphRunPos { cursorPos - glyphsRendered };
		const QString textGlyphRun{ QStringView{text}.mid(glyphsRendered, sizeGlyphRun).toString() };

		// Compares a glyph run with and without ligatures. Ligature glyphs are detected as differences
		// in these two lists. A non-empty newCursorGlyphList indicates glyph substitution is required.
		const auto newCursorGlyphList { RemoveLigaturesUnderCursor(glyphRun, textGlyphRun, cursorGlyphRunPos) };
		if (!newCursorGlyphList.empty()) {
			glyphRun.setGlyphIndexes(newCursorGlyphList);
		}

		p.drawGlyphRun(pos, glyphRun);
		glyphsRendered += sizeGlyphRun;

		const QRect cursorCellRect{ neovimCursorRect() };
		paintNeovimCursorBackground(p, cursorCellRect);
		paintNeovimCursorForeground(
			p, cursorCellRect, glyphRun.positions().at(cursorGlyphRunPos).toPoint() + pos,
			textGlyphRun.at(cursorGlyphRunPos));
	}
}

void ShellWidget::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	p.setClipping(true);

	for (auto rect{ ev->region().cbegin() }; rect != ev->region().cend(); rect++) {
		if (isLigatureModeEnabled()) {
			paintRectLigatures(p, *rect);
		}
		else {
			paintRectNoLigatures(p, *rect);
		}
	}

	p.setClipping(false);

	const QRect shellArea{ absoluteShellRect(0, 0, m_contents.rows(), m_contents.columns()) };
	const QRegion margins{ QRegion(rect()).subtracted(shellArea) };
	const QRegion marginsIntersected{ margins.intersected(ev->region()) };
	for (auto margin{ marginsIntersected.cbegin() }; margin != marginsIntersected.cend(); margin++) {
		p.fillRect(*margin, background());
	}

	// Uncomment to draw Debug rulers
	//for (int i=m_cellSize.width(); i<width(); i+=m_cellSize.width()) {
	//	p.setPen(QPen(Qt::red, 1,  Qt::DashLine));
	//	p.drawLine(i, 0, i, height());
	//}
	//for (int i=m_cellSize.height(); i<width(); i+=m_cellSize.height()) {
	//	p.setPen(QPen(Qt::red, 1,  Qt::DashLine));
	//	p.drawLine(0, i, width(), i);
	//}
}

void ShellWidget::paintRectNoLigatures(QPainter& p, const QRect rect) noexcept
{
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

			// Only paint bg/fg if this is not the second cell of a wide char
			if (j <= 0 || !contents().constValue(i, j-1).IsDoubleWidth()) {

				const QPoint curPos{ j, i };
				const bool isCursorVisibleAtCell{ m_cursor.IsVisible() && m_cursor_pos == curPos };

				paintBackgroundClearCell(p, cell, r, isCursorVisibleAtCell);
				paintForegroundCellText(p, cell, r, isCursorVisibleAtCell);
			}

			paintUnderline(p, cell, r);
			paintUndercurl(p, cell, r);
			paintStrikeThrough(p, cell, r);
		}
	}
}

void ShellWidget::paintRectLigatures(QPainter& p, const QRect rect) noexcept
{
	int start_row = rect.top() / m_cellSize.height();
	int end_row = rect.bottom() / m_cellSize.height();
	int start_col = 0;
	int end_col = m_contents.columns() - 1;

	// Paint margins
	if (end_col >= m_contents.columns()) {
		end_col = m_contents.columns() - 1;
	}
	if (end_row >= m_contents.rows()) {
		end_row = m_contents.rows() - 1;
	}

	// end_col/row is inclusive
	for (int i=start_row; i<=end_row; i++) {
		for (int j=start_col; j<=end_col; j++) {

			const Cell& firstCell{ m_contents.constValue(i,j) };
			const int chars{ firstCell.IsDoubleWidth() ? 2 : 1 };
			const QRect firstCellRect{ absoluteShellRect(i, j, 1, chars) };

			QString blockText;
			int blockCursorPos{ -1 };

			while (j <= end_col)
			{
				const Cell& checkCell{ m_contents.constValue(i, j) };

				if (!firstCell.IsStyleEquivalent(checkCell)) {
					j--;
					break;
				}

				const QPoint checkPos{ j, i };
				if (m_cursor_pos == checkPos) {
					blockCursorPos = blockText.size();
				}

				const uint cellCharacter{ checkCell.GetCharacter() };
				blockText += QString::fromUcs4(&cellCharacter, 1);

				if (checkCell.IsDoubleWidth()) {
					j++;
				}

				j++;
			}

			const Cell& lastCell{ m_contents.constValue(i,j) };
			int lastCellChars = lastCell.IsDoubleWidth() ? 2 : 1;
			QRect lastCellRect = absoluteShellRect(i, j, 1, lastCellChars);

			QRect blockRect{ firstCellRect.topLeft(), lastCellRect.bottomRight() };

			paintBackgroundClearCell(p, firstCell, blockRect, false);
			paintForegroundTextBlock(p, firstCell, blockRect, blockText, blockCursorPos);
			paintUnderline(p, firstCell, blockRect);
			paintUndercurl(p, firstCell, blockRect);
			paintStrikeThrough(p, firstCell, blockRect);
		}
	}
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
	bool strikethrough,
	bool reverse)
{
	HighlightAttribute hl_attr = { fg, bg, sp,
		reverse, italic, bold, underline, undercurl, strikethrough };
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
		if (isLigatureModeEnabled()) {
			update(absoluteShellRectRow(row));
		}
		else {
			QRect rect = absoluteShellRect(row, column, 1, cols_changed);
			update(rect);
		}
	}
	return cols_changed;
}

void ShellWidget::clearRow(int row)
{
	m_contents.clearRow(row);
	QRect rect = absoluteShellRectRow(row);
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

QRect ShellWidget::absoluteShellRect(int row0, int col0, int rowcount, int colcount) const noexcept
{
	return QRect(col0*m_cellSize.width(), row0*m_cellSize.height(),
			colcount*m_cellSize.width(), rowcount*m_cellSize.height());
}

QRect ShellWidget::absoluteShellRectRow(int row) const noexcept
{
	return absoluteShellRect(row, 0, 1, m_contents.columns());
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
	if (isLigatureModeEnabled()) {
		uint64_t oldCursorRow{ static_cast<uint64_t>(m_cursor_pos.y()) };

		// Ligature mode only requires clear during cursor row changes.
		if (row != oldCursorRow) {
			update(absoluteShellRectRow(oldCursorRow));
		}
	}
	else {
		update(neovimCursorRect());
	}

	// Update cursor position
	m_cursor_pos = QPoint(col, row);
	m_cursor.ResetTimer();

	// Draw cursor at new location
	update((isLigatureModeEnabled()) ?
		absoluteShellRectRow(row) : neovimCursorRect());
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

QString ShellWidget::fontDesc() const noexcept
{
	return fontDesc(font());
}

/*static*/ QString ShellWidget::fontDesc(const QFont& font) noexcept
{
	QString fdesc{ QStringLiteral("%1:h%2").arg(font.family()).arg(font.pointSize()) };

	switch (font.weight())
	{
		case QFont::Light:
			fdesc += ":l";
			break;

		case QFont::Normal:
			break;

		case QFont::DemiBold:
			fdesc += ":sb";
			break;

		case QFont::Bold:
			fdesc += ":b";
			break;

		default:
			fdesc += ":w" + QString::number(font.weight());
			break;
	}

	if (font.italic()) {
		fdesc += ":i";
	}

	return fdesc;
}

QVariant ShellWidget::TryGetQFontFromDescription(const QString& fdesc) const noexcept
{
	return TryGetQFontFromDescription(fdesc, font());
}

/*static*/ QVariant ShellWidget::TryGetQFontFromDescription(const QString& fdesc, const QFont& curFont) noexcept
{
	QStringList attrs = fdesc.split(':');
	if (attrs.size() < 1) {
		return QStringLiteral("Invalid font");
	}

	qreal pointSizeF = curFont.pointSizeF();
	int weight = -1;
	bool italic = false;
	for (const auto& attr : attrs) {
		if (attr.size() >= 2 && attr[0] == 'h') {
			bool ok{ false };
			// TODO: the additional toString() call is needed to be compatible with
			// both Qt5/6. Remove when the minimum Qt version is 5.15 or above
			qreal height = QStringView{attr}.mid(1).toString().toFloat(&ok);
			if (!ok || height < 0) {
				return QStringLiteral("Invalid font height");
			}
			pointSizeF = height;
		} else if (attr == "b") {
			weight = QFont::Bold;
		} else if (attr == "l") {
			weight = QFont::Light;
		} else if (attr == "sb") {
			weight = QFont::DemiBold;
		} else if (attr.length() > 0 && attr.at(0) == 'w') {
			// TODO: the additional toString() call is needed to be compatible
			// both Qt5/6. Remove when the minimum Qt version is 5.15 or above
			weight = (QStringView{attr}.right(attr.length()-1)).toString().toInt();
			if (weight < 0 || weight > 99) {
				return QStringLiteral("Invalid font weight");
			}
		} else if (attr == "i") {
			italic = true;
		}
	}

	QFont font{ attrs.at(0), -1 /*pointSize*/, weight, italic };

	font.setPointSizeF(pointSizeF);
	font.setStyleHint(QFont::TypeWriter, QFont::StyleStrategy(QFont::PreferDefault | QFont::PreferMatch));
	font.setFixedPitch(true);
	font.setKerning(false);

	return font;
}

/*static*/ bool ShellWidget::IsValidFont(const QVariant& variant) noexcept
{
	return static_cast<QMetaType::Type>(variant.type()) == QMetaType::QFont;
}

/*static*/ bool ShellWidget::isBadMonospace(const QFont& f) noexcept
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
			fm_italic.maxWidth()*2 != GetHorizontalAdvance(fm_italic, "MM") ) {
		QFontInfo info(fi);
		qDebug() << fi.family() << "Average and Maximum font width mismatch for Italic font; QFont::exactMatch() is" << fi.exactMatch()
			<< "Real font is " << info.family() << info.pointSize();
		return true;
	}

	// Bold
	if ( fm_bold.averageCharWidth() != fm_bold.maxWidth() ||
			fm_bold.maxWidth()*2 != GetHorizontalAdvance(fm_bold, "MM") ) {
		QFontInfo info(fb);
		qDebug() << fb.family() << "Average and Maximum font width mismatch for Bold font; QFont::exactMatch() is" << fb.exactMatch()
			<< "Real font is " << info.family() << info.pointSize();
		return true;
	}

	// Bold+Italic
	if ( fm_boldit.averageCharWidth() != fm_boldit.maxWidth() ||
			fm_boldit.maxWidth()*2 != GetHorizontalAdvance(fm_boldit, "MM") ) {
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

void ShellWidget::setLigatureMode(bool isEnabled) noexcept
{
	m_isLigatureModeEnabled = isEnabled;
	update();
}
