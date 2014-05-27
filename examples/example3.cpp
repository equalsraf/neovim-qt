#include <QCoreApplication>
#include "neovimconnector.h"
#include <QLocalSocket>
#include <QMetaMethod>

/**
 * This is the closest I can get to writing a 5 minute unit test without
 * putting any effort into this
 *
 * -- Don't take this too seriously, I just wanted to trigger some unpacking
 *    bugs :D
 */
int main(int argc, char **argv)
{
	QCoreApplication app(argc, argv);

	QLocalSocket s;
	s.connectToServer(QLatin1String("/tmp/neovim"));
	qDebug() << s.waitForConnected();

	NeoVimQt::NeoVimConnector c(&s);

	// You must wait for the ready signal before trying to use the neovim object
	// otherwise funky things will happen, like all your calls being dropped
	QObject::connect(&c, &NeoVimQt::NeoVimConnector::ready,
		[&c]() {
		NeoVimQt::NeoVim *obj = c.neovimObject();
		const QMetaObject *meta = obj->metaObject();
		for (int i=0; i<meta->methodCount(); i++) {
			QMetaMethod meth = meta->method(i);
			if ( meth.methodType() != QMetaMethod::Slot ) {
				continue;
			}

			if ( meth.parameterNames().size() == 0 && meth.name() != "deleteLater") {
				qDebug() << "Calling" << meth.name();
				meth.invoke(obj);
			} else if ( meth.parameterNames().size() == 1 ) {
				if ( meth.parameterTypes().at(0) == "Window" ) {
					meth.invoke(obj, Q_ARG(int64_t, 1));
				}
			} else {
				qDebug() << "Skipping" << meth.methodSignature();
			}
		}
		});

	return app.exec();
}
