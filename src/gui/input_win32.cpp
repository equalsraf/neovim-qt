#include "input.h"

namespace NeovimQt { namespace Input {


Qt::KeyboardModifiers ControlModifier() noexcept
{
	return Qt::ControlModifier;
}

Qt::KeyboardModifiers CmdModifier() noexcept
{
	return static_cast<Qt::KeyboardModifiers>(0);
}

Qt::Key Key_Control() noexcept
{
	return Qt::Key_Control;
}

Qt::Key Key_Cmd() noexcept
{
	return static_cast<Qt::Key>(0);
}

QString GetModifierPrefix(Qt::KeyboardModifiers mod) noexcept
{
	QString modprefix;
	if (mod & ControlModifier() && !(mod & Qt::AltModifier)) {
		modprefix += "C-";
	}
	if ( mod & Qt::ShiftModifier ) {
		modprefix += "S-";
	}
	if (mod & Qt::AltModifier && !(mod & ControlModifier())) {
		modprefix += "A-";
	}

	return modprefix;
}

} } // namespace NeovimQt::Input
