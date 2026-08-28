#pragma once

#include <QSignalSpy>

/// Waits for the signal with timeout (default 2s). Works even for signals
/// that have already been fired. Returns false on timeout.
bool SPYWAIT(QSignalSpy& spy, int timeout = 2000, qsizetype minSignals = 1) noexcept;
