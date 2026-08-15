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
	void MacOptionIsMetaNone() noexcept;
	void MacOptionIsMetaBoth() noexcept;
	void MacOptionIsMetaBothNumbers() noexcept;
	void MacOptionIsMetaBothSpecialKeys() noexcept;
	void MacOptionIsMetaBothLessThanBackslash() noexcept;
	void MacOptionIsMetaBothCmdCombo() noexcept;
	void MacOptionIsMetaBothGermanLayout() noexcept;
	void MacOptionIsMetaLeftRight() noexcept;
	void MacOptionIsMetaLeftRightBothPressed() noexcept;
	void MacOptionIsMetaModeToggle() noexcept;
	void GetOptionAsMetaTextDirect() noexcept;
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
		// On Mac Meta is the Control key, treated as C-.
		QList<InputTest> keyEventList{
			{ QEvent::KeyPress, k, Qt::NoModifier,       "<%1>" },
			{ QEvent::KeyPress, k, Qt::ControlModifier,  "<D-%1>" },
			{ QEvent::KeyPress, k, Qt::AltModifier,      "<A-%1>" },
			{ QEvent::KeyPress, k, Qt::MetaModifier,     "<C-%1>" },
		};

		for (const auto& keyTest : keyEventList) {
			auto event = QKeyEvent(keyTest.event_type, keyTest.key, keyTest.modifiers);
			QCOMPARE(NeovimQt::Input::convertKey(event),
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

void TestInputMac::MacOptionIsMetaNone() noexcept
{
	// Default mode: Option+key produces Unicode character, not <A-key>
	NeovimQt::Input::SetMacOptionIsMeta(NeovimQt::Input::MacOptionMetaMode::None);

	QKeyEvent evAltJ{ QEvent::KeyPress, Qt::Key_J, Qt::AltModifier, "∆" };
	QCOMPARE(NeovimQt::Input::convertKey(evAltJ), QString{ "∆" });

	QKeyEvent evAltB{ QEvent::KeyPress, Qt::Key_B, Qt::AltModifier, "∫" };
	QCOMPARE(NeovimQt::Input::convertKey(evAltB), QString{ "∫" });

	QKeyEvent evAltA{ QEvent::KeyPress, Qt::Key_A, Qt::AltModifier, "å" };
	QCOMPARE(NeovimQt::Input::convertKey(evAltA), QString{ "å" });
}

void TestInputMac::MacOptionIsMetaBoth() noexcept
{
	NeovimQt::Input::SetMacOptionIsMeta(NeovimQt::Input::MacOptionMetaMode::Both);

	// Option+J should produce <A-j> instead of ∆
	QKeyEvent evAltJ{ QEvent::KeyPress, Qt::Key_J, Qt::AltModifier, "∆" };
	QCOMPARE(NeovimQt::Input::convertKey(evAltJ), QString{ "<A-j>" });

	// Option+B should produce <A-b> instead of ∫
	QKeyEvent evAltB{ QEvent::KeyPress, Qt::Key_B, Qt::AltModifier, "∫" };
	QCOMPARE(NeovimQt::Input::convertKey(evAltB), QString{ "<A-b>" });

	// Option+A should produce <A-a> instead of å
	QKeyEvent evAltA{ QEvent::KeyPress, Qt::Key_A, Qt::AltModifier, "å" };
	QCOMPARE(NeovimQt::Input::convertKey(evAltA), QString{ "<A-a>" });

	// Option+Shift+J should produce <A-J>
	QKeyEvent evAltShiftJ{ QEvent::KeyPress, Qt::Key_J, Qt::AltModifier | Qt::ShiftModifier, "Ô" };
	QCOMPARE(NeovimQt::Input::convertKey(evAltShiftJ), QString{ "<A-J>" });

	// Ctrl(Meta on Mac)+Option+J should produce <C-A-j>
	QKeyEvent evCtrlAltJ{ QEvent::KeyPress, Qt::Key_J, Qt::MetaModifier | Qt::AltModifier, "∆" };
	QCOMPARE(NeovimQt::Input::convertKey(evCtrlAltJ), QString{ "<C-A-j>" });

	// Ctrl+Shift+Option+J should produce <C-S-A-J>
	QKeyEvent evCtrlShiftAltJ{
		QEvent::KeyPress, Qt::Key_J, Qt::MetaModifier | Qt::ShiftModifier | Qt::AltModifier, "Ô"
	};
	QCOMPARE(NeovimQt::Input::convertKey(evCtrlShiftAltJ), QString{ "<C-S-A-J>" });

	// Restore default
	NeovimQt::Input::SetMacOptionIsMeta(NeovimQt::Input::MacOptionMetaMode::None);
}

void TestInputMac::MacOptionIsMetaBothNumbers() noexcept
{
	NeovimQt::Input::SetMacOptionIsMeta(NeovimQt::Input::MacOptionMetaMode::Both);

	// Option+1 should produce <A-1> instead of ¡
	QKeyEvent evAlt1{ QEvent::KeyPress, Qt::Key_1, Qt::AltModifier, "¡" };
	QCOMPARE(NeovimQt::Input::convertKey(evAlt1), QString{ "<A-1>" });

	// Option+2 should produce <A-2> instead of ™
	QKeyEvent evAlt2{ QEvent::KeyPress, Qt::Key_2, Qt::AltModifier, "™" };
	QCOMPARE(NeovimQt::Input::convertKey(evAlt2), QString{ "<A-2>" });

	// Option+0 should produce <A-0> instead of º
	QKeyEvent evAlt0{ QEvent::KeyPress, Qt::Key_0, Qt::AltModifier, "º" };
	QCOMPARE(NeovimQt::Input::convertKey(evAlt0), QString{ "<A-0>" });

	// Restore default
	NeovimQt::Input::SetMacOptionIsMeta(NeovimQt::Input::MacOptionMetaMode::None);
}

void TestInputMac::MacOptionIsMetaBothSpecialKeys() noexcept
{
	NeovimQt::Input::SetMacOptionIsMeta(NeovimQt::Input::MacOptionMetaMode::Both);

	// Special keys (Backspace, Tab, Escape, etc.) go through the specialKeys
	// map before the MacOptionIsMeta override. They should still be handled
	// correctly via the specialKeys path with the A- prefix.
	QKeyEvent evAltBS{ QEvent::KeyPress, Qt::Key_Backspace, Qt::AltModifier };
	QCOMPARE(NeovimQt::Input::convertKey(evAltBS), QString{ "<A-BS>" });

	QKeyEvent evAltTab{ QEvent::KeyPress, Qt::Key_Tab, Qt::AltModifier };
	QCOMPARE(NeovimQt::Input::convertKey(evAltTab), QString{ "<A-Tab>" });

	QKeyEvent evAltEsc{ QEvent::KeyPress, Qt::Key_Escape, Qt::AltModifier };
	QCOMPARE(NeovimQt::Input::convertKey(evAltEsc), QString{ "<A-Esc>" });

	QKeyEvent evAltEnter{ QEvent::KeyPress, Qt::Key_Return, Qt::AltModifier };
	QCOMPARE(NeovimQt::Input::convertKey(evAltEnter), QString{ "<A-Enter>" });

	QKeyEvent evAltSpace{ QEvent::KeyPress, Qt::Key_Space, Qt::AltModifier, " " };
	QCOMPARE(NeovimQt::Input::convertKey(evAltSpace), QString{ "<A-Space>" });

	// Arrow keys with Alt should be unaffected
	QKeyEvent evAltUp{ QEvent::KeyPress, Qt::Key_Up, Qt::AltModifier };
	QCOMPARE(NeovimQt::Input::convertKey(evAltUp), QString{ "<A-Up>" });

	// Restore default
	NeovimQt::Input::SetMacOptionIsMeta(NeovimQt::Input::MacOptionMetaMode::None);
}

void TestInputMac::MacOptionIsMetaBothLessThanBackslash() noexcept
{
	NeovimQt::Input::SetMacOptionIsMeta(NeovimQt::Input::MacOptionMetaMode::Both);

	// Option+< should produce <A-lt>. The text override replaces the Unicode
	// char, and the "<" special handling in convertKey formats it as lt.
	QKeyEvent evAltLessThan{
		QEvent::KeyPress, Qt::Key_Less, Qt::AltModifier | Qt::ShiftModifier, "¯"
	};
	QCOMPARE(NeovimQt::Input::convertKey(evAltLessThan), QString{ "<A-lt>" });

	// Option+\ should produce <A-Bslash>
	QKeyEvent evAltBackslash{ QEvent::KeyPress, Qt::Key_Backslash, Qt::AltModifier, "«" };
	QCOMPARE(NeovimQt::Input::convertKey(evAltBackslash), QString{ "<A-Bslash>" });

	// Restore default
	NeovimQt::Input::SetMacOptionIsMeta(NeovimQt::Input::MacOptionMetaMode::None);
}

void TestInputMac::MacOptionIsMetaBothCmdCombo() noexcept
{
	NeovimQt::Input::SetMacOptionIsMeta(NeovimQt::Input::MacOptionMetaMode::Both);

	// Cmd(D-)+Option+J should produce <D-A-j>
	// On Mac, ControlModifier maps to Cmd/D-
	QKeyEvent evCmdAltJ{ QEvent::KeyPress, Qt::Key_J, Qt::ControlModifier | Qt::AltModifier, "∆" };
	QCOMPARE(NeovimQt::Input::convertKey(evCmdAltJ), QString{ "<D-A-j>" });

	// Cmd+Ctrl+Option+J should produce <D-C-A-j>
	QKeyEvent evCmdCtrlAltJ{
		QEvent::KeyPress, Qt::Key_J, Qt::ControlModifier | Qt::MetaModifier | Qt::AltModifier, "∆"
	};
	QCOMPARE(NeovimQt::Input::convertKey(evCmdCtrlAltJ), QString{ "<D-C-A-j>" });

	// Restore default
	NeovimQt::Input::SetMacOptionIsMeta(NeovimQt::Input::MacOptionMetaMode::None);
}

void TestInputMac::MacOptionIsMetaBothGermanLayout() noexcept
{
	NeovimQt::Input::SetMacOptionIsMeta(NeovimQt::Input::MacOptionMetaMode::Both);

	// When MacOptionIsMeta is Both, German keyboard Option+8 should produce
	// <A-8> instead of "{". The user opted into Meta mode, so all Option
	// presses become Alt modifiers — overriding locale-specific behavior.
	QKeyEvent evOption8{ QEvent::KeyPress, Qt::Key_8, Qt::AltModifier, "{" };
	QCOMPARE(NeovimQt::Input::convertKey(evOption8), QString{ "<A-8>" });

	QKeyEvent evOption5{ QEvent::KeyPress, Qt::Key_5, Qt::AltModifier, "[" };
	QCOMPARE(NeovimQt::Input::convertKey(evOption5), QString{ "<A-5>" });

	QKeyEvent evOption7{ QEvent::KeyPress, Qt::Key_7, Qt::AltModifier, "|" };
	QCOMPARE(NeovimQt::Input::convertKey(evOption7), QString{ "<A-7>" });

	// Restore default and verify German layout works again
	NeovimQt::Input::SetMacOptionIsMeta(NeovimQt::Input::MacOptionMetaMode::None);

	QKeyEvent evOption8None{ QEvent::KeyPress, Qt::Key_8, Qt::AltModifier, "{" };
	QCOMPARE(NeovimQt::Input::convertKey(evOption8None), QString{ "{" });
}

void TestInputMac::MacOptionIsMetaLeftRight() noexcept
{
	constexpr quint32 NativeLeftOptionMask{ 0x00000020 };
	constexpr quint32 NativeRightOptionMask{ 0x00000040 };

	// Mode: Left — only left Option acts as Meta
	NeovimQt::Input::SetMacOptionIsMeta(NeovimQt::Input::MacOptionMetaMode::Left);

	QKeyEvent evLeftAltJ{ QEvent::KeyPress,
		Qt::Key_J,
		Qt::AltModifier,
		0 /*nativeScanCode*/,
		0 /*nativeVirtualKey*/,
		NativeLeftOptionMask,
		"∆" };
	QCOMPARE(NeovimQt::Input::convertKey(evLeftAltJ), QString{ "<A-j>" });

	// Right Option should pass through Unicode character
	QKeyEvent evRightAltJ{
		QEvent::KeyPress, Qt::Key_J, Qt::AltModifier, 0, 0, NativeRightOptionMask, "∆"
	};
	QCOMPARE(NeovimQt::Input::convertKey(evRightAltJ), QString{ "∆" });

	// Left Option + Shift
	QKeyEvent evLeftAltShiftJ{ QEvent::KeyPress,
		Qt::Key_J,
		Qt::AltModifier | Qt::ShiftModifier,
		0,
		0,
		NativeLeftOptionMask,
		"Ô" };
	QCOMPARE(NeovimQt::Input::convertKey(evLeftAltShiftJ), QString{ "<A-J>" });

	// Mode: Right — only right Option acts as Meta
	NeovimQt::Input::SetMacOptionIsMeta(NeovimQt::Input::MacOptionMetaMode::Right);

	QKeyEvent evRightAltB{
		QEvent::KeyPress, Qt::Key_B, Qt::AltModifier, 0, 0, NativeRightOptionMask, "∫"
	};
	QCOMPARE(NeovimQt::Input::convertKey(evRightAltB), QString{ "<A-b>" });

	// Left Option should pass through Unicode character
	QKeyEvent evLeftAltB{
		QEvent::KeyPress, Qt::Key_B, Qt::AltModifier, 0, 0, NativeLeftOptionMask, "∫"
	};
	QCOMPARE(NeovimQt::Input::convertKey(evLeftAltB), QString{ "∫" });

	// Right Option + Ctrl
	QKeyEvent evRightCtrlAltB{ QEvent::KeyPress,
		Qt::Key_B,
		Qt::MetaModifier | Qt::AltModifier,
		0,
		0,
		NativeRightOptionMask,
		"∫" };
	QCOMPARE(NeovimQt::Input::convertKey(evRightCtrlAltB), QString{ "<C-A-b>" });

	// Restore default
	NeovimQt::Input::SetMacOptionIsMeta(NeovimQt::Input::MacOptionMetaMode::None);
}

void TestInputMac::MacOptionIsMetaLeftRightBothPressed() noexcept
{
	constexpr quint32 NativeLeftOptionMask{ 0x00000020 };
	constexpr quint32 NativeRightOptionMask{ 0x00000040 };
	constexpr quint32 NativeBothOptionMask{ NativeLeftOptionMask | NativeRightOptionMask };

	// When both Option keys are pressed simultaneously
	// Mode: Left — should still trigger because left bit is set
	NeovimQt::Input::SetMacOptionIsMeta(NeovimQt::Input::MacOptionMetaMode::Left);

	QKeyEvent evBothAltJ{
		QEvent::KeyPress, Qt::Key_J, Qt::AltModifier, 0, 0, NativeBothOptionMask, "∆"
	};
	QCOMPARE(NeovimQt::Input::convertKey(evBothAltJ), QString{ "<A-j>" });

	// Mode: Right — should also trigger because right bit is set
	NeovimQt::Input::SetMacOptionIsMeta(NeovimQt::Input::MacOptionMetaMode::Right);

	QKeyEvent evBothAltK{
		QEvent::KeyPress, Qt::Key_K, Qt::AltModifier, 0, 0, NativeBothOptionMask, "˚"
	};
	QCOMPARE(NeovimQt::Input::convertKey(evBothAltK), QString{ "<A-k>" });

	// Mode: Both — should always trigger
	NeovimQt::Input::SetMacOptionIsMeta(NeovimQt::Input::MacOptionMetaMode::Both);

	QKeyEvent evBothAltL{
		QEvent::KeyPress, Qt::Key_L, Qt::AltModifier, 0, 0, NativeBothOptionMask, "¬"
	};
	QCOMPARE(NeovimQt::Input::convertKey(evBothAltL), QString{ "<A-l>" });

	// Restore default
	NeovimQt::Input::SetMacOptionIsMeta(NeovimQt::Input::MacOptionMetaMode::None);
}

void TestInputMac::MacOptionIsMetaModeToggle() noexcept
{
	// Verify that toggling the mode at runtime works correctly

	// Start with None
	NeovimQt::Input::SetMacOptionIsMeta(NeovimQt::Input::MacOptionMetaMode::None);
	QKeyEvent evAltJ1{ QEvent::KeyPress, Qt::Key_J, Qt::AltModifier, "∆" };
	QCOMPARE(NeovimQt::Input::convertKey(evAltJ1), QString{ "∆" });

	// Switch to Both
	NeovimQt::Input::SetMacOptionIsMeta(NeovimQt::Input::MacOptionMetaMode::Both);
	QKeyEvent evAltJ2{ QEvent::KeyPress, Qt::Key_J, Qt::AltModifier, "∆" };
	QCOMPARE(NeovimQt::Input::convertKey(evAltJ2), QString{ "<A-j>" });

	// Switch back to None
	NeovimQt::Input::SetMacOptionIsMeta(NeovimQt::Input::MacOptionMetaMode::None);
	QKeyEvent evAltJ3{ QEvent::KeyPress, Qt::Key_J, Qt::AltModifier, "∆" };
	QCOMPARE(NeovimQt::Input::convertKey(evAltJ3), QString{ "∆" });

	// Switch to Left
	NeovimQt::Input::SetMacOptionIsMeta(NeovimQt::Input::MacOptionMetaMode::Left);
	// Without native modifiers, Mode::Left won't match (nativeMod is 0)
	QKeyEvent evAltJ4{ QEvent::KeyPress, Qt::Key_J, Qt::AltModifier, "∆" };
	QCOMPARE(NeovimQt::Input::convertKey(evAltJ4), QString{ "∆" });

	// Restore default
	NeovimQt::Input::SetMacOptionIsMeta(NeovimQt::Input::MacOptionMetaMode::None);
}

void TestInputMac::GetOptionAsMetaTextDirect() noexcept
{
	// Directly test the platform-dispatched function.

	// Mode::None — always returns std::nullopt
	NeovimQt::Input::SetMacOptionIsMeta(NeovimQt::Input::MacOptionMetaMode::None);
	QKeyEvent evAltJ{ QEvent::KeyPress, Qt::Key_J, Qt::AltModifier, "∆" };
	QCOMPARE(NeovimQt::Input::GetOptionAsMetaText(evAltJ), std::nullopt);

	// Mode::Both — returns overridden text for Alt events
	NeovimQt::Input::SetMacOptionIsMeta(NeovimQt::Input::MacOptionMetaMode::Both);
	QCOMPARE(NeovimQt::Input::GetOptionAsMetaText(evAltJ), std::optional<QString>{ "j" });

	// Shift+Alt — returns uppercase
	QKeyEvent evAltShiftJ{ QEvent::KeyPress, Qt::Key_J, Qt::AltModifier | Qt::ShiftModifier, "Ô" };
	QCOMPARE(NeovimQt::Input::GetOptionAsMetaText(evAltShiftJ), std::optional<QString>{ "J" });

	// No AltModifier — returns std::nullopt even when mode is Both
	QKeyEvent evPlainJ{ QEvent::KeyPress, Qt::Key_J, Qt::NoModifier, "j" };
	QCOMPARE(NeovimQt::Input::GetOptionAsMetaText(evPlainJ), std::nullopt);

	// Left mode with right native modifier — returns std::nullopt
	constexpr quint32 NativeRightOptionMask{ 0x00000040 };
	NeovimQt::Input::SetMacOptionIsMeta(NeovimQt::Input::MacOptionMetaMode::Left);
	QKeyEvent evRightAltJ{
		QEvent::KeyPress, Qt::Key_J, Qt::AltModifier, 0, 0, NativeRightOptionMask, "∆"
	};
	QCOMPARE(NeovimQt::Input::GetOptionAsMetaText(evRightAltJ), std::nullopt);

	// Left mode with left native modifier — returns text
	constexpr quint32 NativeLeftOptionMask{ 0x00000020 };
	QKeyEvent evLeftAltJ{
		QEvent::KeyPress, Qt::Key_J, Qt::AltModifier, 0, 0, NativeLeftOptionMask, "∆"
	};
	QCOMPARE(NeovimQt::Input::GetOptionAsMetaText(evLeftAltJ), std::optional<QString>{ "j" });

	// Restore default
	NeovimQt::Input::SetMacOptionIsMeta(NeovimQt::Input::MacOptionMetaMode::None);
}

#include "tst_input_mac.moc"
QTEST_MAIN(TestInputMac)
