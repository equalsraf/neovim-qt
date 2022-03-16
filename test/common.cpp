#include "common.h"

#include <QFileInfo>

bool SPYWAIT(QSignalSpy& spy, int timeout) noexcept
{
	// For more details, see:
	// http://stackoverflow.com/questions/22390208/google-test-mock-with-qt-signals
	return spy.count() > 0 || spy.wait(timeout);
}

QString GetRuntimeAbsolutePath() noexcept
{
	static const QFileInfo cs_runtime{ QStringLiteral(CMAKE_SOURCE_DIR)
		+ QStringLiteral("/src/gui/runtime") };

	if (!cs_runtime.exists()) {
		qFatal("Unable to find GUI runtime!");
	}

	return cs_runtime.absoluteFilePath();
}
