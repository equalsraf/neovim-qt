#include "input.h"

namespace NeovimQt { namespace Input {

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

} } // namespace NeovimQt::Input
