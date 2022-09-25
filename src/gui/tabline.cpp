#include "tabline.h"

#include <mutex>
#include <QFileIconProvider>
#include <QFileInfo>
#include <QLayout>
#include <QMessageBox>
#include <QPointer>
#include <QRegularExpression>
#include <QSettings>
#include <QVariant>

#include "msgpackrequest.h"

static constexpr auto cs_showBuffersOptionName { "Tabline/OptionShowBuffers" };
static constexpr auto cs_showTablineOptionName { "ext_tabline" };

namespace NeovimQt {

Tabline::Tabline(NeovimConnector& nvim, QWidget* parent) noexcept
	: m_nvim{ nvim }
	, m_tabline{ this }
	, m_bufferline{ this }
	, m_spacer{ this }
{
	setAllowedAreas(Qt::TopToolBarArea);
	setContextMenuPolicy(Qt::PreventContextMenu);
	setFloatable(false);
	setMovable(false);
	setObjectName("GuiTabline");

	// Avoid margins around the QTabBar
	layout()->setContentsMargins(0, 0, 0, 0);

	auto InititializeQTabBar = [](QTabBar& initTabBar) noexcept
	{
		initTabBar.setDrawBase(false);
		initTabBar.setExpanding(false);
		initTabBar.setDocumentMode(true);
		initTabBar.setTabsClosable(true);
		initTabBar.setFocusPolicy(Qt::NoFocus);
	};

	InititializeQTabBar(m_tabline);
	InititializeQTabBar(m_bufferline);

	// Spacer between Tabs + Buffers
	m_spacer.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

	m_tablineAction = addWidget(&m_tabline);
	m_spacerAction = addWidget(&m_spacer);
	m_bufferlineAction = addWidget(&m_bufferline);

	connect(&m_nvim, &NeovimConnector::ready, this, &Tabline::neovimConnectorReady);
	connect(&m_tabline, &QTabBar::currentChanged, this, &Tabline::currentChangedTabline);
	connect(&m_tabline, &QTabBar::tabCloseRequested, this, &Tabline::closeRequestedTabline);
	connect(&m_bufferline, &QTabBar::currentChanged, this, &Tabline::currentChangedBufline);
	connect(&m_bufferline, &QTabBar::tabCloseRequested, this, &Tabline::closeRequestedBufline);

	QSettings settings;
	m_isEnabled = settings.value(cs_showTablineOptionName, cs_defaultIsTablineEnabled).toBool();
	m_isBufferlineEnabled = settings.value(cs_showBuffersOptionName, true).toBool();
	updateTablineVisibility();
}

void Tabline::neovimConnectorReady() noexcept
{
	connect(
		m_nvim.api0(), &NeovimApi0::neovimNotification, this, &Tabline::handleNeovimNotification);
	m_nvim.api0()->vim_subscribe("Gui");
}

void Tabline::handleNeovimNotification(const QByteArray& name, const QVariantList& args) noexcept
{
	if (name == "Gui") {
		handleGuiOption(args);
		return;
	}

	if (name == "redraw") {
		Shell::DispatchRedrawNotifications(this, args);
		return;
	}
}

void Tabline::handleRedraw(const QByteArray& name, const QVariantList& args) noexcept
{
	if (name == "tabline_update") {
		handleTablineUpdate(args);
		return;
	}

	if (name == "option_set") {
		handleOptionShowTabline(args);
	}
}

void Tabline::handleGuiOption(const QVariantList& args) noexcept
{
	if (args.size() < 2 || !args.at(0).canConvert<QString>() || !args.at(1).canConvert<QString>()) {
		return;
	}

	const QString guiEventName{ args.at(0).toString() };

	if (guiEventName != "Option") {
		return;
	}

	const QString option{ args.at(1).toString() };

	if (option == "Tabline") {
		handleGuiTabline(args);
		return;
	}
	if (option == "TablineBuffers") {
		handleGuiTablineBuffers(args);
		return;
	}
}

void Tabline::handleGuiTabline(const QVariantList& args) noexcept
{
	if (args.size() < 3 || !args.at(2).canConvert<bool>()) {
		qWarning() << "Unexpected format for GuiTabline:" << args;
		return;
	}

	const bool isEnabled{ args.at(2).toBool() };

	m_isEnabled = isEnabled;

	QSettings settings;
	settings.setValue(cs_showTablineOptionName, isEnabled);

	if (m_nvim.api1()) {
		m_nvim.api1()->nvim_ui_set_option(cs_showTablineOptionName, isEnabled);
	}

	updateTablineVisibility();
}


void Tabline::handleGuiTablineBuffers(const QVariantList& args) noexcept
{
	if (args.size() < 3 || !args.at(2).canConvert<bool>()) {
		qWarning() << "Unexpected format for GuiTablineBuffers:" << args;
		return;
	}

	const bool isEnabled{ args.at(2).toBool() };

	m_isBufferlineEnabled = isEnabled;

	QSettings settings;
	settings.setValue(cs_showBuffersOptionName, isEnabled);

	updateTablineVisibility();
}

static std::vector<Tab> ParseTablineVariant(const QVariantList tabs) noexcept
{
	std::vector<Tab> tabList;

	for (const auto& varTab : tabs) {
		if (static_cast<QMetaType::Type>(varTab.type()) != QMetaType::QVariantMap) {
			qWarning() << "Unexpected varTab value in tabline_update:" << varTab;
			continue;
		}

		const QVariantMap tabMap = varTab.toMap();
		const QString key{ (tabMap.contains("tab")) ? "tab" : "buffer" };

		if (!tabMap.contains(key) || !tabMap.contains("name")) {
			qWarning() << "Unexpected tabMap value in tabline_update:" << tabMap;
			continue;
		}

		const uint64_t tab{ tabMap.value(key).toULongLong() };
		const QString name{ tabMap.value("name").toString() };
		tabList.emplace_back(name, tab);
	}

	return tabList;
}

void Tabline::handleTablineUpdate(const QVariantList& args) noexcept
{
	if (args.size() < 2 || !args.at(0).canConvert<uint64_t>()
		|| static_cast<QMetaType::Type>(args.at(1).type()) != QMetaType::QVariantList) {
		qWarning() << "Unexpected argument for tabline_update:" << args;
		return;
	}

	const uint64_t curtab{ args.at(0).toULongLong() };
	const QVariantList tabs = args.at(1).toList();

	const std::vector<Tab> tabList{ ParseTablineVariant(tabs) };

	if (args.size() < 4) {
		drawTablineUpdates(tabList, curtab, {}, 0);
		return;
	}

	if (!args.at(2).canConvert<uint64_t>()
		|| static_cast<QMetaType::Type>(args.at(3).type()) != QMetaType::QVariantList) {
		qWarning() << "Unexpected argument for tabline_update:" << args;
		return;
	}

	const uint64_t curbuf{ args.at(2).toULongLong() };
	const QVariantList buffers = args.at(3).toList();

	const std::vector<Tab> bufferList{ ParseTablineVariant(buffers) };

	drawTablineUpdates(tabList, curtab, bufferList, curbuf);
}

void Tabline::handleOptionShowTabline(const QVariantList& args) noexcept
{
	if (args.size() < 1 || !args.at(0).canConvert<QString>()) {
		return;
	}

	const QString optionName{ args.at(0).toString() };

	if (optionName != "showtabline") {
		return;
	}

	if (args.size() < 2 || !args.at(1).canConvert<int>()) {
		qWarning() << "Tabline unexpected format for option showtabline:" << args;
	}

	const int value{ args.at(1).toInt() };

	auto OptionFromInteger = [](int value) noexcept -> OptionShowTabline
	{
		const OptionShowTabline enumValue{ static_cast<OptionShowTabline>(value) };

		switch (enumValue) {
			case OptionShowTabline::Never:
			case OptionShowTabline::AtLeastTwo:
			case OptionShowTabline::Always:
				return enumValue;
		}

		// Error: Unrecognized value, fallback to default value 1 (AtLeastTwo)
		qWarning() << QStringLiteral("Error: unrecognized value for showtabline { %1 }").arg(value);
		return OptionShowTabline::AtLeastTwo;
	};

	m_optionShowTabline = OptionFromInteger(value);
	updateTablineVisibility();
}

static QIcon GetIconFromFilePath(const QString& path) noexcept
{
	static std::mutex s_iconCacheLock;

	static QFileIconProvider s_iconProvider;

	using CachedBufferIcon = std::pair<QString, QIcon>;
	static std::vector<CachedBufferIcon> s_iconCache;

	{
		std::lock_guard<std::mutex> guard(s_iconCacheLock);

		auto pathMatchesExact = [&](const CachedBufferIcon& cacheEntry) noexcept
		{
			// We assume paths are case sensitive strings, this is not always correct.
			// False positives may result in duplicate entries, this is okay.
			// Examples: Windows paths, Windows short vs long paths, etc
			return cacheEntry.first == path;
		};

		auto result{ std::find_if(s_iconCache.begin(), s_iconCache.end(), pathMatchesExact) };

		// Use cached icon if present, avoids repeated disk I/O
		if (result != s_iconCache.end()) {
			return result->second;
		}

		// No icon exists for the path, create one
		QIcon icon{ s_iconProvider.icon(QFileInfo{ path }) };

		// Some file-does-not-exist cases display strange icons, use generic text icon
		const QString iconName{ icon.name() };
		if (iconName == "unknown" || iconName == "application-octet-stream") {
			icon = QIcon::fromTheme("text-x-generic");
		}

		s_iconCache.emplace_back(path, icon);
		return icon;
	}
}

static void SetTabIconAndTooltipCallback(
	QPointer<QTabBar> bufferline, int bufIndex, const QVariant& resp) noexcept
{
	if (!resp.canConvert<QString>()) {
		qWarning() << "Unexpected buffer path format in drawTablineUpdates";
		return;
	}

	if (!bufferline) {
		return;
	}

	const QString bufferPath{ resp.toString() };

	if (bufferPath.isEmpty()) {
		return;
	}
	
	bufferline->setTabToolTip(bufIndex, bufferPath);
	bufferline->setTabIcon(bufIndex, GetIconFromFilePath(bufferPath));
}

void Tabline::drawTablineUpdates(
	const std::vector<Tab> tabList,
	uint64_t curtab,
	const std::vector<Tab>& bufferList,
	uint64_t curbuf) noexcept
{
	updateTabControl(m_tabline, m_nvim.api0(), tabList, curtab, false /*drawTabIcons*/);
	updateTabControl(m_bufferline, m_nvim.api0(), bufferList, curbuf, true /*drawTabIcons*/);
	updateTablineVisibility();
}

void Tabline::updateTabControl(
	QTabBar& tabControl,
	NeovimApi0* nvimApi0,
	const std::vector<Tab> tabList,
	uint64_t curtab,
	bool drawTabIcons) noexcept
{
	// Remove closed/deleted tabs
	for (int i = tabList.size(); i < tabControl.count(); i++) {
		tabControl.removeTab(i);
	}

	int tabIndex{ 0 };
	for (const auto& tab : tabList) {
		// Required: Set Tab Text
		QString text{ tab.GetName() };

		// Escape & in tab name otherwise it will be interpreted as
		// a keyboard shortcut (#357) - escaping is done using &&
		text.replace("&", "&&");

		if (tabControl.count() <= tabIndex) {
			tabControl.addTab(text);
		}
		else {
			tabControl.setTabText(tabIndex, text);
		}

		// Required: Set Tab Neovim Handle
		tabControl.setTabData(tabIndex, QVariant::fromValue(tab.GetHandle()));

		// Optional: Mark active tab
		if (curtab == tab.GetHandle()) {
			tabControl.setCurrentIndex(tabIndex);
		}

		// Optional: Add filetype icons
		if (drawTabIcons && nvimApi0) {
			auto reqBufferPath{ nvimApi0->vim_eval(
				QStringLiteral("expand('#%1:p')").arg(tab.GetHandle()).toLatin1()) };

			QPointer<QTabBar> spTabControl{ &tabControl };
			auto handle = [spTabControl, tabIndex](quint32, quint64, const QVariant& resp) noexcept
			{
				SetTabIconAndTooltipCallback(spTabControl, tabIndex, resp);
			};
			connect(reqBufferPath, &MsgpackRequest::finished, this, handle);
		}

		tabIndex++;
	}
}

void Tabline::updateTablineVisibility() noexcept
{
	if (!m_isEnabled) {
		setVisible(false);
		return;
	}

	if (!m_tablineAction || !m_bufferlineAction) {
		qWarning() << "Tabline is missing Buffer/Tab QAction!";
		return;
	}

	// Legacy Mode: Neovim does not provide buffer info to GuiTabline
	// Support for tabs + buffers was added in Neovim API 8
	const bool isLegacyMode{ !m_isBufferlineEnabled || m_bufferline.count() == 0 };

	const bool isAtLeastTwoTabs{ m_tabline.count() >= 2 };
	const bool isAtLeastTwoBuffers{ m_bufferline.count() >= 2 };

	switch (m_optionShowTabline) {
		case OptionShowTabline::Never:
			setVisible(false);
			m_bufferlineAction->setVisible(false);
			m_spacerAction->setVisible(false);
			m_tablineAction->setVisible(false);
			break;

		case OptionShowTabline::AtLeastTwo:
			setVisible(isAtLeastTwoTabs || isAtLeastTwoBuffers);
			m_bufferlineAction->setVisible(!isLegacyMode && isAtLeastTwoBuffers);
			m_spacerAction->setVisible(!isLegacyMode && isAtLeastTwoTabs);
			m_tablineAction->setVisible(isAtLeastTwoTabs);
			break;

		// Users expect buffers to appear as tabs, similar to vim-airline.
		// When no vim-tabs are present, we display the vim-buffers on the left.
		// Once two or more vim-tabs are present, we display vim-tabs on the left
		// and vim-buffers on the right; similar behavior to vim-airline.
		case OptionShowTabline::Always:
			setVisible(true);
			m_bufferlineAction->setVisible(!isLegacyMode);
			m_spacerAction->setVisible(!isLegacyMode && isAtLeastTwoTabs);
			m_tablineAction->setVisible(isAtLeastTwoTabs || isLegacyMode);
			break;
	}
}

void Tabline::currentChangedTabline(int index) noexcept
{
	if (!m_nvim.api0()) {
		return;
	}

	const uint64_t handle{ m_tabline.tabData(index).toULongLong() };

	m_nvim.api0()->vim_set_current_tabpage(handle);
}

void Tabline::closeRequestedTabline(int index) noexcept
{
	if (!m_nvim.api0()) {
		return;
	}

	const uint64_t handle{ m_bufferline.tabData(index).toULongLong() };
	m_nvim.api0()->vim_command(QStringLiteral("tabclose %1").arg(handle).toLatin1());
}

void Tabline::currentChangedBufline(int index) noexcept
{
	if (!m_nvim.api0()) {
		return;
	}

	const uint64_t handle{ m_bufferline.tabData(index).toULongLong() };
	m_nvim.api0()->vim_command(QStringLiteral("buffer! %1").arg(handle).toLatin1());
}

static QString GetSanitizedErrorString(const QVariant& err) noexcept
{
	static const QString s_errorUnknown{
		"Unknown error closing buffer!\nPlease save and try again."
	};
	static const QString s_errorUnsaved{
		"No write since last change!\nPlease save and try again."
	};

	if (err.type() != QVariant::Type::List) {
		return s_errorUnknown;
	}

	const QVariantList errList = err.toList();
	if (errList.size() < 2 || !errList.at(1).canConvert<QString>()) {
		return s_errorUnknown;
	}

	const QString errorText{ errList.at(1).toString() };

	static const QRegularExpression s_reUnsaved{
		"^.*E89: No write since last change for buffer (\\d+) \\(add \\! to override\\)$"
	};
	if (s_reUnsaved.match(errorText).hasMatch()) {
		return s_errorUnsaved;
	}

	return s_errorUnknown;
}

void Tabline::handleCloseBufferError(quint32 msgid, quint64 fun, const QVariant& err) noexcept
{
	QMessageBox msgBox;
	msgBox.setText(GetSanitizedErrorString(err));
	msgBox.setIcon(QMessageBox::Icon::Warning);
	msgBox.exec();
}

void Tabline::closeRequestedBufline(int index) noexcept
{
	if (!m_nvim.api0()) {
		return;
	}

	const uint64_t handle{ m_bufferline.tabData(index).toULongLong() };

	auto reqCloseBuffer{ m_nvim.api0()->vim_command(
		QStringLiteral("bdel %1").arg(handle).toLatin1()) };
	connect(reqCloseBuffer, &MsgpackRequest::error, this, &Tabline::handleCloseBufferError);
}

} // namespace NeovimQt
