#include "app.h"

#include <QDir>
#include <QFileInfo>

namespace NeovimQt {

QString App::getRuntimePath() noexcept
{
	QString path{ QString::fromLocal8Bit(qgetenv("NVIM_QT_RUNTIME_PATH")) };

	if (QFileInfo(path).isDir()) {
		return path;
	}

	// Look for the runtime relative to the nvim-qt binary
	const QDir d = QDir{ applicationDirPath() }.filePath("../share/nvim-qt/runtime");

	if (d.exists()) {
		return d.path();
	}

	return {};
}

} // namespace NeovimQt
