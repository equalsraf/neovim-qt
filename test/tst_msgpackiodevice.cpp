#include <QTest>
#include <QSignalSpy>
#include <QTcpServer>
#include <QTcpSocket>
#include <QRegularExpression>
#include <QBuffer>

#include <msgpackiodevice.h>
#include <msgpackrequest.h>
#include "common.h"

namespace NeovimQt {

/**
 * A dummy request handler to help testing, it responds to all calls
 * with response
 */
class RequestHandler: public QObject, public MsgpackRequestHandler
{
	Q_OBJECT
public:
	RequestHandler(QObject *parent=0):QObject(parent) {}
	virtual void handleRequest(MsgpackIODevice* dev, quint32 msgid, const QByteArray& method, const QVariantList& params) {
		dev->sendResponse(msgid, QVariant(), response);
		emit receivedRequest(msgid, method, params);
	}
	QVariant response;
signals:
	void receivedRequest(quint32 msgid, const QByteArray&, const QVariantList&);

};

class Test: public QObject
{
	Q_OBJECT
private:
	MsgpackIODevice *one, *two;
	void resetLoop() {
		QTcpServer *server = new QTcpServer();
		QVERIFY(server->listen(QHostAddress::LocalHost));

		QVERIFY(server->isListening());

		QTcpSocket *client = new QTcpSocket(this);
		QSignalSpy onConnected(client, SIGNAL(connected()));
		QVERIFY(onConnected.isValid());
		QSignalSpy onNewConnection(server, SIGNAL(newConnection()));
		QVERIFY(onNewConnection.isValid());

		client->connectToHost(server->serverAddress(), server->serverPort());
		QVERIFY(SPYWAIT(onConnected));
		QVERIFY(SPYWAIT(onNewConnection));

		QTcpSocket *other = server->nextPendingConnection();

		one = new MsgpackIODevice(client);
		two = new MsgpackIODevice(other);
		QVERIFY(one->isOpen());
		QVERIFY(two->isOpen());
	}
private slots:
	void init() {
		resetLoop();
	}

	void cleanup() {
		delete one;
		delete two;
	}

	void invalidDevice() {
		MsgpackIODevice *io = new MsgpackIODevice(new QBuffer);
		QCOMPARE(io->errorCause(), MsgpackIODevice::InvalidDevice);
	}

	void defaultValues() {
		QCOMPARE(one->errorCause(), MsgpackIODevice::NoError);
	}

	/**
	 * These errors are not fatal but increase coverage
	 */
	void recvError() {
		one->send(QByteArray("Hello!"));
		
		// An array of size 1 is an invalid msgpack-rpc
		QVariantList brokenRequest;
		brokenRequest << 42;
		one->send(brokenRequest);

		// Invalid method
		QVariantList brokenRequest2;
		brokenRequest2 << "X" << 42 << 42 << 42;
		one->send(brokenRequest2);

		// Invalid method
		QVariantList brokenRequest3;
		brokenRequest2 << 0 << 42 << 42 << 42;
		one->send(brokenRequest3);

		// Just to finish
		auto req = one->startRequestUnchecked("testRequest", 0);
		QSignalSpy gotResp(req, SIGNAL(error(quint32, quint64, QVariant)));
		QVERIFY(gotResp.isValid());
		QVERIFY2(SPYWAIT(gotResp), "By default all requests get an error");
	}

	void notification() {
		QVariantList list;
		list << 44 << 45;

		QVariantList params;
		params << 1 << QByteArray("one") << -3 << list;
		QByteArray method("testNotification");

		QSignalSpy onNotification(two, SIGNAL(notification(QByteArray, QVariantList)));
		QVERIFY(onNotification.isValid());

		one->sendNotification(method, params);
		QVERIFY(SPYWAIT(onNotification));

		QVariantList n = onNotification.at(0);
		QCOMPARE(n.at(0).toByteArray(), method);
		QCOMPARE(n.at(1).toList(), params);
	}

	void request() {
		auto req = one->startRequestUnchecked("testRequest", 0);
		
		QSignalSpy gotResp(req, SIGNAL(error(quint32, quint64, QVariant)));
		QVERIFY(gotResp.isValid());

		QVERIFY2(SPYWAIT(gotResp), "By default all requests get an error");

		// Now test with an actual response
		RequestHandler *handler = new RequestHandler(two);
		handler->response = 42;
		two->setRequestHandler(handler);

		QSignalSpy gotReq(handler, SIGNAL(receivedRequest(quint32, QByteArray, QVariantList)));
		QVERIFY(gotReq.isValid());

		auto req2 = one->startRequestUnchecked("testRequest2", 1);
		one->send(QByteArray("hello"));
		QVERIFY(SPYWAIT(gotReq));

		// Make sure the request msgid/method/params matches the ones we sent
		QVariantList signal = gotReq.at(0);
		QCOMPARE(signal.at(0).toUInt(), req2->id);
		QCOMPARE(signal.at(1).toByteArray(), QByteArray("testRequest2"));
		QCOMPARE(signal.at(2).toList().at(0).toByteArray(), QByteArray("hello"));

		QSignalSpy gotResp2(req2, SIGNAL(finished(quint32, quint64, QVariant)));
		QVERIFY(gotResp2.isValid());
		QVERIFY2(SPYWAIT(gotResp2), "RequestHandler sends back a response");
	}

	void checkVariant()
	{
		// Some Unsupported types
		QVERIFY(!one->checkVariant(QRect()));

		// Supported types
		QVERIFY(one->checkVariant(QVariant()));
		QVERIFY(one->checkVariant(true));
		QVERIFY(one->checkVariant(42));
		QVERIFY(one->checkVariant(4.4));
		QVERIFY(one->checkVariant(QString("test")));
		QVERIFY(one->checkVariant(QByteArray()));
		QVERIFY(one->checkVariant(QVariantList() << "test"));
		QVERIFY(one->checkVariant(QVariantMap()));
		QVERIFY(one->checkVariant(QPoint(1,1)));
	}

	void msgId() {
		QCOMPARE(one->msgId(), (quint32)0);
		QCOMPARE(one->msgId(), (quint32)1);
		// Sending a request increases the id
		one->startRequestUnchecked("testRequestMsgId", 0);
		QCOMPARE(one->msgId(), (quint32)3);
		// Notifications dont
		one->sendNotification("testNotificationMsgId", QVariantList());
		QCOMPARE(one->msgId(), (quint32)4);
	}

	void timeout() {
		QBuffer buf;
		buf.open(QBuffer::ReadWrite);

		// This will actually trigger a fatal error because the buffer
		// is not a valid device, but is still enough to get the timeout
		// below
		MsgpackIODevice *dev = new MsgpackIODevice(&buf);

		MsgpackRequest *r = dev->startRequestUnchecked("testTimeout", 0);
		r->setTimeout(3);

		QSignalSpy timedOut(r, SIGNAL(timeout(quint32)));
		QVERIFY(timedOut.isValid());
		QVERIFY(SPYWAIT(timedOut));
		QVariantList params = timedOut.at(0);
		QCOMPARE(params.at(0).toUInt(), r->id);
	}

};

} // Namespace NeovimQt
QTEST_MAIN(NeovimQt::Test)
#include "tst_msgpackiodevice.moc"
