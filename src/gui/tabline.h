#pragma once

#include <QTabBar>
#include <QToolBar>

#include "neovimconnector.h"
#include "shell.h"
#include "shelloptions.h"
#include "tab.h"

namespace NeovimQt {

class Tabline : public QToolBar
{
	Q_OBJECT

public:
	Tabline(NeovimConnector& nvim, QWidget* parent) noexcept;

	void handleNeovimNotification(const QByteArray& name, const QVariantList& args) noexcept;
	void handleRedraw(const QByteArray& name, const QVariantList& opargs) noexcept;

private slots:
	void currentChangedTabline(int index) noexcept;
	void closeRequestedTabline(int index) noexcept;
	void currentChangedBufline(int index) noexcept;
	void closeRequestedBufline(int index) noexcept;

private:
	void neovimConnectorReady() noexcept;

	void handleGuiOption(const QVariantList& args) noexcept;
	void handleGuiTabline(const QVariantList& args) noexcept;
	void handleGuiTablineBuffers(const QVariantList& args) noexcept;
	void handleTablineUpdate(const QVariantList& args) noexcept;
	void handleOptionShowTabline(const QVariantList& args) noexcept;
	void handleCloseBufferError(quint32 msgid, quint64 fun, const QVariant& err) noexcept;

	void drawTablineUpdates(
		const std::vector<Tab> tabList,
		uint64_t curtab,
		const std::vector<Tab>& bufferList,
		uint64_t curbuf) noexcept;

	void updateTabControl(
		QTabBar& tabControl,
		NeovimApi0* nvimApi0,
		const std::vector<Tab> tabList,
		uint64_t curtab,
		bool drawTabIcons) noexcept;

	void updateTablineVisibility() noexcept;
	void updateTablineVisibilityLegacyMode() noexcept;
	void updateTablinePathCallback() noexcept;

	enum class OptionShowTabline : int
	{
		Never = 0,
		AtLeastTwo = 1,
		Always = 2,
	};

	NeovimConnector& m_nvim;
	bool m_isEnabled{ cs_defaultIsTablineEnabled };
	bool m_isBufferlineEnabled{ true };

	QTabBar m_tabline;
	QAction* m_tablineAction{};

	QTabBar m_bufferline;
	QAction* m_bufferlineAction{};

	QWidget m_spacer;
	QAction* m_spacerAction{};

	OptionShowTabline m_optionShowTabline{ OptionShowTabline::AtLeastTwo };
};

} // namespace NeovimQt
