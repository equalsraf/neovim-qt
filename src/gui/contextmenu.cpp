#include "contextmenu.h"

namespace NeovimQt {

ContextMenu::ContextMenu(NeovimConnector* nvim, QWidget* parent) noexcept
	: QMenu(parent)
	, m_nvim{ nvim }
{
	if (!m_nvim) {
		qFatal("Fatal Error: ContextMenu must have a valid NeovimConnector!");
	}

	m_actCut.setText(QObject::tr("Cut"));
	m_actCopy.setText(QObject::tr("Copy"));
	m_actPaste.setText(QObject::tr("Paste"));
	m_actSelectAll.setText(QObject::tr("Select All"));

	m_actCut.setIcon(QIcon::fromTheme("edit-cut"));
	m_actCopy.setIcon(QIcon::fromTheme("edit-copy"));
	m_actPaste.setIcon(QIcon::fromTheme("edit-paste"));
	m_actSelectAll.setIcon(QIcon::fromTheme("edit-select-all"));

	addAction(&m_actCut);
	addAction(&m_actCopy);
	addAction(&m_actPaste);
	addSeparator();
	addAction(&m_actSelectAll);

	connect(&m_actCut, &QAction::triggered, this, &ContextMenu::neovimSendCut);
	connect(&m_actCopy, &QAction::triggered, this, &ContextMenu::neovimSendCopy);
	connect(&m_actPaste, &QAction::triggered, this, &ContextMenu::neovimSendPaste);
	connect(&m_actSelectAll, &QAction::triggered, this, &ContextMenu::neovimSendSelectAll);
}

void ContextMenu::neovimSendCut() noexcept
{
	m_nvim->api0()->vim_command_output(R"(normal! "+x)");
}

void ContextMenu::neovimSendCopy() noexcept
{
	m_nvim->api0()->vim_command(R"(normal! "+y)");
}

void ContextMenu::neovimSendPaste() noexcept
{
	m_nvim->api0()->vim_command(R"(normal! "+gP)");
}

void ContextMenu::neovimSendSelectAll() noexcept
{
	m_nvim->api0()->vim_command("normal! ggVG");
}

void ContextMenu::showContextMenu() noexcept
{
	popup(QCursor::pos());
}

} // namespace NeovimQt
