
#include <QtTest/QtTest>
#include <QLocalSocket>
#include <neovimconnector.h>

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
private:
	NeovimQt::NeovimConnector *m_c;
	bool m_test_event_string;
	bool m_test_event_uint;
	bool m_test_event_stringlist;
};

void TestNeovimObject::delayedSetup()
{
	NeovimQt::Neovim *n = m_c->neovimObject();

	m_test_event_string = false;
	m_test_event_uint = false;
	m_test_event_stringlist = false;
	connect(n, &NeovimQt::Neovim::neovimNotification,
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

void TestNeovimObject::initTestCase()
{
	m_c = NeovimQt::NeovimConnector::spawn({"-u", "NORC"});
	connect(m_c, &NeovimQt::NeovimConnector::ready,
			this, &TestNeovimObject::delayedSetup);
	QTest::qWait(1500);
	QVERIFY(m_c->errorCause() == NeovimQt::NeovimConnector::NoError);
	QVERIFY(m_c->neovimObject());
}

QTEST_MAIN(TestNeovimObject)
#include "tst_neovimobject.moc"

