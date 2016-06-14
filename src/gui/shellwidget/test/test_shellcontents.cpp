#include <QtTest/QtTest>
#include "shellcontents.h"
#include "helpers.h"

class Test: public QObject
{
	Q_OBJECT

public:
	// Initialize a shell with contents
	//     a b c d e ..
	//     a b c d e ..
	//     ...
	ShellContents initShell(int rows, int cols) {
		ShellContents s(rows,cols);
		for (int i=0; i<rows; i++) {
			for (int j=0; j<cols; j++) {
				s.value(i, j).c = QChar('a'+j);
			}
		}
		return s;
	}
	// Check if the contents are the same as returned
	// by initShell
	void checkInitShell(const ShellContents& s) {
		for (int i=0; i<s.rows(); i++) {
			for (int j=0; j<s.columns(); j++) {
				QCOMPARE(s.constValue(i, j).c, QChar('a'+j));
			}
		}
	}
	// True if the shell is clear
	bool checkClear(const ShellContents& s) {
		for (int i=0; i<s.rows(); i++) {
			for (int j=0; j<s.columns(); j++) {
				QChar c = s.constValue(i, j).c;
				if (c != QChar(' ')) {
					return false;
				}
			}
		}
		return true;
	}
	// Create a shell with
	//     a a a a
	//     b b b b
	//     ...
	ShellContents initShellScroll(int rows, int cols) {
		ShellContents s(rows,cols);
		for (int i=0; i<rows; i++) {
			for (int j=0; j<cols; j++) {
				s.value(i, j).c = QChar('a'+i);
			}
		}
		return s;
	}
	bool checkScrollShell(const ShellContents& s) {
		for (int i=0; i<s.rows(); i++) {
			for (int j=0; j<s.columns(); j++) {
				QChar c = s.constValue(i, j).c;
				if (c != QChar('a'+i)) {
					return false;
				}
			}
		}
		return true;
	}

private slots:
	void cellDefault() {
		int rows = 40;
		int cols = 60;
		ShellContents s(rows,cols);
		for (int i=0; i<rows; i++) {
			for (int j=0; j<cols; j++) {
				QCOMPARE(s.value(i, j).c, QChar(' '));
			}
		}

		// invalid cells are initialized as red X
		QCOMPARE(s.value(-1, -1).c, QChar('X'));
		QCOMPARE(s.value(-1, -1).backgroundColor, QColor(Qt::red));

		QBENCHMARK {
			ShellContents s(100,100);
		}
	}

	void resize() {
		int rows = 40;
		int cols = 60;
		ShellContents s = initShell(rows, cols);

		// These should have no effect
		s.resize(-10, -10);
		s.resize(0, 0);

		// resize() changes columns()/rows()
		s.resize(rows-1, cols);
		QCOMPARE(s.rows(), rows-1);
		s.resize(rows-1,cols-1);
		QCOMPARE(s.columns(), cols-1);

		// shrink
		s.resize(20, 20);
		for (int i=0; i<20; i++) {
			for (int j=0; j<20; j++) {
				QCOMPARE(s.value(i, j).c, QChar('a'+j));
			}
		}

		// grow
		s.resize(30, 30);
		for (int i=0; i<20; i++) {
			for (int j=0; j<20; j++) {
				QCOMPARE(s.value(i, j).c, QChar('a'+j));
			}
		}
		for (int i=20; i<30; i++) {
			for (int j=20; j<30; j++) {
				QCOMPARE(s.value(i, j).c, QChar(' '));
			}
		}

	}

	void resizeBench() {
		ShellContents s(100,100);
		QBENCHMARK {
			s.resize(50, 50);
		}
	}

	void clearRow() {
		int rows = 40;
		int cols = 60;
		ShellContents s = initShell(rows, cols);

		s.clearRow(3);
		// These should have no effect
		s.clearRow(-1);
		s.clearRow(55);

		for (int i=0; i<rows; i++) {
			if (i == 3) {
				for (int j=0; j<cols; j++) {
					QCOMPARE(s.value(i, j).c, QChar(' '));
				}
			} else {
				for (int j=0; j<cols; j++) {
					QCOMPARE(s.value(i, j).c, QChar('a'+j));
				}
			}
		}

		ShellContents s1 = initShell(rows, cols);
		// Clear row (starting at col 10)
		s1.clearRow(20, 10);
		for (int i=0; i<rows; i++) {
			for (int j=0; j<cols; j++) {
				if (i == 20 && j >= 10) {
					QCOMPARE(s1.value(i, j).c, QChar(' '));
				} else {
					QCOMPARE(s1.value(i, j).c, QChar('a'+j));
				}
			}
		}
	}

	void clearRowBench() {
		ShellContents s = initShell(100, 100);
		QBENCHMARK {
			s.clearRow(50);
		}
	}

	void clearRegion() {
		int rows = 100;
		int columns = 100;
		ShellContents s = initShell(rows, columns);

		// These should have no effect
		s.clearRegion(200, 0, 10, 10);
		s.clearRegion(0, 200, 10, 10);
		s.clearRegion(0, 0, -10, 10);
		s.clearRegion(0, 0, 10, -10);
		checkInitShell(s);

		s.clearRegion(0, 0, 10, 10);
		for (int i=0; i<10; i++) {
			for (int j=0; j<10; j++) {
				QCOMPARE(s.value(i, j), Cell());
			}
		}
		// Similar to checkInitShell
		for (int i=10; i<rows; i++) {
			for (int j=0; j<columns; j++) {
				Cell c; c.c = 'a'+j;
				QCOMPARE(s.value(i, j), c);
			}
		}

		ShellContents s2 = ShellContents(1, 1);
		QCOMPARE(s2.value(0, 0), Cell());
		// A region with size 0 causes no change
		s2.clearRegion(0, 0, 0, 0, Qt::blue);
		QCOMPARE(s2.value(0, 0), Cell());

		ShellContents s3 = ShellContents(4, 4);
		s3.clearRegion(1, 1, 3, 3, Qt::blue);
		saveShellContents(s3, "clearRegion.jpg");
		QCOMPARE(s3.value(0, 0), Cell());
		QCOMPARE(s3.value(0, 1), Cell());
		QCOMPARE(s3.value(0, 1), Cell());
		QCOMPARE(s3.value(1, 1), Cell::bg(Qt::blue));

		QCOMPARE(s3.value(3, 3), Cell());
	}

	void clearAll() {
		int rows = 26;
		int cols = 100;
		ShellContents s = initShell(rows, cols);
		
		s.clearAll();
		for (int i=0; i<rows; i++) {
			for (int j=0; j<cols; j++) {
				QCOMPARE(s.value(i, j), Cell());
			}
		}
	}

	void scrollRegion() {
		int rows = 100;
		int cols = 100;

		ShellContents s0 = initShell(rows, cols);
		// This does nothing
		s0.scroll(0);
		checkInitShell(s0);

		// This is the same as clear
		s0.scroll(100);
		QVERIFY(checkClear(s0));

		ShellContents s1 = initShell(rows, cols);
		// This is the same as clear (scrolls to bottom)
		s1.scroll(-100);
		QVERIFY(checkClear(s1));

		// Scroll 10 lines up
		ShellContents s2 = initShellScroll(rows,cols);
		s2.scroll(10);
		for (int i=0; i<rows-10; i++) {
			for (int j=0; j<cols; j++) {
				QCOMPARE(s2.value(i, j).c, QChar('a'+i+10));
			}
		}
		for (int i=rows-10; i<rows; i++) {
			for (int j=0; j<cols; j++) {
				// The bottom region is empty
				QCOMPARE(s2.value(i, j).c, QChar(' '));
			}
		}

		// Scroll 10 lines down
		ShellContents s3 = initShellScroll(rows,cols);
		s3.scroll(-10);
		for (int i=10; i<rows; i++) {
			for (int j=0; j<cols; j++) {
				QCOMPARE(s3.value(i, j).c, QChar('a'+i-10));
			}
		}
		for (int i=0; i<10; i++) {
			for (int j=0; j<cols; j++) {
				// The top region is empty
				QCOMPARE(s3.value(i, j).c, QChar(' '));
			}
		}
	}

	void scrollRegion2() {
		int rows = 100;
		int cols = 100;

		ShellContents s0 = initShell(rows, cols);
		s0.clearRow(0);
		s0.clearRow(99);

		// FIXME: is this inclusive or NOT?
		s0.scrollRegion(1, 99, 0, 100, 1);

		for (int j=0; j<cols; j++) {
			QCOMPARE(s0.value(0, j),  Cell());
			QCOMPARE(s0.value(cols-1, j),  Cell());
		}
		s0.scrollRegion(1, 99, 0, 100, -1);
		for (int j=0; j<cols; j++) {
			QCOMPARE(s0.value(0, j),  Cell());
			QCOMPARE(s0.value(cols-1, j),  Cell());
		}
	}

	void put() {
		int rows = 10;
		int cols = 10;

		ShellContents s0(rows, cols);
		s0.put("HelloWorld", 0, 0);

		QCOMPARE(s0.value(0, 0).c, QChar('H'));
		QCOMPARE(s0.value(0, 1).c, QChar('e'));
		QCOMPARE(s0.value(0, 2).c, QChar('l'));
		QCOMPARE(s0.value(0, 3).c, QChar('l'));
		QCOMPARE(s0.value(0, 4).c, QChar('o'));
		QCOMPARE(s0.value(0, 5).c, QChar('W'));
		QCOMPARE(s0.value(0, 6).c, QChar('o'));
		QCOMPARE(s0.value(0, 7).c, QChar('r'));
		QCOMPARE(s0.value(0, 8).c, QChar('l'));
		QCOMPARE(s0.value(0, 9).c, QChar('d'));

		s0.put("HelloWorld", 5, 5);
		saveShellContents(s0, "shell.jpg");
		QCOMPARE(s0.value(5, 0).c, QChar(' '));
		QCOMPARE(s0.value(5, 1).c, QChar(' '));
		QCOMPARE(s0.value(5, 2).c, QChar(' '));
		QCOMPARE(s0.value(5, 3).c, QChar(' '));
		QCOMPARE(s0.value(5, 4).c, QChar(' '));
		QCOMPARE(s0.value(5, 5).c, QChar('H'));
		QCOMPARE(s0.value(5, 6).c, QChar('e'));
		QCOMPARE(s0.value(5, 7).c, QChar('l'));
		QCOMPARE(s0.value(5, 8).c, QChar('l'));
		QCOMPARE(s0.value(5, 9).c, QChar('o'));
	}

	void putSurrogate() {
		int rows = 10;
		int cols = 10;

		ShellContents s0(rows, cols);
		s0.put("🎐", 0, 0);
		QCOMPARE(s0.value(0, 0).text(), QString("🎐"));

		s0.put("🎐", 0, 9);
		QCOMPARE(s0.value(0, 9).text(), QString("🎐"));
	}

	// Grab test cases from ../test/shellcontents
	void cases() {
		QDir dir("../test/shellcontents/");
		QFileInfoList files = dir.entryInfoList();
		foreach(const QFileInfo &fi, files) {
			if (!fi.isFile() || !fi.fileName().startsWith("shell")
					|| fi.suffix() != "txt") {
				continue;
			}
			ShellContents s(0, 0);
			QCOMPARE(s.fromFile(fi.absoluteFilePath()), true);
			
			QString out("test/");
			out.append(fi.fileName().append(".out.jpeg"));
			saveShellContents(s, out);

			// Compare file with previous test output
		
		}
	}
};

QTEST_MAIN(Test)
#include "test_shellcontents.moc"
