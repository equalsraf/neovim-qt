#ifndef NEOVIM_QT_PRINTINFO
#define NEOVIM_QT_PRINTINFO

#include <QString>

namespace NeovimQt {

/// Performs a cross-platform display of information to the user.
/// MacOS: Prints to stdout
/// Unix/Linux: Prints to stdout
/// Windows: Displays a messagebox
///
/// Intended to behave the same way as QCommandLineParser's '--help' option.
void PrintInfo(const QString& message) noexcept;

} // Namespace NeovimQt

#endif // NEOVIM_QT_PRINTINFO
