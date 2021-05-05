#include <QtTest/QtTest>

#include <gui/input.h>

class TestInputWin32 : public QObject
{
	Q_OBJECT

private slots:
	void LessThanModifierKeys() noexcept;
	void SpecialKeys() noexcept;
	void CtrlCaretWellFormed() noexcept;
	void ShiftModifierLetter() noexcept;
	void GermanKeyboardLayout() noexcept;
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
		// On Mac Meta is the Control key, treated as C-.
		QList<QPair<QKeyEvent, QString>> keyEventList{
			{ { QEvent::KeyPress, k, Qt::NoModifier, {} },      "<%1>" },
			{ { QEvent::KeyPress, k, Qt::ControlModifier, {} }, "<C-%1>" },
			{ { QEvent::KeyPress, k, Qt::AltModifier, {} },     "<A-%1>" },
			{ { QEvent::KeyPress, k, Qt::MetaModifier, {} },    "<%1>" },
		};

		for (const auto& keyEvent : keyEventList) {
			QCOMPARE(NeovimQt::Input::convertKey(keyEvent.first),
				keyEvent.second.arg(NeovimQt::Input::GetSpecialKeysMap().value(k)));
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

#include "tst_input_win32.moc"
QTEST_MAIN(TestInputWin32)
