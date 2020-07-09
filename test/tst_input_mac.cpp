#include <QtTest/QtTest>

#include <gui/input.h>
#include "tst_input.h"

class TestInputMac : public QObject
{
	Q_OBJECT

private slots:
	void AltSpecialCharacters() noexcept;
	void LessThanModifierKeys() noexcept;
	void SpecialKeys() noexcept;
	void KeyboardLayoutUnicodeHexInput() noexcept;
	void CtrlCaretWellFormed() noexcept;
	void ShiftModifierLetter() noexcept;
	void GermanKeyboardLayout() noexcept;
};

void TestInputMac::AltSpecialCharacters() noexcept
{
	// Issue#510: MacOS Alt special key input does not work.
	QKeyEvent evAltA{ QEvent::KeyPress, Qt::Key_A, Qt::AltModifier, "å" };
	QCOMPARE(NeovimQt::Input::convertKey(evAltA), QString{ "å" });

	QKeyEvent evShiftAltA{ QEvent::KeyPress, Qt::Key_A, Qt::ShiftModifier | Qt::AltModifier, "Å" };
	QCOMPARE(NeovimQt::Input::convertKey(evShiftAltA), QString{ "Å" });

	QKeyEvent evShiftAltL{ QEvent::KeyPress, Qt::Key_L, Qt::ShiftModifier | Qt::AltModifier, "Ò" };
	QCOMPARE(NeovimQt::Input::convertKey(evShiftAltL), QString{ "Ò" });
}

void TestInputMac::LessThanModifierKeys() noexcept
{
	QKeyEvent evLessThanControl{ QEvent::KeyPress, Qt::Key::Key_Less, Qt::ShiftModifier | Qt::ControlModifier, "<" };
	QCOMPARE(NeovimQt::Input::convertKey(evLessThanControl), QString{ "<D-lt>" });

	QKeyEvent evLessThanAlt{ QEvent::KeyPress, Qt::Key::Key_Less, Qt::ShiftModifier | Qt::AltModifier, "<" };
	QCOMPARE(NeovimQt::Input::convertKey(evLessThanAlt), QString{ "<A-lt>" });

	QKeyEvent evLessThanMeta{ QEvent::KeyPress, Qt::Key::Key_Less, Qt::ShiftModifier | Qt::MetaModifier, "<" };
	QCOMPARE(NeovimQt::Input::convertKey(evLessThanMeta), QString{ "<C-lt>" });
}

void TestInputMac::SpecialKeys() noexcept
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
			{ { QEvent::KeyPress, k, Qt::ControlModifier, text }, "<D-%1>" },
			{ { QEvent::KeyPress, k, Qt::AltModifier, text },     "<A-%1>" },
			{ { QEvent::KeyPress, k, Qt::MetaModifier,text },     "<C-%1>" },
		};

		for (const auto& keyTest : keyEventList) {
			QCOMPARE(NeovimQt::Input::convertKey(keyTest.event),
				keyTest.expected_input.arg(NeovimQt::Input::GetSpecialKeysMap().value(k)));
		}
	}
}

void TestInputMac::KeyboardLayoutUnicodeHexInput() noexcept
{
	// Issue#579: Cannot map <A-...> on MacOS
	QKeyEvent evAltA{ QEvent::KeyPress, Qt::Key_A, Qt::AltModifier };
	QCOMPARE(NeovimQt::Input::convertKey(evAltA), QString{ "<A-a>" });

	QKeyEvent evAltShiftA{ QEvent::KeyPress, Qt::Key_A, Qt::AltModifier | Qt::ShiftModifier };
	QCOMPARE(NeovimQt::Input::convertKey(evAltShiftA), QString{ "<A-A>" });

	QKeyEvent evCtrlAltA{ QEvent::KeyPress, Qt::Key_A, Qt::MetaModifier | Qt::AltModifier };
	QCOMPARE(NeovimQt::Input::convertKey(evCtrlAltA), QString{ "<C-A-a>" });

	QKeyEvent evCtrlAltShiftA{ QEvent::KeyPress, Qt::Key_A,
		Qt::MetaModifier | Qt::AltModifier | Qt::ShiftModifier };
	QCOMPARE(NeovimQt::Input::convertKey(evCtrlAltShiftA), QString{ "<C-S-A-A>" });
}

void TestInputMac::CtrlCaretWellFormed() noexcept
{
	QKeyEvent evCtrl6{ QEvent::KeyPress, Qt::Key_6, Qt::MetaModifier };
	QCOMPARE(NeovimQt::Input::convertKey(evCtrl6), QString{ "<C-^>" });

	QKeyEvent evCtrlShift6{ QEvent::KeyPress, Qt::Key_AsciiCircum, Qt::MetaModifier | Qt::ShiftModifier };
	QCOMPARE(NeovimQt::Input::convertKey(evCtrlShift6), QString{ "<C-^>" });

	QKeyEvent evCtrlShiftMeta6{ QEvent::KeyPress, Qt::Key_AsciiCircum,
		Qt::MetaModifier | Qt::ShiftModifier | Qt::ControlModifier };
	QCOMPARE(NeovimQt::Input::convertKey(evCtrlShiftMeta6), QString{ "<C-^>" });
}

void TestInputMac::ShiftModifierLetter() noexcept
{
	// Issue#817: Shift should be sent if modifier keys are present
	// For example, Ctrl + Shift + A is <C-S-A> and not <C-A>

	// CTRL + B
	QKeyEvent evCtrlB{ QEvent::KeyPress, Qt::Key_B, Qt::MetaModifier };
	QCOMPARE(NeovimQt::Input::convertKey(evCtrlB), QString{ "<C-b>" });

	// CTRL + SHIFT + B
	QKeyEvent evCtrlShiftB{ QEvent::KeyPress, Qt::Key_B, Qt::MetaModifier | Qt::ShiftModifier };
	QCOMPARE(NeovimQt::Input::convertKey(evCtrlShiftB), QString{ "<C-S-B>" });
}

void TestInputMac::GermanKeyboardLayout() noexcept
{
	QKeyEvent evOption5{ QEvent::KeyPress, Qt::Key_5, Qt::AltModifier, "[" };
	QCOMPARE(NeovimQt::Input::convertKey(evOption5), QString{ "[" });

	QKeyEvent evOption6{ QEvent::KeyPress, Qt::Key_6, Qt::AltModifier, "]" };
	QCOMPARE(NeovimQt::Input::convertKey(evOption6), QString{ "]" });

	QKeyEvent evOption7{ QEvent::KeyPress, Qt::Key_7, Qt::AltModifier, "|" };
	QCOMPARE(NeovimQt::Input::convertKey(evOption7), QString{ "|" });

	QKeyEvent evOption8{ QEvent::KeyPress, Qt::Key_8, Qt::AltModifier, "{" };
	QCOMPARE(NeovimQt::Input::convertKey(evOption8), QString{ "{" });

	QKeyEvent evOption9{ QEvent::KeyPress, Qt::Key_9, Qt::AltModifier, "}" };
	QCOMPARE(NeovimQt::Input::convertKey(evOption9), QString{ "}" });

	QKeyEvent evOptionTilde{ QEvent::KeyPress, Qt::Key_N, Qt::AltModifier, "~" };
	QCOMPARE(NeovimQt::Input::convertKey(evOptionTilde), QString{ "~" });

	QKeyEvent evOptionAtSign{ QEvent::KeyPress, Qt::Key_L, Qt::AltModifier, "@" };
	QCOMPARE(NeovimQt::Input::convertKey(evOptionAtSign), QString{ "@" });
}

#include "tst_input_mac.moc"
QTEST_MAIN(TestInputMac)
