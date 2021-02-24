#include "position.h"

#include <QDebug>

namespace NeovimQt { namespace Cmdline {

/// Converts QSettings string (case insensitive) to `Position`.
/// The default fallback value is: `Position::Center`.
Position PositionFromString(const QString& position) noexcept
{
	if (QString::compare(position, "center", Qt::CaseInsensitive) == 0) {
		return Position::Center;
	}

	if (QString::compare(position, "top", Qt::CaseInsensitive) == 0) {
		return Position::Top;
	}

	if (QString::compare(position, "bottom", Qt::CaseInsensitive) == 0) {
		return Position::Bottom;
	}

	qDebug() << "Unrecognize value for `Position`, using Center";
	return Position::Center;
}

} } // namespace NeovimQt::Cmdline
