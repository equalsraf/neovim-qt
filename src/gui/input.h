#pragma once

#include <QEvent>
#include <QKeyEvent>
#include <QMap>
#include <QPoint>
#include <QString>

namespace NeovimQt { namespace Input {

/// Returns the QMap of all NeoVim-recognized special keys.
const QMap<int, QString>& GetSpecialKeysMap() noexcept;

/// Convert Qt key input into Neovim key-notation. See QKeyEvent.
QString convertKey(const QKeyEvent& ev) noexcept;

/// Return keyboard modifier prefix. Ex) "C-", "A-" or "C-S-A-"
///
/// NOTE: On Win32 Ctrl+Alt are never passed together, since we can't distinguish
/// between Ctrl+Alt and AltGr (see Vim/os_win32.c).
QString GetModifierPrefix(Qt::KeyboardModifiers mod) noexcept;

/// Convert mouse event information into Neovim key notation. See QMouseEvent.
///
/// @type is one of the Qt mouse event types
/// @pos is in Neovim Coordinates
/// @clickCount is the number of consecutive mouse clicks
///   1 for a single click, 2 for a double click, up to 4.
///   This value is only used for LeftMouse events.
///
/// @return The Neovim event string, or an empty string if the event is invalid.
QString convertMouse(
	Qt::MouseButton bt,
	QEvent::Type type,
	Qt::KeyboardModifiers mod,
	QPoint pos,
	uint8_t clicksCount) noexcept;

/// Platform specific Qt key modifier bitmask for 'Control'.
Qt::KeyboardModifiers ControlModifier() noexcept;

/// Platform specific Qt::Key used as 'Control'.
Qt::Key Key_Control() noexcept;

/// Platform specific Qt key modifier bitmask for 'Cmd'/'Meta'.
Qt::KeyboardModifiers CmdModifier() noexcept;

/// Platform specific Qt::Key used as 'Cmd'/'Meta'.
Qt::Key Key_Cmd() noexcept;

/// Modify QKeyEvent parameters specific to the current platform.
QKeyEvent CreatePlatformNormalizedKeyEvent(
	QEvent::Type type,
	int key,
	Qt::KeyboardModifiers mod,
	const QString& text) noexcept;

} } // namespace NeovimQt:Input
