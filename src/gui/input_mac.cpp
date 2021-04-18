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

// Fixup Modifiers, Apples OPTION key
Qt::KeyboardModifiers de_DE(Qt::KeyboardModifiers mod, QChar c) noexcept
{
 	std::vector<QChar> removeAltCharList  { '[', ']', '|', '{', '}', '~', '@', '\'' };
	if(mod & Qt::AltModifier) {
		if (std::find(removeAltCharList.begin(), removeAltCharList.end(), c) != removeAltCharList.end()) {
			mod &= ~Qt::AltModifier;
		}
	}
	return mod;
}
QKeyEvent CreatePlatformNormalizedKeyEvent(
	QEvent::Type type,
	int key,
	Qt::KeyboardModifiers mod,
	const QString& text,
	const QLocale* locale) noexcept
{
	
	
	
	if (!text.isEmpty())
	{
		const QChar c{ text.at(0) };
		if (c.unicode() >= 0x80 && c.isPrint()) {
			mod &= ~Qt::AltModifier;
		}
		
		// German/Austrian/Swiss MacOS QWERTZ keyboard, remove AltModifier (OPTION KEY)
		if(locale) {
			if(locale->name().startsWith("de_", Qt::CaseInsensitive)) {
				mod = de_DE(mod, c);
			}
		}
	}
	
	

	

	return { type, key, mod, text };
}

} } // namespace NeovimQt::Input
