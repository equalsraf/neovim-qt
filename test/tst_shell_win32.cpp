#include "tst_shell.h"

#include <QTest>

namespace NeovimQt {

QClipboard::Mode GetClipboardMode(char /*reg*/) noexcept
{
	return QClipboard::Clipboard;
}

QString GetPlatformTestFont() noexcept
{
	return QStringLiteral("DejaVu Sans Mono");
}

QStringList BinaryAndArgumentsNoForkWithCommand(const QString& command) noexcept
{
	return { NVIM_QT_BINARY, "--",  "-c", command };
}

void AddPlatformSpecificExitCodeCases() noexcept
{
	// Unlike UNIX-like operating systems, Windows represents the exit code as
	// a signed 32-bit integer. 256 doesn't overflows hence :256cq returns
	// 256.
	QTest::newRow("Exit with Code 256: 256cq")
		<< 256 << 256 << QByteArray("256cq");
}

} // namespace NeovimQt
