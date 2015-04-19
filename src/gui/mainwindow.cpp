#include "mainwindow.h"

#include <QCloseEvent>

namespace NeovimQt {

MainWindow::MainWindow(NeovimConnector *c, QWidget *parent)
:QMainWindow(parent), m_errorWidget(0), m_shell(0), m_nvim(0)
{
	m_errorWidget = new ErrorWidget();
	newDockWidgetFor(m_errorWidget);
	m_errorWidget->setVisible(false);

	init(c);
}

void MainWindow::init(NeovimConnector *c)
{
	if (m_shell) {
		m_shell->deleteLater();
	}
	if (m_nvim) {
		m_nvim->deleteLater();
	}

	m_nvim = c;
	m_shell = new Shell(c);
	setCentralWidget(m_shell);

	connect(m_nvim, &NeovimConnector::processExited,
			this, &MainWindow::neovimExited);
	connect(m_errorWidget, &ErrorWidget::reconnectNeovim,
			this, &MainWindow::reconnectNeovim);
	m_shell->setFocus(Qt::OtherFocusReason);
}

QDockWidget* MainWindow::newDockWidgetFor(QWidget *w)
{
	QDockWidget *dock = new QDockWidget(this);
	dock->setAllowedAreas(Qt::TopDockWidgetArea);
	dock->setWidget(w);
	dock->setFeatures(QDockWidget::NoDockWidgetFeatures);
	dock->setTitleBarWidget(new QWidget(this));
	addDockWidget(Qt::TopDockWidgetArea, dock);
	return dock;
}

/** The Neovim process has exited */
void MainWindow::neovimExited(int status)
{
	if (m_nvim->errorCause() != NeovimConnector::NoError) {
		m_errorWidget->setText(m_nvim->errorString());
		m_errorWidget->showReconnect(m_nvim->canReconnect());
		m_errorWidget->setVisible(true);
	}
}

void MainWindow::reconnectNeovim()
{
	if (m_nvim->canReconnect()) {
		init(m_nvim->reconnect());
	}
	m_errorWidget->setVisible(false);
}

void MainWindow::closeEvent(QCloseEvent *ev)
{
	// Never unless the Neovim shell closes too
	if (m_shell->close()) {
		QWidget::closeEvent(ev);
	} else {
		ev->ignore();
	}
}

}; // Namespace

