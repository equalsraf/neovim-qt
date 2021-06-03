#include <QtTest/QtTest>

#include <gui/input.h>
#include "tst_input.h"

class TestInputWin32 : public QObject
{
	Q_OBJECT

private slots:
	void LessThanModifierKeys() noexcept;
	void SpecialKeys() noexcept;
	void CtrlCaretWellFormed() noexcept;
	void ShiftModifierLetter() noexcept;
	void GermanKeyboardLayout() noexcept;
	void FrenchBepoKeyboardLayout() noexcept;
	void SpanishKeyboardLayout() noexcept;
};

void TestInputWin32::LessThanModifierKeys() noexcept
{
	QKeyEvent evLessThanControl{ QEvent::KeyPress, Qt::Key::Key_Less, Qt::ShiftModifier | Qt::ControlModifier, "<" };
	QCOMPARE(NeovimQt::Input::convertKey(evLessThanControl), QString{ "<C-lt>" });

	QKeyEvent evLessThanAlt{ QEvent::KeyPress, Qt::Key::Key_Less, Qt::ShiftModifier | Qt::AltModifier, "<" };
	QCOMPARE(NeovimQt::Input::convertKey(evLessThanAlt), QString{ "<A-lt>" });

	QKeyEvent evLessThanMeta{ QEvent::KeyPress, Qt::Key::Key_Less, Qt::ShiftModifier | Qt::MetaModifier, "<" };
	QCOMPARE(NeovimQt::Input::convertKey(evLessThanMeta), QString{ "<lt>" });
}

void TestInputWin32::SpecialKeys() noexcept
{
	const QList<int> specialKeys{ NeovimQt::Input::GetSpecialKeysMap().keys() };

	for (const auto k : specialKeys) {
		// Key_Space events send with text=" "
		QString text;
		if (k == Qt::Key_Space) {
			text = QStringLiteral(" ");
		}

		// On Mac Meta is the Control key, treated as C-.
		QList<InputTest> keyEventList{
			{ { QEvent::KeyPress, k, Qt::NoModifier, text },      "<%1>" },
			{ { QEvent::KeyPress, k, Qt::ControlModifier, text }, "<C-%1>" },
			{ { QEvent::KeyPress, k, Qt::AltModifier, text },     "<A-%1>" },
			{ { QEvent::KeyPress, k, Qt::MetaModifier, text },    "<%1>" },
		};

		for (const auto& keyTest : keyEventList) {
			QCOMPARE(NeovimQt::Input::convertKey(keyTest.event),
				keyTest.expected_input.arg(NeovimQt::Input::GetSpecialKeysMap().value(k)));
		}
	}
}

void TestInputWin32::CtrlCaretWellFormed() noexcept
{
	QKeyEvent evCtrl6{ QEvent::KeyPress, Qt::Key_6, Qt::ControlModifier, { "6" } };
	QCOMPARE(NeovimQt::Input::convertKey(evCtrl6), QString{ "<C-^>" });

	QKeyEvent evCtrlShift6{ QEvent::KeyPress, Qt::Key_AsciiCircum, Qt::ControlModifier | Qt::ShiftModifier, { "\u001E" } };
	QCOMPARE(NeovimQt::Input::convertKey(evCtrlShift6), QString{ "<C-^>" });
}

void TestInputWin32::ShiftModifierLetter() noexcept
{
	// CTRL + B
	QKeyEvent evCtrlB{ QEvent::KeyPress, Qt::Key_B, Qt::ControlModifier, QString{ "\u0002" } };
	QCOMPARE(NeovimQt::Input::convertKey(evCtrlB), QString{ "<C-b>" });

	// CTRL + SHIFT + B
	QKeyEvent evCtrlShiftB{ QEvent::KeyPress, Qt::Key_B, Qt::ControlModifier | Qt::ShiftModifier,
		QString{ "\u0002" } };
	QCOMPARE(NeovimQt::Input::convertKey(evCtrlShiftB), QString{ "<C-S-B>" });
}

void TestInputWin32::GermanKeyboardLayout() noexcept
{
	QKeyEvent evAlt7{ QEvent::KeyPress, Qt::Key_BraceLeft, Qt::ControlModifier | Qt::AltModifier, "{" };
	QCOMPARE(NeovimQt::Input::convertKey(evAlt7), QString{ "{" });

	QKeyEvent evAlt8{ QEvent::KeyPress, Qt::Key_BracketLeft, Qt::ControlModifier | Qt::AltModifier, "[" };
	QCOMPARE(NeovimQt::Input::convertKey(evAlt8), QString{ "[" });

	QKeyEvent evAlt9{ QEvent::KeyPress, Qt::Key_BracketRight, Qt::ControlModifier | Qt::AltModifier, "]" };
	QCOMPARE(NeovimQt::Input::convertKey(evAlt9), QString{ "]" });

	QKeyEvent evAlt0{ QEvent::KeyPress, Qt::Key_BraceRight, Qt::ControlModifier | Qt::AltModifier, "}" };
	QCOMPARE(NeovimQt::Input::convertKey(evAlt0), QString{ "}" });

	QKeyEvent evKeyAt{ QEvent::KeyPress, Qt::Key_At, Qt::ControlModifier | Qt::AltModifier, "@" };
	QCOMPARE(NeovimQt::Input::convertKey(evKeyAt), QString{ "@" });

	QKeyEvent evKeyBSlash{ QEvent::KeyPress, Qt::Key_Backslash, Qt::ControlModifier | Qt::AltModifier, "\\" };
	QCOMPARE(NeovimQt::Input::convertKey(evKeyBSlash), QString{ "<Bslash>" });

	QKeyEvent evKeyTilde{ QEvent::KeyPress, Qt::Key_AsciiTilde, Qt::ControlModifier | Qt::AltModifier, "~" };
	QCOMPARE(NeovimQt::Input::convertKey(evKeyTilde), QString{ "~" });

	QKeyEvent evLeftAlt8{ QEvent::KeyPress, Qt::Key_8, Qt::AltModifier, "8" };
	QCOMPARE(NeovimQt::Input::convertKey(evLeftAlt8), QString{ "<A-8>" });
}

void TestInputWin32::FrenchBepoKeyboardLayout() noexcept
{
	QKeyEvent evTilde{ QEvent::KeyPress, Qt::Key_Space, Qt::NoModifier, "~" };
	QCOMPARE(NeovimQt::Input::convertKey(evTilde), QString{ "~" });

	QKeyEvent evCircum{ QEvent::KeyPress, Qt::Key_Space, Qt::NoModifier, "^" };
	QCOMPARE(NeovimQt::Input::convertKey(evCircum), QString{ "^" });

	QKeyEvent evShiftCircum{ QEvent::KeyPress, Qt::Key_Space, Qt::ShiftModifier, "^" };
	QCOMPARE(NeovimQt::Input::convertKey(evShiftCircum), QString{ "<S-^>" });

	QKeyEvent evSpace{ QEvent::KeyPress, Qt::Key_Space, Qt::NoModifier, " " };
	QCOMPARE(NeovimQt::Input::convertKey(evSpace), QString{ "<Space>" });
}

void TestInputWin32::SpanishKeyboardLayout() noexcept
{
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

#include "tst_input_win32.moc"
QTEST_MAIN(TestInputWin32)
