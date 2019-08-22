#pragma once

#include "highlight.h"
#include "konsole_wcwidth.h"

#include <QColor>

class Cell {
public:
	Cell(
		uint character,
		QColor fgColor,
		QColor bgColor,
		QColor spColor,
		bool bold,
		bool italic,
		bool underline,
		bool undercurl);

	/// Create an empty Cell with a background color
	Cell(QColor bgColor)
		: m_highlight{ QColor::Invalid, bgColor, QColor::Invalid,
			false, false, false, false, false }
	{
	}

	/// Default cells are space characters using invalid/default colors
	Cell() = default;

	/// Create a cell marked as invalid
	static Cell MakeInvalidCell();

	void SetCharacter(uint character);

	uint GetCharacter() const { return m_character; };

	bool IsDoubleWidth() const { return m_isDoubleWidth; };

	QColor GetForegroundColor() const { return m_highlight.GetForegroundColor(); }

	QColor GetBackgroundColor() const { return m_highlight.GetBackgroundColor(); }

	QColor GetSpecialColor() const { return m_highlight.GetSpecialColor(); }

	bool IsReverse() const { return m_highlight.IsReverse(); }

	bool IsItalic() const { return m_highlight.IsItalic(); }

	bool IsBold() const { return m_highlight.IsBold(); }

	bool IsUnderline() const { return m_highlight.IsUnderline(); }

	bool IsUndercurl() const { return m_highlight.IsUndercurl(); }

	/// Two cells are equal if both are valid, and all attributes are the same
	bool operator==(const Cell& other) const;

private:
	uint m_character{ ' ' };
	bool m_isValid{ true };
	bool m_isDoubleWidth{ false };

	HighlightAttribute m_highlight;
};
