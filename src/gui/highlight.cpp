#include "highlight.h"

namespace NeovimQt {

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

} // namespace NeovimQt
