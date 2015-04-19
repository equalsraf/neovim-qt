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

protected:
        QDockWidget* newDockWidgetFor(QWidget *);
	virtual void closeEvent(QCloseEvent *ev) Q_DECL_OVERRIDE;
private slots:
	void neovimExited(int status);
	void reconnectNeovim();
private:
	void init(NeovimConnector *);
        NeovimConnector *m_nvim;
	ErrorWidget *m_errorWidget;
	Shell *m_shell;
};

} // Namespace

#endif
