#pragma once

namespace NeovimQt {

class Tab final
{
public:
	Tab(QString name, uint64_t handle) noexcept
		: m_name{ name }
		, m_handle{ handle }
	{
	}

	const QString& GetName() const noexcept { return m_name; }

	uint64_t GetHandle() const noexcept { return m_handle; }

private:
	const QString m_name;
	const uint64_t m_handle{};
};

} // namespace NeovimQt
