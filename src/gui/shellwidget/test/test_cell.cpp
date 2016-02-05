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
		QVERIFY(!c.foregroundColor.isValid());
		QVERIFY(!c.backgroundColor.isValid());

		QBENCHMARK {
			Cell c;
		}
	}

	void cellValue() {
		QBENCHMARK {
			Cell c('z', Qt::black, Qt::white,
					false, false, false, false);
		}
	}
	void cellValueRgb() {
		QBENCHMARK {
			Cell c('z', QRgb(33), QRgb(66),
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
};

QTEST_MAIN(Test)
#include "test_cell.moc"
