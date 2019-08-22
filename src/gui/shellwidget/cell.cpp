#include "cell.h"

Cell::Cell(
	uint character,
	QColor fgColor,
	QColor bgColor,
	QColor spColor,
	bool bold,
	bool italic,
	bool underline,
	bool undercurl)
	: m_highlight{ fgColor, bgColor, spColor, false /*reverse*/,
		italic, bold, underline, undercurl }
{
	SetCharacter(character);
};

/*static*/ Cell Cell::MakeInvalidCell()
{
	Cell invalidCell;
	invalidCell.m_character = 'X';
	invalidCell.m_isValid = false;
	invalidCell.m_highlight = { Qt::white, Qt::red, QColor::Invalid,
		false, false, false, false, false };

	return invalidCell;
}

void Cell::SetCharacter(uint character)
{
	m_character = character;
	m_isDoubleWidth = konsole_wcwidth(character) > 1;
}

bool Cell::operator==(const Cell& other) const
{
	if (!m_isValid || !other.m_isValid) {
		return false;
	}

	return m_character == other.m_character &&
		m_isDoubleWidth == other.m_isDoubleWidth &&
		m_highlight == other.m_highlight;
}
