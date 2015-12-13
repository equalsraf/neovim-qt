#include "mainwindow.h"

#include <QCloseEvent>
#include <QVBoxLayout>

namespace NeovimQt {

MainWindow::MainWindow(NeovimConnector *c, QWidget *parent)
:QMainWindow(parent), m_nvim(0), m_errorWidget(0), m_shell(0), m_layout(0)
{
	m_errorWidget = new ErrorWidget();
	m_errorWidget->setVisible(false);

	QWidget *w = new QWidget(this);
	m_layout = new QVBoxLayout(w);
	m_layout->setSpacing(0);
	m_layout->setContentsMargins(0, 0, 0, 0);
	m_layout->addWidget(m_errorWidget);
	w->setLayout(m_layout);
	setCentralWidget(w);

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
	m_shell->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_layout->takeAt(1);
	m_layout->addWidget(m_shell);

	connect(m_shell, SIGNAL(neovimTitleChanged(const QString &)),
			this, SLOT(neovimSetTitle(const QString &)));
	connect(m_nvim, &NeovimConnector::processExited,
			this, &MainWindow::neovimExited);
	connect(m_nvim, &NeovimConnector::error,
			this, &MainWindow::neovimError);
	connect(m_errorWidget, &ErrorWidget::reconnectNeovim,
			this, &MainWindow::reconnectNeovim);
	connect(m_shell, &Shell::neovimResized,
			this, &MainWindow::neovimWidgetResized);
	m_shell->setFocus(Qt::OtherFocusReason);

	if (m_nvim->errorCause()) {
		neovimError(m_nvim->errorCause());
	}
}

/** The Neovim process has exited */
void MainWindow::neovimExited(int status)
{
	if (m_nvim->errorCause() != NeovimConnector::NoError) {
		m_errorWidget->setText(m_nvim->errorString());
		m_errorWidget->showReconnect(m_nvim->canReconnect());
		m_errorWidget->setVisible(true);
	} else if (status != 0) {
		m_errorWidget->setText(QString("Neovim exited with status code (%1)").arg(status));
		m_errorWidget->showReconnect(m_nvim->canReconnect());
		m_errorWidget->setVisible(true);
	} else {
		close();
	}
}
void MainWindow::neovimError(NeovimConnector::NeovimError err)
{
	switch(err) {
	case NeovimConnector::FailedToStart:
		m_errorWidget->setText("Unable to start nvim: " + m_nvim->errorString());
		break;
	default:
		m_errorWidget->setText(m_nvim->errorString());
	}
	m_errorWidget->showReconnect(m_nvim->canReconnect());
	m_errorWidget->setVisible(true);
}

void MainWindow::neovimSetTitle(const QString &title)
{
	this->setWindowTitle(title);
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

void MainWindow::neovimWidgetResized(const QSize& newSize)
{
	if ((windowState() & Qt::WindowMaximized)) {
		if (newSize.width() > width() || newSize.height() > height()) {
			// If the Neovim shell is larger than the main window, resize it
			// to fit
			// TODO: size() is not ideal, if the mainwindow has toolbars, or
			// docks this will be incorrect
			m_shell->resizeNeovim(size());
		}
	} else {
		adjustSize();
	}
}

} // Namespace

