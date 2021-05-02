
#include "compat.h"
#include <QScreen>

namespace NeovimQt { namespace Compat {

QRect screenAvailableGeometry(const QWidget* w) {
	return w->screen()->availableGeometry();
}

} } // namespace NeovimQt::Compat
