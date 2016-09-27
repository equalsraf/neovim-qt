
#include "input.h"
#include <QDebug>

namespace NeovimQt {

InputConv Input;

InputConv::InputConv() {
	// see :h key-notation

	// special keys i.e. no textual representation
	specialKeys.insert(Qt::Key_Up, "Up");
	specialKeys.insert(Qt::Key_Down, "Down");
	specialKeys.insert(Qt::Key_Left, "Left");
	specialKeys.insert(Qt::Key_Right, "Right");

	specialKeys.insert(Qt::Key_F1, "F1");
	specialKeys.insert(Qt::Key_F2, "F2");
	specialKeys.insert(Qt::Key_F3, "F3");
	specialKeys.insert(Qt::Key_F4, "F4");
	specialKeys.insert(Qt::Key_F5, "F5");
	specialKeys.insert(Qt::Key_F6, "F6");
	specialKeys.insert(Qt::Key_F7, "F7");
	specialKeys.insert(Qt::Key_F8, "F8");
	specialKeys.insert(Qt::Key_F9, "F9");
	specialKeys.insert(Qt::Key_F10, "F10");
	specialKeys.insert(Qt::Key_F11, "F11");
	specialKeys.insert(Qt::Key_F12, "F12");

	specialKeys.insert(Qt::Key_Backspace, "BS");
	specialKeys.insert(Qt::Key_Delete, "Del");
	specialKeys.insert(Qt::Key_Insert, "Insert");
	specialKeys.insert(Qt::Key_Home, "Home");
	specialKeys.insert(Qt::Key_End, "End");
	specialKeys.insert(Qt::Key_PageUp, "PageUp");
	specialKeys.insert(Qt::Key_PageDown, "PageDown");

	specialKeys.insert(Qt::Key_Return, "Enter");
	specialKeys.insert(Qt::Key_Enter, "Enter");
	specialKeys.insert(Qt::Key_Tab, "Tab");
	specialKeys.insert(Qt::Key_Backtab, "Tab");
	specialKeys.insert(Qt::Key_Escape, "Esc");

	specialKeys.insert(Qt::Key_Backslash, "Bslash");
	specialKeys.insert(Qt::Key_Less, "lt");
	specialKeys.insert(Qt::Key_Space, "Space");
}

/**
 * Return keyboard modifier prefix 
 *
 * e.g. C-, A- or C-S-A-
 *
 * WIN32: Ctrl+Alt are never passed together, since we can't distinguish
 * between Ctrl+Alt and AltGr (see Vim/os_win32.c).
 */
QString InputConv::modPrefix(Qt::KeyboardModifiers mod)
{
	QString modprefix;
#if defined(Q_OS_MAC) || defined(Q_OS_UNIX)
	if ( mod & CmdModifier ) {
		modprefix += "D-"; // like MacVim does
	}
#endif
	if ( mod & ControlModifier
#ifdef Q_OS_WIN32
		&& !(mod & AltModifier)
#endif
	   ) {
		modprefix += "C-";
	}
	if ( mod & ShiftModifier ) {
		modprefix += "S-";
	}
	if ( mod & AltModifier
#ifdef Q_OS_WIN32
		&& !(mod & ControlModifier)
#endif
	   ) {
		modprefix += "A-";
	}
	return modprefix;
}

/**
 * Convert mouse event information into Neovim key notation
 *
 * @type is one of the Qt mouse event types
 * @pos is in Neovim Coordinates
 * @clickCount is the number of consecutive mouse clicks
 *   1 for a single click, 2 for a double click, up to 4.
 *   This value is only used for LeftMouse events.
 *
 * see QMouseEvent
 *
 * If the event is not valid for Neovim, returns an empty string
 */
QString InputConv::convertMouse(Qt::MouseButton bt, QEvent::Type type, Qt::KeyboardModifiers mod, QPoint pos, short clickCount)
{
	QString buttonName;
	switch(bt) {
	case Qt::LeftButton:
		// In practice Neovim only supports the clickcount for Left
		// mouse presses, even if our shell can support other buttons
		if (clickCount > 1 && clickCount <= 4) {
			buttonName = QString("%1-Left").arg(clickCount);
		} else {
			buttonName += "Left";
		}
		break;
	case Qt::RightButton:
		buttonName += "Right";
		break;
	case Qt::MidButton:
		buttonName += "Middle";
		break;
	case Qt::NoButton:
		break;
	default:
		return "";
	}

	QString evType;
	switch(type) {
	case QEvent::MouseButtonDblClick:
		// Treat this as a regular MouseButtonPress. Repeated
		// clicks are handled above.
	case QEvent::MouseButtonPress:
		evType += "Mouse";
		break;
	case QEvent::MouseButtonRelease:
		evType += "Release";
		break;
	case QEvent::MouseMove:
		evType += "Drag";
		break;
	default:
		return "";
	}

	QString inp = QString("<%1%2%3><%4,%5>").arg(modPrefix(mod)).arg(buttonName).arg(evType).arg(pos.x()).arg(pos.y());
	return inp;
}

/**
 * Convert Qt key input into Neovim key-notation
 *
 * see QKeyEvent
 */
QString InputConv::convertKey(const QString& text, int k, Qt::KeyboardModifiers mod)
{
	QChar c;
	if (specialKeys.contains(k)) {
		c = text.at(0);
		if (text.isEmpty() || c.isSpace() || !c.isPrint()) {
			return QString("<%1%2>").arg(modPrefix(mod)).arg(specialKeys.value(k));
		} else {
			// 'k' include the information about SHIFT so do not check 'mod'
			// See #134
			return QString("<%1>").arg(specialKeys.value(k));
		}
	}

	if (text.isEmpty()) {
		// on macs, text is empty for ctrl+key and cmd+key combos (with or without alt)
		if (mod & ControlModifier || mod & CmdModifier) {
			// ignore ctrl, alt and cmd key combos by themselves
			QList<Qt::Key> keys = { Key_Control, Key_Alt, Key_Cmd };
			if (keys.contains((Qt::Key)k)) {
				return QString();
			} else {
				// key code will be the value of the char (hopefully)
				c = QChar(k);
			}
		} else {
			// This is a special key we can't handle
			return QString();
		}
	} else {
		// Key event compression is disabled, text has one char
		c = text.at(0);
	}

	// Remove SHIFT
	if (c.unicode() < 0x100 && !c.isLetterOrNumber() && c.isPrint()) {
		mod &= ~ShiftModifier;
	}

	// Remove CTRL empty characters at the start of the ASCII range
	if (c.unicode() < 0x20) {
		mod &= ~ControlModifier;
	}

	// Format with prefix if necessary
	QString prefix = modPrefix(mod);
	if (!prefix.isEmpty()) {
		return QString("<%1%2>").arg(prefix).arg(c);
	}

	return QString(c);
}

} // Namespace
