#include <QApplication>
#include "neovimconnector.h"
#include <QLocalSocket>
#include <QLineEdit>

/**
 *
 * This just gives you a linewidget that you can use to
 * write :commands
 *
 */
int main(int argc, char **argv)
{
	QApplication app(argc, argv);

	QLocalSocket s;
	s.connectToServer(QLatin1String("/tmp/neovim"));
	qDebug() << s.waitForConnected();

	NeovimQt::NeovimConnector c(&s);
	QLineEdit line;
	line.setEnabled(false);
	line.show();

	QObject::connect(&line, &QLineEdit::returnPressed,
			[&c, &line]() {
			c.neovimObject()->vim_command(line.text());
			line.clear();
			});
	
	// You must wait for the ready signal before trying to use the neovim object
	// otherwise funky things will happen, like all your calls being dropped
	QObject::connect(&c, &NeovimQt::NeovimConnector::ready,
			[&c, &line]() {
			line.setEnabled(true);
			});

	return app.exec();
}
