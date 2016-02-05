/**
 * Simple font inspector to check out monospace
 * font metrics
 *
 * Usage: testfont <Font Family>
 */
#include <QGuiApplication>
#include <QTextStream>
#include <QFont>
#include <QFontInfo>
#include <QFontMetrics>

QTextStream& qStdOut()
{
	static QTextStream ts(stdout);
	return ts;
}

void printFontInfo(const QFont& f)
{
	QFontInfo info(f);
	qStdOut() << "Font family: " << f.family() << endl;
	qStdOut() << "Exact match: " << info.exactMatch() << endl;
	qStdOut() << "Real family: " << info.family() << endl;
}

void printFontMetrics(const QFont& f)
{
	QFontMetrics fm(f);
	QFont fi(f);
	fi.setItalic(true);
	QFontMetrics fm_italic(fi);
	QFont fb(f);
	fb.setBold(true);
	QFontMetrics fm_bold(fb);
	QFont fbi(fb);
	fbi.setItalic(true);
	QFontMetrics fm_boldit(fbi);

	// Regular
	char err = (fm.averageCharWidth() != fm.maxWidth() ||
			fm.width("MM") != fm.maxWidth()*2) ? '!' : ' ';
	qStdOut() << err;
	qStdOut() << "  (Regular) Average char width: " << fm.averageCharWidth();
	qStdOut() << " Max char width: " << fm.maxWidth();
	qStdOut() << " Width(MM): " << fm.width("MM") << endl;

	// Italic
	err = (fm_italic.averageCharWidth() != fm_italic.maxWidth() ||
			fm_italic.width("MM") != fm_italic.maxWidth()*2 ||
			fm_italic.maxWidth() != fm.maxWidth()) ? '!' : ' ';
	qStdOut() << err;
	qStdOut() << "   (Italic) Average char width: " << fm_italic.averageCharWidth();
	qStdOut() << " Max char width: " << fm_italic.maxWidth();
	qStdOut() << " Width(MM): " << fm_italic.width("MM") << endl;

	// Bold
	err = (fm_bold.averageCharWidth() != fm_bold.maxWidth() ||
			fm_bold.width("MM") != fm_bold.maxWidth()*2 ||
			fm_bold.maxWidth() != fm.maxWidth()) ? '!' : ' ';
	qStdOut() << err;
	qStdOut() << "     (Bold) Average char width: " << fm_bold.averageCharWidth();
	qStdOut() << " Max char width: " << fm_bold.maxWidth();
	qStdOut() << " Width(MM): " << fm_bold.width("MM") << endl;

	// BoldItalic
	err = (fm_boldit.averageCharWidth() != fm_boldit.maxWidth() ||
			fm_boldit.width("MM") != fm_boldit.maxWidth()*2 ||
			fm_boldit.maxWidth() != fm.maxWidth()) ? '!' : ' ';
	qStdOut() << err;
	qStdOut() << "  (Bold+It) Average char width: " << fm_boldit.averageCharWidth();
	qStdOut() << " Max char width: " << fm_boldit.maxWidth();
	qStdOut() << " Width(MM): " << fm_boldit.width("MM") << endl;
}

int main(int argc, char **argv)
{
	QGuiApplication app(argc, argv);

	if (app.arguments().size() < 2) {
		qStdOut() << "Usage: testfont <family>";
		return -1;
	}

	QFont f;
	f.setStyleHint(QFont::TypeWriter, QFont::StyleStrategy(QFont::PreferDefault | QFont::ForceIntegerMetrics));
	f.setFamily(QStringList(app.arguments().mid(1)).join(" "));
	f.setFixedPitch(true);
	f.setKerning(false);

	printFontInfo(f);
	for (int i=10; i<=18; i++) {
		qStdOut() << "Font size: " << i << endl;
		f.setPointSize(i);
		printFontMetrics(f);
	}
	return 0;
}
