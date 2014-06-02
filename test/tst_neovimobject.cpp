
#include <QtTest/QtTest>
#include <QLocalSocket>
#include <neovimconnector.h>

class TestNeovimObject: public QObject
{
	Q_OBJECT
public slots:
	void notification(const QByteArray& name, const QVariant& v);
	void test_event(const QVariant&);

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

void TestNeovimObject::notification(const QByteArray& name, const QVariant& val)
{
	qDebug() << name << val;
}

void TestNeovimObject::delayedSetup()
{
	m_c->setNeovimEventHandler(this);
	NeovimQt::Neovim *n = m_c->neovimObject();

	m_test_event_string = false;
	m_test_event_uint = false;
	m_test_event_stringlist = false;
	n->vim_command(QString("call send_event(%1, \"test_event\", \"WAT\")").arg(m_c->channel()));
	n->vim_command(QString("call send_event(%1, \"test_event\", 42)").arg(m_c->channel()));
	n->vim_command(QString("call send_event(%1, \"test_event\", [\"one\", \"two\", \"\"])").arg(m_c->channel()));
}

void TestNeovimObject::test_event(const QVariant& v)
{
	if ( (QMetaType::Type)v.type() == QMetaType::QString ) {
		Q_ASSERT(v.toString() == "WAT");
		m_test_event_string = true;
	}

	if ( (QMetaType::Type)v.type() == QMetaType::ULongLong ) {
		Q_ASSERT(v.toInt() == 42);
		m_test_event_uint = true;
	}

	if (v.canConvert(QMetaType::QStringList)) {
		QStringList l = v.toStringList();
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
	QLocalSocket *s = new QLocalSocket();
	s->connectToServer(QLatin1String("/tmp/neovim"));
	Q_ASSERT(s->waitForConnected());
	m_c = new NeovimQt::NeovimConnector(s);
	Q_ASSERT(m_c->neovimObject());

	connect(m_c, &NeovimQt::NeovimConnector::ready,
			this, &TestNeovimObject::delayedSetup);
	connect(m_c, &NeovimQt::NeovimConnector::notification,
			this, &TestNeovimObject::notification);
	QTest::qWait(500);
}

QTEST_MAIN(TestNeovimObject)
#include "tst_neovimobject.moc"

