
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
	connect(m_c, &NeovimQt::NeovimConnector::neovimEvent,
			this, &TestNeovimObject::test_event);

	n->vim_command(QString("call rpcnotify(%1, \"test_event\", \"WAT\")").arg(m_c->channel()).toUtf8());
	n->vim_command(QString("call rpcnotify(%1, \"test_event\", 42)").arg(m_c->channel()).toUtf8());
	n->vim_command(QString("call rpcnotify(%1, \"test_event\", [\"one\", \"two\", \"\"])").arg(m_c->channel()).toUtf8());
}

void TestNeovimObject::test_event(const QByteArray& name, const QVariantList& params)
{
	QVariant arg0 = params.at(0);
	if ( (QMetaType::Type)arg0.type() == QMetaType::QByteArray ) {
		Q_ASSERT(arg0.toString() == "WAT");
		m_test_event_string = true;
	}

	if ( (QMetaType::Type)arg0.type() == QMetaType::ULongLong ) {
		Q_ASSERT(arg0.toInt() == 42);
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
	Q_ASSERT(m_test_event_string);
	Q_ASSERT(m_test_event_uint);
	Q_ASSERT(m_test_event_stringlist);
}

void TestNeovimObject::initTestCase()
{
	m_c = NeovimQt::NeovimConnector::spawn();
	Q_ASSERT(m_c->neovimObject());

	connect(m_c, &NeovimQt::NeovimConnector::ready,
			this, &TestNeovimObject::delayedSetup);
	QTest::qWait(500);
}

QTEST_MAIN(TestNeovimObject)
#include "tst_neovimobject.moc"

