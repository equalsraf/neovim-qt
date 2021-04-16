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

QKeyEvent CreatePlatformNormalizedKeyEvent(
	QEvent::Type type,
	int key,
	Qt::KeyboardModifiers mod,
	const QString& text) noexcept
{
	
	std::vector<QString> removeAltCharList  {"¡","“","¶","¢","[","]","|","{","}","≠","¿","±","‘","–","…","∞","µ","~","∫","√","ç","≈","¥","≤","å","‚","∂","ƒ","©","ª","º","∆","@","œ","æ","‘","±","•","π","⁄","¨","Ω","†","®","€","∑","«","„","\"","¿"};
	
	if (!text.isEmpty())
	{
		const QChar c{ text.at(0) };
		if (c.unicode() >= 0x80 && c.isPrint()) {
			mod &= ~Qt::AltModifier;
		}
		
		// German MacOS QWERTZ keyboard, remove AltModifier (OPTION KEY)
		if (std::find(removeAltCharList.begin(), removeAltCharList.end(), text) != removeAltCharList.end()) {
			if(mod & Qt::AltModifier) {
        		mod &= ~Qt::AltModifier;
			}
    	}
	}
	
	

	

	return { type, key, mod, text };
}

} } // namespace NeovimQt::Input
