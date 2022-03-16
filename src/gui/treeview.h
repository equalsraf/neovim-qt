#pragma once

#include <QFileSystemModel>
#include <QTreeView>
#include <QUrl>

#include "neovimconnector.h"

namespace NeovimQt {

class TreeView : public QTreeView {
	Q_OBJECT

public:
	TreeView(NeovimConnector* nvim, QWidget* parent) noexcept;

public slots:
	void open(const QModelIndex& index) noexcept;
	void handleNeovimNotification(const QByteArray& name, const QVariantList& args) noexcept;
	void handleDirectoryChanged(const QVariantList& args) noexcept;
	void handleGuiTreeView(const QVariantList& args) noexcept;
	void handleShowHide(const QVariantList& args) noexcept;

	void neovimConnectorReady() noexcept;

private:
	void updateVisibility(bool isVisible) noexcept;

	QFileSystemModel m_model;
	NeovimConnector* m_nvim;
};

} // namespace NeovimQt
