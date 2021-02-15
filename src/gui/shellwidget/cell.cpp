#include "cell.h"

/*static*/ Cell Cell::MakeInvalidCell()
{
	Cell invalidCell;
	invalidCell.m_character = 'X';
	invalidCell.m_isValid = false;
	invalidCell.m_highlight = { Qt::white, Qt::red, QColor::Invalid,
		false, false, false, false, false, false };

	return invalidCell;
}

void Cell::SetCharacter(uint character)
{
	m_character = character;
	m_isDoubleWidth = konsole_wcwidth(character) > 1;
}

bool Cell::IsStyleEquivalent(const Cell& other) const
{
	if (!m_isValid || !other.m_isValid) {
		return false;
	}

	if (IsDoubleWidth() != other.IsDoubleWidth()) {
		return false;
	}

	return m_highlight == other.m_highlight;
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
