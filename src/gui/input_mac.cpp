#include "input.h"

namespace NeovimQt::Input {

Qt::KeyboardModifiers ControlModifier() noexcept
{
	return Qt::MetaModifier;
}

Qt::KeyboardModifiers CmdModifier() noexcept
{
	return Qt::ControlModifier;
}

Qt::Key Key_Control() noexcept
{
	return Qt::Key_Meta;
}

Qt::Key Key_Cmd() noexcept
{
	return Qt::Key_Control;
}

QString GetModifierPrefix(Qt::KeyboardModifiers mod) noexcept
{
	QString modprefix;
	if (mod & CmdModifier()) {
		// This behavior is from MacVim.
		modprefix += "D-";
	}
	if (mod & ControlModifier()) {
		modprefix += "C-";
	}
	if (mod & Qt::ShiftModifier) {
		modprefix += "S-";
	}
	if (mod & Qt::AltModifier) {
		modprefix += "A-";
	}

	return modprefix;
}

// Some keyboard locales require Alt to input low-ascii characters (German "{").
// Other keyboard locales do not require Alt (US "{"). Remove Alt on layouts where
// the key modifies text input. Qt does not provide a robust layout-detection
// mechanism. Instead, measure Qt::Key and text mis-match with Alt present.
//
// See Pull Requests 837 and 841 for more details.
static bool IsAsciiCharRequiringAlt(int key, Qt::KeyboardModifiers mod, QChar c) noexcept
{
	// Ignore all key events where Alt is not pressed
	if (!(mod & Qt::AltModifier)) {
		return false;
	}

	// These low-ascii characters may require AltModifier on MacOS
	if ((c == '[' && key != Qt::Key_BracketLeft)
		|| (c == ']' && key != Qt::Key_BracketRight)
		|| (c == '{' && key != Qt::Key_BraceLeft)
		|| (c == '}' && key != Qt::Key_BraceRight)
		|| (c == '|' && key != Qt::Key_Bar)
		|| (c == '~' && key != Qt::Key_AsciiTilde)
		|| (c == '@' && key != Qt::Key_At)) {
		return true;
	}

	return false;
}

QKeyEvent CreatePlatformNormalizedKeyEvent(
	QEvent::Type type,
	int key,
	Qt::KeyboardModifiers mod,
	const QString& text) noexcept
{
	if (text.isEmpty())
	{
		return { type, key, mod, text };
	}

	const QChar c{ text.at(0) };
	if (c.unicode() >= 0x80 && c.isPrint()) {
		mod &= ~Qt::AltModifier;
	}

	// Issue#833: Some locales require Alt for basic low-ascii characters,
	// remove AltModifer. Ex) German layouts use Alt for "{".
	if (IsAsciiCharRequiringAlt(key, mod, c)) {
		mod &= ~Qt::AltModifier;
	}

	return { type, key, mod, text };
}

static MacOptionMetaMode s_macOptionIsMeta{ MacOptionMetaMode::None };

void SetMacOptionIsMeta(MacOptionMetaMode mode) noexcept
{
	s_macOptionIsMeta = mode;
}

MacOptionMetaMode GetMacOptionIsMeta() noexcept
{
	return s_macOptionIsMeta;
}

std::optional<QString> GetOptionAsMetaText(const QKeyEvent& ev) noexcept
{
	if (s_macOptionIsMeta == MacOptionMetaMode::None) {
		return std::nullopt;
	}

	const Qt::KeyboardModifiers mod{ ev.modifiers() };
	if (!(mod & Qt::AltModifier)) {
		return std::nullopt;
	}

	// macOS native modifier masks for left/right Option keys
	constexpr quint32 NativeLeftOptionMask{ 0x00000020 };
	constexpr quint32 NativeRightOptionMask{ 0x00000040 };

	const quint32 nativeMod{ ev.nativeModifiers() };
	const bool isLeftAlt{ (nativeMod & NativeLeftOptionMask) != 0 };
	const bool isRightAlt{ (nativeMod & NativeRightOptionMask) != 0 };

	const bool treatAsMeta{ (s_macOptionIsMeta == MacOptionMetaMode::Both)
		|| (s_macOptionIsMeta == MacOptionMetaMode::Left && isLeftAlt)
		|| (s_macOptionIsMeta == MacOptionMetaMode::Right && isRightAlt) };

	if (!treatAsMeta) {
		return std::nullopt;
	}

	// Use key() to derive the base key name, ignoring macOS
	// Unicode transformation from text().
	const int key{ ev.key() };

	// Filter out modifier-only events and keys outside the BMP
	// (e.g. Key_Alt = 0x01000023 cannot be represented as QChar).
	if (key <= 0 || key > 0xFFFF) {
		return std::nullopt;
	}

	QString text{ QChar{ static_cast<char16_t>(key) } };
	if (!(mod & Qt::ShiftModifier)) {
		text = text.toLower();
	}

	return text;
}

} // namespace NeovimQt::Input
