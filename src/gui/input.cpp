
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
	specialKeys.insert(Qt::Key_F13, "F13");
	specialKeys.insert(Qt::Key_F14, "F14");
	specialKeys.insert(Qt::Key_F15, "F15");
	specialKeys.insert(Qt::Key_F16, "F16");
	specialKeys.insert(Qt::Key_F17, "F17");
	specialKeys.insert(Qt::Key_F18, "F18");
	specialKeys.insert(Qt::Key_F19, "F19");
	specialKeys.insert(Qt::Key_F20, "F20");
	specialKeys.insert(Qt::Key_F21, "F21");
	specialKeys.insert(Qt::Key_F22, "F22");
	specialKeys.insert(Qt::Key_F23, "F23");
	specialKeys.insert(Qt::Key_F24, "F24");

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
    if ( mod & Qt::KeypadModifier ) {
		switch (k) {
		case Qt::Key_Home:
			return QString("<%1kHome>").arg(modPrefix(mod));
		case Qt::Key_End:
			return QString("<%1kEnd>").arg(modPrefix(mod));
		case Qt::Key_PageUp:
			return QString("<%1kPageUp>").arg(modPrefix(mod));
		case Qt::Key_PageDown:
			return QString("<%1kPageDown>").arg(modPrefix(mod));
		case Qt::Key_Plus:
			return QString("<%1kPlus>").arg(modPrefix(mod));
		case Qt::Key_Minus:
			return QString("<%1kMinus>").arg(modPrefix(mod));
		case Qt::Key_multiply:
			return QString("<%1kMultiply>").arg(modPrefix(mod));
		case Qt::Key_division:
			return QString("<%1kDivide>").arg(modPrefix(mod));
		case Qt::Key_Enter:
			return QString("<%1kEnter>").arg(modPrefix(mod));
		case Qt::Key_Period:
			return QString("<%1kPoint>").arg(modPrefix(mod));
		case Qt::Key_0:
			return QString("<%1k0>").arg(modPrefix(mod));
		case Qt::Key_1:
			return QString("<%1k1>").arg(modPrefix(mod));
		case Qt::Key_2:
			return QString("<%1k2>").arg(modPrefix(mod));
		case Qt::Key_3:
			return QString("<%1k3>").arg(modPrefix(mod));
		case Qt::Key_4:
			return QString("<%1k4>").arg(modPrefix(mod));
		case Qt::Key_5:
			return QString("<%1k5>").arg(modPrefix(mod));
		case Qt::Key_6:
			return QString("<%1k6>").arg(modPrefix(mod));
		case Qt::Key_7:
			return QString("<%1k7>").arg(modPrefix(mod));
		case Qt::Key_8:
			return QString("<%1k8>").arg(modPrefix(mod));
		case Qt::Key_9:
			return QString("<%1k9>").arg(modPrefix(mod));
		}
    }

	if (specialKeys.contains(k)) {
		return QString("<%1%2>").arg(modPrefix(mod)).arg(specialKeys.value(k));
	}

	QChar c;
	// Escape < and backslash
	if (text == "<") {
		return QString("<lt>");
	} else if (text == "\\") {
		return QString("<%1%2>").arg(modPrefix(mod)).arg("Bslash");
	} else if (text.isEmpty()) {
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
    if (c.unicode() >= 0x80 || (!c.isLetterOrNumber() && c.isPrint())) {
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
