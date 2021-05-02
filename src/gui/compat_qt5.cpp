
#include "compat.h"
#include <QApplication>
#include <QDesktopWidget>

namespace NeovimQt { namespace Compat {

QRect screenAvailableGeometry(const QWidget* w) {
	return QApplication::desktop()->availableGeometry(w);
}

} } // namespace NeovimQt::Compat
