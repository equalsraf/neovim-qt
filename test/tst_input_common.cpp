#include <QtTest/QtTest>

#include <gui/input.h>

class TestInputCommon : public QObject
{
	Q_OBJECT

private slots:
	void LessThanKey() noexcept;
	void ModifierOnlyKeyEventsIgnored() noexcept;
	void ShiftKeyEventWellFormed() noexcept;
	void CapsLockIgnored() noexcept;
	void AltGrAloneIgnored() noexcept;
	void AltGrKeyEventWellFormed() noexcept;
};

void TestInputCommon::LessThanKey() noexcept
{
	// Issue#607: Shift is implied with "<", send "<lt>" instead.
	QKeyEvent evIssue607{ QEvent::KeyPress, Qt::Key::Key_Less, Qt::KeyboardModifier::ShiftModifier, "<" };
	QCOMPARE(NeovimQt::Input::convertKey(evIssue607), QString{ "<lt>" });
}

void TestInputCommon::ModifierOnlyKeyEventsIgnored() noexcept
{
	QKeyEvent meta{ QEvent::KeyPress, Qt::Key_Meta, Qt::MetaModifier, {} };
	QCOMPARE(NeovimQt::Input::convertKey(meta), QString{});

	QKeyEvent ctrl{ QEvent::KeyPress, Qt::Key_Control, Qt::ControlModifier, {} };
	QCOMPARE(NeovimQt::Input::convertKey(ctrl), QString{});

	QKeyEvent alt{ QEvent::KeyPress, Qt::Key_Alt, Qt::AltModifier, {} };
	QCOMPARE(NeovimQt::Input::convertKey(alt), QString{});

	const QList<Qt::Key> modifierKeyList {
		Qt::Key::Key_Alt,
		Qt::Key::Key_AltGr,
		Qt::Key::Key_Control,
		Qt::Key::Key_Meta,
		Qt::Key::Key_Shift,
		Qt::Key::Key_Super_L,
		Qt::Key::Key_Super_R,
	};

	const QList<Qt::KeyboardModifier> keyboardModifierList {
		Qt::KeyboardModifier::NoModifier,
		Qt::KeyboardModifier::AltModifier,
		Qt::KeyboardModifier::ControlModifier,
		Qt::KeyboardModifier::MetaModifier,
		Qt::KeyboardModifier::ShiftModifier,
		Qt::KeyboardModifier::GroupSwitchModifier,
	};

	for (const auto& key : modifierKeyList) {
		for (const auto& mod : keyboardModifierList) {
			QKeyEvent keyEvent{ QEvent::KeyPress, key, mod };
			QCOMPARE(NeovimQt::Input::convertKey(keyEvent), QString{});
		}
	}

	// Issue#344: <C-S-> appears as <C-Space>
	QKeyEvent evIssue344{ QEvent::KeyPress, Qt::Key::Key_Control, Qt::KeyboardModifier::ShiftModifier};
	QCOMPARE(NeovimQt::Input::convertKey(evIssue344), QString{});

	// Issue#593: Pressing Control + Super inserts ^S
	QKeyEvent evIssue593{ QEvent::KeyPress, Qt::Key::Key_Super_L, Qt::KeyboardModifier::ControlModifier};
	QCOMPARE(NeovimQt::Input::convertKey(evIssue593), QString{});
}

void TestInputCommon::AltGrAloneIgnored() noexcept
{
	// Issue#510: Pressing AltGr inserts weird char
	QKeyEvent event{ QEvent::KeyPress, Qt::Key::Key_AltGr, Qt::KeyboardModifier::GroupSwitchModifier};
	QCOMPARE(NeovimQt::Input::convertKey(event), QString{});
}

void TestInputCommon::AltGrKeyEventWellFormed() noexcept
{
	QKeyEvent event{ QEvent::KeyPress, Qt::Key::Key_AsciiTilde, Qt::KeyboardModifier::GroupSwitchModifier};
	QCOMPARE(NeovimQt::Input::convertKey(event), QString{ "~" });
}

void TestInputCommon::ShiftKeyEventWellFormed() noexcept
{
	QKeyEvent evShiftBracket{ QEvent::KeyPress, Qt::Key_BraceRight, Qt::ShiftModifier, "}" };
	QCOMPARE(NeovimQt::Input::convertKey(evShiftBracket), QString{ "}" });

	QKeyEvent evShiftQuote{ QEvent::KeyPress, Qt::Key_QuoteDbl, Qt::ShiftModifier, "\"" };
	QCOMPARE(NeovimQt::Input::convertKey(evShiftQuote), QString{ "\"" });

	QKeyEvent evShiftColon{ QEvent::KeyPress, Qt::Key_Colon, Qt::ShiftModifier, ":" };
	QCOMPARE(NeovimQt::Input::convertKey(evShiftColon), QString{ ":" });

	QKeyEvent evShiftA{ QEvent::KeyPress, Qt::Key_A, Qt::ShiftModifier, "A" };
	QCOMPARE(NeovimQt::Input::convertKey(evShiftA), QString{ "A" });

	QKeyEvent evShiftB{ QEvent::KeyPress, Qt::Key_B, Qt::ShiftModifier, "B" };
	QCOMPARE(NeovimQt::Input::convertKey(evShiftB), QString{ "B" });

	QKeyEvent evShiftC{ QEvent::KeyPress, Qt::Key_C, Qt::ShiftModifier, "C" };
	QCOMPARE(NeovimQt::Input::convertKey(evShiftC), QString{ "C" });
}

void TestInputCommon::CapsLockIgnored() noexcept
{
	// Issue#199: Pressing Control + CapsLock inserts $
	QKeyEvent evCapsLock{ QEvent::KeyPress, Qt::Key_CapsLock, Qt::NoModifier};
	QCOMPARE(NeovimQt::Input::convertKey(evCapsLock), QString{ "" });

	QKeyEvent evCtrlCapsLock{ QEvent::KeyPress, Qt::Key_CapsLock, Qt::ControlModifier };
	QCOMPARE(NeovimQt::Input::convertKey(evCtrlCapsLock), QString{ "" });

	QKeyEvent evMetaCapsLock{ QEvent::KeyPress, Qt::Key_CapsLock, Qt::MetaModifier};
	QCOMPARE(NeovimQt::Input::convertKey(evMetaCapsLock), QString{ "" });
}

#include "tst_input_common.moc"
QTEST_MAIN(TestInputCommon)
