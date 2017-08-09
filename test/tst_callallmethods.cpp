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
	void vim_get_current_buffer();
	void vim_list_runtime_paths();
	void vim_call_function();
	void callAll();

private:
	NeovimQt::NeovimConnector *m_c;
};

void TestCallAllMethods::initTestCase()
{
	m_c = NeovimQt::NeovimConnector::spawn({"-u", "NORC"});
	QSignalSpy onReady(m_c, SIGNAL(ready()));
	QVERIFY(onReady.isValid());

	QVERIFY(SPYWAIT(onReady));
}

void TestCallAllMethods::vim_get_current_buffer()
{
	QVERIFY(m_c->neovimObject());
	NeovimQt::Neovim *obj = m_c->neovimObject();

	QSignalSpy result(obj, SIGNAL(on_vim_get_current_buffer(int64_t)));
	QVERIFY(result.isValid());

	obj->vim_get_current_buffer();
	QVERIFY(SPYWAIT(result));
}

void TestCallAllMethods::vim_list_runtime_paths()
{
	QVERIFY(m_c->neovimObject());
	NeovimQt::Neovim *obj = m_c->neovimObject();

	QSignalSpy result(obj, SIGNAL(on_vim_list_runtime_paths(QList<QByteArray>)));
	QVERIFY(result.isValid());

	obj->vim_list_runtime_paths();
	QVERIFY(SPYWAIT(result));
}

void TestCallAllMethods::callAll()
{
	QVERIFY(m_c->neovimObject());

	NeovimQt::Neovim *obj = m_c->neovimObject();
	const QMetaObject *meta = obj->metaObject();
	QSignalSpy neovimErrors(m_c, SIGNAL(error(NeovimError)));
	QVERIFY(neovimErrors.isValid());
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

/// vim_call_functions() was the first API function
/// to use the Array type
void TestCallAllMethods::vim_call_function()
{
	QVERIFY(m_c->neovimObject());
	NeovimQt::Neovim *obj = m_c->neovimObject();

	QSignalSpy result(obj, SIGNAL(on_vim_call_function(QVariant)));
	QVERIFY(result.isValid());

	QVariantList args;
	args << -2;
	obj->vim_call_function("abs", args);
	QVERIFY(SPYWAIT(result));
	QCOMPARE(result.at(0).at(0), QVariant(2));
}

QTEST_MAIN(TestCallAllMethods)
#include "tst_callallmethods.moc"
