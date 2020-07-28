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

private:
	ShellWidget* s{ nullptr };

private slots:
	void init()
	{
		const QString path{ QDir{ QDir{ TEST_SOURCE_DIR }.filePath("shellcontents") }.filePath("shell2.txt") };
		s = ShellWidget::fromFile(path);
		s->show();
	}

	void benchPaint()
	{
		s->setLigatureMode(false);
		QBENCHMARK{ s->repaint(); }

		// Uncomment to save a image of the canvas
		//s->grab().save("widgetpaint.png");
	}

	void benchPaintLigatures()
	{
		s->setLigatureMode(true);
		QBENCHMARK{ s->repaint(); }

		// Uncomment to save a image of the canvas
		//s->grab().save("widgetpaint.png");
	}
};

QTEST_MAIN(Test)
#include "bench_paint.moc"
