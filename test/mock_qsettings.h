#pragma once

#include <QSettings>

namespace NeovimQt { namespace MockQSettings {

/// Overrides QSettings to use the mock format by default
void EnableByDefault() noexcept;

/// Clears all contents store in MockQSettings
void ClearAllContents() noexcept;

/// Overwrites call contents stored in MockQSettings with newValue
void OverwriteContents(QSettings::SettingsMap newValue) noexcept;

}} // namespace NeovimQt::MockQSettings
