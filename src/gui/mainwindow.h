#ifndef NEOVIM_QT_MAINWINDOW
#define NEOVIM_QT_MAINWINDOW

#include <QMainWindow>
#include <QStackedWidget>
#include <QTabBar>
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

	MainWindow(NeovimConnector *, QWidget *parent=0);
	bool neovimAttached() const;
	Shell* shell();
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
	void neovimFullScreen(bool);
	void neovimGuiCloseRequest();
	void neovimExited(int status);
	void neovimError(NeovimConnector::NeovimError);
	void reconnectNeovim();
	void showIfDelayed();
	void neovimAttachmentChanged(bool);
	void neovimIsUnsupported();
	void neovimTablineUpdate(int64_t curtab, QList<Tab> tabs);
	void changeTab(int index);
private:
	void init(NeovimConnector *);
        NeovimConnector *m_nvim;
	ErrorWidget *m_errorWidget;
	Shell *m_shell;
	DelayedShow m_delayedShow;
	QStackedWidget m_stack;
	QTabBar *m_tabline;
	QToolBar *m_tabline_bar;
};

} // Namespace

#endif
