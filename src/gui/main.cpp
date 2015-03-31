#include <QApplication>
#include <QFontDatabase>
#include "neovimconnector.h"
#include "shell.h"

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

	// Load bundled fonts
	if (QFontDatabase::addApplicationFont(":/DejaVuSansMono.ttf") == -1) {
		qWarning("Unable to load bundled font");
	}
	if (QFontDatabase::addApplicationFont(":/DejaVuSansMono-Bold.ttf") == -1) {
		qWarning("Unable to load bundled bold font");
	}
	if (QFontDatabase::addApplicationFont(":/DejaVuSansMono-Oblique.ttf") == -1) {
		qWarning("Unable to load bundled italic font");
	}
	if (QFontDatabase::addApplicationFont(":/DejaVuSansMono-BoldOblique.ttf") == -1) {
		qWarning("Unable to load bundled bold/italic font");
	}

	QString server;
	QStringList args = app.arguments().mid(1);
	int serverIdx = args.indexOf("--server");
	if (serverIdx != -1 && args.size() > serverIdx+1) {
		server = args.at(serverIdx+1);
		args.removeAt(serverIdx);
		args.removeAt(serverIdx);
	}

	NeovimQt::NeovimConnector *c;
	if (!server.isEmpty()) {
		c = NeovimQt::NeovimConnector::connectToNeovim(server);
	} else {
		c = NeovimQt::NeovimConnector::spawn(args);
	}

	NeovimQt::Shell *shell = new NeovimQt::Shell(c);
	shell->show();
	return app.exec();
}
