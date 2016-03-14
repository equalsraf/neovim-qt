#ifndef NEOVIM_QT_MAINWINDOW
#define NEOVIM_QT_MAINWINDOW

#include <QMainWindow>
#include <QDockWidget>
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
public slots:
	void delayedShow(DelayedShow type=DelayedShow::Normal);
protected:
        QDockWidget* newDockWidgetFor(QWidget *);
	virtual void closeEvent(QCloseEvent *ev) Q_DECL_OVERRIDE;
private slots:
	void neovimSetTitle(const QString &title);
	void neovimWidgetResized();
	void neovimExited(int status);
	void neovimError(NeovimConnector::NeovimError);
	void reconnectNeovim();
	void showIfDelayed();
private:
	void init(NeovimConnector *);
        NeovimConnector *m_nvim;
	ErrorWidget *m_errorWidget;
	Shell *m_shell;
	DelayedShow m_delayedShow;
};

} // Namespace

#endif
