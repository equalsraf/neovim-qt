#pragma once

#include <QStringList>

namespace NeovimQt {

QStringList BinaryAndArgumentsNoForkWithCommand(const QString& command) noexcept;

void AddPlatformSpecificExitCodeCases() noexcept;

class Test;

} // Namespace NeovimQt
