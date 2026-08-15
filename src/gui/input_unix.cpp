#include "input.h"

namespace NeovimQt::Input {

Qt::KeyboardModifiers ControlModifier() noexcept
{
	return Qt::ControlModifier;
}

Qt::KeyboardModifiers CmdModifier() noexcept
{
	return Qt::MetaModifier;
}

Qt::Key Key_Control() noexcept
{
	return Qt::Key_Control;
}

Qt::Key Key_Cmd() noexcept
{
	return Qt::Key_Meta;
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
	return { type, key, mod, text };
}

std::optional<QString> GetOptionAsMetaText(const QKeyEvent& /*ev*/) noexcept
{
	return std::nullopt;
}

void SetMacOptionIsMeta(MacOptionMetaMode /*mode*/) noexcept {}

MacOptionMetaMode GetMacOptionIsMeta() noexcept
{
	return MacOptionMetaMode::None;
}

} // namespace NeovimQt::Input
