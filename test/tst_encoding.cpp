
#include <QtTest/QtTest>
#include <QLocalSocket>
#include <neovimconnector.h>

class TestEncoding: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();
	void encodeString();
protected:
	NeovimQt::NeovimConnector *m_c;
};

void TestEncoding::encodeString()
{
	bool failed_to_set = false;
	//
	// FIXME: This will fail for two reasons
	// 1. QString cannot be safelly encoded
	// 2. As a quick fix the encoder will replace "value" with Nil causing
	//    Neovim to not store the variable
	//
	m_c->neovimObject()->vim_set_var("testing-neovim-qt", QString("value"));
	m_c->neovimObject()->vim_get_var("testing-neovim-qt");
	connect(m_c->neovimObject(), &NeovimQt::Neovim::err_vim_get_var,
			[&failed_to_set](const QString &msg, const msgpack_object& ) {
				failed_to_set = true;
			});
	QTest::qWait(500);
	Q_ASSERT(failed_to_set);

	bool var_set = false;
	m_c->neovimObject()->vim_set_var("testing-neovim-qt", QByteArray("value"));
	m_c->neovimObject()->vim_get_var("testing-neovim-qt");
	connect(m_c->neovimObject(), &NeovimQt::Neovim::on_vim_get_var,
			[&var_set](const QVariant& v) {
				var_set = true;
				Q_ASSERT(v == QVariant(QByteArray("value")));
			});
	QTest::qWait(500);
	Q_ASSERT(var_set);
}

void TestEncoding::initTestCase()
{
	bool ready = false;
	m_c = NeovimQt::NeovimConnector::spawn();
	Q_ASSERT(m_c->errorCause() == NeovimQt::NeovimConnector::NoError);
	connect(m_c, &NeovimQt::NeovimConnector::ready,
		[&ready](){
			ready = true;
		});
	QTest::qWait(500);
	Q_ASSERT(ready);
}

QTEST_MAIN(TestEncoding)
#include "tst_encoding.moc"

