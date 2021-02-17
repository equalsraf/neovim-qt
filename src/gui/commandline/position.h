#pragma once

#include <QString>

namespace NeovimQt { namespace Cmdline {

enum class Position {
	Center,
	Top,
	Bottom,
};

/// Converts QSettings string (case insensitive) to `Position`.
/// The fallback/default value is: `Position::Center`
Position PositionFromString(const QString& position) noexcept;

} } // namespace NeovimQt::Cmdline
