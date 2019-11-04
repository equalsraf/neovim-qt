#include <QtTest/QtTest>
#include <QScreen>
#include "shellwidget.h"
#include "helpers.h"

#if defined(Q_OS_WIN) && defined(USE_STATIC_QT)
#include <QtPlugin>
Q_IMPORT_PLUGIN (QWindowsIntegrationPlugin);
#endif

class Test: public QObject
{
       Q_OBJECT
		ShellWidget *s;
private slots:
		void init() {
			auto path = QDir(QDir(TEST_SOURCE_DIR).filePath("shellcontents"))
				.filePath("shell2.txt");
			s = ShellWidget::fromFile(path);
			s->show();
		}
		void cleanupTestCase() {
			// save a image of the widget after the last test
			s->grab().save("widgetpaint.png");
		}
       void benchPaint() {
               QBENCHMARK {
				   s->repaint();
               }
       }
};

QTEST_MAIN(Test)
#include "bench_widgetpaint.moc"
