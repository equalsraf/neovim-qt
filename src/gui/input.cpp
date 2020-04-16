#include "input.h"

#include <QMap>
#include <QVariant>

namespace NeovimQt { namespace Input {

const QMap<int, QString>& GetSpecialKeysMap() noexcept
{
	static const QMap<int, QString> specialKeys {
		{ Qt::Key_Up, "Up" },
		{ Qt::Key_Down, "Down" },
		{ Qt::Key_Left, "Left" },
		{ Qt::Key_Right, "Right" },
		{ Qt::Key_F1, "F1" },
		{ Qt::Key_F2, "F2" },
		{ Qt::Key_F3, "F3" },
		{ Qt::Key_F4, "F4" },
		{ Qt::Key_F5, "F5" },
		{ Qt::Key_F6, "F6" },
		{ Qt::Key_F7, "F7" },
		{ Qt::Key_F8, "F8" },
		{ Qt::Key_F9, "F9" },
		{ Qt::Key_F10, "F10" },
		{ Qt::Key_F11, "F11" },
		{ Qt::Key_F12, "F12" },
		{ Qt::Key_F13, "F13" },
		{ Qt::Key_F14, "F14" },
		{ Qt::Key_F15, "F15" },
		{ Qt::Key_F16, "F16" },
		{ Qt::Key_F17, "F17" },
		{ Qt::Key_F18, "F18" },
		{ Qt::Key_F19, "F19" },
		{ Qt::Key_F20, "F20" },
		{ Qt::Key_F21, "F21" },
		{ Qt::Key_F22, "F22" },
		{ Qt::Key_F23, "F23" },
		{ Qt::Key_F24, "F24" },
		{ Qt::Key_Backspace, "BS" },
		{ Qt::Key_Delete, "Del" },
		{ Qt::Key_Insert, "Insert" },
		{ Qt::Key_Home, "Home" },
		{ Qt::Key_End, "End" },
		{ Qt::Key_PageUp, "PageUp" },
		{ Qt::Key_PageDown, "PageDown" },
		{ Qt::Key_Return, "Enter" },
		{ Qt::Key_Enter, "Enter" },
		{ Qt::Key_Tab, "Tab" },
		{ Qt::Key_Backtab, "Tab" },
		{ Qt::Key_Escape, "Esc" },
		{ Qt::Key_Backslash, "Bslash" },
		{ Qt::Key_Space, "Space" },
	};

	return specialKeys;
}

static QVariant GetButtonName(
	Qt::MouseButton bt,
	uint8_t clickCount) noexcept
{
	// NOTE: In practice Neovim only supports the clickcount for Left
	// mouse presses, even if our shell can support other buttons
	switch(bt) {
		case Qt::LeftButton:
		{
			if (clickCount > 1 && clickCount <= 4) {
				return QString("%1-Left").arg(clickCount);
			}
			return QStringLiteral("Left");
		}

		case Qt::RightButton:
			return QStringLiteral("Right");

		case Qt::MidButton:
			return QStringLiteral("Middle");

		case Qt::NoButton:
			return QStringLiteral("");

		default:
			return {}; // Invalid
	}
}

static QString GetEventString(QEvent::Type type) noexcept
{
	// NOTE: MouseButtonDblClick is treated as MouseButtonPress.
	// Repeated mouse clicks are handled in GetButtonName above.
	switch(type) {
		case QEvent::MouseButtonDblClick:
		case QEvent::MouseButtonPress:
			return QStringLiteral("Mouse");

		case QEvent::MouseButtonRelease:
			return QStringLiteral("Release");

		case QEvent::MouseMove:
			return QStringLiteral("Drag");

		default:
			return {};
	}
}

QString convertMouse(
	Qt::MouseButton bt,
	QEvent::Type type,
	Qt::KeyboardModifiers mod,
	QPoint pos,
	uint8_t clickCount) noexcept
{
	const QVariant varButtonName{ GetButtonName(bt, clickCount) };
	if (!varButtonName.isValid() || !varButtonName.canConvert<QString>()) {
		return {};
	}

	return QString{ "<%1%2%3><%4,%5>" }
		.arg(GetModifierPrefix(mod))
		.arg(varButtonName.toString())
		.arg(GetEventString(type))
		.arg(pos.x())
		.arg(pos.y());
}

QString ToKeyString(const QString& modPrefix, const QString& key) noexcept
{
	return QString{ "<%1%2>" }.arg(modPrefix).arg(key);
}

QString convertKey(const QKeyEvent& ev) noexcept
{
	QString text{ ev.text() };
	Qt::KeyboardModifiers mod{ ev.modifiers() };
	int key{ ev.key() };

	static const QMap<int, QString> keypadKeys {
		{ Qt::Key_Home, "<%1kHome>" },
		{ Qt::Key_End, "<%1kEnd>" },
		{ Qt::Key_PageUp, "<%1kPageUp>" },
		{ Qt::Key_PageDown, "<%1kPageDown>" },
		{ Qt::Key_Plus, "<%1kPlus>" },
		{ Qt::Key_Minus, "<%1kMinus>" },
		{ Qt::Key_multiply, "<%1kMultiply>" },
		{ Qt::Key_division, "<%1kDivide>" },
		{ Qt::Key_Enter, "<%1kEnter>" },
		{ Qt::Key_Period, "<%1kPoint>" },
		{ Qt::Key_0, "<%1k0>" },
		{ Qt::Key_1, "<%1k1>" },
		{ Qt::Key_2, "<%1k2>" },
		{ Qt::Key_3, "<%1k3>" },
		{ Qt::Key_4, "<%1k4>" },
		{ Qt::Key_5, "<%1k5>" },
		{ Qt::Key_6, "<%1k6>" },
		{ Qt::Key_7, "<%1k7>" },
		{ Qt::Key_8, "<%1k8>" },
		{ Qt::Key_9, "<%1k9>" },
	};

	if (mod & Qt::KeypadModifier && keypadKeys.contains(key)) {
		return keypadKeys.value(key).arg(GetModifierPrefix(mod));
	}

	const QMap<int, QString>& specialKeys { GetSpecialKeysMap() };

	if (specialKeys.contains(key)) {
		return ToKeyString(GetModifierPrefix(mod), specialKeys.value(key));
	}

	// The key "<" should be sent as "<lt>"
	//   Issue#607: Remove ShiftModifier from "<", shift is implied
	if (text == "<") {
		const Qt::KeyboardModifiers modNoShift { mod & ~Qt::KeyboardModifier::ShiftModifier };
		return ToKeyString(GetModifierPrefix(modNoShift), "lt");
	}

	// Issue#170: Normalize modifiers, CTRL+^ always sends as <C-^>
	const bool isCaretKey{ key == Qt::Key_6 || key == Qt::Key_AsciiCircum };
	if (isCaretKey && mod & ControlModifier()) {
		const Qt::KeyboardModifiers modNoShiftMeta{
			mod & ~Qt::KeyboardModifier::ShiftModifier & ~CmdModifier() };
		return ToKeyString(GetModifierPrefix(modNoShiftMeta), "^");
	}

	if (text == "\\") {
		return ToKeyString(GetModifierPrefix(mod), "Bslash");
	}

	if (text.isEmpty()) {
		// Ignore all modifier-only key events.
		//   Issue#344: Ignore Ctrl-Shift, C-S- being treated as C-Space
		//   Issue#593: Pressing Control + Super inserts ^S
		//   Issue#199: Pressing Control + CapsLock inserts $
		if (key == Qt::Key::Key_Alt
			|| key == Qt::Key::Key_AltGr
			|| key == Qt::Key::Key_CapsLock
			|| key == Qt::Key::Key_Control
			|| key == Qt::Key::Key_Meta
			|| key == Qt::Key::Key_Shift
			|| key == Qt::Key::Key_Super_L
			|| key == Qt::Key::Key_Super_R) {
			return {};
		}

		// Ignore special keys
		//   Issue#671: `q`/`p`/`r` key is sent by Mute/Volume DOWN/Volume UP
		if (key == Qt::Key::Key_VolumeDown
			|| key == Qt::Key::Key_VolumeMute
			|| key == Qt::Key::Key_VolumeUp) {
			return {};
		}

		// If QKeyEvent does not provide text, then use the value of key
		//   Issue#579: Cannot map <A-...> on MacOS
		text = QChar{ key };
		if (!(mod & Qt::ShiftModifier))
		{
			text = text.toLower();
		}
	}

	const QChar c{ text.at(0) };

	// Remove Shift
	if (c.unicode() >= 0x80 || c.isPrint()) {
		mod &= ~Qt::ShiftModifier;
	}

	// Remove Ctrl empty characters at the start of the ASCII range
	if (c.unicode() < 0x20) {
		mod &= ~ControlModifier();
	}

	// Perform any platform specific QKeyEvent modifications
	QKeyEvent evNormalized{ CreatePlatformNormalizedKeyEvent(ev.type(), key, mod, text) };

	// Format with prefix if necessary
	const QString prefix{ GetModifierPrefix(evNormalized.modifiers()) };
	if (!prefix.isEmpty()) {
		return ToKeyString(prefix, evNormalized.text());
	}

	return evNormalized.text();
}

} } // namespace NeovimQt::Input
