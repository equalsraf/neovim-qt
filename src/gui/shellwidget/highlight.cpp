#include "highlight.h"

QColor HighlightAttribute::GetForegroundColor() const {
	if (IsReverse()) {
		return m_background;
	}

	return m_foreground;
}

QColor HighlightAttribute::GetBackgroundColor() const {
	if (IsReverse()) {
		return m_foreground;
	}

	return m_background;
}

bool HighlightAttribute::operator==(const HighlightAttribute& other) const {
	return
		m_foreground == other.m_foreground &&
		m_background == other.m_background &&
		m_special == other.m_special &&
		m_reverse == other.m_reverse &&
		m_italic == other.m_italic &&
		m_bold == other.m_bold &&
		m_underline == other.m_underline &&
		m_undercurl == other.m_undercurl;
}
