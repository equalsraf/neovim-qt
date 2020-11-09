#include "app.h"

#include <QDir>
#include <QFileInfo>
#include <QFileOpenEvent>
#include <QSettings>
#include <QTextStream>

#include "arguments.h"
#include "mainwindow.h"
#include "printinfo.h"
#include "version.h"

namespace NeovimQt {

static ShellOptions GetShellOptionsFromQSettings() noexcept;

/// A log handler for Qt messages, all messages are dumped into the file
/// passed via the NVIM_QT_LOG variable. Some information is only available
/// in debug builds (e.g. qDebug is only called in debug builds).
///
/// In UNIX Qt prints messages to the console output, but in Windows this is
/// the only way to get Qt's debug/warning messages.
void logger(QtMsgType type, const QMessageLogContext& ctx, const QString& msg)
{
	QFile logFile(qgetenv("NVIM_QT_LOG"));
	if (logFile.open(QIODevice::Append | QIODevice::Text)) {
		QTextStream stream(&logFile);
		stream << msg << "\n";
	}
}

#ifdef Q_OS_MAC
bool getLoginEnvironment(const QString& path)
{
	QProcess proc;
	proc.start(path, {"-l", "-c", "env", "-i"});
	if (!proc.waitForFinished()) {
		qDebug() << "Failed to execute shell to get environemnt" << path;
		return false;
	}

	QByteArray out = proc.readAllStandardOutput();
	foreach(const QByteArray& item, out.split('\n')) {
		int index = item.indexOf('=');
		if (index > 0) {
			qputenv(item.mid(0, index), item.mid(index+1));
			qDebug() << item.mid(0, index) << item.mid(index+1);
		}
	}
	return true;
}
#endif

App::App(int &argc, char ** argv) noexcept
:QApplication(argc, argv)
{
	setWindowIcon(QIcon(":/neovim.svg"));
	setApplicationDisplayName("Neovim");

#ifdef Q_OS_MAC
	QByteArray shellPath = qgetenv("SHELL");
	if (!getLoginEnvironment(shellPath)) {
		getLoginEnvironment("/bin/bash");
	}
#endif

	if (!qEnvironmentVariableIsEmpty("NVIM_QT_LOG")) {
		qInstallMessageHandler(logger);
	}

	QByteArray stylesheet_path = qgetenv("NVIM_QT_STYLESHEET");
	if (!stylesheet_path.isEmpty()) {
		QFile qssfile(stylesheet_path);
		if (qssfile.open(QIODevice::ReadOnly)) {
			setStyleSheet(qssfile.readAll());
		} else {
			qWarning("Unable to open stylesheet from $NVIM_QT_STYLESHEET");
		}
	}

	processCommandlineOptions(m_parser, arguments());
}

bool App::event(QEvent *event) noexcept
{
	if( event->type()  == QEvent::FileOpen) {
		QFileOpenEvent * fileOpenEvent = static_cast<QFileOpenEvent *>(event);
		if(fileOpenEvent) {
			emit openFilesTriggered({fileOpenEvent->url()});
		}
	}
	return QApplication::event(event);
}

void App::showUi() noexcept
{
	ShellOptions opts{ GetShellOptionsFromQSettings() };

	if (m_parser.isSet("no-ext-tabline")) {
		opts.enable_ext_tabline = false;
	}

	if (m_parser.isSet("no-ext-popupmenu")) {
		opts.enable_ext_popupmenu = false;
	}

#ifdef NEOVIMQT_GUI_WIDGET
	NeovimQt::Shell *win = new NeovimQt::Shell(c);
	win->show();
	if (m_parser.isSet("fullscreen")) {
		win->showFullScreen();
	} else if (m_parser.isSet("maximized")) {
		win->showMaximized();
	} else {
		win->show();
	}
#else
	NeovimQt::MainWindow *win = new NeovimQt::MainWindow(m_connector.get(), opts);

	QObject::connect(instance(), SIGNAL(openFilesTriggered(QList<QUrl>)),
		win->shell(), SLOT(openFiles(QList<QUrl>)));

	// Window geometry should be restored only when the user does not specify
	// one of the following command line arguments. Argument "maximized" can
	// be safely ignored, as the loaded geometry only takes effect after the
	// user un-maximizes the window; this behavior is desirable. Function
	// `isSet` will issue qWarning() messages if the argument doesn't exist.
	// Do not call isSet(...) for arguments which may not exist.
	if (!m_parser.isSet("fullscreen") &&
		(!hasGeometryArg() || !m_parser.isSet("geometry")) &&
		(!hasQWindowGeometryArg() || !m_parser.isSet("qwindowgeometry")))
	{
		win->restoreWindowGeometry();
	}

	if (m_parser.isSet("fullscreen")) {
		win->delayedShow(NeovimQt::MainWindow::DelayedShow::FullScreen);
	} else if (m_parser.isSet("maximized")) {
		win->delayedShow(NeovimQt::MainWindow::DelayedShow::Maximized);
	} else {
		win->delayedShow();
	}
#endif
}

/// Initialize CLI parser with all the nvim-qt options, process the
/// provided arguments and check for errors.
///
/// When appropriate this function will call QCommandLineParser::showHelp()
/// terminating the program.
void App::processCommandlineOptions(QCommandLineParser& parser, QStringList arguments) noexcept
{
	parser.addOption(QCommandLineOption("nvim",
				QCoreApplication::translate("main", "nvim executable path"),
				QCoreApplication::translate("main", "nvim_path"),
				"nvim"));
	parser.addOption(QCommandLineOption("timeout",
				QCoreApplication::translate("main", "Error if nvim does not responde after count milliseconds"),
				QCoreApplication::translate("main", "ms"),
				"20000"));

	// Some platforms use --qwindowgeometry, while other platforms use the --geometry.
	// Make the correct help message is displayed.
	if (hasGeometryArg()) {
		parser.addOption(QCommandLineOption("geometry",
			QCoreApplication::translate("main", "Set initial window geometry"),
			QCoreApplication::translate("main", "width>x<height")));
	}
	if (hasQWindowGeometryArg()) {
		parser.addOption(QCommandLineOption("qwindowgeometry",
			QCoreApplication::translate("main", "Set initial window geometry"),
			QCoreApplication::translate("main", "width>x<height")));
	}

	parser.addOption(QCommandLineOption("stylesheet",
				QCoreApplication::translate("main", "Apply qss stylesheet from file"),
				QCoreApplication::translate("main", "stylesheet")));
	parser.addOption(QCommandLineOption("maximized",
				QCoreApplication::translate("main", "Maximize the window on startup")));
	parser.addOption(QCommandLineOption("no-ext-tabline",
				QCoreApplication::translate("main", "Disable the external GUI tabline")));
	parser.addOption(QCommandLineOption("no-ext-popupmenu",
				QCoreApplication::translate("main", "Disable the external GUI popup menu")));
	parser.addOption(QCommandLineOption("fullscreen",
				QCoreApplication::translate("main", "Open the window in fullscreen on startup")));
	parser.addOption(QCommandLineOption("embed",
				QCoreApplication::translate("main", "Communicate with Neovim over stdin/out")));
	parser.addOption(QCommandLineOption("server",
				QCoreApplication::translate("main", "Connect to existing Neovim instance"),
				QCoreApplication::translate("main", "addr")));
	parser.addOption(QCommandLineOption("spawn",
				QCoreApplication::translate("main", "Treat positional arguments as the nvim argv")));
	parser.addOption(QCommandLineOption({ "v", "version" },
				QCoreApplication::translate("main", "Displays version information.")));

	parser.addHelpOption();

#ifdef Q_OS_UNIX
	parser.addOption(QCommandLineOption("nofork",
				QCoreApplication::translate("main", "Run in foreground")));
#endif

	parser.addPositionalArgument("file",
			QCoreApplication::translate("main", "Edit specified file(s)"), "[file...]");
	parser.addPositionalArgument("-- [nvim_args]", "Additional arguments are forwarded to Neovim: \"nvim-qt -- -u NONE\"", "[-- nvim_args]");

	parser.process(arguments);
}

void App::checkArgumentsMayTerminate(QCommandLineParser& parser) noexcept
{
	if (parser.isSet("help")) {
		parser.showHelp();
	}

	if (parser.isSet("version")) {
		showVersionInfo(parser);
		::exit(0);
	}

	int exclusive = parser.isSet("server") + parser.isSet("embed") + parser.isSet("spawn");
	if (exclusive > 1) {
		qWarning() << "Options --server, --spawn and --embed are mutually exclusive\n";
		::exit(-1);
	}

	if (!parser.positionalArguments().isEmpty() &&
			(parser.isSet("embed") || parser.isSet("server"))) {
		qWarning() << "--embed and --server do not accept positional arguments\n";
		::exit(-1);
	}

	if (parser.positionalArguments().isEmpty() && parser.isSet("spawn")) {
		qWarning() << "--spawn requires at least one positional argument\n";
		::exit(-1);
	}

	bool valid_timeout;
	int timeout_opt = parser.value("timeout").toInt(&valid_timeout);
	if (!valid_timeout || timeout_opt <= 0) {
		qWarning() << "Invalid argument for --timeout" << parser.value("timeout");
		::exit(-1);
	}
}

void App::setupRequestTimeout() noexcept
{
	if (!m_connector)
	{
		return;
	}

	m_connector->setRequestTimeout(m_parser.value("timeout").toInt());
}

QStringList App::getNeovimArgs() noexcept
{
	QStringList neovimArgs{ "--cmd","set termguicolors" };

	QString runtimePath{ getRuntimePath() };
	if (runtimePath.isEmpty()) {
		return { "--cmd","set termguicolors" };
	}

	return { "--cmd", QString{ "let &rtp.=',%1'" }.arg(runtimePath),
		"--cmd","set termguicolors" };
}

void App::connectToRemoteNeovim() noexcept
{
	if (m_parser.isSet("embed")) {
		m_connector = std::unique_ptr<NeovimConnector>{ NeovimQt::NeovimConnector::fromStdinOut() };
		setupRequestTimeout();
		return;
	}

	if (m_parser.isSet("server")) {
		QString server = m_parser.value("server");
		m_connector = std::unique_ptr<NeovimConnector>{ NeovimQt::NeovimConnector::connectToNeovim(server) };
		setupRequestTimeout();
		return;
	}

	if (m_parser.isSet("spawn") && !m_parser.positionalArguments().isEmpty()) {
		const QStringList& args = m_parser.positionalArguments();
		m_connector = std::unique_ptr<NeovimConnector> { NeovimQt::NeovimConnector::spawn(args.mid(1), args.at(0)) };
		setupRequestTimeout();
		return;
	}

	QStringList neovimArgs{ getNeovimArgs() };

	// Append positional file arguments to nvim.
	neovimArgs.append(m_parser.positionalArguments());

	m_connector = std::unique_ptr<NeovimConnector>{  NeovimQt::NeovimConnector::spawn(neovimArgs, m_parser.value("nvim")) };
	setupRequestTimeout();
	return;
}

static ShellOptions GetShellOptionsFromQSettings() noexcept
{
	ShellOptions opts{};
	QSettings settings("nvim-qt", "nvim-qt");

	QVariant ext_linegrid{ settings.value("ext_linegrid", opts.enable_ext_linegrid) };
	QVariant ext_popupmenu{ settings.value("ext_popupmenu", opts.enable_ext_popupmenu) };
	QVariant ext_tabline{ settings.value("ext_tabline", opts.enable_ext_popupmenu) };

	if (ext_linegrid.canConvert<bool>())
	{
		opts.enable_ext_linegrid = ext_linegrid.toBool();
	}

	if (ext_popupmenu.canConvert<bool>())
	{
		opts.enable_ext_popupmenu = ext_popupmenu.toBool();
	}

	if (ext_tabline.canConvert<bool>())
	{
		opts.enable_ext_tabline= ext_tabline.toBool();
	}

	return opts;
}

static QString GetNeovimVersionInfo(const QString& nvim) noexcept
{
	QProcess nvimproc;
	nvimproc.start(nvim, { "--version" });
	if (!nvimproc.waitForFinished(2000 /*msec*/)) {
		return QCoreApplication::translate("main", "Neovim Not Found!");
	}

	return nvimproc.readAllStandardOutput();
}

// TODO Issue#752: Remove `endl` and `QT_ENDL` wrapper
#if (QT_VERSION < QT_VERSION_CHECK(5, 14, 0))
	#define QT_ENDL endl
#else
	#define QT_ENDL Qt::endl
#endif

void App::showVersionInfo(QCommandLineParser& parser) noexcept
{
	QString versionInfo;
	QTextStream out{ &versionInfo };

	const QString nvimExecutable { (parser.isSet("nvim")) ?
		parser.value("nvim") : "nvim" };

	out << "NVIM-QT v" << PROJECT_VERSION << QT_ENDL;
	out << "Build type: " << CMAKE_BUILD_TYPE << QT_ENDL;
	out << "Compilation:" << CMAKE_CXX_FLAGS << QT_ENDL;
	out << "Qt Version: " << QT_VERSION_STR << QT_ENDL;
	out << "Environment: " << QT_ENDL;
	out << "  nvim: " << nvimExecutable << QT_ENDL;
	out << "  args: " << getNeovimArgs().join(" ") << QT_ENDL;
	out << "  runtime: " << getRuntimePath() << QT_ENDL;

	out << QT_ENDL;

	out << GetNeovimVersionInfo(nvimExecutable) << QT_ENDL;

	PrintInfo(versionInfo);
}

} // namespace NeovimQt
