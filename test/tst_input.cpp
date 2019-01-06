
#include <QtTest/QtTest>
#include <gui/input.h>

class TestInput: public QObject
{
	Q_OBJECT
private slots:
	void specialKeys();
	void modifiersAreNotKeys();
private:
	NeovimQt::InputConv input;
};

void TestInput::modifiersAreNotKeys()
{
	QCOMPARE(input.convertKey("", Qt::Key_Meta, Qt::MetaModifier),
			QString());
	QCOMPARE(input.convertKey("", Qt::Key_Control, Qt::ControlModifier),
			QString());
	QCOMPARE(input.convertKey("", Qt::Key_Alt, Qt::AltModifier),
			QString());
}

void TestInput::specialKeys()
{
	foreach(int k, input.specialKeys.keys()) {
		QCOMPARE(input.convertKey("", k, Qt::NoModifier),
			QString("<%1>").arg(input.specialKeys.value(k)));

#ifdef Q_OS_MAC
		// On Mac Control is actually the Cmd key
		QCOMPARE(input.convertKey("", k, Qt::ControlModifier),
			QString("<D-%1>").arg(input.specialKeys.value(k)));
#else
		QCOMPARE(input.convertKey("", k, Qt::ControlModifier),
			QString("<C-%1>").arg(input.specialKeys.value(k)));
#endif

		QCOMPARE(input.convertKey("", k, Qt::AltModifier),
			QString("<A-%1>").arg(input.specialKeys.value(k)));

#if defined(Q_OS_MAC)
		QCOMPARE(input.convertKey("", k, Qt::MetaModifier),
			// On Mac Meta is actually the Control key
			QString("<C-%1>").arg(input.specialKeys.value(k)));
#elif defined(Q_OS_UNIX)
		QCOMPARE(input.convertKey("", k, Qt::MetaModifier),
			// On UNIX Meta is the Windows key, treated as D-
			QString("<D-%1>").arg(input.specialKeys.value(k)));
#else
		QCOMPARE(input.convertKey("", k, Qt::MetaModifier),
			// Meta is not handled right now
			QString("<%1>").arg(input.specialKeys.value(k)));
#endif
	}
}

QTEST_MAIN(TestInput)
#include "tst_input.moc"
