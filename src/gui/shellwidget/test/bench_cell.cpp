#include <QtTest/QtTest>
#include "cell.h"

#if defined(Q_OS_WIN) && defined(USE_STATIC_QT)
#include <QtPlugin>
Q_IMPORT_PLUGIN (QWindowsIntegrationPlugin);
#endif

class Test: public QObject
{
	Q_OBJECT
private slots:
	void benchCell() {
		QBENCHMARK {
			Cell c('1', Qt::red, Qt::blue, QColor(), false, false, false, false);
		}
	}
};

QTEST_MAIN(Test)
#include "bench_cell.moc"
