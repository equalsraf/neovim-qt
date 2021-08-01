#pragma once

#include <QWidget>

#include "shellcontents.h"
#include "cursor.h"

class ShellWidget: public QWidget
{
	Q_OBJECT
	Q_PROPERTY(QColor background READ background WRITE setBackground NOTIFY backgroundChanged)
	Q_PROPERTY(QColor foreground READ foreground WRITE setForeground NOTIFY foregroundChanged)
	Q_PROPERTY(int rows READ rows NOTIFY rowsChanged)
	Q_PROPERTY(int columns READ columns NOTIFY columnsChanged)
	Q_PROPERTY(QSize cellSize READ cellSize NOTIFY cellSizeChanged)
	Q_PROPERTY(bool ligatureMode MEMBER m_isLigatureModeEnabled READ isLigatureModeEnabled WRITE setLigatureMode NOTIFY ligatureModeChanged)

public:
	ShellWidget(QWidget *parent=0);

	/// Represents Neovim 'background' option. Used for rendering default theme colors (-1).
	/// See 'default_colors_set' in Neovim ':help ui-linegrid' for more details.
	enum class Background : int
	{
		Dark,
		Light
	};

	bool setShellFont(const QFont& font, bool force = false) noexcept;

	QColor background() const;
	QColor foreground() const;
	QColor special() const;

	static ShellWidget* fromFile(const QString& path);

	int rows() const;
	int columns() const;
	QSize cellSize() const;
	const ShellContents& contents() const;
	QSize sizeHint() const Q_DECL_OVERRIDE;

	Background getBackgroundType() const { return m_background; }

	void setBackgroundType(Background type) { m_background = type; }

	/// Get a Neovim font description of `font()`.
	QString fontDesc() const noexcept;

	/// Get a Neovim font description from QFont.
	static QString fontDesc(const QFont& font) noexcept;

	/// Converts a Neovim font description to QFont
	///
	/// Returns a QFont for valid descriptions.
	/// If an invalid description is provided, returns a QString error message.
	QVariant TryGetQFontFromDescription(const QString& fdesc) const noexcept;

	/// Manually provided a `font()` for TryGetQFontFromDescription above.
	static QVariant TryGetQFontFromDescription(const QString& fdesc, const QFont& font) noexcept;

	/// Helper to check the result of TryGetQFontFromDescription
	static bool IsValidFont(const QVariant& variant) noexcept;

	/// Check if a font can be safely used as a fixed pitch font
	///
	/// This function is not perfect and some broken fonts may still return false,
	/// or font substitution may cause good fonts to fail. The font max/average
	/// metrics are compared with the italic/bold double width variants.
	static bool isBadMonospace(const QFont& f) noexcept;

	/// Font ligatures do not render by default, since each character is an individual
	/// entity on the Neovim grid. When ligatureMode is enabled, segments of text on
	/// a given line with the same style are rendered together. This allows the display
	/// of font ligatures such as those found in FiraCode.
	void setLigatureMode(bool ligatureMode) noexcept;

	/// Ligature rendering is off-by-default.
	bool isLigatureModeEnabled() const noexcept
	{
		return m_isLigatureModeEnabled;
	}

signals:
	void shellFontChanged();
	void fontError(const QString& msg);
	void backgroundChanged(QColor bgColor);
	void foregroundChanged(QColor fgColor);
	void rowsChanged(int rows);
	void columnsChanged(int cols);
	void cellSizeChanged(QSize size);
	void ligatureModeChanged(bool isEnabled);

public slots:
	void resizeShell(int rows, int columns);
	void setSpecial(const QColor& color);
	void setBackground(const QColor& color);
	void setForeground(const QColor& color);
	void setDefaultFont();

	/// Returns the platform-specific default font
	static QString getDefaultFontFamily() noexcept;

	int put(
		const QString& text,
		int row,
		int column,
		QColor fg = {},
		QColor bg = {},
		QColor sp = {},
		bool bold = false,
		bool italic = false,
		bool underline = false,
		bool undercurl = false,
		bool strikethrough = false,
		bool reverse = false);

	int put(
		const QString& text,
		int row,
		int column,
		const HighlightAttribute& hl_attr);

	void clearRow(int row);
	void clearShell(QColor bg = QColor::Invalid);
	void clearRegion(int row0, int col0, int row1, int col1);
	void scrollShell(int rows);
	void scrollShellRegion(int row0, int row1, int col0,
			int col1, int rows);
	void setLineSpace(int height);
protected:
	/// Cursor position in shell coordinates
	QPoint m_cursor_pos;

	/// Abstraction for guicursor options and styles
	Cursor m_cursor;

	/// The top left corner position (pixel) for the cursor
	QPoint neovimCursorTopLeft() const noexcept;

	/// Get the area filled by the cursor
	QRect neovimCursorRect() const noexcept;

	std::vector<QFont> m_guifontwidelist;

	/// Move the neovim cursor for text insertion and display
	void setNeovimCursor(uint64_t col, uint64_t row) noexcept;

	virtual void paintEvent(QPaintEvent *ev) Q_DECL_OVERRIDE;

	virtual void resizeEvent(QResizeEvent *ev) Q_DECL_OVERRIDE;

	void setCellSize();

	/// Converts an area in row/col coordinates into pixel coordinates.
	///
	/// (row0, col0) is the start position and rowcount/colcount the size
	QRect absoluteShellRect(int row0, int col0, int rowcount, int colcount) const noexcept;

	/// Computes the entire row position and size in pixel coordinates.
	QRect absoluteShellRectRow(int row) const noexcept;

	void setGuiFontList(const std::vector<QFont>&& fontList) noexcept
	{
		m_guifontwidelist = fontList;
	}

private:
	void setFont(const QFont&);
	void handleCursorChanged();
	QRect getNeovimCursorRect(QRect cellRect) noexcept;
	void paintRectLigatures(QPainter& p, QRect rect) noexcept;
	void paintRectNoLigatures(QPainter& p, QRect rect) noexcept;
	void paintNeovimCursorBackground(QPainter& p, QRect cellRect) noexcept;
	void paintNeovimCursorForeground(QPainter& p, QRect cellRect, QPoint pos, const QString& character) noexcept;
	void paintUnderline(QPainter& p, const Cell& cell, QRect cellRect) noexcept;
	void paintUndercurl(QPainter& p, const Cell& cell, QRect cellRect) noexcept;
	void paintStrikeThrough(QPainter& p, const Cell& cell, QRect cellRect) noexcept;
	void paintBackgroundClearCell(QPainter& p, const Cell& cell, QRect cellRect, bool isCursorCell) noexcept;
	void paintForegroundCellText(QPainter& p, const Cell& cell, QRect cellRect, bool isCursorCell) noexcept;

	void paintForegroundTextBlock(
		QPainter& p,
		const Cell& cell,
		QRect blockRect,
		const QString& text,
		int cursorPos) noexcept;

	QFont GetCellFont(const Cell& cell) const noexcept;
	QPen getForegroundPen(const Cell& cell) noexcept;
	QPen getSpecialPen(const Cell& cell) noexcept;

	ShellContents m_contents{ 0, 0 };
	QSize m_cellSize;
	int m_ascent;
	QColor m_bgColor{ Qt::white };
	QColor m_fgColor{ Qt::black };
	QColor m_spColor;
	int m_lineSpace{ 0 };
	bool m_isLigatureModeEnabled{ false };

	Background m_background{ Background::Dark };
};
