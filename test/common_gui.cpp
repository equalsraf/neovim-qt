#include "common_gui.h"

#include <gui/mainwindow.h>

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

	const qsizetype signalCount{ onAttached.count() };
	Q_ASSERT(signalCount == 1);

	Q_ASSERT(obj->isNeovimAttached());
}

QSharedPointer<Shell> CreateShellWidget() noexcept
{
	DisableLocalGInitVim();
	NeovimConnector* c{ NeovimConnector::spawn(cs_argsNone) };
	Shell* s{ new Shell{ c } };
	s->setAttribute(Qt::WA_DeleteOnClose, true);

	s->show();

	ValidateNeovimConnection(s);

	return QSharedPointer<Shell>(s, [](Shell *s) {
			if (s) {
				s->close();
			}
			});
}

QSharedPointer<MainWindow> CreateMainWindow() noexcept
{
	NeovimConnector* c{ NeovimConnector::spawn(cs_argsNone) };
	MainWindow* w{ new MainWindow{ c } };
	w->setAttribute(Qt::WA_DeleteOnClose, true);

	w->show();

	ValidateNeovimConnection(w);

	return QSharedPointer<MainWindow>(w, [](MainWindow *w) {
			if (w) {
				w->close();
			}
			});
}

QSharedPointer<MainWindow> CreateMainWindowWithRuntime() noexcept
{
	static const QStringList cs_argsNoneRuntime{
		"-u", "NONE", "--cmd", "set rtp+=" + GetRuntimeAbsolutePath()
	};

	DisableLocalGInitVim();
	NeovimConnector* c{ NeovimConnector::spawn(cs_argsNoneRuntime) };
	MainWindow* w{ new MainWindow{ c } };
	w->setAttribute(Qt::WA_DeleteOnClose, true);

	w->show();

	ValidateNeovimConnection(w);

	return QSharedPointer<MainWindow>(w, [](MainWindow *w) {
			if (w) {
				w->close();
			}
			});
}

} // namespace NeovimQt
