#include <QtTest/QtTest>
#include <QPainter>
#include "shellwidget.h"

#if defined(Q_OS_WIN) && defined(USE_STATIC_QT)
#include <QtPlugin>
Q_IMPORT_PLUGIN (QWindowsIntegrationPlugin);
#endif

class Test: public QObject
{
	Q_OBJECT

private slots:
	void clearRegion();
	void render();
private:
	QString outputFolderPath();
	QString outputFilePath(const QString &name);
	QString originalFilePath(const QString &name);
	void saveWidgetOutput(ShellWidget &w, QString name);
	void diffWidgetOutput(QString name);
};

QString Test::outputFolderPath()
{
	auto folderName = "out";
	QDir().mkpath(folderName);
	return folderName;
}

/// Render Output file path to save a file
QString Test::outputFilePath(const QString &name)
{
	return QDir(outputFolderPath()).filePath(name);
}

/// Previous output from rendering, from the source test folder
QString Test::originalFilePath(const QString &name)
{
	return QDir(QDir(TEST_SOURCE_DIR).filePath("renderoutput")).filePath(name);
}

void Test::clearRegion()
{
	ShellWidget *w = new ShellWidget();
	w->show();
	w->resizeShell(2, 2);
}

void Test::saveWidgetOutput(ShellWidget &w, QString name)
{
	QImage img(w.size(), QImage::Format_ARGB32);
	QPainter painter(&img);
	w.render(&painter);
	QCOMPARE(img.save(outputFilePath(name)), true);
}

void Test::diffWidgetOutput(QString name)
{
	auto p1 = outputFilePath(name);
	auto output = QImage(p1);
	qDebug() << "Loading" << p1 << output;
	auto p2 = originalFilePath(name);
	auto expected = QImage(p2);
	qDebug() << "Loading" << p2 << expected;

	QCOMPARE(output.isNull(), false);
	QCOMPARE(expected.isNull(), false);

	QCOMPARE(output.width(), expected.width());
	QCOMPARE(output.height(), expected.height());

	QImage diff(output.size(), QImage::Format_RGB32);
	diff.fill(Qt::white);

	bool failed = false;

	for(int y=0; y<output.height(); y++){
		for(int x=0; x<output.width(); x++){
			auto outputColor = output.pixel(x, y);
			auto expectedColor = expected.pixel(x, y);

			if (outputColor != expectedColor) {
				qWarning() << "Pixel color mismatch at position " << x << y;
				qWarning() << "  output:" << outputColor << "expected:" << expectedColor;
				diff.setPixel(x, y, Qt::red);
				failed = true;
			}
		}
	}

	auto outpath = outputFilePath("diff_" + name);
	qDebug() << "Saving diff" << outpath;
	QCOMPARE(diff.save(outpath), true);

	if (failed) {
		qWarning() << "Failing diff, check the output diff image" << outpath;
		QCOMPARE(failed, false);
	}
}

/// This is mostly an example it renders a shell widget and saves it as an image.
/// For more complicated tests we may want to parametrize these.
void Test::render()
{
	ShellWidget w;
	w.setShellFont("Monospace", 11, -1, false, false);
	w.resizeShell(20, 20);
	w.show();

	QCOMPARE(w.font().key(), QString("Monospace,11,-1,2,50,0,0,0,1,0"));

	w.put("hello", 2, 2, HighlightAttribute());
	w.put("fffffff", 3, 3, HighlightAttribute(Qt::red, Qt::black, Qt::white, false, false, false, false, false));
	w.put("italic text", 4, 3, HighlightAttribute(Qt::white, Qt::black, Qt::white, false, true, false, false, false));
	w.put("bold text", 5, 3, HighlightAttribute(Qt::white, Qt::black, Qt::white, false, false, true, false, false));
	w.put("underline text", 6, 3, HighlightAttribute(Qt::white, Qt::black, Qt::white, false, false, false, true, false));
	w.put("undercurl text", 7, 3, HighlightAttribute(Qt::white, Qt::black, Qt::red, false, false, false, false, true));

	auto name = "shellwidget_render_works.png";
	saveWidgetOutput(w, name);
	diffWidgetOutput(name);
}

QTEST_MAIN(Test)
#include "test_shellwidget.moc"
