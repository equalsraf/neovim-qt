#include "app.h"

#include <QFileOpenEvent>
#include <QTextStream>
#include <QFileInfo>
#include <QDir>
#include "mainwindow.h"

namespace NeovimQt {

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

App::App(int &argc, char ** argv)
:QApplication(argc, argv)
{
	setWindowIcon(QIcon(":/neovim.png"));
	setApplicationDisplayName("Neovim");

#ifdef Q_OS_MAC
	QByteArray shellPath = qgetenv("SHELL");
	if (!getLoginEnvironment(shellPath)) {
		getLoginEnvironment("/bin/bash");
	}
#endif

	if (!qgetenv("NVIM_QT_LOG").isEmpty()) {
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
}

bool App::event(QEvent *event)
{
	if( event->type()  == QEvent::FileOpen) {
		QFileOpenEvent * fileOpenEvent = static_cast<QFileOpenEvent *>(event);
		if(fileOpenEvent) {
			emit openFilesTriggered({fileOpenEvent->url()});
		}
	}
	return QApplication::event(event);
}

void App::showUi(NeovimConnector *c, const QCommandLineParser& parser)
{
#ifdef NEOVIMQT_GUI_WIDGET
	NeovimQt::Shell *win = new NeovimQt::Shell(c);
	win->show();
	if (parser.isSet("fullscreen")) {
		win->showFullScreen();
	} else if (parser.isSet("maximized")) {
		win->showMaximized();
	} else {
		win->show();
	}
#else
	NeovimQt::MainWindow *win = new NeovimQt::MainWindow(c);

	QObject::connect(instance(), SIGNAL(openFilesTriggered(const QList<QUrl>)),
		win->shell(), SLOT(openFiles(const QList<QUrl>)));

	if (parser.isSet("fullscreen")) {
		win->delayedShow(NeovimQt::MainWindow::DelayedShow::FullScreen);
	} else if (parser.isSet("maximized")) {
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
void App::processCliOptions(QCommandLineParser &parser, const QStringList& arguments)
{
	parser.addOption(QCommandLineOption("nvim",
				QCoreApplication::translate("main", "nvim executable path"),
				QCoreApplication::translate("main", "nvim_path"),
				"nvim"));
	parser.addOption(QCommandLineOption("timeout",
				QCoreApplication::translate("main", "Error if nvim does not responde after count milliseconds"),
				QCoreApplication::translate("main", "ms"),
				"10000"));
	parser.addOption(QCommandLineOption("geometry",
				QCoreApplication::translate("main", "Initial window geometry"),
				QCoreApplication::translate("main", "geometry")));
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
	parser.addHelpOption();

#ifdef Q_OS_UNIX
	parser.addOption(QCommandLineOption("nofork",
				QCoreApplication::translate("main", "Run in foreground")));
#endif

	parser.addPositionalArgument("file",
			QCoreApplication::translate("main", "Edit specified file(s)"), "[file...]");
	parser.addPositionalArgument("...", "Additional arguments are fowarded to Neovim", "[-- ...]");

	parser.process(arguments);

	if (parser.isSet("help")) {
		parser.showHelp();
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

NeovimConnector* App::createConnector(const QCommandLineParser& parser)
{
	if (parser.isSet("embed")) {
		return NeovimQt::NeovimConnector::fromStdinOut();
	} else if (parser.isSet("server")) {
		QString server = parser.value("server");
		return NeovimQt::NeovimConnector::connectToNeovim(server);
	} else if (parser.isSet("spawn") && !parser.positionalArguments().isEmpty()) {
		const QStringList& args = parser.positionalArguments();
		return NeovimQt::NeovimConnector::spawn(args.mid(1), args.at(0));
	} else {
		QStringList neovimArgs;
		neovimArgs << "--cmd";
		neovimArgs << "set termguicolors";
		auto path = qgetenv("NVIM_QT_RUNTIME_PATH");
		if (QFileInfo(path).isDir()) {
			neovimArgs.insert(0, "--cmd");
			neovimArgs.insert(1, QString("let &rtp.=',%1'")
					.arg(QString::fromLocal8Bit(path)));
		}
#ifdef NVIM_QT_RUNTIME_PATH
		else if (QFileInfo(NVIM_QT_RUNTIME_PATH).isDir()) {
			neovimArgs.insert(0, "--cmd");
			neovimArgs.insert(1, QString("let &rtp.=',%1'")
					.arg(NVIM_QT_RUNTIME_PATH));
		} else
#endif
		{
			// Look for the runtime relative to the nvim-qt binary
			QDir d = QFileInfo(QCoreApplication::applicationDirPath()).dir();
#ifdef Q_OS_MAC
			// within the bundle at ../Resources/runtime
			d.cd("Resources");
			d.cd("runtime");
#else
			// ../share/nvim-qt/runtime
			d.cd("share");
			d.cd("nvim-qt");
			d.cd("runtime");
#endif

			if (d.exists()) {
				neovimArgs.insert(0, "--cmd");
				neovimArgs.insert(1, QString("let &rtp.=',%1'")
						.arg(d.path()));
			}
		}

		// Pass positional file arguments to Neovim
		neovimArgs.append(parser.positionalArguments());
		return NeovimQt::NeovimConnector::spawn(neovimArgs, parser.value("nvim"));
	}
}


} // Namespace
