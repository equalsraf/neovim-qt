#include <QCoreApplication>
#include "neovimconnector.h"
#include <QLocalSocket>

int main(int argc, char **argv)
{
	QCoreApplication app(argc, argv);

	QLocalSocket s;
	s.connectToServer(QLatin1String("/tmp/neovim"));
	qDebug() << s.waitForConnected();

	NeovimQt::NeovimConnector c(&s);

	// You must wait for the ready signal before trying to use the neovim object
	// otherwise funky things will happen, like all your calls being dropped
	QObject::connect(&c, &NeovimQt::NeovimConnector::ready,
			[&c]() {
			c.neovimObject()->vim_command(QString("call send_event(%1, \"test-event\", [1,2,3])").arg(c.channel()));
			c.neovimObject()->vim_command(QString("call send_event(%1, \"test-event2\", [1,2,\"WAT\"])").arg(c.channel()));

			});


	return app.exec();
}
