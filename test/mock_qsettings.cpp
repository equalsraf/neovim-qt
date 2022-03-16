#include "mock_qsettings.h"

#include <QIODevice>

namespace NeovimQt { namespace MockQSettings {

static QSettings::SettingsMap s_mockSettingsMap; // clazy:exclude=non-pod-global-static

static bool read(QIODevice& /*device*/, QSettings::SettingsMap& map) noexcept
{
	map = s_mockSettingsMap;
	return true;
}

static bool write(QIODevice& device, const QSettings::SettingsMap& map) noexcept
{
	// QSettings will not call readMockQSettings without content in `device`.
	// https://stackoverflow.com/questions/41495765/cannot-get-qsettings-to-read-from-custom-storage-format
	device.write("X", 1);

	s_mockSettingsMap = map;
	return true;
}

void EnableByDefault() noexcept
{
	static const QSettings::Format s_mockFormat{ QSettings::registerFormat("mock", read, write) };
	QSettings::setDefaultFormat(s_mockFormat);
}

void ClearAllContents() noexcept
{
	s_mockSettingsMap = {};
}

void OverwriteContents(QSettings::SettingsMap newValue) noexcept
{
	s_mockSettingsMap = std::move(newValue);
}

}} // namespace NeovimQt::MockQSettings
