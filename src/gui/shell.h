#ifndef NEOVIM_QT_SHELL
#define NEOVIM_QT_SHELL

#include <QWidget>
#include <QVariantList>
#include <QFont>
#include <QBackingStore>
#include <QLabel>
#include <QTimer>
#include "neovimconnector.h"

namespace NeovimQt {

class Shell: public QWidget
{
	Q_OBJECT
	Q_PROPERTY(bool neovimBusy READ neovimBusy() NOTIFY neovimBusy())
public:
	Shell(NeovimConnector *nvim, QWidget *parent=0);
	~Shell();
	QSize sizeIncrement() const;
	QSize sizeHint() const Q_DECL_OVERRIDE;
	static QColor color(qint64 color, const QColor& fallback=QColor());
	virtual QVariant inputMethodQuery(Qt::InputMethodQuery) const Q_DECL_OVERRIDE;
	bool neovimBusy() const;

signals:
	void neovimTitleChanged(const QString &title);
	void neovimResized(const QSize& newSize);
	void neovimBusy(bool);

public slots:
	void handleNeovimNotification(const QByteArray &name, const QVariantList& args);
	void resizeNeovim(const QSize&);

protected slots:
	void neovimIsReady();
	void neovimError(NeovimConnector::NeovimError);
	void neovimExited(int);
	void neovimResizeFinished();
	void mouseClickReset();
	void mouseClickIncrement(Qt::MouseButton bt);

protected:
	void tooltip(const QString& text);
	virtual void inputMethodEvent(QInputMethodEvent *event) Q_DECL_OVERRIDE;
	virtual void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

	int neovimWidth() const;
	int neovimHeight() const;
	quint64 neovimRowHeight() const;
	quint64 neovimCellWidth() const;
	QSize neovimSize() const;
	QPoint neovimCursorTopLeft() const;
	QSize neovimCharSize() const;
	void setNeovimCursor(quint64 col, quint64 row);
	void setupPainter(QPainter&);

	virtual void resizeEvent(QResizeEvent *ev) Q_DECL_OVERRIDE;
	virtual void keyPressEvent(QKeyEvent *ev) Q_DECL_OVERRIDE;
        void paintLogo(QPainter&);
	virtual void paintEvent(QPaintEvent *ev) Q_DECL_OVERRIDE;
	virtual void changeEvent(QEvent *ev) Q_DECL_OVERRIDE;
	virtual void closeEvent(QCloseEvent *ev) Q_DECL_OVERRIDE;

	virtual void handleResize(uint64_t cols, uint64_t rows);
	virtual void handlePut(const QVariantList& args, QPainter&);
	virtual void handleHighlightSet(const QVariantMap& args, QPainter& painter);
	virtual void handleRedraw(const QByteArray& name, const QVariantList& args, QPainter& painter);
	virtual void handleScroll(const QVariantList& args, QPainter& painter);
	virtual void handleModeChange(const QString& mode);
	virtual void handleSetTitle(const QVariantList& opargs);
	virtual void handleSetScrollRegion(const QVariantList& opargs);
	virtual void handleBusy(bool);

	void neovimMouseEvent(QMouseEvent *ev);
	virtual void mousePressEvent(QMouseEvent *ev) Q_DECL_OVERRIDE;
	virtual void mouseReleaseEvent(QMouseEvent *ev) Q_DECL_OVERRIDE;
	virtual void mouseMoveEvent(QMouseEvent *ev) Q_DECL_OVERRIDE;
private:
	bool m_attached;
        void setAttached(bool);

	NeovimConnector *m_nvim;
	uint64_t m_rows, m_cols;
	QRect m_scroll_region;

	QFont m_font;
	bool m_font_bold, m_font_italic, m_font_underline, m_font_undercurl;
	QFontMetrics *m_fm;

	QImage m_image;

	// shell bg/fg - updated through redraw:foreground/background
	QColor m_foreground, m_background;
	// highlight fg/bg - from redraw:highlightset - by default we
	// use the values from above
	QColor m_hg_foreground, m_hg_background;
	QColor m_cursor_color;

	QPoint m_cursor_pos;
	bool m_cursor;
	bool m_insertMode;
	bool m_resizing;
	QLabel *m_tooltip;
        QPixmap m_logo;
	QPoint m_mouse_pos;
	// 2/3/4 mouse click tracking
	QTimer m_mouseclick_timer;
	short m_mouseclick_count;
	Qt::MouseButton m_mouseclick_pending;

	// Properties
	bool m_neovimBusy;
};

} // Namespace
#endif
