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

		NeovimConnector *spawned = NeovimConnector::spawn({"-u", "NONE"});
		QCOMPARE(spawned->connectionType(), NeovimConnector::SpawnedConnection);
		QCOMPARE(spawned->canReconnect(), true);

		spawned->reconnect();
	}

	void isReady() {

		NeovimConnector *c = NeovimConnector::spawn({"-u", "NONE"});
		QSignalSpy onReady(c, SIGNAL(ready()));
		QVERIFY(onReady.isValid());

		QVERIFY(SPYWAIT(onReady));
		QVERIFY(c->isReady());
	}

	void connectToNeovimTCP() {
		NeovimConnector *c = NeovimConnector::connectToNeovim("127.0.0.1:64999");
		QCOMPARE(c->connectionType(), NeovimConnector::HostConnection);
		QSignalSpy onError(c, SIGNAL(error(NeovimError)));
		QVERIFY(onError.isValid());

		// Pull Request #612: On MacOS, this test can fail without a long timeout value.
		QVERIFY(SPYWAIT(onError, 120000 /*msec*/));

		QCOMPARE(c->errorCause(), NeovimConnector::SocketError);
		c->deleteLater();
	}

	void connectToNeovimSocket() {
		NeovimConnector *c = NeovimConnector::connectToNeovim("NoSuchFile");
		QCOMPARE(c->connectionType(), NeovimConnector::SocketConnection);
		QSignalSpy onError(c, SIGNAL(error(NeovimError)));
		QVERIFY(onError.isValid());

		// Test Performance: timeout expected, set value carefully.
		QVERIFY(!SPYWAIT(onError, 5000 /*msec*/));

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


#ifdef Q_OS_UNIX
	void connectToSocket_data() {
		QTest::addColumn<QString>("socketname");

		QTest::newRow("relative") << "relnvimsock";
		QTest::newRow("./relative") << "./relnvimsock";
		QTest::newRow("absolute") << QFileInfo("absnvimsock").absoluteFilePath();
	}

	// https://github.com/equalsraf/neovim-qt/issues/936
	// QLocalSocket cannot open relative paths. This happens
	// both in Linux and Mac.
	void connectToSocket() {
		QFETCH(QString, socketname);

		QDir().remove(socketname);
		// Start nvim
		QProcess p;
		p.setProgram("nvim");
		QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

		auto path_info = QFileInfo(socketname);
		if (path_info.isAbsolute()) {
			env.insert("NVIM_LISTEN_ADDRESS", socketname);
		}
		else {
			env.insert("NVIM_LISTEN_ADDRESS", "./" + socketname);
		}
		p.setProcessEnvironment(env);
		p.setArguments({"--headless", "-u", "NONE"});
		p.start();
		p.waitForStarted();

		QTest::qWait(1500);
		// connect
		NeovimConnector *c = NeovimConnector::connectToSocket(socketname);
		qDebug() << c->connectionDescription();
		QSignalSpy onReady(c, SIGNAL(ready()));
		QVERIFY(onReady.isValid());
		QVERIFY(SPYWAIT(onReady, 5000 /*msec*/));

		QCOMPARE(c->connectionType(), NeovimConnector::SocketConnection);
	}
#endif

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

		// Test Performance: timeout expected, set value carefully.
		c->setRequestTimeout(1000 /*msec*/);

		QSignalSpy onError(c, SIGNAL(error(NeovimError)));
		QVERIFY(onError.isValid());

		QVERIFY(SPYWAIT(onError, 5000 /*msec*/));

		QCOMPARE(c->errorCause(), NeovimConnector::RuntimeMsgpackError);
		c->deleteLater();
	}
};

} // Namespace NeovimQt
QTEST_MAIN(NeovimQt::Test)
#include "tst_neovimconnector.moc"
