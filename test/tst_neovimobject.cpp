
#include <QtTest/QtTest>
#include <QLocalSocket>
#include <neovimconnector.h>
#include "common.h"

class TestNeovimObject: public QObject
{
	Q_OBJECT
public slots:
	void test_event(const QByteArray& name, const QVariantList&);

protected slots:
	void delayedSetup();

private slots:
	void initTestCase();
	void eventTypes();
	void extDecodeApi0();
	void extDecodeApi1();
	void extDecodeApi2();
private:
	NeovimQt::NeovimConnector *m_c;
	bool m_test_event_string;
	bool m_test_event_uint;
	bool m_test_event_stringlist;
};

void TestNeovimObject::delayedSetup()
{
	QVERIFY(m_c->neovimObject());
	auto *n = m_c->neovimObject();

	m_test_event_string = false;
	m_test_event_uint = false;
	m_test_event_stringlist = false;
	connect(n, &NeovimQt::NeovimApi1::neovimNotification,
			this, &TestNeovimObject::test_event);

	n->vim_command(QString("call rpcnotify(%1, \"test_event\", \"WAT\")").arg(m_c->channel()).toUtf8());
	n->vim_command(QString("call rpcnotify(%1, \"test_event\", 42)").arg(m_c->channel()).toUtf8());
	n->vim_command(QString("call rpcnotify(%1, \"test_event\", [\"one\", \"two\", \"\"])").arg(m_c->channel()).toUtf8());
}

void TestNeovimObject::test_event(const QByteArray& name, const QVariantList& params)
{
	QVariant arg0 = params.at(0);
	if ( (QMetaType::Type)arg0.type() == QMetaType::QByteArray ) {
		QVERIFY(arg0.toString() == "WAT");
		m_test_event_string = true;
	}

	if ( (QMetaType::Type)arg0.type() == QMetaType::ULongLong ) {
		QVERIFY(arg0.toInt() == 42);
		m_test_event_uint = true;
	}

	if (arg0.canConvert(QMetaType::QStringList)) {
		QStringList l = arg0.toStringList();
		m_test_event_stringlist = true;
	}
}

//
// Tests start here
//
void TestNeovimObject::eventTypes()
{
	QVERIFY(m_test_event_string);
	QVERIFY(m_test_event_uint);
	QVERIFY(m_test_event_stringlist);
}

/// Check EXT types with the Tabpage type
void TestNeovimObject::extDecodeApi0()
{
	auto *obj = m_c->api0();
	QSignalSpy result(obj, SIGNAL(on_vim_get_current_tabpage(int64_t)));
	QVERIFY(result.isValid());

	obj->vim_get_current_tabpage();
	QVERIFY(SPYWAIT(result));
	QCOMPARE(result.at(0).at(0), QVariant(1));
}

void TestNeovimObject::extDecodeApi1()
{
	auto *obj = m_c->api1();
	QSignalSpy result(obj, SIGNAL(on_nvim_get_current_tabpage(int64_t)));
	QVERIFY(result.isValid());

	obj->nvim_get_current_tabpage();
	QVERIFY(SPYWAIT(result));
	QCOMPARE(result.at(0).at(0), QVariant(1));
}

void TestNeovimObject::extDecodeApi2()
{
	auto *obj = m_c->api2();
	QSignalSpy result(obj, SIGNAL(on_nvim_get_current_tabpage(int64_t)));
	QVERIFY(result.isValid());

	obj->nvim_get_current_tabpage();
	QVERIFY(SPYWAIT(result));
	QCOMPARE(result.at(0).at(0), QVariant(1));
}

void TestNeovimObject::initTestCase()
{
	// needed for the nvim api signal spy
	qRegisterMetaType<int64_t>("int64_t");
	m_c = NeovimQt::NeovimConnector::spawn({"-u", "NORC"});
	connect(m_c, &NeovimQt::NeovimConnector::ready,
			this, &TestNeovimObject::delayedSetup);
	QTest::qWait(1500);
	QVERIFY(m_c->errorCause() == NeovimQt::NeovimConnector::NoError);
}

QTEST_MAIN(TestNeovimObject)
#include "tst_neovimobject.moc"

