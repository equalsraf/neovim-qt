#ifndef NEOVIM_QT_MAINWINDOW
#define NEOVIM_QT_MAINWINDOW

#include <QMainWindow>
#include <QCloseEvent>
#include <QStackedWidget>
#include <QTabBar>
#include <QScrollBar>
#include <QSplitter>
#include "treeview.h"
#include "neovimconnector.h"
#include "errorwidget.h"
#include "shell.h"

namespace NeovimQt {

class MainWindow: public QMainWindow
{
	Q_OBJECT
public:
	enum DelayedShow {
		Disabled,
		Normal,
		Maximized,
		FullScreen,
	};

	MainWindow(NeovimConnector *, ShellOptions opts, QWidget *parent=0);
	bool neovimAttached() const;
	Shell* shell();
	void restoreWindowGeometry();
public slots:
	void delayedShow(DelayedShow type=DelayedShow::Normal);
signals:
	void neovimAttached(bool);
	void closing(int);
protected:
	virtual void closeEvent(QCloseEvent *ev) Q_DECL_OVERRIDE;
	virtual void changeEvent(QEvent *ev) Q_DECL_OVERRIDE;
private slots:
	void neovimSetTitle(const QString &title);
	void neovimWidgetResized();
	void neovimMaximized(bool);
	void neovimSuspend();
	void neovimFullScreen(bool);
	void neovimGuiCloseRequest(int);
	void neovimExited(int status);
	void neovimError(NeovimConnector::NeovimError);
	void reconnectNeovim();
	void showIfDelayed();
	void neovimAttachmentChanged(bool);
	void neovimIsUnsupported();
	void neovimShowtablineSet(int);
	void neovimTablineUpdate(int64_t curtab, QList<Tab> tabs);
	void neovimShowContextMenu();
	void neovimSendCut();
	void neovimSendCopy();
	void neovimSendPaste();
	void neovimSendSelectAll();
	void extTablineSet(bool);
	void changeTab(int index);
	void saveWindowGeometry();
	void setGuiScrollBarVisible(bool isVisible);
	void neovimCursorMovedUpdateScrollBar(
		uint64_t minLineVisible, uint64_t bufferSize, uint64_t windowHeigt);
	void neovimScrollEvent(int64_t rows);

private:
	void init(NeovimConnector *);
	NeovimConnector *m_nvim;
	ErrorWidget *m_errorWidget;
	QSplitter *m_window;
	TreeView *m_tree;
	Shell *m_shell;
	DelayedShow m_delayedShow;
	QStackedWidget m_stack;
	QTabBar *m_tabline;
	QToolBar *m_tabline_bar;
	ShellOptions m_shell_options;
	bool m_neovim_requested_close;
	QMenu *m_contextMenu;
	QAction *m_actCut;
	QAction *m_actCopy;
	QAction *m_actPaste;
	QAction *m_actSelectAll;
	QScrollBar *m_rightScrollBar;

	/// The scrollbar updates on cursor movement and GUI paint/scroll events.
	/// In cases where the scroll event triggers cursor movement, this can result
	/// in double registration. Detect and ignore this double registration.
	int m_scrollbarLastDelta{ 0 };
	int m_exitStatus = 0;
};

} // Namespace

#endif
