#pragma once

#include <QString>

namespace NeovimQt { namespace Commandline {

enum class Mode {
	Dynamic,
	Fixed,
	// TerminalEmulation, FIXME Add This? emulate TUI w/ ext_cmdline?
};

/// Converts QSettings string (case insensitive) to `Mode`.
/// The fallback/default value is: `Mode::Dynamic`
Mode ModeFromString(const QString& mode) noexcept;

} } // namespace NeovimQt::Commandline
