#include <QtTest/QtTest>
#include <gui/input.h>

class TestInput: public QObject
{
	Q_OBJECT

private slots:
	void TestSpecialKeys();
	void TestModifierKeys();
};

void TestInput::TestModifierKeys()
{
	QCOMPARE(NeovimQt::Input::convertKey({}, Qt::Key_Meta, Qt::MetaModifier), {});
	QCOMPARE(NeovimQt::Input::convertKey({}, Qt::Key_Control, Qt::ControlModifier), {});
	QCOMPARE(NeovimQt::Input::convertKey({}, Qt::Key_Alt, Qt::AltModifier), {});
}

void TestInput::TestSpecialKeys()
{
	const QMap<int, QString>& specialKeys { NeovimQt::Input::GetSpecialKeysMap() };

	for (const auto& k : specialKeys.keys()) {
		QCOMPARE(NeovimQt::Input::convertKey({}, k, Qt::NoModifier),
			QString("<%1>").arg(specialKeys.value(k)));

		// On Mac Control is actually the Cmd key
		QCOMPARE(NeovimQt::Input::convertKey({}, k, Qt::ControlModifier),
			QString("<D-%1>").arg(specialKeys.value(k)));

		QCOMPARE(NeovimQt::Input::convertKey({}, k, Qt::AltModifier),
			QString("<A-%1>").arg(specialKeys.value(k)));

		// On Mac Meta is the Control key, treated as C-.
		QCOMPARE(NeovimQt::Input::convertKey({}, k, Qt::MetaModifier),
			QString("<C-%1>").arg(specialKeys.value(k)));
	}
}

QTEST_MAIN(TestInput)
#include "tst_input_mac.moc"
