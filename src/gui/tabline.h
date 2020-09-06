#pragma once

#include <QToolBar>
#include <QTabBar>

#include "neovimconnector.h"
#include "shell.h"
#include "tab.h"

namespace NeovimQt {

class Tabline: public QToolBar
{
	Q_OBJECT

public:
	Tabline(NeovimConnector& nvim, const ShellOptions& options, QWidget* parent) noexcept;

	void handleNeovimNotification(const QByteArray& name, const QVariantList& args) noexcept;
	void handleRedraw(const QByteArray& name, const QVariantList& opargs) noexcept;

private slots:
	void currentChangedTabline(int index);
	void currentChangedBufline(int index);

private:
	void neovimConnectorReady() noexcept;

	void handleGuiTabline(const QVariantList& args) noexcept;
	void handleTablineUpdate(const QVariantList& args) noexcept;

	void drawTablineUpdates(
		const std::vector<Tab> tabList,
		uint64_t curtab,
		const std::vector<Tab>& bufferList,
		uint64_t curbuf) noexcept;

	NeovimConnector& m_nvim;
	bool m_isEnabled{ false };

	QTabBar m_tabline;
	QAction* m_tablineAction{};

	QTabBar m_bufferline;
	QAction* m_bufferlineAction{};

	QAction* m_spacerAction{};
};

} // namespace NeovimQt
