#ifndef NEOVIM_QT_MAINWINDOW
#define NEOVIM_QT_MAINWINDOW

#include <QMainWindow>
#include "neovimconnector.h"
#include "errorwidget.h"
#include "shell.h"

class QVBoxLayout;

namespace NeovimQt {

class MainWindow: public QMainWindow
{
	Q_OBJECT
public:
	MainWindow(NeovimConnector *, QWidget *parent=0);
protected:
	virtual void closeEvent(QCloseEvent *ev) Q_DECL_OVERRIDE;
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
	QVBoxLayout *m_layout;
};

} // Namespace

#endif
