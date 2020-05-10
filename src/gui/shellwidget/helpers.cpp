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
