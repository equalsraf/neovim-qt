#pragma once
// Compatibility functions between different Qt versions

#include <QRect>
#include <QWidget>

namespace NeovimQt { namespace Compat {

// Get the available geometry in a screen where this widget is placed
QRect screenAvailableGeometry(const QWidget*);

} } // namespace NeovimQt::Compat
