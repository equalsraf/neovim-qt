#include <QLocalSocket>
#include <QMetaMethod>
#include <QtTest/QtTest>
#include <neovimconnector.h>
#include <auto/neovim.h>

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

protected slots:
	void callAll();

private slots:
	void initTestCase();
	void connectionError(NeovimQt::NeovimConnector::NeovimError);
	void error(const QString& errmsg);

private:
	NeovimQt::NeovimConnector *m_c;
	bool m_errors;
};

void TestCallAllMethods::initTestCase()
{
	m_errors = false;

	m_c = NeovimQt::NeovimConnector::spawn();

	connect(m_c, &NeovimQt::NeovimConnector::ready,
			this, &TestCallAllMethods::callAll);
	QTest::qWait(500);
	Q_ASSERT(!m_errors);
}

void TestCallAllMethods::error(const QString& errmsg)
{
	qWarning() << errmsg;
	m_errors = true;
}

void TestCallAllMethods::callAll()
{
	Q_ASSERT(m_c->neovimObject());

	NeovimQt::Neovim *obj = m_c->neovimObject();
	connect(m_c->neovimObject(), &NeovimQt::Neovim::error,
			this, &TestCallAllMethods::error);
	const QMetaObject *meta = obj->metaObject();
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

}

void TestCallAllMethods::connectionError(NeovimQt::NeovimConnector::NeovimError err)
{
	qDebug() << m_c->errorString();
	Q_ASSERT(false);
}

QTEST_MAIN(TestCallAllMethods)
#include "tst_callallmethods.moc"
