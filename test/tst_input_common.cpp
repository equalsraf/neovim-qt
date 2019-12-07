#include <QtTest/QtTest>

#include <gui/input.h>

class TestInputCommon : public QObject
{
	Q_OBJECT

private slots:
	void ModifierOnlyKeyEventsIgnored() noexcept;
};

void TestInputCommon::ModifierOnlyKeyEventsIgnored() noexcept
{
	QKeyEvent meta{ QEvent::KeyPress, Qt::Key_Meta, Qt::MetaModifier, {} };
	QCOMPARE(NeovimQt::Input::convertKey(meta), QString{});

	QKeyEvent ctrl{ QEvent::KeyPress, Qt::Key_Control, Qt::ControlModifier, {} };
	QCOMPARE(NeovimQt::Input::convertKey(ctrl), QString{});

	QKeyEvent alt{ QEvent::KeyPress, Qt::Key_Alt, Qt::AltModifier, {} };
	QCOMPARE(NeovimQt::Input::convertKey(alt), QString{});

	const QList<Qt::Key> modifierKeyList {
		Qt::Key::Key_Super_L,
		Qt::Key::Key_Super_R,
		Qt::Key::Key_Control,
		Qt::Key::Key_Meta,
		Qt::Key::Key_Alt,
		Qt::Key::Key_Shift,
	};

	const QList<Qt::KeyboardModifier> keyboardModifierList {
		Qt::KeyboardModifier::NoModifier,
		Qt::KeyboardModifier::ShiftModifier,
		Qt::KeyboardModifier::ControlModifier,
		Qt::KeyboardModifier::AltModifier,
		Qt::KeyboardModifier::MetaModifier,
	};

	for (const auto& key : modifierKeyList) {
		for (const auto& mod : keyboardModifierList) {
			QKeyEvent keyEvent{ QEvent::KeyPress, key, mod };
			QCOMPARE(NeovimQt::Input::convertKey(keyEvent), QString{});
		}
	}

	// Issue#344: <C-S-> appears as <C-Space>
	QKeyEvent evIssue344{ QEvent::KeyPress, Qt::Key::Key_Control, Qt::KeyboardModifier::ShiftModifier};
	QCOMPARE(NeovimQt::Input::convertKey(evIssue344), QString{});

	// Issue#593: Pressing Control + Super inserts ^S
	QKeyEvent evIssue593{ QEvent::KeyPress, Qt::Key::Key_Super_L, Qt::KeyboardModifier::ControlModifier};
	QCOMPARE(NeovimQt::Input::convertKey(evIssue593), QString{});
}

#include "tst_input_common.moc"
QTEST_MAIN(TestInputCommon)
