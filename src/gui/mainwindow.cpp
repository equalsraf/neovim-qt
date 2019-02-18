#include "mainwindow.h"

#include <QCloseEvent>
#include <QToolBar>
#include <QLayout>

namespace NeovimQt {

MainWindow::MainWindow(NeovimConnector *c, ShellOptions opts, QWidget *parent)
:QMainWindow(parent), m_nvim(0), m_errorWidget(0), m_shell(0),
	m_delayedShow(DelayedShow::Disabled), m_tabline(0), m_tabline_bar(0),
	m_shell_options(opts), m_neovim_requested_close(false), 
	m_neovim_gui_style_requested_update(false), m_neovim_gui_font_requested_update (false),
	m_default_font(font()), m_default_palette(palette())
{
	m_errorWidget = new ErrorWidget();
    m_style = QStyleFactory::create("Fusion");
    setStyle(m_style);
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

	m_last_fg_color = Qt::white;
	m_last_bg_color = Qt::black;

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
	m_tabline_bar->setVisible(m_shell_options.enable_ext_tabline);

	m_nvim = c;

	m_tree = new TreeView(c);
	m_shell = new Shell(c, m_shell_options);

	m_window = new QSplitter();
	m_window->addWidget(m_tree);
	m_tree->hide();
	m_window->addWidget(m_shell);

    m_window->setStyle(m_style);
    m_tree->setStyle(m_style);
    m_tabline_bar->setStyle(m_style);
    m_tabline->setStyle(m_style);

	m_stack.insertWidget(1, m_window);
	m_stack.setCurrentIndex(1);

	connect(m_shell, SIGNAL(neovimAttached(bool)),
			this, SLOT(neovimAttachmentChanged(bool)));
	connect(m_shell, SIGNAL(neovimTitleChanged(const QString &)),
			this, SLOT(neovimSetTitle(const QString &)));
	connect(m_shell, &Shell::neovimResized,
			this, &MainWindow::neovimWidgetResized);
	connect(m_shell, &Shell::neovimMaximized,
			this, &MainWindow::neovimMaximized);
	connect(m_shell, &Shell::neovimSuspend,
			this, &MainWindow::neovimSuspend);
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
	connect(m_shell, &Shell::neovimExtTablineSet,
			this, &MainWindow::extTablineSet);
	connect(m_shell, &Shell::neovimTablineUpdate,
			this, &MainWindow::neovimTablineUpdate);
	connect(m_shell, &Shell::neovimShowtablineSet,
			this, &MainWindow::neovimShowtablineSet);
	connect(m_shell, SIGNAL(neovimGuiColorsAdaptiveEnabled(bool)), 
			this, SLOT(neovimGuiColorsAdaptiveChanged(bool)));
	connect(m_shell, SIGNAL(neovimGuiFontAdaptiveEnabled(bool)), 
			this, SLOT(neovimGuiFontAdaptiveChanged(bool)));
	m_shell->setFocus(Qt::OtherFocusReason);

	if (m_nvim->errorCause()) {
		neovimError(m_nvim->errorCause());
        }
}

void MainWindow::updateStyle()
{
	//update font (if needed)
	if (m_neovim_gui_font_requested_update) {
		auto newFont = m_shell->font();
		if (font() != newFont) {
			setFont(newFont);
		}
	}
	else if (font() != m_default_font) {
			setFont(m_default_font);
	}

	if (!m_neovim_gui_style_requested_update) {
		m_palette = m_default_palette;
		m_last_bg_color = m_palette.color(QPalette::Window);
        m_last_fg_color = m_palette.color(QPalette::WindowText);
		setNewPalette();
		return;
	}

    auto bg_color = m_shell->getBackground();
    auto fg_color = m_shell->getForeground();

    if (bg_color == fg_color) //usually in the default theme: bug or feature?
    	return;

    if (m_last_bg_color == bg_color && m_last_fg_color == fg_color){
    	return;
    } else {
    	m_last_bg_color = bg_color;
    	m_last_fg_color = fg_color;
    }

    m_palette.setColor(QPalette::Window, bg_color);
    m_palette.setColor(QPalette::WindowText, fg_color);
    m_palette.setColor(QPalette::Disabled, QPalette::WindowText, Qt::white);

    m_palette.setColor(QPalette::Base, bg_color);
    m_palette.setColor(QPalette::AlternateBase, fg_color);
    m_palette.setColor(QPalette::Text, fg_color);
    m_palette.setColor(QPalette::Disabled, QPalette::Text, Qt::white); 

	m_palette.setColor(QPalette::Highlight, fg_color);
	m_palette.setColor(QPalette::Disabled, QPalette::Highlight, Qt::white);
	m_palette.setColor(QPalette::HighlightedText, bg_color);
	m_palette.setColor(QPalette::Disabled, QPalette::HighlightedText, Qt::black);

    m_palette.setColor(QPalette::ToolTipBase, bg_color);
    m_palette.setColor(QPalette::ToolTipText, fg_color);

	m_palette.setColor(QPalette::Button, bg_color);
	m_palette.setColor(QPalette::ButtonText, fg_color);
    m_palette.setColor(QPalette::Disabled, QPalette::ButtonText, Qt::white); 

    m_palette.setColor(QPalette::BrightText, Qt::red); //defualt qt color
    m_palette.setColor(QPalette::Link, Qt::blue); //default qt color
    m_palette.setColor(QPalette::LinkVisited, Qt::magenta); //default qt color

    m_palette.setColor(QPalette::Dark, QColor(34, 34, 34));
    m_palette.setColor(QPalette::Shadow, QColor(21, 21, 21));

    setNewPalette();
}

void MainWindow::setNewPalette() {

    m_window->setPalette(m_palette);
    m_tree->setPalette(m_palette);
    m_tabline_bar->setPalette(m_palette);
    m_tabline->setPalette(m_palette);
    setPalette(m_palette);
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
	if (isMaximized() || isFullScreen()) {
		QSize size = geometry().size();
		if (m_tabline_bar->isVisible()) {
			size.setHeight(size.height() - m_tabline_bar->geometry().size().height());
		}
		if (m_tree->isVisible()) {
			size.scale(size.width() - m_tree->geometry().size().width(),
				size.height(), Qt::IgnoreAspectRatio);
		}
		m_shell->resizeNeovim(size);
	} else {
		m_shell->resizeNeovim(m_shell->size());
	}

    updateStyle();
}

void MainWindow::neovimMaximized(bool set)
{
	if (set) {
		setWindowState(windowState() | Qt::WindowMaximized);
	} else {
		setWindowState(windowState() & ~Qt::WindowMaximized);
	}
}

void MainWindow::neovimSuspend()
{
	qDebug() << "Minimizing window";
	setWindowState(windowState() | Qt::WindowMinimized);
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
	m_neovim_requested_close = true;
	QMainWindow::close();
	m_neovim_requested_close = false;
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
	if (m_neovim_requested_close) {
		// If this was requested by nvim, shutdown
		QWidget::closeEvent(ev);
	} else if (m_shell->close()) {
		// otherwise only if the Neovim shell closes too
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
	if (attached) {
		if (isWindow() && m_shell != NULL) {
			m_shell->updateGuiWindowState(windowState());
		}
	} else {
		m_tabline->deleteLater();
		m_tabline_bar->deleteLater();
	}
}

Shell* MainWindow::shell()
{
	return m_shell;
}

void MainWindow::extTablineSet(bool val)
{
	bool old = m_shell_options.enable_ext_tabline;
	m_shell_options.enable_ext_tabline = val;
	// redraw if state changed
	if (old != m_shell_options.enable_ext_tabline) {
		if (!val) m_tabline_bar->setVisible(false);
		m_nvim->api0()->vim_command("silent! redraw!");
	}
}

void MainWindow::neovimShowtablineSet(int val)
{
	m_shell_options.nvim_show_tabline = val;
}

void MainWindow::neovimTablineUpdate(int64_t curtab, QList<Tab> tabs)
{
	if (!m_shell_options.enable_ext_tabline) {
		return;
	}

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
	if (m_shell_options.nvim_show_tabline==0) {
		m_tabline_bar->setVisible(false);
	} else if (m_shell_options.nvim_show_tabline==2) {
		m_tabline_bar->setVisible(true);
	} else {
		m_tabline_bar->setVisible(tabs.size() > 1);
	}

	Q_ASSERT(tabs.size() == m_tabline->count());
}

void MainWindow::changeTab(int index)
{
	if (!m_shell_options.enable_ext_tabline) {
		return;
	}

	if (m_nvim->api2() == NULL) {
		return;
	}

	int64_t tab = m_tabline->tabData(index).toInt();
	m_nvim->api2()->nvim_set_current_tabpage(tab);
}

void MainWindow::neovimGuiColorsAdaptiveChanged(bool val)
{
	m_neovim_gui_style_requested_update = val;
	updateStyle();
}

void MainWindow::neovimGuiFontAdaptiveChanged(bool val)
{
	m_neovim_gui_font_requested_update = val;
	updateStyle();
}

}  // namespace NeovimQt
