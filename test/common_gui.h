#pragma once

#include <gui/mainwindow.h>
#include <gui/shell.h>
#include <neovimconnector.h>
#include <utility>

namespace NeovimQt {

std::pair<NeovimConnector*, Shell*> CreateShellWidget() noexcept;

std::pair<NeovimConnector*, MainWindow*> CreateMainWindow() noexcept;

std::pair<NeovimConnector*, MainWindow*> CreateMainWindowWithRuntime() noexcept;

void LoadLocalDejaVuTestFonts() noexcept;

} // namespace NeovimQt
