#pragma once

#include <QSharedPointer>
#include <gui/mainwindow.h>
#include <gui/shell.h>
#include <neovimconnector.h>
#include <utility>

namespace NeovimQt {

QSharedPointer<Shell> CreateShellWidget() noexcept;

QSharedPointer<MainWindow> CreateMainWindow() noexcept;

QSharedPointer<MainWindow> CreateMainWindowWithRuntime() noexcept;

} // namespace NeovimQt
