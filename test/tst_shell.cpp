#include <QtTest/QtTest>
#include <QLocalSocket>
#include <QFontDatabase>
#include <gui/mainwindow.h>
#include <msgpackrequest.h>
#include "common.h"

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
			QFontDatabase::addApplicationFont(path);
		}
	}

	void benchStart() {
		QBENCHMARK {
			NeovimConnector *c = NeovimConnector::spawn({"-u", "NORC"});
			QSignalSpy onReady(c, SIGNAL(ready()));
			QVERIFY(onReady.isValid());
			QVERIFY(SPYWAIT(onReady));

			Shell *s = new Shell(c);
			QSignalSpy onResize(s, SIGNAL(neovimResized(int, int)));
			QVERIFY(onResize.isValid());
			QVERIFY(SPYWAIT(onResize));
		}
	}

	void uiStart() {
		QStringList args;
		args << "-u" << "NONE";
		NeovimConnector *c = NeovimConnector::spawn(args);
		Shell *s = new Shell(c);
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
		Shell *s = new Shell(c);
		QSignalSpy onAttached(s, SIGNAL(neovimAttached(bool)));
		QVERIFY(onAttached.isValid());
		QVERIFY(SPYWAIT(onAttached));
		QVERIFY(s->neovimAttached());
		checkStartVars(c);
	}

	void startVarsMainWindow() {
		QStringList args = {"-u", "NONE"};
		NeovimConnector *c = NeovimConnector::spawn(args);
		MainWindow *s = new MainWindow(c);
		QSignalSpy onAttached(s, SIGNAL(neovimAttached(bool)));
		QVERIFY(onAttached.isValid());
		QVERIFY(SPYWAIT(onAttached));
		QVERIFY(s->neovimAttached());
		checkStartVars(c);
	}

	void guiShimCommands() {
		// This function needs to be able to find the GUI runtime
		// plugin or this test WILL FAIL
		QFileInfo fi = QFileInfo("src/gui/runtime");
		QVERIFY2(fi.exists(), "Unable to find GUI runtime");
		QStringList args = {"-u", "NONE",
			"--cmd", "set rtp+=" + fi.absoluteFilePath()};
		NeovimConnector *c = NeovimConnector::spawn(args);
		MainWindow *s = new MainWindow(c);
		QSignalSpy onAttached(s, SIGNAL(neovimAttached(bool)));
		QVERIFY(onAttached.isValid());
		QVERIFY(SPYWAIT(onAttached));
		QVERIFY(s->neovimAttached());

		checkCommand(c, "GuiFont");
		checkCommand(c, "GuiLinespace");

		// Test font attributes
#ifdef Q_OS_MAC
		checkCommand(c, "GuiFont Monaco:h14", false);
		QCOMPARE(s->shell()->fontDesc(), QString("Monaco:h14"));
#else
		checkCommand(c, "GuiFont DejaVu Sans Mono:h14", false);
		QCOMPARE(s->shell()->fontDesc(), QString("DejaVu Sans Mono:h14"));
#endif

		// Normalization removes the :b attribute
#ifdef Q_OS_MAC
		checkCommand(c, "GuiFont Monaco:h14:b:l", false);
		QCOMPARE(s->shell()->fontDesc(), QString("Monaco:h14:l"));
#else
		checkCommand(c, "GuiFont DejaVu Sans Mono:h14:b:l", false);
		QCOMPARE(s->shell()->fontDesc(), QString("DejaVu Sans Mono:h14:l"));
#endif
	}

protected:

	void checkCommand(NeovimConnector *c, const QString& cmd,
			bool output=false) {
		auto req = c->neovimObject()->vim_command_output(c->encode(cmd));
		QSignalSpy cmdOk(req, SIGNAL(finished(quint32, quint64, QVariant)));
		QVERIFY(cmdOk.isValid());
		QObject::connect(c->neovimObject(), &NeovimApi1::err_vim_command_output, [cmd](QString msg, const QVariant& err) {
				qDebug() << cmd << msg;
			});
		qDebug() << cmd;
		// Force the Neovim event loop to advance with async vim_input()
		c->neovimObject()->vim_input("<cr>");
		QVERIFY(SPYWAIT(cmdOk));
		qDebug() << cmdOk << cmdOk.size();

		if (output) {
			// Make sure the command output is not empty
			QVERIFY(!cmdOk.at(0).at(2).toByteArray().isEmpty());
		}
	}

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
	}
};

} // Namespace NeovimQt
QTEST_MAIN(NeovimQt::Test)
#include "tst_shell.moc"
