#include "tst_shell.h"

#include <gui/mainwindow.h>
#include <msgpackrequest.h>
#include <QClipboard>
#include <QFontDatabase>
#include <QLocalSocket>
#include <QtGlobal>
#include <QtTest/QtTest>

#include "common.h"
#include "common_gui.h"

#if defined(Q_OS_WIN) && defined(USE_STATIC_QT)
#include <QtPlugin>
Q_IMPORT_PLUGIN (QWindowsIntegrationPlugin);
#endif

namespace NeovimQt {

class TestShell : public QObject
{
	Q_OBJECT

private slots:
	void initTestCase() noexcept;
	void benchStart() noexcept;
	void startVarsShellWidget() noexcept;
	void startVarsMainWindow() noexcept;
	void gviminit() noexcept;
	void guiShimCommands() noexcept;
	void CloseEvent_data() noexcept;
	void CloseEvent() noexcept;
	void GetClipboard_data() noexcept;
	void GetClipboard() noexcept;
	void SetClipboard_data() noexcept;
	void SetClipboard() noexcept;

protected:
	void checkStartVars(NeovimQt::NeovimConnector* conn) noexcept;
	void grabShellScreenshot(Shell& s, const QString& filename) noexcept;
};

static void SignalPrintError(QString msg, const QVariant& err) noexcept
{
	qWarning() << "Error Signal!" << msg << err;
}

void TestShell::initTestCase() noexcept
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

void TestShell::benchStart() noexcept
{
	QBENCHMARK
	{
		auto cs{ CreateShellWidget() };
		Shell* s{ cs.second };

		QSignalSpy onResize(s, &Shell::neovimResized);
		QVERIFY(onResize.isValid());
		QVERIFY(SPYWAIT(onResize));
	}
}

void TestShell::startVarsShellWidget() noexcept
{
	auto cs{ CreateShellWidget() };
	NeovimConnector* c{ cs.first };

	checkStartVars(c);
}

void TestShell::startVarsMainWindow() noexcept
{
	auto cw{ CreateMainWindow() };
	NeovimConnector* c{ cw.first };

	checkStartVars(c);
}

void TestShell::gviminit() noexcept
{
	qputenv("GVIMINIT", "let g:test_gviminit = 1");
	NeovimConnector* c{ CreateShellWidget().first };

	MsgpackRequest* req{ c->api0()->vim_command_output(QByteArrayLiteral("echo g:test_gviminit")) };
	QSignalSpy cmd{ req, &MsgpackRequest::finished };
	QVERIFY(cmd.isValid());
	QVERIFY(SPYWAIT(cmd));
	QCOMPARE(cmd.at(0).at(2).toByteArray(), QByteArray("1"));
}

void TestShell::guiShimCommands() noexcept
{
	auto cw{ CreateMainWindowWithRuntime() };
	NeovimConnector* c{ cw.first };
	MainWindow* w{ cw.second };

	QObject::connect(c->neovimObject(), &NeovimApi1::err_vim_command_output, SignalPrintError);

	QSignalSpy cmd_font(c->neovimObject()->vim_command_output(QByteArrayLiteral("GuiFont!")),
		&MsgpackRequest::finished);
	QVERIFY(cmd_font.isValid());
	QVERIFY2(SPYWAIT(cmd_font), "Waiting for GuiFont");

	QSignalSpy cmd_ls(c->neovimObject()->vim_command_output(QByteArrayLiteral("GuiLinespace")),
		&MsgpackRequest::finished);
	QVERIFY(cmd_ls.isValid());
	QVERIFY2(SPYWAIT(cmd_ls), "Waiting for GuiLinespace");

	// Test font attributes
	const QString cmdFontSize14{ QStringLiteral("GuiFont! %1:h14").arg(GetPlatformTestFont()) };
	const QString expectedFontSize14{ QStringLiteral("%1:h14").arg(GetPlatformTestFont()) };
	QSignalSpy cmd_gf{ c->neovimObject()->vim_command_output(cmdFontSize14.toUtf8()),
		&MsgpackRequest::finished };
	QVERIFY(cmd_gf.isValid());
	QVERIFY(SPYWAIT(cmd_gf));

	QSignalSpy spy_fontchange(w->shell(), &ShellWidget::shellFontChanged);

	// Test Performance: timeout occurs often, set value carefully.
	SPYWAIT(spy_fontchange, 2500 /*msec*/);

	QCOMPARE(w->shell()->fontDesc(), expectedFontSize14);

	// Normalization removes the :b attribute
	const QString cmdFontBoldRemoved{
		QStringLiteral("GuiFont! %1:h16:b:l").arg(GetPlatformTestFont())
	};
	const QString expectedFontBoldRemoved{ QStringLiteral("%1:h16:l").arg(GetPlatformTestFont()) };
	QSignalSpy spy_fontchange2(w->shell(), &ShellWidget::shellFontChanged);
	QSignalSpy cmd_gf2{ c->neovimObject()->vim_command_output(cmdFontBoldRemoved.toUtf8()),
		&MsgpackRequest::finished };
	QVERIFY(cmd_gf2.isValid());
	QVERIFY(SPYWAIT(cmd_gf2, 5000));

	// Test Performance: timeout occurs often, set value carefully.
	SPYWAIT(spy_fontchange2, 5000 /*msec*/);

	QCOMPARE(w->shell()->fontDesc(), expectedFontBoldRemoved);
}

void TestShell::CloseEvent_data() noexcept
{
	QTest::addColumn<int>("msgpack_status");
	QTest::addColumn<int>("exit_status");
	QTest::addColumn<QByteArray>("command");

	QTest::newRow("Normal Exit: q") << 0 << 0 << QByteArray("q");

	QTest::newRow("Exit with Code 1: cq") << 1 << 1 << QByteArray("cq");

	QTest::newRow("Exit with Code 2: 2cq") << 2 << 2 << QByteArray("2cq");

	QTest::newRow("Exit with Code 255: 255cq") << 255 << 255 << QByteArray("255cq");

	// Some exit-status scenarios are platform dependent.
	// Ex) Overflow on UNIX-like operating systems.
	AddPlatformSpecificExitCodeCases();
}

void TestShell::CloseEvent() noexcept
{
	auto cw{ CreateMainWindowWithRuntime() };
	NeovimConnector* c{ cw.first };
	MainWindow* w{ cw.second };

	QFETCH(int, msgpack_status);
	QFETCH(int, exit_status);
	QFETCH(QByteArray, command);

	// GUI shim Close event
	QSignalSpy onClose(w->shell(), &Shell::neovimGuiCloseRequest);
	QVERIFY(onClose.isValid());

	QSignalSpy onWindowClosing(w, &MainWindow::closing);
	QVERIFY(onWindowClosing.isValid());

	c->api0()->vim_command(command);

	QVERIFY(SPYWAIT(onClose));
	QCOMPARE(onClose.takeFirst().at(0).toInt(), msgpack_status);

	QVERIFY(SPYWAIT(onWindowClosing));
	QCOMPARE(onWindowClosing.takeFirst().at(0).toInt(), msgpack_status);

	// and finally a call to nvim-qt
	QProcess p;
	p.setProgram(NVIM_QT_BINARY);
	QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
	env.insert("NVIM_QT_RUNTIME_PATH", GetRuntimeAbsolutePath());
	p.setProcessEnvironment(env);
	p.setArguments(BinaryAndArgumentsNoForkWithCommand(command));
	p.start();
	p.waitForFinished(-1);
	QCOMPARE(p.exitStatus(), QProcess::NormalExit);
	int actual_exit_status{ p.exitCode() };

	QCOMPARE(actual_exit_status, exit_status);
}

void TestShell::GetClipboard_data() noexcept
{
	// * or +
	QTest::addColumn<char>("reg");
	// data set in the register when starting test, this is set
	// externaly (i.e. without going through the provider)
	QTest::addColumn<QByteArray>("register_data");

	QTest::newRow("empty *") << '*' << QByteArray();
	QTest::newRow("set *") << '*' << QByteArray("something");
	QTest::newRow("empty +") << '+' << QByteArray();
	QTest::newRow("set +") << '+' << QByteArray("something");
	QTest::newRow("paste *") << '*' << QByteArray("something");
	QTest::newRow("paste +") << '+' << QByteArray("something");
}

void TestShell::GetClipboard() noexcept
{
	auto cw{ CreateMainWindowWithRuntime() };
	NeovimConnector* c{ cw.first };

	QFETCH(char, reg);
	QFETCH(QByteArray, register_data);

	QObject::connect(c->neovimObject(), &NeovimApi1::err_vim_command_output, SignalPrintError);

	// provided by the GUI shim
	c->api0()->vim_command(QByteArrayLiteral("call GuiClipboard()"));

	QGuiApplication::clipboard()->setText(register_data, GetClipboardMode(reg));

	QString getreg_cmd = QString("getreg('%1')").arg(reg);
	QSignalSpy cmd_clip(c->api1()->nvim_eval(getreg_cmd.toUtf8()), &MsgpackRequest::finished);
	QVERIFY(cmd_clip.isValid());
	QVERIFY(SPYWAIT(cmd_clip));
	QCOMPARE(cmd_clip.takeFirst().at(2), QVariant(register_data));
}

void TestShell::SetClipboard_data() noexcept
{
	// * or +
	QTest::addColumn<char>("reg");
	// data set in the register when starting test, this is set
	// externaly (i.e. without going through the provider)
	QTest::addColumn<QByteArray>("register_data");

	QTest::newRow("empty *") << '*' << QByteArray();
	QTest::newRow("set *") << '*' << QByteArray("something");
	QTest::newRow("empty +") << '+' << QByteArray();
	QTest::newRow("set +") << '+' << QByteArray("something");
	QTest::newRow("paste *") << '*' << QByteArray("something");
	QTest::newRow("paste +") << '+' << QByteArray("something");
}

void TestShell::SetClipboard() noexcept
{
	auto cw{ CreateMainWindowWithRuntime() };
	NeovimConnector* c{ cw.first };

	QFETCH(char, reg);
	QFETCH(QByteArray, register_data);

	QObject::connect(c->neovimObject(), &NeovimApi1::err_vim_command_output, SignalPrintError);

	// provided by the GUI shim
	c->api0()->vim_command(QByteArrayLiteral("call GuiClipboard()"));

	QString setreg_cmd =
		QString("setreg('%1', '%2')\n").arg(reg).arg(QString::fromUtf8(register_data));
	c->neovimObject()->vim_command(setreg_cmd.toUtf8());
	QSignalSpy spy_sync(c->neovimObject()->vim_feedkeys("", "", false), &MsgpackRequest::finished);
	SPYWAIT(spy_sync);
	QVERIFY(spy_sync.isValid());
	QVERIFY(SPYWAIT(spy_sync));

	QGuiApplication::clipboard()->setText(register_data, GetClipboardMode(reg));
}

void TestShell::checkStartVars(NeovimQt::NeovimConnector* conn) noexcept
{
	auto* nvim = conn->api1();
	connect(nvim, &NeovimQt::NeovimApi1::err_vim_get_var, SignalPrintError);

	const QStringList vars{ "GuiWindowId", "GuiWindowMaximized", "GuiWindowFullScreen", "GuiFont", "GuiWindowFrameless" };
	for (const auto& var : vars) {
		QSignalSpy onVar(nvim, SIGNAL(on_vim_get_var(QVariant)));
		QVERIFY(onVar.isValid());
		nvim->vim_get_var(var.toUtf8());
		QVERIFY(SPYWAIT(onVar));
	}

	// v:windowid
	QSignalSpy onVarWindowId(nvim, SIGNAL(on_vim_get_vvar(QVariant)));
	QVERIFY(onVarWindowId.isValid());
	nvim->vim_get_vvar(QByteArrayLiteral("windowid"));
	QVERIFY(SPYWAIT(onVarWindowId));
}

void TestShell::grabShellScreenshot(Shell& s, const QString& filename) noexcept
{
	s.repaint();
	QPixmap p{ s.grab() };
	p.save(filename);
}

} // Namespace NeovimQt

QTEST_MAIN(NeovimQt::TestShell)
#include "tst_shell.moc"
