#ifndef NEOVIM_QT_HIGHLIGHT
#define NEOVIM_QT_HIGHLIGHT

#include <QColor>

namespace NeovimQt {

class HighlightAttribute {
public:
	HighlightAttribute(
		QColor foreground,
		QColor background,
		QColor special,
		bool reverse,
		bool italic,
		bool bold,
		bool underline,
		bool undercurl) :
		m_foreground{ foreground },
		m_background{ background },
		m_special{ special },
		m_reverse{ reverse },
		m_italic{ italic },
		m_bold{ bold },
		m_underline{ underline },
		m_undercurl{ undercurl }
	{
	}

	/// Creates safe object with default highlight/style, required for QMap.
	HighlightAttribute() {};

	QColor GetForegroundColor() const;

	QColor GetBackgroundColor() const;

	QColor GetSpecialColor() const { return m_special; }

	bool IsReverse() const { return m_reverse; }

	bool IsItalic() const { return m_italic; }

	bool IsBold() const { return m_bold; }

	bool IsUnderline() const { return m_underline; }

	bool IsUndercurl() const { return m_undercurl; }

private:
	QColor m_foreground{ QColor::Invalid };
	QColor m_background{ QColor::Invalid };
	QColor m_special{ QColor::Invalid };

	bool m_reverse{ false };
	bool m_italic{ false };
	bool m_bold{ false };
	bool m_underline{ false };
	bool m_undercurl{ false };
};

} // namespace NeovimQt

#endif // NEOVIM_QT_HIGHLIGHT
