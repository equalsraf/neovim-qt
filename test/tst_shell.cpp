#include <QtGlobal>
#include <QtTest/QtTest>
#include <QLocalSocket>
#include <QFontDatabase>
#include <QClipboard>
#include <gui/mainwindow.h>
#include <msgpackrequest.h>
#include "common.h"

#if defined(Q_OS_WIN) && defined(USE_STATIC_QT)
#include <QtPlugin>
Q_IMPORT_PLUGIN (QWindowsIntegrationPlugin);
#endif

namespace NeovimQt {

class Test: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase() {
		QStringList fonts;
		fonts << "third-party/DejaVuSansMono.ttf"
			<< "third-party/DejaVuSansMono-Bold.ttf"
			<< "third-party/DejaVuSansMono-BoldOblique.ttf";
		foreach(QString path, fonts) {
		    QString abs_path_to_font(CMAKE_SOURCE_DIR);
		    abs_path_to_font.append("/").append(path);
		    QFontDatabase::addApplicationFont(abs_path_to_font);
		}
	}

	void benchStart() {
		QBENCHMARK {
			NeovimConnector *c = NeovimConnector::spawn({"-u", "NONE"});
			QSignalSpy onReady(c, SIGNAL(ready()));
			QVERIFY(onReady.isValid());
			QVERIFY(SPYWAIT(onReady));

			Shell *s = new Shell(c, ShellOptions());
			QSignalSpy onResize(s, SIGNAL(neovimResized(int, int)));
			QVERIFY(onResize.isValid());
			QVERIFY(SPYWAIT(onResize));
		}
	}

	void uiStart() {
		QStringList args;
		args << "-u" << "NONE";
		NeovimConnector *c = NeovimConnector::spawn(args);
		Shell *s = new Shell(c, ShellOptions());
		QSignalSpy onAttached(s, SIGNAL(neovimAttached(bool)));
		QVERIFY(onAttached.isValid());
		QVERIFY(SPYWAIT(onAttached));
		QVERIFY(s->neovimAttached());

		s->repaint();

		QPixmap p = s->grab();
		p.save("tst_shell_start.jpg");

	}

	void startVarsShellWidget() {
		QStringList args = {"-u", "NONE"};
		NeovimConnector *c = NeovimConnector::spawn(args);
		Shell *s = new Shell(c, ShellOptions());
		QSignalSpy onAttached(s, SIGNAL(neovimAttached(bool)));
		QVERIFY(onAttached.isValid());
		QVERIFY(SPYWAIT(onAttached));
		QVERIFY(s->neovimAttached());
		checkStartVars(c);
	}

	void startVarsMainWindow() {
		QStringList args = {"-u", "NONE"};
		NeovimConnector *c = NeovimConnector::spawn(args);
		MainWindow *s = new MainWindow(c, ShellOptions());
		s->show();
		QSignalSpy onAttached(s, SIGNAL(neovimAttached(bool)));
		QVERIFY(onAttached.isValid());
		QVERIFY(SPYWAIT(onAttached));
		QVERIFY(s->neovimAttached());
		checkStartVars(c);
	}

	void guiExtTablineSet() {
		QStringList args;
		args << "-u" << "NONE";
		NeovimConnector *c = NeovimConnector::spawn(args);
		Shell *s = new Shell(c, ShellOptions());
		QSignalSpy onOptionSet(s, &Shell::neovimExtTablineSet);
		QVERIFY(onOptionSet.isValid());
		QVERIFY(SPYWAIT(onOptionSet));
	}

	void gviminit() {
		qputenv("GVIMINIT", "let g:test_gviminit = 1");
		QStringList args;
		args << "-u" << "NONE";
		NeovimConnector *c = NeovimConnector::spawn(args);
		Shell *s = new Shell(c, ShellOptions());

		QSignalSpy onAttached(s, SIGNAL(neovimAttached(bool)));
		QVERIFY(onAttached.isValid());
		QVERIFY(SPYWAIT(onAttached));
		QVERIFY(s->neovimAttached());

		auto req = c->api0()->vim_command_output(c->encode("echo g:test_gviminit"));
		QSignalSpy cmd(req, SIGNAL(finished(quint32, quint64, QVariant)));
		QVERIFY(cmd.isValid());
		QVERIFY(SPYWAIT(cmd));
		qDebug() << cmd;

		QCOMPARE(cmd.at(0).at(2).toByteArray(), QByteArray("1"));
	}

	void guiShimCommands() {
		// This function needs to be able to find the GUI runtime
		// plugin or this test WILL FAIL
		QString path_to_src_runtime(CMAKE_SOURCE_DIR);
		path_to_src_runtime.append("/src/gui/runtime");
		QFileInfo fi = QFileInfo(path_to_src_runtime);
		QVERIFY2(fi.exists(), "Unable to find GUI runtime");
		QStringList args = {"-u", "NONE",
			"--cmd", "set rtp+=" + fi.absoluteFilePath()};
		NeovimConnector *c = NeovimConnector::spawn(args);
		MainWindow *s = new MainWindow(c, ShellOptions());
		QSignalSpy onAttached(s, SIGNAL(neovimAttached(bool)));
		QVERIFY(onAttached.isValid());
		QVERIFY(SPYWAIT(onAttached));
		QVERIFY(s->neovimAttached());

		QObject::connect(c->neovimObject(), &NeovimApi1::err_vim_command_output, [](QString msg, const QVariant& err) {
				qDebug() << msg << err;
			});

		QSignalSpy cmd_font(c->neovimObject()->vim_command_output(c->encode("GuiFont")), &MsgpackRequest::finished);
		QVERIFY(cmd_font.isValid());
		QVERIFY2(SPYWAIT(cmd_font), "Waiting for GuiFont");

		QSignalSpy cmd_ls(c->neovimObject()->vim_command_output(c->encode("GuiLinespace")), &MsgpackRequest::finished);
		QVERIFY(cmd_ls.isValid());
		QVERIFY2(SPYWAIT(cmd_ls), "Waiting for GuiLinespace");

		// Test font attributes
#ifdef Q_OS_MAC
		QSignalSpy cmd_gf(c->neovimObject()->vim_command_output(c->encode("GuiFont Monaco:h14")), &MsgpackRequest::finished);
#else
		QSignalSpy cmd_gf(c->neovimObject()->vim_command_output(c->encode("GuiFont DejaVu Sans Mono:h14")), &MsgpackRequest::finished);
#endif
		QVERIFY(cmd_gf.isValid());
		QVERIFY(SPYWAIT(cmd_gf));

		QSignalSpy spy_fontchange(s->shell(), &Shell::fontChanged);

		// Test Performance: timeout occurs often, set value carefully.
		SPYWAIT(spy_fontchange, 2500 /*msec*/);

#ifdef Q_OS_MAC
		QCOMPARE(s->shell()->fontDesc(), QString("Monaco:h14"));
#else
		QCOMPARE(s->shell()->fontDesc(), QString("DejaVu Sans Mono:h14"));
#endif

		// Normalization removes the :b attribute
#ifdef Q_OS_MAC
		QSignalSpy cmd_gf2(c->neovimObject()->vim_command_output(c->encode("GuiFont Monaco:h14:b:l")), &MsgpackRequest::finished);
#else
		QSignalSpy cmd_gf2(c->neovimObject()->vim_command_output(c->encode("GuiFont DejaVu Sans Mono:h14:b:l")), &MsgpackRequest::finished);
#endif
		QVERIFY(cmd_gf2.isValid());
		QVERIFY(SPYWAIT(cmd_gf2));

		QSignalSpy spy_fontchange2(s->shell(), &Shell::fontChanged);

		// Test Performance: timeout occurs often, set value carefully.
		SPYWAIT(spy_fontchange2, 2500 /*msec*/);

#ifdef Q_OS_MAC
		QCOMPARE(s->shell()->fontDesc(), QString("Monaco:h14:l"));
#else
		QCOMPARE(s->shell()->fontDesc(), QString("DejaVu Sans Mono:h14:l"));
#endif

		// GuiTabline
		QSignalSpy onOptionSet(s->shell(), &Shell::neovimExtTablineSet);
		QVERIFY(onOptionSet.isValid());
	}

	void GetClipboard_data() {
		// * or +
		QTest::addColumn<char>("reg");
		// data set in the register when starting test, this is set
		// externaly (i.e. without going through the provider)
		QTest::addColumn<QByteArray>("register_data");

		QTest::newRow("empty *")
			<< '*' << QByteArray();
		QTest::newRow("set *")
			<< '*' << QByteArray("something");
		QTest::newRow("empty +")
			<< '+' << QByteArray();
		QTest::newRow("set +")
			<< '+' << QByteArray("something");
		QTest::newRow("paste *")
			<< '*' << QByteArray("something");
		QTest::newRow("paste +")
			<< '+' << QByteArray("something");
	}

	void GetClipboard() {
		QFETCH(char, reg);
		QFETCH(QByteArray, register_data);

		QString path_to_src_runtime(CMAKE_SOURCE_DIR);
		path_to_src_runtime.append("/src/gui/runtime");
		QFileInfo fi = QFileInfo(path_to_src_runtime);
		QStringList args = {"-u", "NONE",
			"--cmd", "set rtp+=" + fi.absoluteFilePath()};
		NeovimConnector *c = NeovimConnector::spawn(args);
		MainWindow *s = new MainWindow(c, ShellOptions());
		QSignalSpy onAttached(s, SIGNAL(neovimAttached(bool)));
		QVERIFY(onAttached.isValid());
		QVERIFY(SPYWAIT(onAttached));
		QVERIFY(s->neovimAttached());

		QObject::connect(c->neovimObject(), &NeovimApi1::err_vim_command_output, [](QString msg, const QVariant& err) {
				qDebug() << msg << err;
			});

		// provided by the GUI shim
		c->api0()->vim_command(c->encode("call GuiClipboard()"));

#if defined(Q_OS_MAC) || defined(Q_OS_WIN32)
		QGuiApplication::clipboard()->setText(register_data, QClipboard::Clipboard);
#else
		if (reg == '+') {
			QGuiApplication::clipboard()->setText(register_data, QClipboard::Clipboard);
		} else {
			QGuiApplication::clipboard()->setText(register_data, QClipboard::Selection);
		}
#endif

		QString getreg_cmd = QString("getreg('%1')").arg(reg);
		QSignalSpy cmd_clip(c->api1()->nvim_eval(c->encode(getreg_cmd)), &MsgpackRequest::finished);
		QVERIFY(cmd_clip.isValid());
		QVERIFY(SPYWAIT(cmd_clip));
		QCOMPARE(cmd_clip.takeFirst().at(2), QVariant(register_data));
	}

	void SetClipboard_data() {
		// * or +
		QTest::addColumn<char>("reg");
		// data set in the register when starting test, this is set
		// externaly (i.e. without going through the provider)
		QTest::addColumn<QByteArray>("register_data");

		QTest::newRow("empty *")
			<< '*' << QByteArray();
		QTest::newRow("set *")
			<< '*' << QByteArray("something");
		QTest::newRow("empty +")
			<< '+' << QByteArray();
		QTest::newRow("set +")
			<< '+' << QByteArray("something");
		QTest::newRow("paste *")
			<< '*' << QByteArray("something");
		QTest::newRow("paste +")
			<< '+' << QByteArray("something");
	}

	void SetClipboard() {
		QFETCH(char, reg);
		QFETCH(QByteArray, register_data);

		QString path_to_src_runtime(CMAKE_SOURCE_DIR);
		path_to_src_runtime.append("/src/gui/runtime");
		QFileInfo fi = QFileInfo(path_to_src_runtime);
		QStringList args = {"-u", "NONE",
			"--cmd", "set rtp+=" + fi.absoluteFilePath()};
		NeovimConnector *c = NeovimConnector::spawn(args);
		MainWindow *s = new MainWindow(c, ShellOptions());
		QSignalSpy onAttached(s, SIGNAL(neovimAttached(bool)));
		QVERIFY(onAttached.isValid());
		QVERIFY(SPYWAIT(onAttached));
		QVERIFY(s->neovimAttached());

		QObject::connect(c->neovimObject(), &NeovimApi1::err_vim_command_output, [](QString msg, const QVariant& err) {
				qDebug() << msg << err;
			});

		// provided by the GUI shim
		c->api0()->vim_command(c->encode("call GuiClipboard()"));

		QString setreg_cmd = QString("setreg('%1', '%2')\n").arg(reg).arg(QString::fromUtf8(register_data));
		c->neovimObject()->vim_command(c->encode(setreg_cmd));
		QSignalSpy spy_sync(c->neovimObject()->vim_feedkeys("", "", false), &MsgpackRequest::finished);
SPYWAIT(spy_sync);
		QVERIFY(spy_sync.isValid());
		QVERIFY(SPYWAIT(spy_sync));

#if defined(Q_OS_MAC) || defined(Q_OS_WIN32)
		QString data = QGuiApplication::clipboard()->text(QClipboard::Clipboard);
#else
		if (reg == '+') {
			QGuiApplication::clipboard()->text(QClipboard::Clipboard);
		} else {
			QGuiApplication::clipboard()->text(QClipboard::Selection);
		}
#endif

		// the additional \n seems to be a side effect from vim_command_output()
//		QCOMPARE(cmd_clip.takeFirst().at(2), QVariant(register_data + "\n"));
	}

protected:
	/// Check for the presence of the GUI variables in Neovim
	void checkStartVars(NeovimQt::NeovimConnector *conn) {
		auto *nvim = conn->api1();
		connect(nvim, &NeovimQt::NeovimApi1::err_vim_get_var,
			[](const QString& err, const QVariant& v) {
				qDebug() << err<< v;
			});

		QStringList vars = {"GuiWindowId", "GuiWindowMaximized",
			"GuiWindowFullScreen", "GuiFont"};
		foreach(const QString& var, vars) {
			qDebug() << "Checking Neovim for Gui var" << var;
			QSignalSpy onVar(nvim, SIGNAL(on_vim_get_var(QVariant)));
			QVERIFY(onVar.isValid());
			nvim->vim_get_var(conn->encode(var));
			QVERIFY(SPYWAIT(onVar));
		}

		// v:windowid
		QSignalSpy onVarWindowId(nvim, SIGNAL(on_vim_get_vvar(QVariant)));
		QVERIFY(onVarWindowId.isValid());
		nvim->vim_get_vvar(conn->encode("windowid"));
		QVERIFY(SPYWAIT(onVarWindowId));
	}
};

} // Namespace NeovimQt
QTEST_MAIN(NeovimQt::Test)
#include "tst_shell.moc"
