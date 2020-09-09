#include "helpers.h"

#include <QDebug>
#include <QImage>
#include <QPainter>

#include "version.h"

template <class T>
static int GetHorizontalAdvanceHelper(const QFontMetrics& fm, const T& text) noexcept
{
#if (QT_VERSION < QT_VERSION_CHECK(5, 11, 0))
	return fm.width(text);
#else
	return fm.horizontalAdvance(text);
#endif
}

// TODO Issue#750: Remove this function, requires Qt 5.11
int GetHorizontalAdvance(const QFontMetrics& fm, QChar character) noexcept
{
	return GetHorizontalAdvanceHelper(fm, character);
}

// TODO Issue#750: Remove this function, requires Qt 5.11
int GetHorizontalAdvance(const QFontMetrics& fm, const QString& text) noexcept
{
	return GetHorizontalAdvanceHelper(fm, text);
}

/// Save shell contents into a file, returns file on error (see QImage::save)
bool saveShellContents(const ShellContents& s, const QString& filename)
{
	QFont f;
	QFontMetrics fm(f);
	int w = GetHorizontalAdvance(fm, 'W');
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
