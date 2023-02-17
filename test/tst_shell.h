#pragma once

#include <QClipboard>
#include <QString>
#include <QStringList>

namespace NeovimQt {

QClipboard::Mode GetClipboardMode(char reg) noexcept;

QStringList BinaryAndArgumentsNoForkWithCommand(const QString& command) noexcept;

void AddPlatformSpecificExitCodeCases() noexcept;

class Test;

} // Namespace NeovimQt
