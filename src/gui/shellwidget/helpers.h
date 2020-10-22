#pragma once

#include <QFontMetrics>

#include "shellcontents.h"

/// Helper for deprecated QFontMetrics::width(...). Eventually remove this code.
int GetHorizontalAdvance(const QFontMetrics& fm, QChar character) noexcept;

/// Helper for deprecated QFontMetrics::width(...). Eventually remove this code.
int GetHorizontalAdvance(const QFontMetrics& fm, const QString& text) noexcept;

bool saveShellContents(const ShellContents& s, const QString& filename);

/// Check if `index` is valid and within the bounds of `container`.
template<class T>
inline bool IsValidIndex(const T& container, int index) noexcept
{
	return index >= 0  && index < container.size();
}
