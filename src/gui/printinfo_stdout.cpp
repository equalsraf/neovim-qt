#include "printinfo.h"

#include <QTextStream>

namespace NeovimQt {

void PrintInfo(const QString& message) noexcept
{
	QTextStream out{ stdout };
	out << message;
}

} // namespace NeovimQt
