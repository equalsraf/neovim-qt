#include <QImage>
#include <QPainter>
#include <QDebug>
#include "helpers.h"

/// Save shell contents into a file, returns file on error (see QImage::save)
bool saveShellContents(const ShellContents& s, const QString& filename)
{
	QFont f;
	QFontMetrics fm(f);
	int w = fm.width('W');
	int h = fm.height();

	QImage img( s.columns()*w,
			s.rows()*h,
			QImage::Format_ARGB32_Premultiplied);
	img.fill(Qt::white);

	QPainter p(&img);
	p.setFont(f);
	for (int i=0; i<s.rows(); i++) {
		// Draw guide
		p.setPen(Qt::red);
		p.drawLine(0, i*h, img.width(), i*h);
		for (int j=0; j<s.columns(); j++) {
			QRect r(j*w, i*h, w, h);
			const Cell& cell = s.constValue(i,j);
			p.setPen(cell.GetForegroundColor());
			if (cell.GetBackgroundColor().isValid()) {
				p.fillRect(r, cell.GetBackgroundColor());
			}
			const uint character{ cell.GetCharacter() };
			p.drawText(r, QString::fromUcs4(&character, 1));
		}
	}
	p.end();

	return img.save(filename);
}

/// Check if a font can be safely used as a fixed pitch font
///
/// This function is not perfect and some broken fonts may still return false,
/// or font substitution may cause good fonts to fail. The font max/average
/// metrics are compared with the italic/bold double width variants.
bool isBadMonospace(const QFont& f)
{
	QFont fi(f);
	fi.setItalic(true);
	QFont fb(f);
	fb.setBold(true);
	QFont fbi(fb);
	fbi.setItalic(true);

	QFontMetrics fm_normal(f);
	QFontMetrics fm_italic(fi);
	QFontMetrics fm_boldit(fbi);
	QFontMetrics fm_bold(fb);

	// Regular
	if ( fm_normal.averageCharWidth() != fm_normal.maxWidth() ) {
		QFontInfo info(f);
		qDebug() << f.family()
			<< "Average and Maximum font width mismatch for Regular font; QFont::exactMatch() is" << f.exactMatch()
			<< "Real font is " << info.family() << info.pointSize();
		return true;
	}

	// Italic
	if ( fm_italic.averageCharWidth() != fm_italic.maxWidth() ||
			fm_italic.maxWidth()*2 != fm_italic.width("MM") ) {
		QFontInfo info(fi);
		qDebug() << fi.family() << "Average and Maximum font width mismatch for Italic font; QFont::exactMatch() is" << fi.exactMatch()
			<< "Real font is " << info.family() << info.pointSize();
		return true;
	}

	// Bold
	if ( fm_bold.averageCharWidth() != fm_bold.maxWidth() ||
			fm_bold.maxWidth()*2 != fm_bold.width("MM") ) {
		QFontInfo info(fb);
		qDebug() << fb.family() << "Average and Maximum font width mismatch for Bold font; QFont::exactMatch() is" << fb.exactMatch()
			<< "Real font is " << info.family() << info.pointSize();
		return true;
	}

	// Bold+Italic
	if ( fm_boldit.averageCharWidth() != fm_boldit.maxWidth() ||
			fm_boldit.maxWidth()*2 != fm_boldit.width("MM") ) {
		QFontInfo info(fbi);
		qDebug() << fbi.family() << "Average and Maximum font width mismatch for Bold+Italic font; QFont::exactMatch() is" << fbi.exactMatch()
			<< "Real font is " << info.family() << info.pointSize();
		return true;
	}

	if ( fm_normal.maxWidth() != fm_italic.maxWidth() ||
		fm_normal.maxWidth() != fm_boldit.maxWidth() ||
		fm_normal.maxWidth() != fm_bold.maxWidth()) {
		qDebug() << f.family() << "Average and Maximum font width mismatch between font types";
		return true;
	}

	return false;
}
