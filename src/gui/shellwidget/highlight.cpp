#include "highlight.h"

static QColor GetQColorFromVariant(const QVariant& color) noexcept
{
	if (color.isNull()) {
		return QColor{};
	}

	if (!color.canConvert<uint32_t>()) {
		return QColor{};
	}

	return QColor{ color.toUInt() };
}

HighlightAttribute::HighlightAttribute(const QVariantMap& rgb_attr) noexcept
{
	m_foreground = GetQColorFromVariant(rgb_attr.value("foreground"));
	m_background = GetQColorFromVariant(rgb_attr.value("background"));
	m_special = GetQColorFromVariant(rgb_attr.value("special"));

	m_reverse = rgb_attr.contains("reverse");
	m_italic = rgb_attr.contains("italic");
	m_bold = rgb_attr.contains("bold");
	m_underline = rgb_attr.contains("underline");
	m_undercurl = rgb_attr.contains("undercurl");
	m_strikethrough = rgb_attr.contains("strikethrough");
}

QColor HighlightAttribute::GetForegroundColor() const noexcept {
	if (IsReverse()) {
		return m_background;
	}

	return m_foreground;
}

QColor HighlightAttribute::GetBackgroundColor() const noexcept {
	if (IsReverse()) {
		return m_foreground;
	}

	return m_background;
}

bool HighlightAttribute::operator==(const HighlightAttribute& other) const noexcept {
	return
		m_foreground == other.m_foreground &&
		m_background == other.m_background &&
		m_special == other.m_special &&
		m_reverse == other.m_reverse &&
		m_italic == other.m_italic &&
		m_bold == other.m_bold &&
		m_underline == other.m_underline &&
		m_undercurl == other.m_undercurl &&
		m_strikethrough == other.m_strikethrough;
}
