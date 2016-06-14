#ifndef QSHELLWIDGET2_CELL
#define QSHELLWIDGET2_CELL

#include <QColor>
#include <QChar>
#include "konsole_wcwidth.h"

struct Cell {
public:


	inline Cell(QChar c, QColor fgColor, QColor bgColor, QColor spColor, bool bold,
			bool italic, bool underline, bool undercurl)
	:foregroundColor(fgColor), backgroundColor(bgColor), specialColor(spColor),
	bold(bold), italic(italic), underline(underline), undercurl(undercurl),
	valid(true), doubleWidth(false) {
		setChar(c);
	}

	/// Default cells are space characters using invalid colors
	inline Cell()
	:c(' '), foregroundColor(QColor()), backgroundColor(QColor()), specialColor(QColor()),
	bold(false), italic(false), underline(false), undercurl(false),
	valid(true), doubleWidth(false) {}

	inline void reset() {
		c = ' ';
		foregroundColor = QColor();
		backgroundColor = foregroundColor;
		specialColor = foregroundColor;
		bold = italic = underline = undercurl = doubleWidth = false;
		valid = true;
	}


	/// Create an empty Cell with a background color
	static Cell bg(QColor bg) {
		Cell c;
		c.backgroundColor = bg;
		return c;
	}

	inline void setChar(const QChar& chr) {
		c = chr;
		doubleWidth = (konsole_wcwidth(c.unicode()) > 1);
	}

	inline void setLowSurrogate(const QChar& chr) {
		lowSurrogate = chr;
		doubleWidth = (string_width(text()));
	}

	inline QString text() const {
		if (c.isHighSurrogate() && lowSurrogate.isLowSurrogate()) {
			return QString(c) + lowSurrogate;
		} else {
			return c;
		}
	}

	static inline Cell invalid() {
		Cell c = Cell('X', Qt::white, Qt::red, QColor(), false, false, false, false);
		c.valid = false;
		return c;
	}

	QChar c;
	QChar lowSurrogate;
	QColor foregroundColor, backgroundColor, specialColor;
	bool bold, italic, underline, undercurl;
	bool valid;
	bool doubleWidth;
};

// Two cells are equal if all atributes are the same
// except if they are invalid.
inline bool operator==(const Cell& c1, const Cell& c2)
{
	if (!c1.valid || !c2.valid) {
		return false;
	}

	return (c1.c == c2.c
			&& c1.foregroundColor == c2.foregroundColor
			&& c1.backgroundColor == c2.backgroundColor
			&& c1.specialColor == c2.specialColor
			&& c1.bold == c2.bold
			&& c1.italic == c2.italic
			&& c1.underline == c2.underline
			&& c1.undercurl == c2.undercurl
	       );
}

#endif
