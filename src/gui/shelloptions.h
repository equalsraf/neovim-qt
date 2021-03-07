#pragma once

namespace NeovimQt {

class ShellOptions final {
public:
	bool IsTablineEnabled() const noexcept { return m_isTablineEnabled; }
	bool IsPopupmenuEnabled() const noexcept { return m_isPopupmenuEnabled; }
	bool IsLineGridEnabled() const noexcept { return m_isLineGridEnabled; }
	int GetOptionShowTabline() const noexcept { return m_showtabline; }

	void SetIsTablineEnabled(bool isEnabled) noexcept { m_isTablineEnabled = isEnabled; }
	void SetIsPopupmenuEnabled(bool isEnabled) noexcept { m_isPopupmenuEnabled = isEnabled; }
	void SetIsLineGridEnabled(bool isEnabled) noexcept { m_isLineGridEnabled = isEnabled; }
	void SetOptionShowTabline(int value) noexcept { m_showtabline = value; }

private:
	bool m_isTablineEnabled{ false };
	bool m_isPopupmenuEnabled{ false };
	bool m_isLineGridEnabled{ true };
	int m_showtabline{ 1 /*TwoOrMore*/ };
};

} // namespace NeovimQt
