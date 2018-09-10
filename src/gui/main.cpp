#include <QApplication>
#include <QtGlobal>
#include <QProcess>
#include <QFile>
#include <QCommandLineParser>
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

	QCommandLineParser parser;
	NeovimQt::App::processCliOptions(parser, app.arguments());

	int timeout = parser.value("timeout").toInt();
	auto c = app.createConnector(parser);
	c->setRequestTimeout(timeout);
	app.showUi(c, parser);
	return app.exec();
}

int cli_main(int argc, char **argv)
{
	QCoreApplication app(argc, argv);

	QCommandLineParser parser;
	NeovimQt::App::processCliOptions(parser, app.arguments());

	QStringList new_args = app.arguments().mid(1);
	new_args.insert(0, "--nofork");
	if (QProcess::startDetached(app.applicationFilePath(), new_args)) {
		return 0;
	} else {
		qWarning() << "Unable to fork into background";
		return -1;
	}
}

int main(int argc, char **argv)
{
#if defined (Q_OS_UNIX) && ! defined (Q_OS_MAC)
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

	if (nofork) {
		return ui_main(argc, argv);
	} else {
		return cli_main(argc, argv);
	}
#else
	return ui_main(argc, argv);
#endif
}

