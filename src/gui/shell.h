#ifndef NEOVIM_QT_SHELL
#define NEOVIM_QT_SHELL

#include <QWidget>
#include <QVariantList>
#include <QFont>
#include <QBackingStore>
#include <QLabel>
#include <QTimer>
#include <QUrl>
#include <QList>
#include "neovimconnector.h"
#include "shellwidget/shellwidget.h"

namespace NeovimQt {

class Tab {
public:
	Tab(int64_t id, QString name) {
		this->tab = id;
		this->name = name;
	}
	/// The tab handle, a unique tab identifier
	int64_t tab;
	QString name;
};

class Shell: public ShellWidget
{
	Q_OBJECT
	Q_PROPERTY(bool neovimBusy READ neovimBusy() NOTIFY neovimBusy())
	Q_PROPERTY(bool neovimAttached READ neovimAttached() NOTIFY neovimAttached())
public:
	Shell(NeovimConnector *nvim, QWidget *parent=0);
	~Shell();
	QSize sizeIncrement() const;
	static QColor color(qint64 color, const QColor& fallback=QColor());
	static bool isBadMonospace(const QFont& f);
	virtual QVariant inputMethodQuery(Qt::InputMethodQuery) const Q_DECL_OVERRIDE;
	bool neovimBusy() const;
	bool neovimAttached() const;
	QString fontDesc();

signals:
	void neovimTitleChanged(const QString &title);
	void neovimBusy(bool);
	void neovimResized(int rows, int cols);
	void neovimAttached(bool);
	void neovimMaximized(bool);
	void neovimFullScreen(bool);
	void neovimGuiCloseRequest();
	/// This signal is emmited if the running neovim version is unsupported by the GUI
	void neovimIsUnsupported();
	/// The tabline needs updating. curtab is the handle of the current tab (not its index)
	/// as seen in Tab::tab.
	void neovimTablineUpdate(int64_t curtab, QList<Tab> tabs);

public slots:
	void handleNeovimNotification(const QByteArray &name, const QVariantList& args);
	void resizeNeovim(const QSize&);
	void resizeNeovim(int n_cols, int n_rows);
	bool setGuiFont(const QString& fdesc, bool force = false);
	void updateGuiWindowState(Qt::WindowStates state);
	void openFiles(const QList<QUrl> url);

protected slots:
	void neovimError(NeovimConnector::NeovimError);
	void neovimExited(int);
	void neovimResizeFinished();
	void mouseClickReset();
	void mouseClickIncrement(Qt::MouseButton bt);
	void init();
	void fontError(const QString& msg);
	void updateWindowId();

protected:
	void tooltip(const QString& text);
	virtual void inputMethodEvent(QInputMethodEvent *event) Q_DECL_OVERRIDE;
	virtual void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
	virtual bool event(QEvent *event) Q_DECL_OVERRIDE;

	QPoint neovimCursorTopLeft() const;
	QRect neovimCursorRect() const;
	QRect neovimCursorRect(QPoint at) const;
	void setNeovimCursor(quint64 col, quint64 row);

	virtual void resizeEvent(QResizeEvent *ev) Q_DECL_OVERRIDE;
	virtual void keyPressEvent(QKeyEvent *ev) Q_DECL_OVERRIDE;
        void paintLogo(QPainter&);
	virtual void paintEvent(QPaintEvent *ev) Q_DECL_OVERRIDE;
	virtual void changeEvent(QEvent *ev) Q_DECL_OVERRIDE;
	virtual void closeEvent(QCloseEvent *ev) Q_DECL_OVERRIDE;
	virtual void focusInEvent(QFocusEvent *ev) Q_DECL_OVERRIDE;
	virtual void focusOutEvent(QFocusEvent *ev) Q_DECL_OVERRIDE;
	virtual void dragEnterEvent(QDragEnterEvent *) Q_DECL_OVERRIDE;
	virtual void dropEvent(QDropEvent *) Q_DECL_OVERRIDE;

	virtual void handleResize(uint64_t cols, uint64_t rows);
	virtual void handlePut(const QVariantList& args);
	virtual void handleHighlightSet(const QVariantMap& args);
	virtual void handleRedraw(const QByteArray& name, const QVariantList& args);
	virtual void handleScroll(const QVariantList& args);
	virtual void handleModeChange(const QString& mode);
	virtual void handleSetTitle(const QVariantList& opargs);
	virtual void handleSetScrollRegion(const QVariantList& opargs);
	virtual void handleBusy(bool);

	void neovimMouseEvent(QMouseEvent *ev);
	virtual void mousePressEvent(QMouseEvent *ev) Q_DECL_OVERRIDE;
	virtual void mouseReleaseEvent(QMouseEvent *ev) Q_DECL_OVERRIDE;
	virtual void mouseMoveEvent(QMouseEvent *ev) Q_DECL_OVERRIDE;

private slots:
        void setAttached(bool attached=true);

private:
	bool m_attached;

	NeovimConnector *m_nvim;

	QList<QUrl> m_deferredOpen;

	QRect m_scroll_region;
	bool m_font_bold, m_font_italic, m_font_underline, m_font_undercurl;
	bool m_mouseHide;

	// highlight fg/bg - from redraw:highlightset - by default we
	// use the values from above
	QColor m_hg_foreground, m_hg_background, m_hg_special;
	QColor m_cursor_color;

	/// Cursor position in shell coordinates
	QPoint m_cursor_pos;
	bool m_cursor;
	bool m_insertMode;
	bool m_resizing;
	QSize m_resize_neovim_pending;
	QLabel *m_tooltip;
	QPoint m_mouse_pos;
	// 2/3/4 mouse click tracking
	QTimer m_mouseclick_timer;
	short m_mouseclick_count;
	Qt::MouseButton m_mouseclick_pending;
	// Accumulates remainder of steppy scroll
	QPoint m_mouse_wheel_delta_fraction;

	// Properties
	bool m_neovimBusy;
};

} // Namespace
#endif
