#include <QApplication>
#include <QtGlobal>
#include <QProcess>
#include <QFile>
#include <QCommandLineParser>
#if defined (Q_OS_MAC)
# include <unistd.h>
# include <stdio.h>
#endif
#include "neovimconnector.h"
#include "app.h"

#if defined(Q_OS_WIN) && defined(USE_STATIC_QT)
#include <QtPlugin>
Q_IMPORT_PLUGIN (QWindowsIntegrationPlugin);
#endif

int ui_main(int argc, char **argv)
{

#ifdef Q_OS_WIN
	// Enables automatic high-DPI scaling
	// https://github.com/equalsraf/neovim-qt/issues/391
	//
	// The other way to do this is to set Qt::AA_EnableHighDpiScaling
	// but it does not seem to work on Windows.
	//
	// @equalsraf: For now I'm setting this in windows only, there open
	// issues in upstream Qt that suggests this may have unexpected effects
	// in other systems (QTBUG-65061, QTBUG-65102), also QTBUG-63580 offers
	// contradictory information with what we have experienced.
	qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", "1");
#endif

	NeovimQt::App app(argc, argv);

	app.checkArgumentsMayTerminate(app.commandLineParser());

	app.showUi();
	return app.exec();
}

int cli_main(int argc, char **argv)
{
	// Issue#641: geometry arguments are truncated by NeovimQt::App(...).
	//
	// Arguments recognized by QApplication are removed from the argument list.
	// All arguments need to be forwarded to the --nofork call. Save any arguments
	// before calling the App CTOR, which may remove arguments.
	QStringList argsNoFork{ "--nofork" };
	for (int i = 1; i < argc; i++)
	{
		argsNoFork << argv[i];
	}

	QCoreApplication app(argc, argv);
	QCommandLineParser p;
	NeovimQt::App::processCommandlineOptions(p, app.arguments());
	NeovimQt::App::checkArgumentsMayTerminate(p);

	if (!QProcess::startDetached(app.applicationFilePath(), argsNoFork)) {
		qWarning() << "Unable to fork into background";
		return -1;
	}

	return 0;
}

int main(int argc, char **argv)
{
#if defined (Q_OS_UNIX)
	// Do an early check for --nofork. We need to do this here, before
	// creating a QApplication, since QCommandLineParser requires one
	bool nofork = false;
	for (int i=1; i<argc; i++) {
		if (QString::compare("--", argv[i]) == 0) {
			break;
		} else if (QString::compare("--spawn", argv[i]) == 0) {
			break;
		} else if (QString::compare("--embed", argv[i]) == 0) {
			// Due to https://github.com/equalsraf/neovim-qt/issues/276 --embed
			// implies --nofork
			nofork = true;
		} else if (QString::compare("--nofork", argv[i]) == 0) {
			nofork = true;
		}
	}

#	if defined (Q_OS_MAC)
	// In MacOS X do not fork when the process is not launched from a tty
	if (nofork == false && isatty(fileno(stdin)) != 1) {
		qDebug() << "stdin is not a tty, forcing --nofork";
		nofork = true;
	}
#	endif

	if (nofork) {
		return ui_main(argc, argv);
	} else {
		return cli_main(argc, argv);
	}
#else
	return ui_main(argc, argv);
#endif
}

