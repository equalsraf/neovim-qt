#include <QtTest/QtTest>

#include "shellwidget.h"

#if defined(Q_OS_WIN) && defined(USE_STATIC_QT)
#include <QtPlugin>
Q_IMPORT_PLUGIN (QWindowsIntegrationPlugin);
#endif

class TestLigatures : public QObject
{
	Q_OBJECT

private slots:
	void AreGlyphPositionsUniform() noexcept;
	void DistributeGlyphPositions() noexcept;
	void RemoveLigaturesUnderCursor() noexcept;
};


void TestLigatures::AreGlyphPositionsUniform() noexcept
{
	QVector<QPointF> positionListValid{
		QPointF{ 0,  17 },
		QPointF{ 18, 17 },
		QPointF{ 27, 17 },
		QPointF{ 36, 17 },
	};

	Q_ASSERT(ShellWidget::AreGlyphPositionsUniform(positionListValid, 9));

	QVector<QPointF> positionListInvalid{
		QPointF{ 0,  17 },
		QPointF{ 18, 17 },
		QPointF{ 29, 17 },
		QPointF{ 36, 17 },
	};
	Q_ASSERT(!ShellWidget::AreGlyphPositionsUniform(positionListInvalid, 9));
}

void TestLigatures::DistributeGlyphPositions() noexcept
{
	// FIXME Add test case: compounding error!
	QGlyphRun operatorMono;

	QVector<QPointF> positionList{
		QPointF{ 0,  17 },
		QPointF{ 19, 17 },
		QPointF{ 28, 17 },
		QPointF{ 37, 17 },
	};

	QVector<uint32_t> indexList{ 446, 2, 2, 2 };

	operatorMono.setPositions(positionList);
	operatorMono.setGlyphIndexes(indexList);

	ShellWidget::DistributeGlyphPositions(operatorMono, "->   ", 9);

	auto resultPostions{ operatorMono.positions() };
	QCOMPARE(resultPostions[0].x(), 0);
	QCOMPARE(resultPostions[1].x(), 18);
	QCOMPARE(resultPostions[2].x(), 27);
	QCOMPARE(resultPostions[3].x(), 36);

	QVector<QPointF> positionListOverFlow {
		{ 0,  17 },
		{ 19, 17 },
		{ 38, 17 },
		{ 57, 17 },
		{ 76, 17 },
		{ 95, 17 },
	};

	QVector<uint32_t> indexListOverFlow{ 446, 446, 446, 446, 446, 446 };

	QGlyphRun operatorMonoOverFlow;
	operatorMonoOverFlow.setGlyphIndexes(indexListOverFlow);
	operatorMonoOverFlow.setPositions(positionListOverFlow);

	ShellWidget::DistributeGlyphPositions(operatorMonoOverFlow, "->->->->->->", 9);


	auto resultOverflowPostions{ operatorMonoOverFlow.positions() };
	QCOMPARE(resultOverflowPostions[0].x(), 0);
	QCOMPARE(resultOverflowPostions[1].x(), 18);
	QCOMPARE(resultOverflowPostions[2].x(), 36);
	QCOMPARE(resultOverflowPostions[3].x(), 54);
	QCOMPARE(resultOverflowPostions[4].x(), 72);
	QCOMPARE(resultOverflowPostions[5].x(), 90);
}

void TestLigatures::RemoveLigaturesUnderCursor() noexcept
{
	{
		QGlyphRun oneCharEncoding;

		QVector<QPointF> positionList{
			{ 0, 20 },
			{ 10, 20 },
			{ 20, 20 },
			{ 30, 20 },
		};

		QVector<uint32_t> indexList{ 0, 20, 21, 0 };

		QVector<uint32_t> indexListNoLigatures{ 0, 1, 2, 0};

		oneCharEncoding.setPositions(positionList);
		oneCharEncoding.setGlyphIndexes(indexList);

		ShellWidget::RemoveLigaturesUnderCursor(oneCharEncoding, " -> ", 2, 10, &indexListNoLigatures);

		auto resultPostions{ oneCharEncoding.positions() };
		QVERIFY(resultPostions.size() == 4);
		QCOMPARE(resultPostions[0].x(), 0);
		QCOMPARE(resultPostions[1].x(), 10);
		QCOMPARE(resultPostions[2].x(), 20);
		QCOMPARE(resultPostions[3].x(), 30);

		auto resultGlyphs{ oneCharEncoding.glyphIndexes() };
		QVERIFY(resultGlyphs.size() == 4);
		QCOMPARE(resultGlyphs[0], 0);
		QCOMPARE(resultGlyphs[1], 1);
		QCOMPARE(resultGlyphs[2], 2);
		QCOMPARE(resultGlyphs[3], 0);
	}

	{
		QGlyphRun oneGlyphEncoding;

		QVector<QPointF> positionList{
			{ 0, 20 },
			{ 10, 20 },
			{ 30, 20 },
		};

		QVector<uint32_t> indexList{ 0, 20, 0 };

		QVector<uint32_t> indexListNoLigatures{ 0, 1, 2, 0};

		oneGlyphEncoding.setPositions(positionList);
		oneGlyphEncoding.setGlyphIndexes(indexList);

		ShellWidget::RemoveLigaturesUnderCursor(oneGlyphEncoding, " ->  ", 2, 10, &indexListNoLigatures);

		auto resultPostions{ oneGlyphEncoding.positions() };
		QVERIFY(resultPostions.size() == 4);
		QCOMPARE(resultPostions[0].x(), 0);
		QCOMPARE(resultPostions[1].x(), 10);
		QCOMPARE(resultPostions[2].x(), 20);
		QCOMPARE(resultPostions[3].x(), 30);

		auto resultGlyphs{ oneGlyphEncoding.glyphIndexes() };
		QVERIFY(resultGlyphs.size() == 4);
		QCOMPARE(resultGlyphs[0], 0);
		QCOMPARE(resultGlyphs[1], 1);
		QCOMPARE(resultGlyphs[2], 2);
		QCOMPARE(resultGlyphs[3], 0);
	}

	{
		qDebug() << "TEST 2";
		QGlyphRun oneCharEncoding;

		QVector<QPointF> positionList{
			{ 0, 20 },
			{ 20, 20 },
		};

		QVector<uint32_t> indexList{ 20, 30 };

		QVector<uint32_t> indexListNoLigatures{ 21, 22, 31, 32};

		oneCharEncoding.setPositions(positionList);
		oneCharEncoding.setGlyphIndexes(indexList);

		ShellWidget::RemoveLigaturesUnderCursor(oneCharEncoding, "==->", 3, 10, &indexListNoLigatures);

		auto resultPostions{ oneCharEncoding.positions() };
		QVERIFY(resultPostions.size() == 3);
		QCOMPARE(resultPostions[0].x(), 0);
		QCOMPARE(resultPostions[1].x(), 20);
		QCOMPARE(resultPostions[2].x(), 30);

		auto resultGlyphs{ oneCharEncoding.glyphIndexes() };
		QVERIFY(resultGlyphs.size() == 3);
		QCOMPARE(resultGlyphs[0], 20);
		QCOMPARE(resultGlyphs[1], 31);
		QCOMPARE(resultGlyphs[2], 32);
	}
}

QTEST_MAIN(TestLigatures)
#include "test_ligatures.moc"
