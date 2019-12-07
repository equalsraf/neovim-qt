#include <QtTest/QtTest>

#include <gui/input.h>

class TestInputWin32 : public QObject
{
	Q_OBJECT

private slots:
	void SpecialKeys() noexcept;
};

void TestInputWin32::SpecialKeys() noexcept
{
	const QMap<int, QString>& specialKeys { NeovimQt::Input::GetSpecialKeysMap() };

	for (const auto k : specialKeys.keys()) {
		// On Mac Meta is the Control key, treated as C-.
		QList<QPair<QKeyEvent, QString>> keyEventList{
			{ { QEvent::KeyPress, k, Qt::NoModifier, {} },      "<%1>" },
			{ { QEvent::KeyPress, k, Qt::ControlModifier, {} }, "<C-%1>" },
			{ { QEvent::KeyPress, k, Qt::AltModifier, {} },     "<A-%1>" },
			{ { QEvent::KeyPress, k, Qt::MetaModifier, {} },    "<%1>" },
		};

		for (const auto& keyEvent : keyEventList) {
			QCOMPARE(NeovimQt::Input::convertKey(keyEvent.first),
				keyEvent.second.arg(specialKeys.value(k)));
		}
	}
}

#include "tst_input_win32.moc"
QTEST_MAIN(TestInputWin32)
