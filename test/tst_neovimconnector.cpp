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
		QCOMPARE(spawned->connectionType(), NeovimConnector::SpawnedConnection);
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

	void connectToNeovimTCP() {
		// These 2 cases WILL FAIL because there is no Neovim instance running
		NeovimConnector *c = NeovimConnector::connectToNeovim("127.0.0.1:64999");
		QCOMPARE(c->connectionType(), NeovimConnector::HostConnection);
		QSignalSpy onError(c, SIGNAL(error(NeovimError)));
		QVERIFY(onError.isValid());
		QVERIFY(SPYWAIT(onError));

		QCOMPARE(c->errorCause(), NeovimConnector::SocketError);
		c->deleteLater();
	}

	void connectToNeovimSocket() {
		NeovimConnector *c = NeovimConnector::connectToNeovim("NoSuchFile");
		QCOMPARE(c->connectionType(), NeovimConnector::SocketConnection);
		QSignalSpy onError(c, SIGNAL(error(NeovimError)));
		QVERIFY(onError.isValid());
		// The signal might be emited before we get to connect
		SPYWAIT(onError);

		QCOMPARE(c->errorCause(), NeovimConnector::SocketError);
		c->deleteLater();
	}

	void connectToNeovimEnvEmpty() {
		// This is the same as ::spawn()
		NeovimConnector *c = NeovimConnector::connectToNeovim("");
		QSignalSpy onReady(c, SIGNAL(ready()));
		QVERIFY(onReady.isValid());
		QVERIFY(SPYWAIT(onReady));

		c->deleteLater();
	}

	void metadataTimeout() {
		// Connect to a TCP socket that will never respond, should trigger
		// a timeout for the discoverMetadata call
		QTcpServer *server = new QTcpServer();
		server->listen(QHostAddress::LocalHost);
		QVERIFY(server->isListening());

		NeovimConnector *c = NeovimConnector::connectToNeovim(
			QString("%1:%2")
				.arg(server->serverAddress().toString())
				.arg(server->serverPort()));
		QSignalSpy onError(c, SIGNAL(error(NeovimError)));
		QVERIFY(onError.isValid());
		QVERIFY(SPYWAIT2(onError, 10000));
		QCOMPARE(c->errorCause(), NeovimConnector::RuntimeMsgpackError);
		c->deleteLater();
	}
};

} // Namespace NeovimQt
QTEST_MAIN(NeovimQt::Test)
#include "tst_neovimconnector.moc"
