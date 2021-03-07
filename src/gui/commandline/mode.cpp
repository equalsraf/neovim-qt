#include "mode.h"

#include <QDebug>

namespace NeovimQt { namespace Cmdline {

Mode ModeFromString(const QString& position) noexcept
{
	if (QString::compare(position, "dynamic", Qt::CaseInsensitive) == 0) {
		return Mode::Dynamic;
	}

	if (QString::compare(position, "fixed", Qt::CaseInsensitive) == 0) {
		return Mode::Fixed;
	}

	qDebug() << "Unrecognize value for `Mode`, using Dynamic";
	return Mode::Dynamic;
}

} } // namespace NeovimQt::Cmdline
