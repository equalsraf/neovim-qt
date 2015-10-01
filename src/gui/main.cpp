#include <QApplication>
#include <QFontDatabase>
#include <QtGlobal>
#include <QFile>
#include "neovimconnector.h"
#include "mainwindow.h"

/**
 * A log handler for Qt messages, all messages are dumped into the file
 * passed via the NVIM_QT_LOG variable. Some information is only available
 * in debug builds (e.g. qDebug is only called in debug builds).
 *
 * In UNIX Qt prints messages to the console output, but in Windows this is
 * the only way to get Qt's debug/warning messages.
 */
void logger(QtMsgType type, const QMessageLogContext& ctx, const QString& msg)
{
	QFile logFile(qgetenv("NVIM_QT_LOG"));
	if (logFile.open(QIODevice::Append | QIODevice::Text)) {
		QTextStream stream(&logFile);
		stream << msg << "\n";
	}
}

/**
 * Neovim Qt GUI
 *
 * Usage:
 *   nvim-qt --server <SOCKET>
 *   nvim-qt [...]
 *
 * When --server is not provided, a Neovim instance will be spawned. All arguments
 * are passed to the Neovim process.
 */
int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	app.setApplicationDisplayName("Neovim");
	app.setWindowIcon(QIcon(":/neovim.png"));

	if (!qgetenv("NVIM_QT_LOG").isEmpty()) {
		qInstallMessageHandler(logger);
	}

	QStringList args = app.arguments().mid(1);
	NeovimQt::NeovimConnector *c;
	if (args.indexOf("--embed") != -1) {
		c = NeovimQt::NeovimConnector::fromStdinOut();
	} else {
		QString server;
		int serverIdx = args.indexOf("--server");
		if (serverIdx != -1 && args.size() > serverIdx+1) {
			server = args.at(serverIdx+1);
			args.removeAt(serverIdx);
			args.removeAt(serverIdx);
		}

		if (!server.isEmpty()) {
			c = NeovimQt::NeovimConnector::connectToNeovim(server);
		} else {
			c = NeovimQt::NeovimConnector::spawn(args);
		}
	}

#ifdef NEOVIMQT_GUI_WIDGET
	NeovimQt::Shell *win = new NeovimQt::Shell(c);
#else
	NeovimQt::MainWindow *win = new NeovimQt::MainWindow(c);
#endif
	win->show();
	return app.exec();
}

