#include "mainwindow.h"

#include <QApplication>
#include <QCloseEvent>
#include <QEventLoop>
#include <QLayout>
#include <QScreen>
#include <QSettings>
#include <QStyleFactory>
#include <QToolBar>

namespace NeovimQt {

static QString DefaultWindowTitle() noexcept
{
	return "Neovim";
}

static void SetDefaultWindowSize(QWidget& widget) noexcept
{
	const QPoint local_position{ widget.width() / 2, 0 };
	const QPoint global_position{ widget.mapToGlobal(local_position) };
	const QScreen* screen{ qApp->screenAt(global_position) };
	if (!screen) {
		return;
	}
	const QRect geometry{ screen->availableGeometry() };
	if (screen->orientation() == Qt::LandscapeOrientation) {
		widget.resize(geometry.width() / 2, geometry.height());
	}
	else {
		widget.resize(geometry.width(), geometry.height() / 2);
	}
}

MainWindow::MainWindow(NeovimConnector* c, QWidget* parent) noexcept
	: QMainWindow{ parent }
	, m_tabline{ *c, this }
	, m_defaultFont{ font() }
	, m_defaultPalette{ palette() }
{
	m_errorWidget = new ErrorWidget();
	m_stack.addWidget(m_errorWidget);
	connect(m_errorWidget, &ErrorWidget::reconnectNeovim,
			this, &MainWindow::reconnectNeovim);
	setCentralWidget(&m_stack);

	setWindowTitle(DefaultWindowTitle());

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

	m_shell = new Shell(c);
	m_shell->setParent(this);

	addToolBar(&m_tabline);

	m_nvim = c;

	// GuiShowContextMenu - right click context menu and actions.
	m_contextMenu = new ContextMenu(c, this);

	// GuiTreeview - side pane file explorer tree view.
	m_tree = new TreeView(c, this);

	// GuiScrollBar - scroll bar for active buffer.
	m_scrollbar = new ScrollBar{ m_nvim, this };

	// ShellWidget + GuiScrollBar Layout
	// QSplitter does not allow layouts directly: QWidget { HLayout { ShellWidget, QScrollBar } }
	QWidget* shellScrollable{ new QWidget() };
	QHBoxLayout* layout{ new QHBoxLayout() };
	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(m_shell);
	layout->addWidget(m_scrollbar);
	shellScrollable->setLayout(layout);

	m_window = new QSplitter();
	m_window->addWidget(m_tree);
	m_window->addWidget(shellScrollable);

	const int splitterWidth{ m_window->width() };
	m_window->setSizes({ splitterWidth * 25 / 100, splitterWidth * 75 / 100 });

	m_stack.insertWidget(1, m_window);
	m_stack.setCurrentIndex(1);

	connect(m_shell, &Shell::neovimAttachmentChanged, this, &MainWindow::handleNeovimAttachment);
	connect(m_shell, SIGNAL(neovimTitleChanged(QString)),
			this, SLOT(neovimSetTitle(QString)));
	connect(m_shell, &Shell::neovimResized,
			this, &MainWindow::neovimWidgetResized);
	connect(m_shell, &Shell::neovimMaximized,
			this, &MainWindow::neovimMaximized);
	connect(m_shell, &Shell::neovimForeground,
			this, &MainWindow::neovimForeground);
	connect(m_shell, &Shell::neovimSuspend,
			this, &MainWindow::neovimSuspend);
	connect(m_shell, &Shell::neovimFullScreen,
			this, &MainWindow::neovimFullScreen);
	connect(m_shell, &Shell::neovimFrameless,
			this, &MainWindow::neovimFrameless);

	// neovimGuiCloseRequest can't be a DirectConnection, since
	// it's not invoked when it is still running. We need it to
	// start executing right away.
	connect(m_shell, &Shell::neovimGuiCloseRequest,
			this, &MainWindow::neovimGuiCloseRequest,Qt::ConnectionType::QueuedConnection);
	connect(m_shell, &Shell::neovimOpacity,
			this, &MainWindow::setWindowOpacity);
	connect(m_nvim, &NeovimConnector::processExited,
			this, &MainWindow::neovimExited);
	connect(m_nvim, &NeovimConnector::error,
			this, &MainWindow::neovimError);
	connect(m_shell, &Shell::neovimIsUnsupported,
			this, &MainWindow::neovimIsUnsupported);
	connect(m_shell, &Shell::neovimShowContextMenu, m_contextMenu, &ContextMenu::showContextMenu);

	// GuiAdaptive Color/Font/Style Signal/Slot Connections
	connect(m_shell, &Shell::setGuiAdaptiveColorEnabled,
			this, &MainWindow::setGuiAdaptiveColorEnabled);
	connect(m_shell, &Shell::setGuiAdaptiveFontEnabled,
			this, &MainWindow::setGuiAdaptiveFontEnabled);
	connect(m_shell, &Shell::setGuiAdaptiveStyle,
			this, &MainWindow::setGuiAdaptiveStyle);
	connect(m_shell, &Shell::showGuiAdaptiveStyleList,
			this, &MainWindow::showGuiAdaptiveStyleList);
	connect(m_shell, &Shell::colorsChanged,
			this, &MainWindow::updateAdaptiveColor);

	m_shell->setFocus(Qt::OtherFocusReason);

	if (m_nvim->errorCause()) {
		neovimError(m_nvim->errorCause());
	}
}

/** The Neovim process has exited */
void MainWindow::neovimExited(int status)
{
	status = m_exitStatus; //status is only 1 byte so we use m_exitStatus
	if (m_nvim->errorCause() != NeovimConnector::NoError) {
		m_errorWidget->setText(m_nvim->errorString());
		m_errorWidget->showReconnect(m_nvim->canReconnect());
		m_stack.setCurrentIndex(0);
	} else if (status != 0) {
		m_errorWidget->setText(QStringLiteral("Neovim exited with status code (%1)").arg(status));
		m_errorWidget->showReconnect(m_nvim->canReconnect());
		m_stack.setCurrentIndex(0);
	}
}
void MainWindow::neovimError(NeovimConnector::NeovimError err)
{
	switch(err) {
	case NeovimConnector::FailedToStart:
		m_errorWidget->setText("Unable to start nvim: " + m_nvim->errorString() + "<br>("
			+ m_nvim->connectionDescription() + ")");
		break;
	default:
		m_errorWidget->setText(
			m_nvim->errorString() + "<br>(" + m_nvim->connectionDescription() + ")");
	}
	m_errorWidget->showReconnect(m_nvim->canReconnect());
	m_stack.setCurrentIndex(0);
}
void MainWindow::neovimIsUnsupported()
{
	m_errorWidget->setText(
		QStringLiteral("Cannot connect to this Neovim, required API version 1, found [%1-%2]")
			.arg(m_nvim->apiCompatibility())
			.arg(m_nvim->apiLevel()));
	m_errorWidget->showReconnect(m_nvim->canReconnect());
	m_stack.setCurrentIndex(0);
}

void MainWindow::neovimSetTitle(const QString &title)
{
	if (title.isEmpty()) {
		setWindowTitle(DefaultWindowTitle());
		return;
	}

	setWindowTitle(title);
}

void MainWindow::neovimWidgetResized()
{
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

void MainWindow::neovimFrameless(bool isFrameless)
{
	// When minimum Qt 5.9 is supported, use this instead:
	//  setWindowFlag(Qt::FramelessWindowHint, isFrameless);
	if (isFrameless)
	{
		setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
	}
	else
	{
		setWindowFlags(windowFlags() & ~Qt::FramelessWindowHint);
	}

	// Calling setWindowFlags can cause the widget to be hidden, show() must
	// be called to make the widget visible again. On some platforms, this may
	// cause focus issue. See Issue#971.
	//
	// Details: https://doc.qt.io/qt-5/qwidget.html#windowFlags-prop
	show();

	m_nvim->api0()->vim_set_var("GuiWindowFrameless", isFrameless ? 1 : 0);

}

void MainWindow::neovimForeground()
{
	setWindowState(windowState() & ~Qt::WindowMinimized);
	show();
	activateWindow();
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

void MainWindow::neovimGuiCloseRequest(int status)
{
	m_exitStatus = status;
	if (!m_inCloseEvent) {
		close();
	}
}

void MainWindow::reconnectNeovim()
{
	if (m_nvim->canReconnect()) {
		init(m_nvim->reconnect());
	}
	m_stack.setCurrentIndex(1);
}

void MainWindow::handleClosing()
{
	// Do not save window geometry in '--fullscreen' mode. If saved, all
	// subsequent Neovim-Qt sessions would default to fullscreen mode.
	if (!isFullScreen()) {
		saveWindowGeometry();
	}
	emit closing(m_exitStatus);
}
void MainWindow::emitForceClose() const noexcept {
	emit m_shell->forceQuit();
}

void MainWindow::closeEvent(QCloseEvent *ev)
{
	m_inCloseEvent = true;

	if (m_shell->close()) {
		ev->accept();
		handleClosing();
	} else {
		ev->ignore();
		m_inCloseEvent = false;
	}
}
void MainWindow::changeEvent(QEvent* ev)
{
	if (m_shell && ev->type() == QEvent::WindowStateChange && isWindow()) {
		m_shell->updateGuiWindowState(windowState());

		m_isActive = (windowState() == Qt::WindowState::WindowActive);
		emit activeChanged(*this);
	}
	QWidget::changeEvent(ev);
}

void MainWindow::handleNeovimAttachment(bool attached)
{
	emit neovimAttachmentChanged(attached);

	if (!attached) {
		return;
	}
	m_exitStatus = 0;

	if (m_shell && isWindow()) {
		m_shell->updateGuiWindowState(windowState());
	}
}

Shell* MainWindow::shell()
{
	return m_shell;
}

void MainWindow::saveWindowGeometry()
{
	QSettings settings("nvim-qt", "window-geometry");
	const bool restore_window_geometry{ settings.value("restore_window_geometry", true).toBool() };
	if (!restore_window_geometry) {
		return;
	}
	settings.setValue("restore_window_geometry", restore_window_geometry);
	settings.setValue("window_geometry", saveGeometry());
	settings.setValue("window_state", saveState());
}

void MainWindow::restoreWindowGeometry()
{
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
	// Workaround for error `QVariant::save: unable to save type 'QList<int>'`.
	// This error can occur when calling the two `settings.value(...)` below.
	qRegisterMetaTypeStreamOperators<QList<int>>("QList<int>");
#endif

	QSettings settings("nvim-qt", "window-geometry");
	if (!settings.value("restore_window_geometry", true).toBool()) {
		return;
	}
	const QVariant geometry{ settings.value("window_geometry") };
	if (geometry.isValid()) {
		restoreGeometry(geometry.toByteArray());
	}
	else {
		SetDefaultWindowSize(*this);
	}
	const QVariant state{ settings.value("window_state") };
	if (state.isValid()) {
		restoreState(state.toByteArray());
	}
}

void MainWindow::setGuiAdaptiveColorEnabled(bool isEnabled)
{
	m_isAdaptiveColorEnabled = isEnabled;

	updateAdaptiveColor();
}

void MainWindow::setGuiAdaptiveFontEnabled(bool isEnabled)
{
	m_isAdaptiveFontEnabled = isEnabled;

	updateAdaptiveFont();
}

void MainWindow::setGuiAdaptiveStyle(const QString& styleName)
{
	// The style may be empty if the name is invalid. This appears to be safe,
	// calling setStyle(nullptr) will restore the default Qt Style.
	QStyle* pStyle{ QStyleFactory::create(styleName) };

	auto childrenWidgets{ findChildren<QWidget*>() + m_shell->findChildren<QWidget*>() };

	for (const auto childWidget : childrenWidgets) {
		childWidget->setStyle(pStyle);
	}

	setStyle(pStyle);
}

void MainWindow::showGuiAdaptiveStyleList()
{
	const QString styleKeys{ QStyleFactory::keys().join("\n") };
	QString echoCommand{ R"(echo "%1")" };
	m_nvim->api0()->vim_command(echoCommand.arg(styleKeys).toLatin1());
}

void MainWindow::updateAdaptiveFont() noexcept
{
	if (!m_shell) {
		return;
	}

	const QFont& font{ (m_isAdaptiveFontEnabled) ?
		m_shell->font() : m_defaultFont };

	setFont(font);

	auto childrenWidgets{ findChildren<QWidget*>() + m_shell->findChildren<QWidget*>() };

	for (const auto childWidget : childrenWidgets) {
		// Do not call setFont() on ShellWidget objects
		if (qobject_cast<ShellWidget*>(childWidget)) {
			continue;
		}

		childWidget->setFont(font);
	}
}

static QPalette CreatePaletteFromHighlightGroups(const Shell& shell) noexcept
{
	const QColor& background{ shell.background() };
	const QColor& foreground{ shell.foreground() };

	QPalette palette;
	palette.setColor(QPalette::Window, background);
	palette.setColor(QPalette::WindowText, foreground);
	palette.setColor(QPalette::Base, background);
	palette.setColor(QPalette::Text, foreground);
	palette.setColor(QPalette::Button, background);
	palette.setColor(QPalette::ButtonText, foreground);

	return palette;
}

void MainWindow::updateAdaptiveColor() noexcept
{
	if (!m_shell) {
		return;
	}

	auto setPaletteAllChildren = [&](const QPalette& palette) noexcept
	{
		setPalette(palette);
		auto childrenWidgets{ findChildren<QWidget*>() + m_shell->findChildren<QWidget*>() };

		for (const auto childWidget : childrenWidgets) {
			childWidget->setPalette(palette);
		}
	};

	if (!m_isAdaptiveColorEnabled) {
		setPaletteAllChildren(m_defaultPalette);
		return;
	}

	setPaletteAllChildren(CreatePaletteFromHighlightGroups(*m_shell));

	// Some widgets support specialized palettes
	PopupMenu& popupMenu{ m_shell->getPopupMenu() };
	const bool isPopupMenuSupported { m_shell->IsHighlightGroup("Pmenu")
		&& m_shell->IsHighlightGroup("PmenuSel") };
	if (isPopupMenuSupported) {
		QPalette palette;

		const HighlightAttribute pmenu{ m_shell->GetHighlightGroup("Pmenu") };
		palette.setColor(QPalette::Base, pmenu.GetBackgroundColor());
		palette.setColor(QPalette::Text, pmenu.GetForegroundColor());

		const HighlightAttribute pmenusel{ m_shell->GetHighlightGroup("PmenuSel") };
		palette.setColor(QPalette::Highlight, pmenusel.GetBackgroundColor());
		palette.setColor(QPalette::HighlightedText, pmenusel.GetForegroundColor());

		auto childrenWidgets{ popupMenu.findChildren<QWidget*>() };

		for (const auto childWidget : childrenWidgets) {
			childWidget->setPalette(palette);
		}

		popupMenu.setPalette(palette);
	}
}

} // namespace NeovimQt
