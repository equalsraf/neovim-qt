
#include <QtTest/QtTest>
#include <QLocalSocket>
#include <neovimconnector.h>
#include <util.h>

#include "common.h"

class TestEncoding: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();
	void encodeString();
	void map();
	void stringsAreBinaryNotUtf8();

protected:
	NeovimQt::NeovimConnector* m_c;
};

void TestEncoding::encodeString()
{
	QMetaObject::Connection conn;
	m_c->api1()->vim_set_var("testing-neovim-qt", QString("value"));
	m_c->api1()->vim_get_var("testing-neovim-qt");
	QSignalSpy onVimGetVar{ m_c->api1(), &NeovimQt::NeovimApi1::on_vim_get_var };
	conn = connect(m_c->api1(), &NeovimQt::NeovimApi1::on_vim_get_var,
			[](const QVariant& v) {
				QVERIFY(v == QByteArray("value"));
			});
	QVERIFY(SPYWAIT(onVimGetVar));
	disconnect(conn);
}

void TestEncoding::map()
{
	QMetaObject::Connection conn;
	QVariantMap map;
	map.insert("answer", 42);

	m_c->api1()->vim_set_var("test-map", map);
	QSignalSpy onVimSetVar{ m_c->api1(), &NeovimQt::NeovimApi1::on_vim_set_var };
	QVERIFY(SPYWAIT(onVimSetVar));

	m_c->api1()->vim_get_var("test-map");
	QSignalSpy onVimGetVar{ m_c->api1(), &NeovimQt::NeovimApi1::on_vim_get_var };
	conn = connect(m_c->api1(), &NeovimQt::NeovimApi1::on_vim_get_var,
			[map](const QVariant& v) {
				qDebug() << "Expected:" << map;
				qDebug() << "Observed:" << v;
				QVERIFY(v == map);
			});
	QVERIFY(SPYWAIT(onVimGetVar));
	disconnect(conn);
	QVERIFY(false); // FIXME Intentional Test Failure Added!
}

// A reminder that Strings in the Neovim API are binary data
// that may or may not conform to encoding
void TestEncoding::stringsAreBinaryNotUtf8()
{
	QByteArray data = "\xc3\x28";

	m_c->api1()->vim_set_current_line(data);
	QSignalSpy onVimSetCurrentLine{ m_c->api1(), &NeovimQt::NeovimApi1::on_vim_set_current_line};
	QVERIFY(SPYWAIT(onVimSetCurrentLine));

	m_c->api1()->vim_get_current_line();
	QMetaObject::Connection conn;
	QSignalSpy onVimGetCurrentLine{ m_c->api1(), &NeovimQt::NeovimApi1::on_vim_get_current_line};
	conn = connect(m_c->api1(), &NeovimQt::NeovimApi1::on_vim_get_current_line,
			[data](const QVariant& v) {
				QCOMPARE(v.toByteArray(), data);
			});
	QVERIFY(SPYWAIT(onVimGetCurrentLine));
	disconnect(conn);
}

void TestEncoding::initTestCase()
{
	m_c = NeovimQt::NeovimConnector::spawn({"-u", "NONE"});
	QVERIFY(m_c->errorCause() == NeovimQt::NeovimConnector::NoError);
	QSignalSpy onReady(m_c, SIGNAL(ready()));
	QVERIFY(onReady.isValid());
	QVERIFY(SPYWAIT(onReady));
}

QTEST_MAIN(TestEncoding)
#include "tst_encoding.moc"

