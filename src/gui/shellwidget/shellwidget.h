#ifndef QSHELLWIDGET2_SHELLWIDGET
#define QSHELLWIDGET2_SHELLWIDGET

#include <QWidget>

#include "shellcontents.h"
#include "cursor.h"

class ShellWidget: public QWidget
{
	Q_OBJECT
	Q_PROPERTY(QColor background READ background WRITE setBackground)
	Q_PROPERTY(QColor foreground READ foreground WRITE setForeground)
	Q_PROPERTY(QString fontFamily READ fontFamily)
	Q_PROPERTY(int fontSize READ fontSize)
	Q_PROPERTY(int rows READ rows)
	Q_PROPERTY(int columns READ columns)
	Q_PROPERTY(QSize cellSize READ cellSize)
public:
	ShellWidget(QWidget *parent=0);

	/// Represents Neovim 'background' option. Used for rendering default theme colors (-1).
	/// See 'default_colors_set' in Neovim ':help ui-linegrid' for more details.
	enum class Background : int
	{
		Dark,
		Light
	};

	bool setShellFont(const QString& family, qreal ptSize, int weight = -1, bool italic = false, bool force = false);

	QColor background() const;
	QColor foreground() const;
	QColor special() const;
	QString fontFamily() const;
	qreal fontSize() const;
	static ShellWidget* fromFile(const QString& path);

	int rows() const;
	int columns() const;
	QSize cellSize() const;
	const ShellContents& contents() const;
	QSize sizeHint() const Q_DECL_OVERRIDE;

	Background getBackgroundType() const { return m_background; }

	void setBackgroundType(Background type) { m_background = type; }

signals:
	void shellFontChanged();
	void fontError(const QString& msg);
public slots:
	void resizeShell(int rows, int columns);
	void setSpecial(const QColor& color);
	void setBackground(const QColor& color);
	void setForeground(const QColor& color);
	void setDefaultFont();

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
		bool reverse = false);

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

	/// Move the neovim cursor for text insertion and display
	void setNeovimCursor(uint64_t col, uint64_t row) noexcept;

	virtual void paintEvent(QPaintEvent *ev) Q_DECL_OVERRIDE;
	virtual void resizeEvent(QResizeEvent *ev) Q_DECL_OVERRIDE;

	void setCellSize();
	QRect absoluteShellRect(int row0, int col0, int rowcount, int colcount);

private:
	void setFont(const QFont&);
	void handleCursorChanged();
	QRect getNeovimCursorRect(QRect cellRect) noexcept;
	void paintNeovimCursorBackground(QPainter& p, QRect cellRect) noexcept;
	void paintNeovimCursorForeground(QPainter& p, QRect cellRect, QPoint pos, QChar character) noexcept;

	ShellContents m_contents{ 0, 0 };
	QSize m_cellSize;
	int m_ascent;
	QColor m_bgColor{ Qt::white };
	QColor m_fgColor{ Qt::black };
	QColor m_spColor;
	int m_lineSpace{ 0 };

	Background m_background{ Background::Dark };
};

#endif
