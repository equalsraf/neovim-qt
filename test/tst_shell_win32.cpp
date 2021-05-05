#include "tst_shell.h"

#include <QStringList>
#include <QTest>

namespace NeovimQt {

QStringList BinaryAndArgumentsNoForkWithCommand(const QString& command) noexcept
{
	return { NVIM_QT_BINARY, "--",  "-c", command };
}

void AddPlatformSpecificExitCodeCases() noexcept
{
	// FIXME Add comment
	QTest::newRow("Exit Code Overflow: 256cq")
		<< 256 << 0 << QByteArray("256cq");
}

} // namespace NeovimQt
