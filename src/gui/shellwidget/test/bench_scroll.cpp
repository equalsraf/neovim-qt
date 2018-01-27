#include <QtTest/QtTest>
#include "shellcontents.h"

#if defined(Q_OS_WIN) && defined(USE_STATIC_QT)
#include <QtPlugin>
Q_IMPORT_PLUGIN (QWindowsIntegrationPlugin);
#endif

class Test: public QObject
{
	Q_OBJECT
private slots:
	void benchScrollRegion() {
		QBENCHMARK {
			ShellContents s(100, 100);
			s.scroll(100);
		}
	}
};

QTEST_MAIN(Test)
#include "bench_scroll.moc"
