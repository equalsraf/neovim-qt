#pragma once

namespace NeovimQt {

constexpr bool cs_defaultIsTablineEnabled{ false };
constexpr bool cs_defaultIsPopupmenuEnabled{ false };
constexpr bool cs_defaultIsLineGridEnabled{ true };

class ShellOptions final {
public:
	bool IsTablineEnabled() const noexcept { return m_isTablineEnabled; }
	bool IsPopupmenuEnabled() const noexcept { return m_isPopupmenuEnabled; }
	bool IsLineGridEnabled() const noexcept { return m_isLineGridEnabled; }

	void SetIsTablineEnabled(bool isEnabled) noexcept { m_isTablineEnabled = isEnabled; }
	void SetIsPopupmenuEnabled(bool isEnabled) noexcept { m_isPopupmenuEnabled = isEnabled; }
	void SetIsLineGridEnabled(bool isEnabled) noexcept { m_isLineGridEnabled = isEnabled; }

private:
	bool m_isTablineEnabled{ cs_defaultIsTablineEnabled };
	bool m_isPopupmenuEnabled{ cs_defaultIsPopupmenuEnabled };
	bool m_isLineGridEnabled{ cs_defaultIsLineGridEnabled };
};

} // namespace NeovimQt
