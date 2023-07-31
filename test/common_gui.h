#pragma once

#include <gui/mainwindow.h>
#include <gui/shell.h>
#include <neovimconnector.h>
#include <utility>

namespace NeovimQt {

Shell* CreateShellWidget() noexcept;

MainWindow* CreateMainWindow() noexcept;

std::pair<NeovimConnector*, MainWindow*> CreateMainWindowWithRuntime() noexcept;

} // namespace NeovimQt
