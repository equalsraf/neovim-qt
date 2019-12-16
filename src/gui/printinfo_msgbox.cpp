#include "printinfo.h"

#include <QCoreApplication>
#include <qt_windows.h>
#include <QVariant>

namespace NeovimQt {

void PrintInfo(const QString& message) noexcept
{
	QString title;
	if (QCoreApplication::instance()) {
		title = QCoreApplication::instance()->property("applicationDisplayName").toString();
	}
	if (title.isEmpty()) {
		title = QCoreApplication::applicationName();
	}

	constexpr UINT flags{ MB_OK | MB_TOPMOST | MB_SETFOREGROUND | MB_ICONINFORMATION };

	MessageBoxW(0, reinterpret_cast<const wchar_t *>(message.utf16()),
		reinterpret_cast<const wchar_t *>(title.utf16()), flags);
}

} // namespace NeovimQt
