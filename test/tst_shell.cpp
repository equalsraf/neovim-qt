#include <QtTest/QtTest>
#include <QLocalSocket>
#include <gui/shell.h>
#include "common.h"

namespace NeovimQt {

class Test: public QObject
{
	Q_OBJECT
private slots:
	void benchStart() {
		QBENCHMARK {
			NeovimConnector *c = NeovimConnector::spawn();
			QSignalSpy onReady(c, SIGNAL(ready()));
			QVERIFY(onReady.isValid());
			QVERIFY(SPYWAIT(onReady));

			Shell *s = new Shell(c);
			QSignalSpy onResize(s, SIGNAL(neovimResized(int, int)));
			QVERIFY(onResize.isValid());
			QVERIFY(SPYWAIT(onResize));
		}
	}
protected:
	NeovimQt::NeovimConnector *m_c;
};

} // Namespace NeovimQt
QTEST_MAIN(NeovimQt::Test)
#include "tst_shell.moc"
