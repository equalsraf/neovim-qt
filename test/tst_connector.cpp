
#include <QtTest/QtTest>
#include <QLocalSocket>
#include <neovimconnector.h>

class TestConnector: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();
	void invalidSocket();
	void ctor();
private:
	NeoVimQt::NeoVimConnector *m_c;

};

void TestConnector::initTestCase()
{
	QLocalSocket *s = new QLocalSocket();
	s->connectToServer(QLatin1String("/tmp/neovim"));
	Q_ASSERT(s->waitForConnected());
	m_c = new NeoVimQt::NeoVimConnector(s);
}

void TestConnector::invalidSocket()
{
	// we expect the connector to complain
	QLocalSocket s;
	NeoVimQt::NeoVimConnector *c = new NeoVimQt::NeoVimConnector(&s);
	Q_ASSERT(c->error() != NeoVimQt::NeoVimConnector::NoError);
}

void TestConnector::ctor()
{

}

QTEST_MAIN(TestConnector)
#include "tst_connector.moc"

