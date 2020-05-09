#include <QtTest/QtTest>
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
};


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

QTEST_MAIN(Test)
#include "test_shellwidget.moc"
