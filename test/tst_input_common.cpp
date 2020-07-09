#include <QtTest/QtTest>

#include <gui/input.h>

class TestInputCommon : public QObject
{
	Q_OBJECT

private slots:
	// Keyboard Input
	void LessThanKey() noexcept;
	void ModifierOnlyKeyEventsIgnored() noexcept;
	void VolumeKeysIgnored() noexcept;
	void ShiftKeyEventWellFormed() noexcept;
	void CapsLockIgnored() noexcept;
	void AltGrAloneIgnored() noexcept;
	void AltGrKeyEventWellFormed() noexcept;
	void ShiftSpaceWellFormed() noexcept;
	void ShiftBackSpaceWellFormed() noexcept;
	void SpanishKeyboardLayout() noexcept;

	// Mouse Input
	void MouseLeftClick() noexcept;
	void MouseRightClick() noexcept;
	void MouseMiddleClick() noexcept;
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

void TestInputCommon::VolumeKeysIgnored() noexcept
{
	const QList<Qt::Key> volumeKeysList {
		Qt::Key::Key_VolumeDown,
		Qt::Key::Key_VolumeMute,
		Qt::Key::Key_VolumeUp,
	};
	for (const auto &key : volumeKeysList) {
		QKeyEvent keyEvent{ QEvent::KeyPress, key, Qt::KeyboardModifier::NoModifier };
		QCOMPARE(NeovimQt::Input::convertKey(keyEvent), QString{});
	}
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

void TestInputCommon::ShiftSpaceWellFormed() noexcept
{
	// Issue#728: Shift + Space inserts ;2u in `:terminal`, mode sent as <S-Space>.
	QKeyEvent evShift{ QEvent::KeyPress, Qt::Key_Shift, Qt::ShiftModifier, "" };
	QCOMPARE(NeovimQt::Input::convertKey(evShift), QString{ "" });

	QKeyEvent evShiftSpace{ QEvent::KeyPress, Qt::Key_Space, Qt::ShiftModifier, " " };
	QCOMPARE(NeovimQt::Input::convertKey(evShiftSpace), QString{ "<Space>" });
}

void TestInputCommon::ShiftBackSpaceWellFormed() noexcept
{
	// Issue#259: Shift + BackSpace inserts 7;2u in `:terminal`, mode sent as <S-BS>.
	QKeyEvent evShift{ QEvent::KeyPress, Qt::Key_Shift, Qt::ShiftModifier, "" };
	QCOMPARE(NeovimQt::Input::convertKey(evShift), QString{ "" });

	QKeyEvent evShiftBackSpace{ QEvent::KeyPress, Qt::Key_Backspace, Qt::ShiftModifier, "\b" };
	QCOMPARE(NeovimQt::Input::convertKey(evShiftBackSpace), QString{ "<BS>" });
}

void TestInputCommon::MouseLeftClick() noexcept
{
	QString leftClickPress{ NeovimQt::Input::convertMouse(
		Qt::LeftButton,
		QEvent::MouseButtonPress,
		Qt::NoModifier,
		{ 1, 2 },
		1 /*clickCount*/) };
	QString leftClickRelease{ NeovimQt::Input::convertMouse(
		Qt::LeftButton,
		QEvent::MouseButtonRelease,
		Qt::NoModifier,
		{ 1, 2 },
		1 /*clickCount*/) };

	QCOMPARE(leftClickPress, QString{ "<LeftMouse><1,2>" });
	QCOMPARE(leftClickRelease, QString{ "<LeftRelease><1,2>" });

	QString leftClickShiftPress{ NeovimQt::Input::convertMouse(
		Qt::LeftButton,
		QEvent::MouseButtonPress,
		Qt::ShiftModifier,
		{ 3, 4 },
		1 /*clickCount*/) };
	QString leftClickShiftRelease{ NeovimQt::Input::convertMouse(
		Qt::LeftButton,
		QEvent::MouseButtonRelease,
		Qt::ShiftModifier,
		{ 3, 4 },
		1 /*clickCount*/) };

	QCOMPARE(leftClickShiftPress, QString{ "<S-LeftMouse><3,4>" });
	QCOMPARE(leftClickShiftRelease, QString{ "<S-LeftRelease><3,4>" });

	QString leftDoubleClickPress{ NeovimQt::Input::convertMouse(
		Qt::LeftButton,
		QEvent::MouseButtonPress,
		Qt::NoModifier,
		{ 5, 6 },
		2 /*clickCount*/) };

	QString leftDoubleClickRelease{ NeovimQt::Input::convertMouse(
		Qt::LeftButton,
		QEvent::MouseButtonRelease,
		Qt::NoModifier,
		{ 5, 6 },
		2 /*clickCount*/) };

	QCOMPARE(leftDoubleClickPress, QString{ "<2-LeftMouse><5,6>" });
	QCOMPARE(leftDoubleClickRelease, QString{ "<2-LeftRelease><5,6>" });

	QString leftDoubleClickShiftPress{ NeovimQt::Input::convertMouse(
		Qt::LeftButton,
		QEvent::MouseButtonPress,
		Qt::ShiftModifier,
		{ 7, 8 },
		2 /*clickCount*/) };

	QString leftDoubleClickShiftRelease{ NeovimQt::Input::convertMouse(
		Qt::LeftButton,
		QEvent::MouseButtonRelease,
		Qt::ShiftModifier,
		{ 7, 8 },
		2 /*clickCount*/) };

	QCOMPARE(leftDoubleClickShiftPress, QString{ "<S-2-LeftMouse><7,8>" });
	QCOMPARE(leftDoubleClickShiftRelease, QString{ "<S-2-LeftRelease><7,8>" });
}

void TestInputCommon::MouseRightClick() noexcept
{
	QString rightClickPress{ NeovimQt::Input::convertMouse(
		Qt::RightButton,
		QEvent::MouseButtonPress,
		Qt::NoModifier,
		{ 1, 2 },
		1 /*clickCount*/) };
	QString rightClickRelease{ NeovimQt::Input::convertMouse(
		Qt::RightButton,
		QEvent::MouseButtonRelease,
		Qt::NoModifier,
		{ 1, 2 },
		1 /*clickCount*/) };

	QCOMPARE(rightClickPress, QString{ "<RightMouse><1,2>" });
	QCOMPARE(rightClickRelease, QString{ "<RightRelease><1,2>" });
}

void TestInputCommon::MouseMiddleClick() noexcept
{
	//Qt::MiddleButton
	QString middleClickPress{ NeovimQt::Input::convertMouse(
		Qt::MiddleButton,
		QEvent::MouseButtonPress,
		Qt::NoModifier,
		{ 1, 2 },
		1 /*clickCount*/) };
	QString middleClickRelease{ NeovimQt::Input::convertMouse(
		Qt::MiddleButton,
		QEvent::MouseButtonRelease,
		Qt::NoModifier,
		{ 1, 2 },
		1 /*clickCount*/) };

	QCOMPARE(middleClickPress, QString{ "<MiddleMouse><1,2>" });
	QCOMPARE(middleClickRelease, QString{ "<MiddleRelease><1,2>" });
}

void TestInputCommon::SpanishKeyboardLayout() noexcept
{
	// Issue 720: Spanish layout ignores Left Square Bracket [
	// NOTE: The "`" referenced below is "[" on a US layout keyboard for Windows/Linux and literal for MacOS.

	// Windows ` + Space. Prints: `
	QKeyEvent evAccentSpace{ QKeyEvent::KeyPress, Qt::Key_Space, Qt::NoModifier, QStringLiteral("`") };
	QCOMPARE(NeovimQt::Input::convertKey(evAccentSpace), QStringLiteral("`"));

	// Windows ``: two events are sent on the second key event. Prints: ``
	// NOTE: Linux/MacOS do not send QKeyEvents for this scenario.
	QKeyEvent evAccentFirst{ QKeyEvent::KeyPress, Qt::Key_QuoteLeft, Qt::NoModifier, QStringLiteral("`") };
	QKeyEvent evAccentSecond{ QKeyEvent::KeyPress, 0, Qt::NoModifier, QStringLiteral("`") };

	// Windows AltGr (Right Alt) + `. Prints: [
	QKeyEvent evAltGrSquareBracketWindows{ QKeyEvent::KeyPress, Qt::Key_AsciiCircum, Qt::AltModifier, QStringLiteral("[") };
	QCOMPARE(NeovimQt::Input::convertKey(evAltGrSquareBracketWindows), QStringLiteral("["));

	// Linux AltGr (Right Alt) + `. Prints: [
	QKeyEvent evAltGrSquareBracketLinux{ QKeyEvent::KeyPress, Qt::Key_BracketLeft, Qt::GroupSwitchModifier, QStringLiteral("[") };
	QCOMPARE(NeovimQt::Input::convertKey(evAltGrSquareBracketLinux), QStringLiteral("["));

//	// MacOS Alt + `: Prints [
//	QKeyEvent evAltLeftSquareBracketMacOS{ QKeyEvent::KeyPress, Qt::Key_Less, Qt::AltModifier, QStringLiteral("[") };
//	QCOMPARE(NeovimQt::Input::convertKey(evAltLeftSquareBracketMacOS), QStringLiteral("["));
//
//	// MacOS Alt + \: Prints [
//	QKeyEvent evAltRightSquareBracketMacOS{ QKeyEvent::KeyPress, Qt::Key_Apostrophe, Qt::AltModifier, QStringLiteral("]") };
//	QCOMPARE(NeovimQt::Input::convertKey(evAltRightSquareBracketMacOS), QStringLiteral("["));

	// Windows Shift + ` then Space. Prints ^
	// NOTE: Linux does not send QKeyEvents for this scenario.
	QKeyEvent evShiftAccentSpace{ QKeyEvent::KeyPress, Qt::Key_Space, Qt::NoModifier, QStringLiteral("^") };
	QCOMPARE(NeovimQt::Input::convertKey(evShiftAccentSpace), QStringLiteral("^"));

	// Windows Shift + ``. Prints ^^ (Windows) and ^ (Linux)
	// NOTE: Linux/MacOS do not send QKeyEvents for this scenario.
	QKeyEvent evShiftAccentAccent1{ QKeyEvent::KeyPress, Qt::Key_AsciiCircum, Qt::ShiftModifier, QStringLiteral("^") };
	QKeyEvent evShiftAccentAccent2{ QKeyEvent::KeyPress, 0, Qt::ShiftModifier, QStringLiteral("^") };
	QCOMPARE(NeovimQt::Input::convertKey(evShiftAccentAccent1), QStringLiteral("^"));
	QCOMPARE(NeovimQt::Input::convertKey(evShiftAccentAccent2), QStringLiteral("^"));

	// Windows ` then e. Prints: è
	// NOTE: Linux/MacOS do not send QKeyEvents for this scenario.
	QKeyEvent evAccentE{ QKeyEvent::KeyPress, Qt::Key_E, Qt::NoModifier, QStringLiteral("ê") };
	QCOMPARE(NeovimQt::Input::convertKey(evAccentE), QStringLiteral("ê"));

	// Windows Shift + ^ then e. Prints: ê
	// NOTE: Linux/MacOS do not send QKeyEvents for this scenario.
	QKeyEvent evShiftAccentE{ QKeyEvent::KeyPress, Qt::Key_E, Qt::NoModifier, QStringLiteral("ê") };
	QCOMPARE(NeovimQt::Input::convertKey(evShiftAccentE), QStringLiteral("ê"));
}

#include "tst_input_common.moc"
QTEST_MAIN(TestInputCommon)
