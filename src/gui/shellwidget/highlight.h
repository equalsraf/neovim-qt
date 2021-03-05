#pragma once

#include <QColor>
#include <QVariantMap>

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
		bool undercurl,
		bool strikethrough) noexcept :
		m_foreground{ foreground },
		m_background{ background },
		m_special{ special },
		m_reverse{ reverse },
		m_italic{ italic },
		m_bold{ bold },
		m_underline{ underline },
		m_undercurl{ undercurl },
		m_strikethrough{ strikethrough }
	{
	}

	/// Creates a HighlightAttribute from the Neovim-MsgPack-Map format.
	HighlightAttribute(const QVariantMap& map) noexcept;

	/// Creates safe object with default highlight/style, required for QMap.
	HighlightAttribute() noexcept {};

	QColor GetForegroundColor() const noexcept;

	QColor GetBackgroundColor() const noexcept;

	QColor GetSpecialColor() const noexcept { return m_special; }

	bool IsReverse() const noexcept { return m_reverse; }

	bool IsItalic() const noexcept { return m_italic; }

	bool IsBold() const noexcept { return m_bold; }

	bool IsUnderline() const noexcept { return m_underline; }

	bool IsUndercurl() const noexcept { return m_undercurl; }

	bool IsStrikeThrough() const noexcept { return m_strikethrough; }

	bool operator==(const HighlightAttribute& other) const noexcept;

private:
	QColor m_foreground{ QColor::Invalid };
	QColor m_background{ QColor::Invalid };
	QColor m_special{ QColor::Invalid };

	bool m_reverse{ false };
	bool m_italic{ false };
	bool m_bold{ false };
	bool m_underline{ false };
	bool m_undercurl{ false };
	bool m_strikethrough{ false };
};
