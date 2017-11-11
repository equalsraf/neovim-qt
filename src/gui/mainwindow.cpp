#include "mainwindow.h"

#include <QCloseEvent>
#include <QToolBar>
#include <QLayout>

namespace NeovimQt {

MainWindow::MainWindow(NeovimConnector *c, QWidget *parent)
:QMainWindow(parent), m_nvim(0), m_errorWidget(0), m_shell(0),
	m_delayedShow(DelayedShow::Disabled), m_tabline(0), m_tabline_bar(0)
{
	m_errorWidget = new ErrorWidget();
	m_stack.addWidget(m_errorWidget);
	connect(m_errorWidget, &ErrorWidget::reconnectNeovim,
			this, &MainWindow::reconnectNeovim);
	setCentralWidget(&m_stack);

	init(c);
}

void MainWindow::init(NeovimConnector *c)
{
	if (m_shell) {
		m_shell->deleteLater();
		m_stack.removeWidget(m_shell);
	}
	if (m_nvim) {
		m_nvim->deleteLater();
	}

	m_tabline_bar = addToolBar("tabline");
	m_tabline_bar->setObjectName("tabline");
	m_tabline_bar->setAllowedAreas(Qt::TopToolBarArea);
	m_tabline_bar->setMovable(false);
	m_tabline_bar->setFloatable(false);
	// Avoid margins around the tabbar
	m_tabline_bar->layout()->setContentsMargins(0, 0, 0, 0);

	m_tabline = new QTabBar(m_tabline_bar);
	m_tabline->setDrawBase(false);
	m_tabline->setExpanding(false);
	m_tabline->setDocumentMode(true);
	m_tabline->setFocusPolicy(Qt::NoFocus);
	connect(m_tabline, &QTabBar::currentChanged,
			this, &MainWindow::changeTab);

	m_tabline_bar->addWidget(m_tabline);

	m_nvim = c;
	m_shell = new Shell(c);
	m_stack.insertWidget(1, m_shell);
	m_stack.setCurrentIndex(1);
	connect(m_shell, SIGNAL(neovimAttached(bool)),
			this, SLOT(neovimAttachmentChanged(bool)));
	connect(m_shell, SIGNAL(neovimTitleChanged(const QString &)),
			this, SLOT(neovimSetTitle(const QString &)));
	connect(m_shell, &Shell::neovimResized,
			this, &MainWindow::neovimWidgetResized);
	connect(m_shell, &Shell::neovimMaximized,
			this, &MainWindow::neovimMaximized);
	connect(m_shell, &Shell::neovimFullScreen,
			this, &MainWindow::neovimFullScreen);
	connect(m_shell, &Shell::neovimGuiCloseRequest,
			this, &MainWindow::neovimGuiCloseRequest);
	connect(m_nvim, &NeovimConnector::processExited,
			this, &MainWindow::neovimExited);
	connect(m_nvim, &NeovimConnector::error,
			this, &MainWindow::neovimError);
	connect(m_shell, &Shell::neovimIsUnsupported,
			this, &MainWindow::neovimIsUnsupported);
	connect(m_shell, &Shell::neovimTablineUpdate,
			this, &MainWindow::neovimTablineUpdate);
	m_shell->setFocus(Qt::OtherFocusReason);

	if (m_nvim->errorCause()) {
		neovimError(m_nvim->errorCause());
	}
}

bool MainWindow::neovimAttached() const
{
	return (m_shell != NULL && m_shell->neovimAttached());
}

/** The Neovim process has exited */
void MainWindow::neovimExited(int status)
{
	showIfDelayed();

	if (m_nvim->errorCause() != NeovimConnector::NoError) {
		m_errorWidget->setText(m_nvim->errorString());
		m_errorWidget->showReconnect(m_nvim->canReconnect());
		m_stack.setCurrentIndex(0);
	} else if (status != 0) {
		m_errorWidget->setText(QString("Neovim exited with status code (%1)").arg(status));
		m_errorWidget->showReconnect(m_nvim->canReconnect());
		m_stack.setCurrentIndex(0);
	} else {
		close();
	}
}
void MainWindow::neovimError(NeovimConnector::NeovimError err)
{
	showIfDelayed();

	switch(err) {
	case NeovimConnector::FailedToStart:
		m_errorWidget->setText("Unable to start nvim: " + m_nvim->errorString());
		break;
	default:
		m_errorWidget->setText(m_nvim->errorString());
	}
	m_errorWidget->showReconnect(m_nvim->canReconnect());
	m_stack.setCurrentIndex(0);
}
void MainWindow::neovimIsUnsupported()
{
	showIfDelayed();
	m_errorWidget->setText(QString("Cannot connect to this Neovim, required API version 1, found [%1-%2]")
			.arg(m_nvim->apiCompatibility())
			.arg(m_nvim->apiLevel()));
	m_errorWidget->showReconnect(m_nvim->canReconnect());
	m_stack.setCurrentIndex(0);
}

void MainWindow::neovimSetTitle(const QString &title)
{
	this->setWindowTitle(title);
}

void MainWindow::neovimWidgetResized()
{
	// Neovim finished resizing, resize it back to the actual
	// widget size - this avoids situations when neovim wants a size that
	// exceeds the available widget size i.e. the GUI tells neovim its
	// size, not the other way around.
	m_shell->resizeNeovim(m_shell->size());
}

void MainWindow::neovimMaximized(bool set)
{
	if (set) {
		setWindowState(windowState() | Qt::WindowMaximized);
	} else {
		setWindowState(windowState() & ~Qt::WindowMaximized);
	}
}

void MainWindow::neovimFullScreen(bool set)
{
	if (set) {
		setWindowState(windowState() | Qt::WindowFullScreen);
	} else {
		setWindowState(windowState() & ~Qt::WindowFullScreen);
	}
}

void MainWindow::neovimGuiCloseRequest()
{
	QMainWindow::close();
}

void MainWindow::reconnectNeovim()
{
	if (m_nvim->canReconnect()) {
		init(m_nvim->reconnect());
	}
	m_stack.setCurrentIndex(1);
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
void MainWindow::changeEvent( QEvent *ev)
{
	if (ev->type() == QEvent::WindowStateChange && isWindow()) {
		m_shell->updateGuiWindowState(windowState());
	}
	QWidget::changeEvent(ev);
}

/// Call show() after a 1s delay or when Neovim attachment
/// is complete, whichever comes first
void MainWindow::delayedShow(DelayedShow type)
{
	m_delayedShow = type;
	if (m_nvim->errorCause() != NeovimConnector::NoError) {
		showIfDelayed();
		return;
	}

	if (type != DelayedShow::Disabled) {
		QTimer *t = new QTimer(this);
		t->setSingleShot(true);
		t->setInterval(1000);
		connect(m_shell, &Shell::neovimResized, this, &MainWindow::showIfDelayed);
		connect(t, &QTimer::timeout, this, &MainWindow::showIfDelayed);
		t->start();
	}
}

void MainWindow::showIfDelayed()
{
	if (!isVisible()) {
		if (m_delayedShow == DelayedShow::Normal) {
			show();
		} else if (m_delayedShow == DelayedShow::Maximized) {
			showMaximized();
		} else if (m_delayedShow == DelayedShow::FullScreen) {
			showFullScreen();
		}
	}
	m_delayedShow = DelayedShow::Disabled;
}

void MainWindow::neovimAttachmentChanged(bool attached)
{
	emit neovimAttached(attached);
	if (isWindow() && m_shell != NULL) {
		m_shell->updateGuiWindowState(windowState());
	}
}

Shell* MainWindow::shell()
{
	return m_shell;
}

void MainWindow::neovimTablineUpdate(int64_t curtab, QList<Tab> tabs)
{
	// remove extra tabs
	for (int index=tabs.size(); index<m_tabline->count(); index++) {
		m_tabline->removeTab(index);
	}


	for (int index=0; index<tabs.size(); index++) {
		// Escape & in tab name otherwise it will be interpreted as
		// a keyboard shortcut (#357) - escaping is done using &&
		QString text = tabs[index].name;
		text.replace("&", "&&");

		if (m_tabline->count() <= index) {
			m_tabline->addTab(text);
		} else {
			m_tabline->setTabText(index, text);
		}

		m_tabline->setTabData(index, QVariant::fromValue(tabs[index].tab));

		if (curtab == tabs[index].tab) {
			m_tabline->setCurrentIndex(index);
		}
	}

	// hide/show the tabline toolbar
	m_tabline_bar->setVisible(tabs.size() > 1);

	Q_ASSERT(tabs.size() == m_tabline->count());
}

void MainWindow::changeTab(int index)
{
	if (m_nvim->api2() == NULL) {
		return;
	}

	int64_t tab = m_tabline->tabData(index).toInt();
	m_nvim->api2()->nvim_set_current_tabpage(tab);
}
} // Namespace

