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

	// These two requests might fail because I've hardcoded the function Id
	NeovimQt::NeovimRequest *r = c.startRequestUnchecked(54, 0);
	QObject::connect(r, &NeovimQt::NeovimRequest::finished,
		[](uint32_t id, NeovimQt::Function::FunctionId, bool error, const msgpack_object& obj){
			qDebug() << obj;
		});

	NeovimQt::NeovimRequest *r2 = c.startRequestUnchecked(43, 1);
	c.send(QLatin1String("WAT THE HELL"));
	QObject::connect(r2, &NeovimQt::NeovimRequest::finished,
		[](uint32_t id, NeovimQt::Function::FunctionId, bool error, const msgpack_object& obj){
			qDebug() << "set_line";
		});

	// You must wait for the ready signal before trying to use the neovim object
	// otherwise funky things will happen, like all your calls being dropped
	QObject::connect(&c, &NeovimQt::NeovimConnector::ready,
			[&c]() {
			c.neovimObject()->vim_set_current_line(QLatin1String("and now it is better"));

			c.neovimObject()->vim_get_vvar(QLatin1String("lang"));
			});


	return app.exec();
}
