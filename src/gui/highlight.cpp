#include "highlight.h"

namespace NeovimQt {

QColor HighlightAttribute::GetForegroundColor() const {
	if (m_foreground == QColor::Invalid) {
		return m_defaultForeground;
	}

	if (IsReverse()) {
		return m_background;
	}

	return m_foreground;
}

QColor HighlightAttribute::GetBackgroundColor() const {
	if (m_background == QColor::Invalid) {
		return m_defaultBackground;
	}

	if (IsReverse()) {
		return m_foreground;
	}

	return m_background;
}

QColor HighlightAttribute::GetSpecialColor() const {
	if (m_special == QColor::Invalid) {
		return m_defaultSpecial;
	}

	return m_special;
}

/*static*/ QColor HighlightAttribute::m_defaultForeground{ Qt::black };

/*static*/ QColor HighlightAttribute::m_defaultBackground{ Qt::white };

/*static*/ QColor HighlightAttribute::m_defaultSpecial{ Qt::black };

} // namespace NeovimQt
