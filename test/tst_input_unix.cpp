#include <QtTest/QtTest>

#include <gui/input.h>

class TestInputUnix : public QObject
{
	Q_OBJECT

private slots:
	void LessThanModifierKeys() noexcept;
	void SpecialKeys() noexcept;
};

void TestInputUnix::LessThanModifierKeys() noexcept
{
	QKeyEvent evLessThanControl{ QEvent::KeyPress, Qt::Key::Key_Less, Qt::ShiftModifier | Qt::ControlModifier, "<" };
	QCOMPARE(NeovimQt::Input::convertKey(evLessThanControl), QString{ "<C-lt>" });

	QKeyEvent evLessThanAlt{ QEvent::KeyPress, Qt::Key::Key_Less, Qt::ShiftModifier | Qt::AltModifier, "<" };
	QCOMPARE(NeovimQt::Input::convertKey(evLessThanAlt), QString{ "<A-lt>" });

	QKeyEvent evLessThanMeta{ QEvent::KeyPress, Qt::Key::Key_Less, Qt::ShiftModifier | Qt::MetaModifier, "<" };
	QCOMPARE(NeovimQt::Input::convertKey(evLessThanMeta), QString{ "<D-lt>" });
}

void TestInputUnix::SpecialKeys() noexcept
{
	const QMap<int, QString>& specialKeys { NeovimQt::Input::GetSpecialKeysMap() };

	for (const auto k : specialKeys.keys()) {
		// On Mac Meta is the Control key, treated as C-.
		QList<QPair<QKeyEvent, QString>> keyEventList{
			{ { QEvent::KeyPress, k, Qt::NoModifier, {} },      "<%1>" },
			{ { QEvent::KeyPress, k, Qt::ControlModifier, {} }, "<C-%1>" },
			{ { QEvent::KeyPress, k, Qt::AltModifier, {} },     "<A-%1>" },
			{ { QEvent::KeyPress, k, Qt::MetaModifier, {} },    "<D-%1>" },
		};

		for (const auto& keyEvent : keyEventList) {
			QCOMPARE(NeovimQt::Input::convertKey(keyEvent.first),
				keyEvent.second.arg(specialKeys.value(k)));
		}
	}
}

#include "tst_input_unix.moc"
QTEST_MAIN(TestInputUnix)
