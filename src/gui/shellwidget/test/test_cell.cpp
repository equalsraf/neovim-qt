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
	void cellDefault() {
		Cell c;
		QCOMPARE(c.GetCharacter(), uint(' '));
		// Default colors are invalid
		QCOMPARE(c.GetForegroundColor(), QColor());
		QCOMPARE(c.GetBackgroundColor(), QColor());
		QCOMPARE(c.GetSpecialColor(), QColor());
		QVERIFY(!c.GetForegroundColor().isValid());
		QVERIFY(!c.GetBackgroundColor().isValid());
		QVERIFY(!c.GetSpecialColor().isValid());

		QBENCHMARK {
			Cell c;
		}
	}

	void cellValue() {
		QBENCHMARK {
			Cell c('z', Qt::black, Qt::white, QColor(),
					false, false, false, false, false, false);
		}
	}
	void cellValueRgb() {
		QBENCHMARK {
			Cell c('z', QRgb(33), QRgb(66), QColor(),
					false, false, false, false, false, false);
		}
	}

	void cellWidth() {
		Cell c;
		QCOMPARE(c.IsDoubleWidth(), false);
		c.SetCharacter(27721);
		QCOMPARE(c.IsDoubleWidth(), true);
	}

	void cellBg() {
		Cell c0;
		Cell c1 = Cell{ QColor::Invalid };
		QCOMPARE(c0, c1);

		Cell c2 = Cell{ QColor{ Qt::red } };
		QCOMPARE(c2.GetBackgroundColor(), QColor{ Qt::red });
	}

	void cellOperatorEquals() noexcept;

	void cellStyleEquivalent() noexcept;
};

void Test::cellOperatorEquals() noexcept
{
	const HighlightAttribute hlTextBlackFillWhite{
		Qt::black /*fgColor*/,
		Qt::white /*bgColor*/,
		Qt::red /*spColor*/,
		false /*bold*/,
		false /*italic*/,
		false /*underline*/,
		false /*undercurl*/,
		false /*strikethrough*/,
		false /*reverse*/ };

	const HighlightAttribute hlTextWhiteFillBlack{
		Qt::white /*fgColor*/,
		Qt::black /*bgColor*/,
		Qt::red /*spColor*/,
		false /*bold*/,
		false /*italic*/,
		false /*underline*/,
		false /*undercurl*/,
		false /*strikethrough*/,
		false /*reverse*/ };

	const HighlightAttribute hlInvTextWhiteFillBlack{
		Qt::white /*fgColor*/,
		Qt::black /*bgColor*/,
		Qt::red /*spColor*/,
		false /*bold*/,
		false /*italic*/,
		false /*underline*/,
		false /*undercurl*/,
		false /*strikethrough*/,
		true /*reverse*/ };

	const HighlightAttribute hlBoldTextWhiteFillBlack{
		Qt::white /*fgColor*/,
		Qt::black /*bgColor*/,
		Qt::red /*spColor*/,
		true /*bold*/,
		false /*italic*/,
		false /*underline*/,
		false /*undercurl*/,
		false /*strikethrough*/,
		false /*reverse*/ };

	bool textDifferent{
		Cell{ ' ', hlTextBlackFillWhite } == Cell{ 'A', hlTextBlackFillWhite } };
	QVERIFY(textDifferent == false);

	bool textSame{
		Cell{ 'B', hlTextBlackFillWhite } == Cell{ 'B', hlTextBlackFillWhite } };
	QVERIFY(textSame == true);

	bool colorsDifferent{
		Cell{ ' ', hlInvTextWhiteFillBlack } == Cell{ ' ', hlTextBlackFillWhite } };
	QVERIFY(colorsDifferent == false);

	// These two styles render identically, but are not equivalent. Their underlying
	// data is different, so they are treated as different.
	bool inverseOpposites{
		Cell{ ' ', hlInvTextWhiteFillBlack } == Cell{ ' ', hlTextBlackFillWhite } };
	QVERIFY(inverseOpposites == false);

	bool stylesDifferent{
		Cell{ ' ', hlBoldTextWhiteFillBlack } == Cell{ ' ', hlTextWhiteFillBlack } };
	QVERIFY(stylesDifferent == false);
}

void Test::cellStyleEquivalent() noexcept
{
	const HighlightAttribute styleA{
		Qt::white /*fgColor*/,
		Qt::black /*bgColor*/,
		Qt::red /*spColor*/,
		true /*bold*/,
		false /*italic*/,
		false /*underline*/,
		false /*undercurl*/,
		false /*strikethrough*/,
		false /*reverse*/ };

	const HighlightAttribute styleB{
		Qt::black /*fgColor*/,
		Qt::white /*bgColor*/,
		Qt::red /*spColor*/,
		true /*bold*/,
		false /*italic*/,
		false /*underline*/,
		false /*undercurl*/,
		false /*strikethrough*/,
		false /*reverse*/ };

	Cell cellStyleATextA{ 'A', styleA };
	Cell cellStyleATextB{ 'B', styleA };
	Cell cellStyleBTextA{ 'B', styleB };
	Cell cellStyleBTextB{ 'B', styleB };

	QVERIFY(cellStyleATextA.IsStyleEquivalent(cellStyleATextB));
	QVERIFY(cellStyleATextB.IsStyleEquivalent(cellStyleATextA));

	QVERIFY(!cellStyleATextA.IsStyleEquivalent(cellStyleBTextA));
	QVERIFY(!cellStyleBTextA.IsStyleEquivalent(cellStyleATextA));

	QVERIFY(cellStyleBTextA.IsStyleEquivalent(cellStyleBTextB));
	QVERIFY(cellStyleBTextB.IsStyleEquivalent(cellStyleBTextA));
}

QTEST_MAIN(Test)
#include "test_cell.moc"
