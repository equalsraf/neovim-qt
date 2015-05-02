#include <QLocalSocket>
#include <QMetaMethod>
#include <QtTest/QtTest>
#include <neovimconnector.h>
#include <auto/neovim.h>
#include "common.h"

/**
 * This is the closest I can get to writing a 5 minute unit test without
 * putting any effort into this - basically we call all API methods that
 * have no arguments.
 *
 * -- Don't take this too seriously, I just wanted to trigger some unpacking
 *    bugs :D
 */

class TestCallAllMethods: public QObject
{
	Q_OBJECT

private slots:
	void initTestCase();
	void callAll();

private:
	NeovimQt::NeovimConnector *m_c;
};

void TestCallAllMethods::initTestCase()
{
	m_c = NeovimQt::NeovimConnector::spawn();
	QSignalSpy onReady(m_c, &NeovimQt::NeovimConnector::ready);
	QVERIFY(SPYWAIT(onReady));
}

void TestCallAllMethods::callAll()
{
	QVERIFY(m_c->neovimObject());

	NeovimQt::Neovim *obj = m_c->neovimObject();
	const QMetaObject *meta = obj->metaObject();
	QSignalSpy neovimErrors(m_c, &NeovimQt::NeovimConnector::error);
	for (int i=0; i<meta->methodCount(); i++) {
		QMetaMethod meth = meta->method(i);
		if ( meth.methodType() != QMetaMethod::Slot ||
				meth.access() != QMetaMethod::Public) {
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
	QVERIFY2(!SPYWAIT(neovimErrors), "Fatal errors");
}

QTEST_MAIN(TestCallAllMethods)
#include "tst_callallmethods.moc"
