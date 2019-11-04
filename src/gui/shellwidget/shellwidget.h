#ifndef QSHELLWIDGET2_SHELLWIDGET
#define QSHELLWIDGET2_SHELLWIDGET

#include <QWidget>

#include "shellcontents.h"

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
	Q_PROPERTY(bool ligatureMode READ ligatureMode WRITE setLigatureMode)
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
	bool ligatureMode() const;

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
	int put(const QString&, int row, int column,
			QColor fg=QColor(), QColor bg=QColor(), QColor sp=QColor(),
			bool bold=false, bool italic=false,
			bool underline=false, bool undercurl=false);
	void clearRow(int row);
	void clearShell(QColor bg = QColor::Invalid);
	void clearRegion(int row0, int col0, int row1, int col1);
	void scrollShell(int rows);
	void scrollShellRegion(int row0, int row1, int col0,
			int col1, int rows);
	void setLineSpace(int height);
	void setLigatureMode(bool ligatureMode);
protected:
	virtual void paintEvent(QPaintEvent *ev) Q_DECL_OVERRIDE;
	virtual void resizeEvent(QResizeEvent *ev) Q_DECL_OVERRIDE;

	void setCellSize();
	QRect absoluteShellRect(int row0, int col0, int rowcount, int colcount);

private:
	void setFont(const QFont&);

	ShellContents m_contents;
	QSize m_cellSize;
	int m_ascent;
	QColor m_bgColor, m_fgColor, m_spColor;
	int m_lineSpace;
	bool m_ligatureMode;

	Background m_background{ Background::Dark };
};

#endif
