#include <QApplication>
#include <QFontDatabase>
#include <QtGlobal>
#include <QFile>
#include <QCommandLineParser>
#include <QFileInfo>
#include <QDir>
#include "neovimconnector.h"
#include "mainwindow.h"


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

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	app.setApplicationDisplayName("Neovim");
	app.setWindowIcon(QIcon(":/neovim.png"));

	if (!qgetenv("NVIM_QT_LOG").isEmpty()) {
		qInstallMessageHandler(logger);
	}

	QCommandLineParser parser;
	parser.addOption(QCommandLineOption("embed",
		QCoreApplication::translate("main", "Communicate with Neovim over stdin/out")));
	parser.addOption(QCommandLineOption("server",
		QCoreApplication::translate("main", "Connect to existing Neovim instance"),
		QCoreApplication::translate("main", "addr")));
	parser.addOption(QCommandLineOption("geometry",
		QCoreApplication::translate("main", "Initial window geometry"),
		QCoreApplication::translate("main", "geometry")));
	parser.addOption(QCommandLineOption("maximized",
		QCoreApplication::translate("main", "Maximize the window on startup")));
	parser.addOption(QCommandLineOption("fullscreen",
		QCoreApplication::translate("main", "Open the window in fullscreen on startup")));
	parser.addPositionalArgument("...", "Additional arguments are fowarded to Neovim", "[-- ...]");
	parser.addHelpOption();

	int sep = app.arguments().indexOf("--");
	QStringList neovimArgs;
	if (sep != -1) {
		QStringList args = app.arguments().mid(0, sep);
		neovimArgs += app.arguments().mid(sep+1);
		parser.process(app.arguments());
	} else {
		parser.process(app.arguments());
	}

	if (parser.isSet("help")) {
		parser.showHelp();
	}

	NeovimQt::NeovimConnector *c;
	if (parser.isSet("embed")) {
		c = NeovimQt::NeovimConnector::fromStdinOut();
	} else {
		if (parser.isSet("server")) {
			QString server = parser.value("server");
			c = NeovimQt::NeovimConnector::connectToNeovim(server);
		} else {
			auto path = qgetenv("NVIM_QT_RUNTIME_PATH");
			if (QFileInfo(path).isDir()) {
				neovimArgs.insert(0, "--cmd");
				neovimArgs.insert(1, QString("set rtp+=%1")
						.arg(QString::fromLocal8Bit(path)));
			}
#ifdef NVIM_QT_RUNTIME_PATH
			else if (QFileInfo(NVIM_QT_RUNTIME_PATH).isDir()) {
				neovimArgs.insert(0, "--cmd");
				neovimArgs.insert(1, QString("set rtp+=%1")
						.arg(NVIM_QT_RUNTIME_PATH));
			} else
#endif
			{
				QDir d = QFileInfo(QCoreApplication::applicationDirPath()).dir();
				d.cd("share");
				d.cd("nvim-qt");
				d.cd("runtime");

				if (d.exists()) {
					neovimArgs.insert(0, "--cmd");
					neovimArgs.insert(1, QString("set rtp+=%1")
							.arg(d.path()));
				}
			}
			c = NeovimQt::NeovimConnector::spawn(neovimArgs);
		}
	}

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
	if (parser.isSet("fullscreen")) {
		win->delayedShow(NeovimQt::MainWindow::DelayedShow::FullScreen);
	} else if (parser.isSet("maximized")) {
		win->delayedShow(NeovimQt::MainWindow::DelayedShow::Maximized);
	} else {
		win->delayedShow();
	}
#endif
	return app.exec();
}

