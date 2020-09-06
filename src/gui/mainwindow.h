#ifndef NEOVIM_QT_MAINWINDOW
#define NEOVIM_QT_MAINWINDOW

#include <QMainWindow>
#include <QStackedWidget>
#include <QTabBar>
#include <QSplitter>
#include "treeview.h"
#include "neovimconnector.h"
#include "errorwidget.h"
#include "scrollbar.h"
#include "shell.h"
#include "tabline.h"

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
protected:
	virtual void closeEvent(QCloseEvent *ev) Q_DECL_OVERRIDE;
	virtual void changeEvent(QEvent *ev) Q_DECL_OVERRIDE;
private slots:
	void neovimSetTitle(const QString &title);
	void neovimWidgetResized();
	void neovimMaximized(bool);
	void neovimSuspend();
	void neovimFullScreen(bool);
	void neovimGuiCloseRequest();
	void neovimExited(int status);
	void neovimError(NeovimConnector::NeovimError);
	void reconnectNeovim();
	void showIfDelayed();
	void neovimAttachmentChanged(bool);
	void neovimIsUnsupported();
	void neovimShowContextMenu();
	void neovimSendCut();
	void neovimSendCopy();
	void neovimSendPaste();
	void neovimSendSelectAll();
	void saveWindowGeometry();

private:
	void init(NeovimConnector *);
	NeovimConnector *m_nvim{};
	ErrorWidget *m_errorWidget{};
	QSplitter *m_window{};
	TreeView *m_tree{};
	Shell *m_shell{};
	DelayedShow m_delayedShow;
	QStackedWidget m_stack;
	ShellOptions m_shell_options;
	bool m_neovim_requested_close{ false };
	QMenu *m_contextMenu{};
	QAction *m_actCut{};
	QAction *m_actCopy{};
	QAction *m_actPaste{};
	QAction *m_actSelectAll{};
	ScrollBar *m_scrollbar{};
	Tabline m_tabline;
};

} // Namespace

#endif
