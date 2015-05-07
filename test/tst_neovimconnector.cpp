#include <QTest>
#include <QSignalSpy>
#include <QTcpServer>
#include <QTcpSocket>
#include <QRegularExpression>
#include <QBuffer>

#include <neovimconnector.h>
#include "common.h"

namespace NeovimQt {

class Test: public QObject
{
	Q_OBJECT
private slots:

	void reconnect() {
		NeovimConnector c(new QBuffer());
		QCOMPARE(c.canReconnect(), false);

		NeovimConnector *spawned = NeovimConnector::spawn();
		QCOMPARE(spawned->canReconnect(), true);

		spawned->reconnect();
	}

	void isReady() {

		NeovimConnector *c = NeovimConnector::spawn();
		QSignalSpy onReady(c, SIGNAL(ready()));
		QVERIFY(onReady.isValid());

		QVERIFY(SPYWAIT(onReady));
		QVERIFY(c->isReady());
	}

	void encodeDecode() {
		NeovimConnector *c = NeovimConnector::spawn();

		// This will print a warning, but should succeed
		QString s = "ç日本語";
		QByteArray bytes = c->encode(s);
		QCOMPARE(c->decode(bytes), s);

		QSignalSpy onReady(c, SIGNAL(ready()));
		QVERIFY(onReady.isValid());
		QVERIFY(SPYWAIT(onReady));

		bytes = c->encode(s);
		QCOMPARE(c->decode(bytes), s);

	}
};

} // Namespace NeovimQt
QTEST_MAIN(NeovimQt::Test)
#include "tst_neovimconnector.moc"
