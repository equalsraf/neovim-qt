
#include <QtTest/QtTest>
#include <QLocalSocket>
#include <neovimconnector.h>

class TestConnector: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();
	void invalidSocket();
	void launch();

private:
	NeovimQt::NeovimConnector *m_c;

};

void TestConnector::initTestCase()
{
	QLocalSocket *s = new QLocalSocket();
	s->connectToServer(QLatin1String("/tmp/neovim"));
	Q_ASSERT(s->waitForConnected());
	m_c = new NeovimQt::NeovimConnector(s);
}

void TestConnector::invalidSocket()
{
	// we expect the connector to complain
	QLocalSocket s;
	NeovimQt::NeovimConnector *c = new NeovimQt::NeovimConnector(&s);
	Q_ASSERT(c->error() != NeovimQt::NeovimConnector::NoError);
}

void TestConnector::launch()
{
	bool ready = false;
	NeovimQt::NeovimConnector *c = NeovimQt::NeovimConnector::launch();
	Q_ASSERT(c->error() == NeovimQt::NeovimConnector::NoError);
	connect(m_c, &NeovimQt::NeovimConnector::ready,
		[&ready](){
			ready = true;
		});
	QTest::qWait(500);
	Q_ASSERT(ready);
}

QTEST_MAIN(TestConnector)
#include "tst_connector.moc"

