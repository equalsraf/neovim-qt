#include "tabline.h"

#include <QLayout>
#include <QVariant>

namespace NeovimQt {

Tabline::Tabline(NeovimConnector& nvim, const ShellOptions& options, QWidget* parent) noexcept
	: m_nvim{ nvim }
	, m_isEnabled{ options.enable_ext_linegrid }
	, m_tabline{ this }
	, m_bufferline{ this }
{
	setObjectName("GuiTabline");
	setAllowedAreas(Qt::TopToolBarArea);
	setMovable(false);
	setFloatable(false);

	// Avoid margins around the tabbar
	layout()->setContentsMargins(0, 0, 0, 0);

	auto InititializeQTabBar = [](QTabBar& initTabBar) noexcept
	{
		initTabBar.setDrawBase(false);
		initTabBar.setExpanding(false);
		initTabBar.setDocumentMode(true);
		initTabBar.setFocusPolicy(Qt::NoFocus);
	};

	InititializeQTabBar(m_tabline);
	InititializeQTabBar(m_bufferline);

	// Spacer
    QWidget* spacer{ new QWidget() };
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

	m_tablineAction = addWidget(&m_tabline);
	m_spacerAction = addWidget(spacer);
	m_bufferlineAction = addWidget(&m_bufferline);

	connect(&m_nvim, &NeovimConnector::ready, this, &Tabline::neovimConnectorReady);
	connect(&m_tabline, &QTabBar::currentChanged, this, &Tabline::currentChangedTabline);
	connect(&m_bufferline, &QTabBar::currentChanged, this, &Tabline::currentChangedBufline);

	setVisible(m_isEnabled);
}

void Tabline::neovimConnectorReady() noexcept
{
	connect(m_nvim.api0(), &NeovimApi0::neovimNotification,
		this, &Tabline::handleNeovimNotification);

	m_nvim.api0()->vim_subscribe("Gui");
}

void Tabline::handleNeovimNotification(const QByteArray& name, const QVariantList& args) noexcept
{
	if (args.size() <= 0) {
		return;
	}

	if (name == "Gui") {
		const QString guiEvName{ m_nvim.decode(args.at(0).toByteArray()) };

		if (guiEvName == "Option") {
			const QString option{ m_nvim.decode(args.at(1).toByteArray()) };

			if (option == "Tabline") {
				const bool isEnabled{ args.at(2).toBool() };

				m_isEnabled = isEnabled;
				setVisible(isEnabled);
			}
			return;
		}
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
	if (args.size() < 2
		|| !args.at(0).canConvert<uint64_t>()
		|| static_cast<QMetaType::Type>(args.at(1).type()) != QMetaType::QVariantList ) {
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
		|| static_cast<QMetaType::Type>(args.at(3).type()) != QMetaType::QVariantList ) {
		qWarning() << "Unexpected argument for tabline_update:" << args;
		return;
	}

	const uint64_t curbuf{ args.at(2).toULongLong() };
	const QVariantList buffers = args.at(3).toList();

	const std::vector<Tab> bufferList{ ParseTablineVariant(buffers) };

	drawTablineUpdates(tabList, curtab, bufferList, curbuf);
}

void Tabline::drawTablineUpdates(
	const std::vector<Tab> tabList,
	uint64_t curtab,
	const std::vector<Tab>& bufferList,
	uint64_t curbuf) noexcept
{
	for (int i=tabList.size();i<m_tabline.count();i++) {
		m_tabline.removeTab(i);
	}

	for (int i=bufferList.size();i<m_bufferline.count();i++) {
		m_bufferline.removeTab(i);
	}

	int tabIndex{ 0 };
	for (const auto& tab : tabList) {
		// Escape & in tab name otherwise it will be interpreted as
		// a keyboard shortcut (#357) - escaping is done using &&
		QString text{ tab.GetName() };
		text.replace("&", "&&");

		if (m_tabline.count() <= tabIndex) {
			m_tabline.addTab(text);
		} else {
			m_tabline.setTabText(tabIndex, text);
		}

		m_tabline.setTabToolTip(tabIndex, text);
		m_tabline.setTabData(tabIndex, QVariant::fromValue(tab.GetHandle()));

		if (curtab == tab.GetHandle()) {
			m_tabline.setCurrentIndex(tabIndex);
		}

		tabIndex++;
	}

	int bufIndex{ 0 };
	for (const auto& buffer : bufferList) {
		// Escape & in buffer name otherwise it will be interpreted as
		// a keyboard shortcut (#357) - escaping is done using &&
		QString text{ buffer.GetName() };
		text.replace("&", "&&");

		if (m_bufferline.count() <= bufIndex) {
			m_bufferline.addTab(text);
		} else {
			m_bufferline.setTabText(bufIndex, text);
		}

		m_bufferline.setTabToolTip(bufIndex, text);
		m_bufferline.setTabData(bufIndex, QVariant::fromValue(buffer.GetHandle()));

		if (curbuf == buffer.GetHandle()) {
			m_bufferline.setCurrentIndex(bufIndex);
		}

		bufIndex++;
	}

	if (!m_tablineAction || !m_bufferlineAction)
	{
		qWarning() << "Tabline is missing QAction!";
		return;
	}

	m_tablineAction->setVisible(m_tabline.count() > 1);
	m_spacerAction->setVisible(m_tabline.count() > 1);
	m_bufferlineAction->setVisible(true);
}

void Tabline::currentChangedTabline(int index)
{
	if (!m_nvim.api1()) {
		return;
	}

	const uint64_t handle{ m_tabline.tabData(index).toULongLong() };

	m_nvim.api1()->nvim_set_current_tabpage(handle);
}

void Tabline::currentChangedBufline(int index)
{
	if (!m_nvim.api1()) {
		return;
	}

	const uint64_t handle{ m_bufferline.tabData(index).toULongLong() };

	//m_nvim.api1()->nvim_set_current_buf(handle);
	m_nvim.api1()->nvim_command(QStringLiteral("buffer! %1").arg(handle).toLatin1());
}

} // namespace NeovimQt
