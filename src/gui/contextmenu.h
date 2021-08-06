#pragma once

#include <QMenu>

#include "neovimconnector.h"

namespace NeovimQt {

class ContextMenu final : public QMenu
{
	Q_OBJECT

public:
	ContextMenu(NeovimConnector* nvim, QWidget* parent) noexcept;

	void neovimSendCut() noexcept;
	void neovimSendCopy() noexcept;
	void neovimSendPaste() noexcept;
	void neovimSendSelectAll() noexcept;

public slots:
	void showContextMenu() noexcept;

private:
	NeovimConnector* m_nvim;

	QAction m_actCut;
	QAction m_actCopy;
	QAction m_actPaste;
	QAction m_actSelectAll;
};

} // namespace NeovimQt
