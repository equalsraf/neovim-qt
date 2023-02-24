#include "common_gui.h"

#include <gui/mainwindow.h>
#include <QFontDatabase>

#include "common.h"

namespace NeovimQt {

static const QStringList cs_argsNone{ "-u", "NONE" }; // clazy:exclude=non-pod-global-static

static void DisableLocalGInitVim() noexcept
{
	// Check environment variable GVIMINIT, skip if already set.
	const QByteArray ginitVar{ "GVIMINIT" };
	if (!qEnvironmentVariableIsEmpty(ginitVar)) {
		return;
	}

	// Do not pull in the local machine's ginit.vim file for tests.
	qputenv(ginitVar, ";");
}

template<class T> static void ValidateNeovimConnection(T* obj) noexcept
{
	QSignalSpy onAttached{ obj, &T::neovimAttachmentChanged };

	Q_ASSERT(onAttached.isValid());

	const bool signalEmitted{ onAttached.wait() };
	Q_ASSERT(signalEmitted);

	const int signalCount{ onAttached.count() };
	Q_ASSERT(signalCount == 1);

	Q_ASSERT(obj->isNeovimAttached());
}

std::pair<NeovimConnector*, Shell*> CreateShellWidget() noexcept
{
	DisableLocalGInitVim();
	NeovimConnector* c{ NeovimConnector::spawn(cs_argsNone) };
	Shell* s{ new Shell{ c } };

	s->show();

	ValidateNeovimConnection(s);

	return { c, s };
}

std::pair<NeovimConnector*, MainWindow*> CreateMainWindow() noexcept
{
	NeovimConnector* c{ NeovimConnector::spawn(cs_argsNone) };
	MainWindow* w{ new MainWindow{ c } };

	w->show();

	ValidateNeovimConnection(w);

	return { c, w };
}

std::pair<NeovimConnector*, MainWindow*> CreateMainWindowWithRuntime() noexcept
{
	static const QStringList cs_argsNoneRuntime{
		"-u", "NONE", "--cmd", "set rtp+=" + GetRuntimeAbsolutePath()
	};

	DisableLocalGInitVim();
	NeovimConnector* c{ NeovimConnector::spawn(cs_argsNoneRuntime) };
	MainWindow* w{ new MainWindow{ c } };

	w->show();

	ValidateNeovimConnection(w);

	return { c, w };
}

void LoadDejaVuSansMonoTestFonts() noexcept
{
	const QStringList fonts{
		QStringLiteral("third-party/DejaVuSansMono.ttf"),
		QStringLiteral("third-party/DejaVuSansMono-Bold.ttf"),
		QStringLiteral("third-party/DejaVuSansMono-BoldOblique.ttf") };

	for (const auto& path : fonts) {
		QString abs_path_to_font(CMAKE_SOURCE_DIR);
		abs_path_to_font.append("/").append(path);
		QFontDatabase::addApplicationFont(abs_path_to_font);
	}
}

} // namespace NeovimQt
