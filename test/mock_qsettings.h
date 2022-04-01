#pragma once

#include <QSettings>

namespace NeovimQt { namespace MockQSettings {

/// Overrides QSettings to use the mock format by default
void EnableByDefault() noexcept;

/// Clears all contents store in MockQSettings
void ClearAllContents() noexcept;

}} // namespace NeovimQt::MockQSettings
