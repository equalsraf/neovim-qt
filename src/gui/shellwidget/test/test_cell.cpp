#include <QtTest/QtTest>
#include "cell.h"

#if defined(Q_OS_WIN) && defined(USE_STATIC_QT)
#include <QtPlugin>
Q_IMPORT_PLUGIN (QWindowsIntegrationPlugin);
#endif

class Test: public QObject
{
	Q_OBJECT
private slots:
	void cellDefault() {
		Cell c;
		QCOMPARE(c.GetCharacter(), uint(' '));
		// Default colors are invalid
		QCOMPARE(c.GetForegroundColor(), QColor());
		QCOMPARE(c.GetBackgroundColor(), QColor());
		QCOMPARE(c.GetSpecialColor(), QColor());
		QVERIFY(!c.GetForegroundColor().isValid());
		QVERIFY(!c.GetBackgroundColor().isValid());
		QVERIFY(!c.GetSpecialColor().isValid());

		QBENCHMARK {
			Cell c;
		}
	}

	void cellValue() {
		QBENCHMARK {
			Cell c('z', Qt::black, Qt::white, QColor(),
					false, false, false, false);
		}
	}
	void cellValueRgb() {
		QBENCHMARK {
			Cell c('z', QRgb(33), QRgb(66), QColor(),
					false, false, false, false);
		}
	}

	void cellWidth() {
		Cell c;
		QCOMPARE(c.IsDoubleWidth(), false);
		c.SetCharacter(27721);
		QCOMPARE(c.IsDoubleWidth(), true);
	}

	void cellBg() {
		Cell c0;
		Cell c1 = Cell{ QColor::Invalid };
		QCOMPARE(c0, c1);

		Cell c2 = Cell{ QColor{ Qt::red } };
		QCOMPARE(c2.GetBackgroundColor(), QColor{ Qt::red });
	}
};

QTEST_MAIN(Test)
#include "test_cell.moc"
