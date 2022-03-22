#pragma once

#include <QSignalSpy>
#include <QString>

/// Waits for the signal with timeout (default 2s). Works even for signals
/// that have already been fired. Returns false on timeout.
bool SPYWAIT(QSignalSpy& spy, int timeout = 2000) noexcept;

/// Returns the filesystem path to the neovim-qt runtime plugin.
QString GetRuntimeAbsolutePath() noexcept;
