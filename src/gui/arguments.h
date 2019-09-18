#pragma once

#include <QCommandLineParser>

namespace NeovimQt {

/// Checks for commandline argument --geometry support.
bool hasGeometryArg();

/// Checks for commandline argument --qwindowgeometry support.
bool hasQWindowGeometryArg();

} // namespace NeovimQt
