#include <QFontDatabase>
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
	void fontDescriptionFromQFont();
	void fontDescriptionToQFont();
	void render();
private:
	QString outputFolderPath();
	QString outputFilePath(const QString& name);
	QString originalFilePath(const QString& name);
	void saveWidgetOutput(ShellWidget& w, QString name);
	void diffWidgetOutput(QString name);

private slots:
	void initTestCase() noexcept;
};


void Test::initTestCase() noexcept
{
	const QStringList fonts{
		QStringLiteral("third-party/DejaVuSansMono.ttf"),
		QStringLiteral("third-party/DejaVuSansMono-Bold.ttf"),
		QStringLiteral("third-party/DejaVuSansMono-BoldOblique.ttf") };

	for (const auto& path : fonts) {
		QString abs_path_to_font(CMAKE_SOURCE_DIR);
		abs_path_to_font.append("/").append(path);
		QFontDatabase::addApplicationFont(abs_path_to_font);
	}
}

QString Test::outputFolderPath()
{
	auto folderName = "test_shellwidget_output";
	QDir().mkpath(folderName);
	return folderName;
}

/// Render Output file path to save a file
QString Test::outputFilePath(const QString& name)
{
	return QDir(outputFolderPath()).filePath(name);
}

/// Previous output from rendering, from the source test folder
QString Test::originalFilePath(const QString& name)
{
	return QDir(QDir(TEST_SOURCE_DIR).filePath("renderoutput")).filePath(name);
}

void Test::clearRegion()
{
	ShellWidget *w = new ShellWidget();
	w->show();
	w->resizeShell(2, 2);
}

void Test::fontDescriptionFromQFont()
{
	const QString fontFamily{ ShellWidget::getDefaultFontFamily() };

	QCOMPARE(ShellWidget::fontDesc({ fontFamily, 11, QFont::Weight::Normal, false /*italic*/ }),
		QString{ "%1:h11" }.arg(fontFamily));

	QCOMPARE(ShellWidget::fontDesc({ fontFamily, 12, QFont::Weight::Light, false /*italic*/ }),
		QString{ "%1:h12:l" }.arg(fontFamily));

	QCOMPARE(ShellWidget::fontDesc({ fontFamily, 14, QFont::Weight::DemiBold, false /*italic*/ }),
		QString{ "%1:h14:sb" }.arg(fontFamily));

	QCOMPARE(ShellWidget::fontDesc({ fontFamily, 16, QFont::Weight::Bold, false /*italic*/ }),
		QString{ "%1:h16:b" }.arg(fontFamily));

	const int arbitraryWeight{ QFont::Weight::Normal + 1 };
	QCOMPARE(ShellWidget::fontDesc({ fontFamily, 16, arbitraryWeight, true/*italic*/ }),
		QString{ "%1:h16:w%2:i" }.arg(fontFamily).arg(arbitraryWeight));
}

static bool ValidateQFont(
	const QVariant& varFont,
	const QString& fontFamily,
	qreal pointSizeF,
	int weight,
	bool italic) noexcept
{
	if (!varFont.canConvert<QFont>()) {
		return false;
	}

	const QFont font{ qvariant_cast<QFont>(varFont) };

	if (font.family() != fontFamily
		|| font.pointSizeF() != pointSizeF
		|| font.weight() != weight
		|| font.italic() != italic) {
		return false;
	}

	return true;
}

void Test::fontDescriptionToQFont()
{
	const QString fontFamily{ ShellWidget::getDefaultFontFamily() };
	const QFont defaultFont{ fontFamily, 11 };

	//
	// Expected Cases, basic functionality
	//
	QVERIFY(ValidateQFont(
		ShellWidget::TryGetQFontFromDescription(QString{ "%1:h11" }.arg(fontFamily), defaultFont),
		fontFamily, 11, QFont::Weight::Normal, false /*italic*/));

	QVERIFY(ValidateQFont(
		ShellWidget::TryGetQFontFromDescription(QString{ "%1:h12:l" }.arg(fontFamily), defaultFont),
		fontFamily, 12, QFont::Weight::Light, false /*italic*/));

	QVERIFY(ValidateQFont(
		ShellWidget::TryGetQFontFromDescription(QString{ "%1:h14:sb" }.arg(fontFamily), defaultFont),
		fontFamily, 14, QFont::Weight::DemiBold, false /*italic*/));


	QVERIFY(ValidateQFont(
		ShellWidget::TryGetQFontFromDescription(QString{ "%1:h16:b" }.arg(fontFamily), defaultFont),
		fontFamily, 16, QFont::Weight::Bold, false /*italic*/));

	const int arbitraryWeight{ QFont::Weight::Normal + 1 };
	QVERIFY(ValidateQFont(
		ShellWidget::TryGetQFontFromDescription(QString{ "%1:h16:w%2" }.arg(fontFamily).arg(arbitraryWeight), defaultFont),
		fontFamily, 16, arbitraryWeight, false /*italic*/));

	//
	// Error cases
	//
	const QVariant varInvalidLargeWeight{
		ShellWidget::TryGetQFontFromDescription(QString{ "%1:h16:w%2" }.arg(fontFamily).arg(10000), defaultFont) };

	QVERIFY(!ShellWidget::IsValidFont(varInvalidLargeWeight));
	QCOMPARE(varInvalidLargeWeight, QVariant{ QString{ "Invalid font weight" } });

	const QVariant varInvalidNegativeWeight{
		ShellWidget::TryGetQFontFromDescription(QString{ "%1:h16:w%2" }.arg(fontFamily).arg(-1), defaultFont) };

	QVERIFY(!ShellWidget::IsValidFont(varInvalidNegativeWeight));
	QCOMPARE(varInvalidNegativeWeight, QVariant{ QString{ "Invalid font weight" } });

	const QVariant varInvalidHeight{
		ShellWidget::TryGetQFontFromDescription(QString{ "FooBarInvalidFont:ha" }, defaultFont) };

	QVERIFY(!ShellWidget::IsValidFont(varInvalidHeight));
	QCOMPARE(varInvalidHeight, QVariant{ QString{ "Invalid font height" } });

	const QVariant varInvalidNegativeHeight{
		ShellWidget::TryGetQFontFromDescription(QString{ "FooBarInvalidFont:h-1" }, defaultFont) };

	QVERIFY(!ShellWidget::IsValidFont(varInvalidNegativeHeight));
	QCOMPARE(varInvalidHeight, QVariant{ QString{ "Invalid font height" } });
}

void Test::saveWidgetOutput(ShellWidget& w, QString name)
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

	QImage diff(output.size(), QImage::Format_RGB32);
	diff.fill(Qt::white);

	bool failed = false;

	for (int y=0; y < output.height(); y++) {
		for (int x=0; x < output.width(); x++) {
			auto outputColor = output.pixel(x, y);
			auto expectedColor = expected.pixel(x, y);

			if (outputColor != expectedColor) {
				//qWarning() << "Pixel color mismatch at position " << x << y;
				//qWarning() << "  output:" << outputColor << "expected:" << expectedColor;
				diff.setPixel(x, y, Qt::red);
				failed = true;
			}
		}
	}

	auto outpath = outputFilePath("diff_" + name);
	qDebug() << "Saving diff" << outpath;


	if (QGuiApplication::platformName() != "xcb") {
		auto msg = QString("Skipping render test in non X11: %1")
			.arg(QGuiApplication::platformName());
		QSKIP(qPrintable(msg));
	}

	QCOMPARE(diff.save(outpath), true);

	QCOMPARE(output.width(), expected.width());
	QCOMPARE(output.height(), expected.height());

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
	w.setShellFont(QFont("DejaVu Sans Mono", 11, QFont::Weight::Normal, false));
	w.resizeShell(20, 20);
	w.show();

	w.put("hello", 2, 2, HighlightAttribute());
	w.put("fffffff", 3, 3, HighlightAttribute(Qt::red, Qt::black, Qt::white, false, false, false, false, false, false));
	w.put("italic text", 4, 3, HighlightAttribute(Qt::white, Qt::black, Qt::white, false, true, false, false, false, false));
	w.put("bold text", 5, 3, HighlightAttribute(Qt::white, Qt::black, Qt::white, false, false, true, false, false, false));
	w.put("underline text", 6, 3, HighlightAttribute(Qt::white, Qt::black, Qt::white, false, false, false, true, false, false));
	w.put("undercurl text", 7, 3, HighlightAttribute(Qt::white, Qt::black, Qt::red, false, false, false, false, true, false));

	auto name = "shellwidget_render_works.png";
	saveWidgetOutput(w, name);
	diffWidgetOutput(name);
}

QTEST_MAIN(Test)
#include "test_shellwidget.moc"
