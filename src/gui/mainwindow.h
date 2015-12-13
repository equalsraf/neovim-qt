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
	MainWindow(NeovimConnector *, QWidget *parent=0);
public slots:
	void delayedShow(bool show=true);
protected:
        QDockWidget* newDockWidgetFor(QWidget *);
	virtual void closeEvent(QCloseEvent *ev) Q_DECL_OVERRIDE;
	void showIfDelayed();
private slots:
	void neovimSetTitle(const QString &title);
	void neovimExited(int status);
	void neovimError(NeovimConnector::NeovimError);
	void reconnectNeovim();
	void neovimWidgetResized(const QSize& newSize);
private:
	void init(NeovimConnector *);
        NeovimConnector *m_nvim;
	ErrorWidget *m_errorWidget;
	Shell *m_shell;
	bool m_delayedShow;
};

} // Namespace

#endif
