#include <QtGlobal>
#include <QProcess>
#include <QtTest/QtTest>

namespace NeovimQt {

class Test: public QObject
{
	Q_OBJECT
private slots:

	void runsWithoutDISPLAY_data() {
		QTest::addColumn<QStringList>("arguments");
		QTest::addColumn<int>("expected_exitcode");

		QTest::newRow("--help") << QStringList({NVIM_QT_BINARY, "--help"}) << 0;
		QTest::newRow("--version") << QStringList({NVIM_QT_BINARY, "--help"}) << 0;
	}

	void runsWithoutDISPLAY() {
		QFETCH(QStringList, arguments);
		QFETCH(int, expected_exitcode);

		QProcess p;
		QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
		env.remove("DISPLAY");
		p.setProcessEnvironment(env);
		p.setProgram(NVIM_QT_BINARY);
		p.setArguments(arguments);
		p.start();
		bool finished = p.waitForFinished();
		auto status = p.exitStatus();
		int exit_code = p.exitCode();

		qDebug() << finished << status << exit_code;
 		QCOMPARE(finished, true);
 		QCOMPARE(status, QProcess::NormalExit);
		QCOMPARE(exit_code, expected_exitcode);
	}

protected:
};

} // Namespace NeovimQt
QTEST_MAIN(NeovimQt::Test)
#include "tst_main.moc"
