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

QTEST_MAIN(Test)
#include "test_shellwidget.moc"
