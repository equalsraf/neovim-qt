#include <QTest>
#include <QSignalSpy>
#include <QTcpServer>
#include <QTcpSocket>
#include <QRegularExpression>
#include <QBuffer>

#include <neovimconnector.h>

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

};

} // Namespace NeovimQt
QTEST_MAIN(NeovimQt::Test)
#include "tst_neovimconnector.moc"
