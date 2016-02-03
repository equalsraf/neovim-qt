
#include <QtTest/QtTest>
#include <gui/input.h>

class TestInput: public QObject
{
	Q_OBJECT
private slots:
	void specialKeys();
private:
	NeovimQt::InputConv input;
};

void TestInput::specialKeys()
{
	foreach(int k, input.specialKeys.keys()) {
		QCOMPARE(input.convertKey("", k, Qt::NoModifier),
			QString("<%1>").arg(input.specialKeys.value(k)));

		QCOMPARE(input.convertKey("", k, Qt::ControlModifier),
#ifdef Q_OS_MAC
			// On Mac Control is actually the Cmd key, which we
			// don't support yet
			QString("<D-%1>").arg(input.specialKeys.value(k)));
#else
			QString("<C-%1>").arg(input.specialKeys.value(k)));
#endif

		QCOMPARE(input.convertKey("", k, Qt::AltModifier),
			QString("<A-%1>").arg(input.specialKeys.value(k)));

		QCOMPARE(input.convertKey("", k, Qt::MetaModifier),
#ifdef Q_OS_MAC
			// On Mac Meta is actually the Control key
			QString("<C-%1>").arg(input.specialKeys.value(k)));
#else
			// Meta is not handled right now
			QString("<%1>").arg(input.specialKeys.value(k)));
#endif
	}
}

QTEST_MAIN(TestInput)
#include "tst_input.moc"
