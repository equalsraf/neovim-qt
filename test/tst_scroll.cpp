#include <gui/input.h>
#include <gui/shell.h>
#include <QtTest/QtTest>

#include "common.h"

#if defined(Q_OS_WIN) && defined(USE_STATIC_QT)
#include <QtPlugin>
Q_IMPORT_PLUGIN (QWindowsIntegrationPlugin);
#endif

namespace NeovimQt {

// FIXME Comment?
constexpr QSize cellSize{ 10, 10 };
constexpr int deltasPerStep { 120 };
constexpr int scrollEventSize{ 8 };
constexpr int eventsPerScroll{ deltasPerStep / scrollEventSize };

class TestScroll : public QObject
{
	Q_OBJECT

private slots:
	void ScrollUp() noexcept;
	void ScrollDown() noexcept;
	void ScrollLeft() noexcept;
	void ScrollRight() noexcept;
	void ScrollDiagonal() noexcept;
	void ScrollAccumulation() noexcept;
	void ScrollModifiers() noexcept;
	void ScrollHighResolution() noexcept;
	void ScrollInvertedEvents() noexcept;
};

static void RepeatWheelEventsNTimesAndAssertEmpty(
	uint32_t repeatCount,
	QPoint& scrollRemainder,
	const std::vector<QWheelEvent>& evList) noexcept
{
	for (uint32_t i = 0; i < repeatCount; i++) {
		for (const auto& ev : evList) {
			const QString evEmptyString{ Shell::GetWheelEventStringAndSetScrollRemainder(
				ev, scrollRemainder, cellSize, deltasPerStep) };
			QVERIFY(evEmptyString.isEmpty());
		}
	}
}

void TestScroll::ScrollUp() noexcept
{
	const QWheelEvent evScrollUp{
		QPointF{ 100.0, 100.0 } /*pos*/,
		QPointF{ 200.0, 200.0 } /*globalPos*/,
		QPoint{ 0, 0 } /*pixelDelta*/,
		QPoint{ 0, scrollEventSize } /*angleDelta*/,
		Qt::NoButton /*buttons*/,
		Qt::NoModifier/*modifiers*/,
		Qt::NoScrollPhase,
		false /*inverted*/
	};

	QPoint scrollRemainder{ 100, 0 };

	RepeatWheelEventsNTimesAndAssertEmpty(eventsPerScroll - 1, scrollRemainder, { evScrollUp } );

	const QString evStringScrollUp{ Shell::GetWheelEventStringAndSetScrollRemainder(
		evScrollUp, scrollRemainder, cellSize, deltasPerStep) };

	QCOMPARE(evStringScrollUp, QString{ "<ScrollWheelUp><10,10>" });
}

void TestScroll::ScrollDown() noexcept
{
	const QWheelEvent evScrollDown{
		QPointF{ 120.0, 199.0 } /*pos*/,
		QPointF{ 200.0, 200.0 } /*globalPos*/,
		QPoint{ 0, 0 } /*pixelDelta*/,
		QPoint{ 0, -scrollEventSize } /*angleDelta*/,
		Qt::NoButton /*buttons*/,
		Qt::NoModifier /*modifiers*/,
		Qt::NoScrollPhase,
		false /*inverted*/
	};

	QPoint scrollRemainder{ 100, 0 };

	RepeatWheelEventsNTimesAndAssertEmpty(eventsPerScroll - 1, scrollRemainder, { evScrollDown } );

	const QString evStringScrollDown{ Shell::GetWheelEventStringAndSetScrollRemainder(
		evScrollDown, scrollRemainder, cellSize, deltasPerStep) };

	QCOMPARE(evStringScrollDown, QString{ "<ScrollWheelDown><12,19>" });
}

void TestScroll::ScrollLeft() noexcept
{
	const QWheelEvent evScrollLeft{
		QPointF{ 9.0, 5.0 } /*pos*/,
		QPointF{ 200.0, 200.0 } /*globalPos*/,
		QPoint{ 0, 0 } /*pixelDelta*/,
		QPoint{ scrollEventSize, 0 } /*angleDelta*/,
		Qt::NoButton /*buttons*/,
		Qt::NoModifier /*modifiers*/,
		Qt::NoScrollPhase,
		false /*inverted*/
	};

	QPoint scrollRemainder{ 0, 100 };

	RepeatWheelEventsNTimesAndAssertEmpty(eventsPerScroll - 1, scrollRemainder, { evScrollLeft});

	const QString evStringScrollLeft{ Shell::GetWheelEventStringAndSetScrollRemainder(
		evScrollLeft, scrollRemainder, cellSize, deltasPerStep) };

	QCOMPARE(evStringScrollLeft, QString{ "<ScrollWheelLeft><0,0>" });
}

void TestScroll::ScrollRight() noexcept
{
	const QWheelEvent evScrollRight{
		QPointF{ 300.0, 150.0 } /*pos*/,
		QPointF{ 200.0, 200.0 } /*globalPos*/,
		QPoint{ 0, 0 } /*pixelDelta*/,
		QPoint{ -scrollEventSize, 0 } /*angleDelta*/,
		Qt::NoButton /*buttons*/,
		Qt::NoModifier /*modifiers*/,
		Qt::NoScrollPhase,
		false /*inverted*/
	};

	QPoint scrollRemainder{ 0, 100 };

	RepeatWheelEventsNTimesAndAssertEmpty(eventsPerScroll - 1, scrollRemainder, { evScrollRight });

	const QString evStringScrollRight{ Shell::GetWheelEventStringAndSetScrollRemainder(
		evScrollRight, scrollRemainder, cellSize, deltasPerStep) };

	QCOMPARE(evStringScrollRight, QString{ "<ScrollWheelRight><30,15>" });
}

void TestScroll::ScrollDiagonal() noexcept
{
	// Combined Event: Up + Left
	const QWheelEvent evScrollUpLeft{
		QPointF{ 100.0, 100.0 } /*pos*/,
		QPointF{ 200.0, 200.0 } /*globalPos*/,
		QPoint{ 0, 0 } /*pixelDelta*/,
		QPoint{ scrollEventSize, scrollEventSize } /*angleDelta*/,
		Qt::NoButton /*buttons*/,
		Qt::NoModifier /*modifiers*/,
		Qt::NoScrollPhase,
		false /*inverted*/
	};

	QPoint scrollRemainder{ 0, 0 };

	RepeatWheelEventsNTimesAndAssertEmpty(eventsPerScroll - 1, scrollRemainder, { evScrollUpLeft });

	const QString evStringScrollUpLeft{ Shell::GetWheelEventStringAndSetScrollRemainder(
		evScrollUpLeft, scrollRemainder, cellSize, deltasPerStep) };

	QCOMPARE(evStringScrollUpLeft, QString{ "<ScrollWheelUp><10,10><ScrollWheelLeft><10,10>" });

	// Combined Event: Down + Right
	const QWheelEvent evScrollDownRight{
		QPointF{ 100.0, 100.0 } /*pos*/,
		QPointF{ 200.0, 200.0 } /*globalPos*/,
		QPoint{ 0, 0 } /*pixelDelta*/,
		QPoint{ -scrollEventSize, -scrollEventSize } /*angleDelta*/,
		Qt::NoButton /*buttons*/,
		Qt::NoModifier /*modifiers*/,
		Qt::NoScrollPhase,
		false /*inverted*/
	};

	scrollRemainder = { 0, 0 };

	RepeatWheelEventsNTimesAndAssertEmpty(eventsPerScroll - 1, scrollRemainder, { evScrollDownRight});

	const QString evStringScrollDownRight{ Shell::GetWheelEventStringAndSetScrollRemainder(
		evScrollDownRight, scrollRemainder, cellSize, deltasPerStep) };

	QCOMPARE(evStringScrollDownRight, QString{ "<ScrollWheelDown><10,10><ScrollWheelRight><10,10>" });
}

void TestScroll::ScrollAccumulation() noexcept
{
	const QWheelEvent evScrollUp{
		QPointF{ 100.0, 100.0 } /*pos*/,
		QPointF{ 200.0, 200.0 } /*globalPos*/,
		QPoint{ 0, 0 } /*pixelDelta*/,
		QPoint{ 0, scrollEventSize } /*angleDelta*/,
		Qt::NoButton /*buttons*/,
		Qt::NoModifier/*modifiers*/,
		Qt::NoScrollPhase,
		false /*inverted*/
	};

	const QWheelEvent evScrollDown{
		QPointF{ 200.0, 200.0 } /*pos*/,
		QPointF{ 200.0, 200.0 } /*globalPos*/,
		QPoint{ 0, 0 } /*pixelDelta*/,
		QPoint{ 0, -scrollEventSize } /*angleDelta*/,
		Qt::NoButton /*buttons*/,
		Qt::NoModifier/*modifiers*/,
		Qt::NoScrollPhase,
		false /*inverted*/
	};

	QPoint scrollRemainder;

	RepeatWheelEventsNTimesAndAssertEmpty(eventsPerScroll - 1, scrollRemainder, { evScrollUp, evScrollDown, evScrollUp });

	const QString evStringScrollUp{ Shell::GetWheelEventStringAndSetScrollRemainder(
		evScrollUp, scrollRemainder, cellSize, deltasPerStep) };

	QCOMPARE(evStringScrollUp, QString{ "<ScrollWheelUp><10,10>" });
}

void TestScroll::ScrollModifiers() noexcept
{
	// Shift Modifier
	const QWheelEvent evScrollShiftDown{
		QPointF{ 0.0, 0.0 } /*pos*/,
		QPointF{ 200.0, 200.0 } /*globalPos*/,
		QPoint{ 0, 0 } /*pixelDelta*/,
		QPoint{ 0, -deltasPerStep} /*angleDelta*/,
		Qt::NoButton /*buttons*/,
		Qt::ShiftModifier/*modifiers*/,
		Qt::NoScrollPhase,
		false /*inverted*/
	};

	QPoint scrollRemainder;

	const QString evStringScrollShiftDown{ Shell::GetWheelEventStringAndSetScrollRemainder(
		evScrollShiftDown, scrollRemainder, cellSize, deltasPerStep) };

	QCOMPARE(evStringScrollShiftDown, QString{ "<S-ScrollWheelDown><0,0>" });

	// Control Modifier
	const QWheelEvent evScrollCtrlDown{
		QPointF{ 0.0, 0.0 } /*pos*/,
		QPointF{ 200.0, 200.0 } /*globalPos*/,
		QPoint{ 0, 0 } /*pixelDelta*/,
		QPoint{ 0, -deltasPerStep} /*angleDelta*/,
		Qt::NoButton /*buttons*/,
		Input::ControlModifier() /*modifiers*/,
		Qt::NoScrollPhase,
		false /*inverted*/
	};

	const QString evStringScrollCtrlDown{ Shell::GetWheelEventStringAndSetScrollRemainder(
		evScrollCtrlDown, scrollRemainder, cellSize, deltasPerStep) };

	QCOMPARE(evStringScrollCtrlDown, QString{ "<C-ScrollWheelDown><0,0>" });

	// Alt Modifier
	const QWheelEvent evScrollAltDown{
		QPointF{ 0.0, 0.0 } /*pos*/,
		QPointF{ 200.0, 200.0 } /*globalPos*/,
		QPoint{ 0, 0 } /*pixelDelta*/,
		QPoint{ 0, -deltasPerStep} /*angleDelta*/,
		Qt::NoButton /*buttons*/,
		Qt::AltModifier /*modifiers*/,
		Qt::NoScrollPhase,
		false /*inverted*/
	};

	const QString evStringScrollAltDown{ Shell::GetWheelEventStringAndSetScrollRemainder(
		evScrollAltDown, scrollRemainder, cellSize, deltasPerStep) };

	QCOMPARE(evStringScrollAltDown, QString{ "<A-ScrollWheelDown><0,0>" });
}

void TestScroll::ScrollHighResolution() noexcept
{
	QVERIFY(false);
}

void TestScroll::ScrollInvertedEvents() noexcept
{
	QVERIFY(false);
}

} // Namespace NeovimQt

QTEST_MAIN(NeovimQt::TestScroll)

#include "tst_scroll.moc"
