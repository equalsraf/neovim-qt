#include "tst_shell.h"

#include <gui/mainwindow.h>
#include <msgpackrequest.h>
#include <QClipboard>
#include <QFontDatabase>
#include <QLocalSocket>
#include <QSettings>
#include <QtGlobal>
#include <QtTest/QtTest>

#include "common.h"

#if defined(Q_OS_WIN) && defined(USE_STATIC_QT)
#include <QtPlugin>
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin);
#endif

namespace NeovimQt {

class Test : public QObject
{
	Q_OBJECT
private slots:
	void init() noexcept;

	void initTestCase() noexcept;
	void benchStart() noexcept;

	void startVarsShellWidget() noexcept;

	void startVarsMainWindow() noexcept;

	void GuiFontStarupValues() noexcept;

	void guiExtTablineSet() noexcept;

	void gviminit() noexcept;

	void guiShimCommands() noexcept;

	void CloseEvent() noexcept;
	void CloseEvent_data() noexcept;

	void GetClipboard() noexcept;
	void GetClipboard_data() noexcept;

	void SetClipboard() noexcept;
	void SetClipboard_data() noexcept;

private:
	void checkStartVars(NeovimConnector* conn) noexcept;

	void grabShellScreenshot(Shell& s, const QString& filename) noexcept;
};

namespace {

QSettings::SettingsMap s_mockSettingsMap; // clazy:exclude=non-pod-global-static

bool readMockQSettings(QIODevice& /*device*/, QSettings::SettingsMap& map) noexcept
{
	map = s_mockSettingsMap;
	return true;
}

bool writeMockQSettings(QIODevice& device, const QSettings::SettingsMap& map) noexcept
{
	// QSettings will not call readMockQSettings without content in `device`.
	// https://stackoverflow.com/questions/41495765/cannot-get-qsettings-to-read-from-custom-storage-format
	device.write("X", 1); // FIXME Is this safe? What random file is being written to my disk???

	s_mockSettingsMap = map;
	return true;
}

void ResetMockQSettings(QSettings::SettingsMap&& initialValue = {}) noexcept
{
	const QSettings::Format MockFormat{ QSettings::registerFormat(
		"mock", readMockQSettings, writeMockQSettings) };

	QSettings::setDefaultFormat(MockFormat);

	s_mockSettingsMap = std::move(initialValue);
}

void SignalPrintError(QString msg, const QVariant& err) noexcept
{
	qDebug() << msg << err;
}

const QStringList cs_argsNone{ "-u", "NONE" }; // clazy:exclude=non-pod-global-static

template<class T> void ValidateNeovimConnection(T* obj) noexcept
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
	NeovimConnector* c{ NeovimConnector::spawn(cs_argsNone) };
	Shell* s{ new Shell{ c } };

	ValidateNeovimConnection(s);

	return { c, s };
}

std::pair<NeovimConnector*, MainWindow*> CreateMainWindow() noexcept
{
	NeovimConnector* c{ NeovimConnector::spawn(cs_argsNone) };
	MainWindow* w{ new MainWindow{ c } };

	ValidateNeovimConnection(w);

	return { c, w };
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

std::pair<NeovimConnector*, MainWindow*> CreateMainWindowWithRuntime() noexcept
{
	static const QStringList cs_argsNoneRuntime{
		"-u", "NONE", "--cmd", "set rtp+=" + GetRuntimeAbsolutePath()
	};

	NeovimConnector* c{ NeovimConnector::spawn(cs_argsNoneRuntime) };
	MainWindow* w{ new MainWindow{ c } };

	ValidateNeovimConnection(w);

	return { c, w };
}

} // namespace

void Test::init() noexcept
{
	// Use static QSettings Mock Implementation.
	// Avoids overwriting real user QSettings values on each run.
	ResetMockQSettings();
}

void Test::initTestCase() noexcept
{
	const QStringList fonts{ "third-party/DejaVuSansMono.ttf",
		"third-party/DejaVuSansMono-Bold.ttf",
		"third-party/DejaVuSansMono-BoldOblique.ttf" };

	for (const auto& path : fonts) {
		QString abs_path_to_font{ CMAKE_SOURCE_DIR };
		abs_path_to_font.append("/").append(path);
		QFontDatabase::addApplicationFont(abs_path_to_font);
	}

	// Do not load the user's real `ginit.vim` file
	qputenv("GVIMINIT", "let g:test_gviminit = 0");
}

void Test::benchStart() noexcept
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

void Test::startVarsShellWidget() noexcept
{
	auto cs{ CreateShellWidget() };
	NeovimConnector* c{ cs.first };

	checkStartVars(c);
}

void Test::startVarsMainWindow() noexcept
{
	auto cw{ CreateMainWindow() };
	NeovimConnector* c{ cw.first };

	checkStartVars(c);
}

static QString GetRequestResult(MsgpackRequest* req) noexcept
{
	QString value;

	auto callback = [&](quint32, quint64, const QVariant& val) noexcept {
		value = val.toString();
	};

	QObject::connect(req, &MsgpackRequest::finished, callback);
	QSignalSpy spy(req, &MsgpackRequest::finished);
	spy.wait();

	return value;
}

void Test::GuiFontStarupValues() noexcept
{
	// Window 1: Load :GuiFont and write to QSettings
	auto cw1{ CreateMainWindowWithRuntime() };
	NeovimConnector* c1{ cw1.first };
	MainWindow* w1{ cw1.second };

	const QString cmdFontSize20{ QStringLiteral("GuiFont %1:h20").arg(GetPlatformTestFont()) };
	const QString expectedFont20{ QStringLiteral("%1:h20").arg(GetPlatformTestFont()) };

	QSignalSpy spyFontChanged(w1->shell(), &ShellWidget::shellFontChanged);
	QSignalSpy cmdSetFont{ c1->neovimObject()->vim_command_output(c1->encode(cmdFontSize20)),
		&MsgpackRequest::finished };

	QVERIFY(cmdSetFont.isValid());
	QVERIFY(SPYWAIT(cmdSetFont));
	QVERIFY(SPYWAIT(spyFontChanged));

	QCOMPARE(w1->shell()->fontDesc(), expectedFont20);
	QVERIFY(s_mockSettingsMap.contains("Gui/Font"));
	QCOMPARE(s_mockSettingsMap.value("Gui/Font").toString(), expectedFont20);

	// Window 2: Load QSettings to :GuiFont and :set guifont
	auto cw2{ CreateMainWindowWithRuntime() };
	NeovimConnector* c2{ cw2.first };

	QCOMPARE(GetRequestResult(c2->api0()->vim_get_var("GuiFont")), expectedFont20);
	QCOMPARE(GetRequestResult(c2->api0()->vim_get_option("guifont")), expectedFont20);
}

void Test::guiExtTablineSet() noexcept
{
	auto cs{ CreateShellWidget() };
	Shell* s{ cs.second };

	QSignalSpy onOptionSet(s, &Shell::neovimExtTablineSet);
	QVERIFY(onOptionSet.isValid());
	QVERIFY(SPYWAIT(onOptionSet));
}

void Test::gviminit() noexcept
{
	qputenv("GVIMINIT", "let g:test_gviminit = 1");
	NeovimConnector* c{ CreateShellWidget().first };

	auto req{ c->api0()->vim_command_output(c->encode("echo g:test_gviminit")) };
	QSignalSpy cmd(req, &MsgpackRequest::finished);
	QVERIFY(cmd.isValid());
	QVERIFY(SPYWAIT(cmd));
	QCOMPARE(cmd.at(0).at(2).toByteArray(), QByteArray("1"));
}

void Test::guiShimCommands() noexcept
{
	auto cw{ CreateMainWindowWithRuntime() };
	NeovimConnector* c{ cw.first };
	MainWindow* w{ cw.second };

	QObject::connect(c->neovimObject(), &NeovimApi1::err_vim_command_output, &SignalPrintError);

	QSignalSpy cmd_font(
		c->neovimObject()->vim_command_output(c->encode("GuiFont")), &MsgpackRequest::finished);
	QVERIFY(cmd_font.isValid());
	QVERIFY2(SPYWAIT(cmd_font), "Waiting for GuiFont");

	QSignalSpy cmd_ls(c->neovimObject()->vim_command_output(c->encode("GuiLinespace")),
		&MsgpackRequest::finished);
	QVERIFY(cmd_ls.isValid());
	QVERIFY2(SPYWAIT(cmd_ls), "Waiting for GuiLinespace");

	// Test font attributes
	const QString cmdFontSize14{ QStringLiteral("GuiFont %1:h14").arg(GetPlatformTestFont()) };
	const QString expectedFont14{ QStringLiteral("%1:h14").arg(GetPlatformTestFont()) };
	QSignalSpy cmd_gf{ c->neovimObject()->vim_command_output(c->encode(cmdFontSize14)),
		&MsgpackRequest::finished };
	QSignalSpy spy_fontchange(w->shell(), &ShellWidget::shellFontChanged);
	QVERIFY(cmd_gf.isValid());
	QVERIFY(SPYWAIT(cmd_gf));

	QVERIFY(SPYWAIT(spy_fontchange, 2500 /*msec*/));
	QCOMPARE(w->shell()->fontDesc(), expectedFont14);

	// Normalization removes the :b attribute
	const QString cmdFontBoldRemoved{
		QStringLiteral("GuiFont %1:h14:b:l").arg(GetPlatformTestFont())
	};
	const QString expectedFontBoldRemoved{ QStringLiteral("%1:h14:l").arg(GetPlatformTestFont()) };
	QSignalSpy spy_fontchange2(w->shell(), &ShellWidget::shellFontChanged);
	QSignalSpy cmd_gf2{ c->neovimObject()->vim_command_output(c->encode(cmdFontBoldRemoved)),
		&MsgpackRequest::finished };
	QVERIFY(cmd_gf2.isValid());
	QVERIFY(SPYWAIT(cmd_gf2));
	QVERIFY(SPYWAIT(spy_fontchange2, 2500 /*msec*/));

	QCOMPARE(w->shell()->fontDesc(), expectedFontBoldRemoved);
}

void Test::CloseEvent() noexcept
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

	c->api0()->vim_command(c->encode(command));

	QVERIFY(SPYWAIT(onClose));
	QCOMPARE(onClose.takeFirst().at(0).toInt(), msgpack_status);

	QVERIFY(SPYWAIT(onWindowClosing));
	QCOMPARE(onWindowClosing.takeFirst().at(0).toInt(), msgpack_status);

	// and finally a call to nvim-qt
	QProcess p;
	p.setProgram(NVIM_QT_BINARY);
	QProcessEnvironment env{ QProcessEnvironment::systemEnvironment() };
	env.insert("NVIM_QT_RUNTIME_PATH", GetRuntimeAbsolutePath());
	p.setProcessEnvironment(env);
	p.setArguments(BinaryAndArgumentsNoForkWithCommand(command));
	p.start();
	p.waitForFinished(-1);
	QCOMPARE(p.exitStatus(), QProcess::NormalExit);
	int actual_exit_status{ p.exitCode() };

	QCOMPARE(actual_exit_status, exit_status);
}

void Test::CloseEvent_data() noexcept
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


void Test::GetClipboard() noexcept
{
	auto cw{ CreateMainWindowWithRuntime() };
	NeovimConnector* c{ cw.first };

	QFETCH(char, reg);
	QFETCH(QByteArray, register_data);

	QObject::connect(c->neovimObject(), &NeovimApi1::err_vim_command_output, &SignalPrintError);

	// provided by the GUI shim
	c->api0()->vim_command(c->encode("call GuiClipboard()"));

	QGuiApplication::clipboard()->setText(register_data, GetClipboardMode(reg));

	const QString getreg_cmd{ QStringLiteral("getreg('%1')").arg(reg) };
	QSignalSpy cmd_clip(c->api1()->nvim_eval(c->encode(getreg_cmd)), &MsgpackRequest::finished);
	QVERIFY(cmd_clip.isValid());
	QVERIFY(SPYWAIT(cmd_clip));
	QCOMPARE(cmd_clip.takeFirst().at(2), QVariant(register_data));
}

void Test::GetClipboard_data() noexcept
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


void Test::SetClipboard() noexcept
{
	auto cw{ CreateMainWindowWithRuntime() };
	NeovimConnector* c{ cw.first };

	QFETCH(char, reg);
	QFETCH(QByteArray, register_data);

	QObject::connect(c->neovimObject(), &NeovimApi1::err_vim_command_output, &SignalPrintError);

	// provided by the GUI shim
	c->api0()->vim_command(c->encode("call GuiClipboard()"));

	const QString setreg_cmd{
		QStringLiteral("setreg('%1', '%2')\n").arg(reg).arg(QString::fromUtf8(register_data))
	};
	c->neovimObject()->vim_command(c->encode(setreg_cmd));
	QSignalSpy spy_sync(c->neovimObject()->vim_feedkeys("", "", false), &MsgpackRequest::finished);
	SPYWAIT(spy_sync);
	QVERIFY(spy_sync.isValid());
	QVERIFY(SPYWAIT(spy_sync));

	QGuiApplication::clipboard()->text(GetClipboardMode(reg));
}

void Test::SetClipboard_data() noexcept
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

void Test::checkStartVars(NeovimQt::NeovimConnector* conn) noexcept
{
	NeovimApi1* nvim{ conn->api1() };
	connect(nvim, &NeovimQt::NeovimApi1::err_vim_get_var, &SignalPrintError);

	const QStringList varList{
		"GuiWindowId", "GuiWindowMaximized", "GuiWindowFullScreen", "GuiFont"
	};

	for (const auto& var : varList) {
		qDebug() << "Checking Neovim for Gui var" << var;
		QSignalSpy onVar(nvim, &NeovimApi1::on_vim_get_var);
		QVERIFY(onVar.isValid());
		nvim->vim_get_var(conn->encode(var));
		QVERIFY(SPYWAIT(onVar, 1000 /*msec*/));
	}

	// v:windowid
	QSignalSpy onVarWindowId(nvim, &NeovimApi1::on_vim_get_vvar);
	QVERIFY(onVarWindowId.isValid());
	nvim->vim_get_vvar(conn->encode("windowid"));
	QVERIFY(SPYWAIT(onVarWindowId));
}

void Test::grabShellScreenshot(Shell& s, const QString& filename) noexcept
{
	s.repaint();
	QPixmap p{ s.grab() };
	p.save(filename);
}

} // namespace NeovimQt

QTEST_MAIN(NeovimQt::Test)
#include "tst_shell.moc"
