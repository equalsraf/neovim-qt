#include <QtTest/QtTest>
#include "cell.h"

class Test: public QObject
{
	Q_OBJECT
private slots:
	void cellDefault() {
		Cell c;
		QCOMPARE(c.c, QChar(' '));
		// Default colors are invalid
		QCOMPARE(c.foregroundColor, QColor());
		QCOMPARE(c.backgroundColor, QColor());
		QCOMPARE(c.specialColor, QColor());
		QVERIFY(!c.foregroundColor.isValid());
		QVERIFY(!c.backgroundColor.isValid());
		QVERIFY(!c.specialColor.isValid());

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
		QCOMPARE(c.doubleWidth, false);
		c.setChar(QChar(27721));
		QCOMPARE(c.doubleWidth, true);
	}

	void cellBg() {
		Cell c0;
		Cell c1 = Cell::bg(QColor());
		QCOMPARE(c0, c1);

		Cell c2 = Cell::bg(Qt::red);
		QCOMPARE(c2.backgroundColor, QColor(Qt::red));
	}

	void cellSurrogate() {
		Cell c0;
		QCOMPARE(c0.lowSurrogate, QChar());
		QCOMPARE(c0.lowSurrogate.isLowSurrogate(), false);

		Cell c1;
		c1.c = QChar(55356);
		c1.setLowSurrogate(QChar(57232));
		QCOMPARE(c1.text(), QString("🎐"));
		QCOMPARE(c1.doubleWidth, true);
	}
};

QTEST_MAIN(Test)
#include "test_cell.moc"
