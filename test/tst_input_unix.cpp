#include <QtTest/QtTest>

#include <gui/input.h>
#include "tst_input.h"

class TestInputUnix : public QObject
{
	Q_OBJECT

private slots:
	void LessThanModifierKeys() noexcept;
	void SpecialKeys() noexcept;
	void CtrlCaretWellFormed() noexcept;
	void ShiftModifierLetter() noexcept;
	void GermanKeyboardLayout() noexcept;
	void ControlSpace() noexcept;
};

void TestInputUnix::LessThanModifierKeys() noexcept
{
	QKeyEvent evLessThanControl{ QEvent::KeyPress, Qt::Key::Key_Less, Qt::ShiftModifier | Qt::ControlModifier, "<" };
	QCOMPARE(NeovimQt::Input::convertKey(evLessThanControl), QString{ "<C-lt>" });

	QKeyEvent evLessThanAlt{ QEvent::KeyPress, Qt::Key::Key_Less, Qt::ShiftModifier | Qt::AltModifier, "<" };
	QCOMPARE(NeovimQt::Input::convertKey(evLessThanAlt), QString{ "<A-lt>" });

	QKeyEvent evLessThanMeta{ QEvent::KeyPress, Qt::Key::Key_Less, Qt::ShiftModifier | Qt::MetaModifier, "<" };
	QCOMPARE(NeovimQt::Input::convertKey(evLessThanMeta), QString{ "<D-lt>" });
}

void TestInputUnix::SpecialKeys() noexcept
{
	const QList<int> specialKeys{ NeovimQt::Input::GetSpecialKeysMap().keys() };

	for (const auto k : specialKeys) {
		// On Mac Meta is the Control key, treated as C-.
		QList<InputTest> keyEventList{
			{ QEvent::KeyPress, k, Qt::NoModifier,		"<%1>" },
			{ QEvent::KeyPress, k, Qt::ControlModifier, "<C-%1>" },
			{ QEvent::KeyPress, k, Qt::AltModifier,     "<A-%1>" },
			{ QEvent::KeyPress, k, Qt::MetaModifier,    "<D-%1>" },
		};

		for (const auto& keyTest : keyEventList) {
			auto event = QKeyEvent(keyTest.event_type, keyTest.key, keyTest.modifiers);
			QCOMPARE(NeovimQt::Input::convertKey(event),
				keyTest.expected_input.arg(NeovimQt::Input::GetSpecialKeysMap().value(k)));
		}
	}
}

void TestInputUnix::CtrlCaretWellFormed() noexcept
{
	QKeyEvent evCtrl6{ QEvent::KeyPress, Qt::Key_6, Qt::ControlModifier, { "\u001E" } };
	QCOMPARE(NeovimQt::Input::convertKey(evCtrl6), QString{ "<C-^>" });

	QKeyEvent evCtrlShift6{ QEvent::KeyPress, Qt::Key_AsciiCircum, Qt::ControlModifier | Qt::ShiftModifier, { "\u001E" } };
	QCOMPARE(NeovimQt::Input::convertKey(evCtrlShift6), QString{ "<C-^>" });

	QKeyEvent evCtrlShiftMeta6{ QEvent::KeyPress, Qt::Key_AsciiCircum,
		Qt::MetaModifier | Qt::ShiftModifier | Qt::ControlModifier , { "\u001E" } };
	QCOMPARE(NeovimQt::Input::convertKey(evCtrlShiftMeta6), QString{ "<C-^>" });
}

void TestInputUnix::ShiftModifierLetter() noexcept
{
	// CTRL + B
	QKeyEvent evCtrlB{ QEvent::KeyPress, Qt::Key_B, Qt::ControlModifier, QString{ "\u0002" } };
	QCOMPARE(NeovimQt::Input::convertKey(evCtrlB), QString{ "<C-b>" });

	// CTRL + SHIFT + B
	QKeyEvent evCtrlShiftB{ QEvent::KeyPress, Qt::Key_B, Qt::ControlModifier | Qt::ShiftModifier,
		QString{ "\u0002" } };
	QCOMPARE(NeovimQt::Input::convertKey(evCtrlShiftB), QString{ "<C-S-B>" });
}

void TestInputUnix::GermanKeyboardLayout() noexcept
{
	QKeyEvent evOption7{ QEvent::KeyPress, Qt::Key_BraceLeft, Qt::GroupSwitchModifier, "{" };
	QCOMPARE(NeovimQt::Input::convertKey(evOption7), QString{ "{" });

	QKeyEvent evOption8{ QEvent::KeyPress, Qt::Key_BracketLeft, Qt::GroupSwitchModifier, "[" };
	QCOMPARE(NeovimQt::Input::convertKey(evOption8), QString{ "[" });

	QKeyEvent evOption9{ QEvent::KeyPress, Qt::Key_BracketRight, Qt::GroupSwitchModifier, "]" };
	QCOMPARE(NeovimQt::Input::convertKey(evOption9), QString{ "]" });

	QKeyEvent evOption0{ QEvent::KeyPress, Qt::Key_BraceRight, Qt::GroupSwitchModifier, "}" };
	QCOMPARE(NeovimQt::Input::convertKey(evOption0), QString{ "}" });

	QKeyEvent evOptionQ{ QEvent::KeyPress, Qt::Key_At, Qt::GroupSwitchModifier, "@" };
	QCOMPARE(NeovimQt::Input::convertKey(evOptionQ), QString{ "@" });

	QKeyEvent evOptionBeta{ QEvent::KeyPress, Qt::Key_Backslash, Qt::GroupSwitchModifier, "\\" };
	QCOMPARE(NeovimQt::Input::convertKey(evOptionBeta), QString{ "<Bslash>" });

	QKeyEvent evOptionPlus{ QEvent::KeyPress, Qt::Key_AsciiTilde, Qt::GroupSwitchModifier, "~" };
	QCOMPARE(NeovimQt::Input::convertKey(evOptionPlus), QString{ "~" });
}

void TestInputUnix::ControlSpace() noexcept
{
	// Intentionally written with QStringLiteral, other alternatives do not create the same QKeyEvent
	QKeyEvent evControlSpace{ QEvent::KeyPress, Qt::Key_Space, Qt::ControlModifier, QStringLiteral( "\u0000" ) };
	QCOMPARE(NeovimQt::Input::convertKey(evControlSpace), QString{ "<C-Space>" });
}

#include "tst_input_unix.moc"
QTEST_MAIN(TestInputUnix)
