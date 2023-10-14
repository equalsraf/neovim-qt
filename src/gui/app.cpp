#include "app.h"

#include <algorithm>
#include <QDir>
#include <QFileInfo>
#include <QFileOpenEvent>
#include <QSettings>
#include <QTextStream>
#include <vector>

#include "arguments.h"
#include "mainwindow.h"
#include "printinfo.h"
#include "version.h"

namespace NeovimQt {

namespace {

// TODO Issue#900: Cleanup Clazy warning. Optional: remove global statics.
MainWindow* s_lastActiveWindow;
std::vector<MainWindow*> s_windows; // clazy:exclude=non-pod-global-static
int s_exitStatus{ 0 };

struct ConnectorInitArgs
{
	enum class Type
	{
		Embed,
		Server,
		Spawn,
		Default
	};

	const Type type{ Type ::Default };
	const int timeout{ 10000 };
	const QString server;
	const QString nvim;
	const QStringList positionalArgs;
	const QStringList neovimArgs;

	ConnectorInitArgs(const QCommandLineParser& parser, QStringList nvimArgs) noexcept;

	ConnectorInitArgs(
		Type _type, int _timeout, QString _server, QString _nvim, QStringList nvimArgs) noexcept;
};

ConnectorInitArgs::Type getConnectorType(const QCommandLineParser& parser) noexcept
{
	if (parser.isSet("server")) {
		return ConnectorInitArgs::Type::Server;
	}

	if (parser.isSet("embed")) {
		return ConnectorInitArgs::Type::Embed;
	}

	if (parser.isSet("spawn") && !parser.positionalArguments().isEmpty()) {
		return ConnectorInitArgs::Type::Spawn;
	}

	return ConnectorInitArgs::Type::Default;
}

ConnectorInitArgs::ConnectorInitArgs(
	const QCommandLineParser& parser, QStringList nvimArgs) noexcept
	: type{ getConnectorType(parser) }
	, timeout{ parser.value("timeout").toInt() }
	, server{ parser.value("server") }
	, nvim{ parser.value("nvim") }
	, positionalArgs{ parser.positionalArguments() }
	, neovimArgs{ std::move(nvimArgs) }
{
}

ConnectorInitArgs::ConnectorInitArgs(
	Type _type, int _timeout, QString _server, QString _nvim, QStringList nvimArgs) noexcept
	: type{ _type }
	, timeout{ _timeout }
	, server{ std::move(_server) }
	, nvim{ std::move(_nvim) }
	, neovimArgs{ std::move(nvimArgs) }
{
}

NeovimConnector& connectToRemoteNeovim(const ConnectorInitArgs& args) noexcept
{
	NeovimConnector* connector{ nullptr };

	switch (args.type)
	{
		case ConnectorInitArgs::Type::Embed:
			connector = NeovimConnector::fromStdinOut();
			break;

		case ConnectorInitArgs::Type::Server:
			connector = NeovimConnector::connectToNeovim(args.server);
			break;

		case ConnectorInitArgs::Type::Spawn:
			if (args.positionalArgs.size() >= 2) {
				connector =
					NeovimConnector::spawn(args.positionalArgs.mid(1), args.positionalArgs.at(0));
			}
			break;

		case ConnectorInitArgs::Type::Default:
			break;
	};

	if (!connector) {
		connector = NeovimConnector::spawn(args.neovimArgs + args.positionalArgs, args.nvim);
	}

	connector->setRequestTimeout(args.timeout);
	return *connector;
}

void onWindowActiveChanged(MainWindow& window) noexcept
{
	s_lastActiveWindow = &window;
}

void onFileOpenEvent(const QList<QUrl>& files) noexcept
{
	s_lastActiveWindow->shell()->openFiles(files);
}

void onWindowClosing(int status) noexcept
{
	s_exitStatus = status;
}

void onWindowDestroyed(QObject* obj) noexcept
{
	s_windows.erase(std::remove(s_windows.begin(), s_windows.end(), (MainWindow*)obj));

	if (s_windows.empty()) {
		qApp->exit(s_exitStatus);
	}
}

MainWindow& createWindow(const ConnectorInitArgs& args) noexcept
{
	NeovimConnector& conn{ connectToRemoteNeovim(args) };
	MainWindow* win{ new MainWindow(&conn) };

	win->setAttribute(Qt::WA_DeleteOnClose);

	App* app{ qobject_cast<App*>(App::instance()) };

	QObject::connect(win, &MainWindow::closing, app, onWindowClosing);
	QObject::connect(win, &MainWindow::destroyed, app, onWindowDestroyed);
	QObject::connect(win, &MainWindow::activeChanged, app, onWindowActiveChanged);

	s_lastActiveWindow = win;
	s_windows.emplace_back(win);
	return *win;
}

} // namespace

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

	setOrganizationName("nvim-qt");
	setApplicationName("nvim-qt");

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

bool App::event(QEvent* event) noexcept
{
	if (event->type() == QEvent::FileOpen) {
		QFileOpenEvent* fileOpenEvent{ static_cast<QFileOpenEvent*>(event) };
		onFileOpenEvent({ fileOpenEvent->url() });
	}
	else if (event->type() == QEvent::Quit) {
		for (auto window : s_windows) {
			if (!window->close()) {
				event->setAccepted(false);
			}
		}

		return event->isAccepted();
	}

	return QApplication::event(event);
}

void App::showUi() noexcept
{
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

	ConnectorInitArgs args{ m_parser, getNeovimArgs() };
	MainWindow* win{ &createWindow(args) };

	// delete the main window when closed to emit `destroyed()` signal to
	// support `:cq` return codes (Pull#644).
	setQuitOnLastWindowClosed(false);

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
		win->showFullScreen();
	} else if (m_parser.isSet("maximized")) {
		win->showMaximized();
	} else {
		win->show();
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
				QCoreApplication::translate("main", "Error if nvim does not respond after count milliseconds"),
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

/*static*/ QString App::getRuntimePath() noexcept
{
	QString path{ QString::fromLocal8Bit(qgetenv("NVIM_QT_RUNTIME_PATH")) };

	if (QFileInfo(path).isDir()) {
		return path;
	}

	// Look for the runtime relative to the nvim-qt binary
	const QDir d{ QDir{ applicationDirPath() }.filePath(NVIM_QT_RELATIVE_RUNTIME_PATH) };
	if (d.exists()) {
		return d.path();
	}

	return {};
}

/*static*/ QStringList App::getNeovimArgs() noexcept
{
	QStringList neovimArgs{ "--cmd","set termguicolors" };

	QString runtimePath{ getRuntimePath() };
	if (runtimePath.isEmpty()) {
		return { "--cmd","set termguicolors" };
	}

	return { "--cmd", QString{ "let &rtp.=',%1'" }.arg(runtimePath),
		"--cmd","set termguicolors" };
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

void App::openNewWindow(const QVariantList& args) noexcept
{
	QString server;
	QString nvim{ "nvim" };
	ConnectorInitArgs::Type type{ ConnectorInitArgs::Type::Default };
	if (args.size() > 1 && args.at(1).type() == QVariant::Type::Map) {
		const QVariantMap initMap{ args.at(1).toMap() };

		if (initMap.contains("nvim")) {
			nvim = initMap.value("nvim").toString();
		}

		if (initMap.contains("server")) {
			server = initMap.value("server").toString();
			type = ConnectorInitArgs::Type::Server;
		}
	}

	ConnectorInitArgs params{ type, 2000, std::move(server), std::move(nvim), getNeovimArgs() };
	MainWindow* win{ &createWindow(params) };
	win->resize(s_lastActiveWindow->size());
	win->show();
}

} // namespace NeovimQt
