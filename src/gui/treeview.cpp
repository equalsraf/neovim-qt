#include "treeview.h"

#include <QDir>
#include <QHeaderView>
#include <QKeyEvent>
#include <QProcess>
#include <QSettings>
#include <QStandardPaths>

namespace NeovimQt {

TreeView::TreeView(NeovimConnector* nvim, QWidget* parent) noexcept
	: QTreeView(parent)
	, m_model{ parent }
	, m_nvim{ nvim }
{
	if (!m_nvim) {
		qFatal("Fatal Error: TreeView must have a valid NeovimConnector!");
	}

	setModel(&m_model);

	header()->hide();

	const int columnCount{ m_model.columnCount() };
	for (int i = 1; i < columnCount; i++) {
		hideColumn(i);
	}

	QSettings settings;
	setVisible(settings.value("Gui/TreeView", false).toBool());

	connect(m_nvim, &NeovimConnector::ready, this, &TreeView::neovimConnectorReady);
}

void TreeView::neovimConnectorReady() noexcept
{
	connect(this, &TreeView::doubleClicked, this, &TreeView::open);

	connect(
		m_nvim->api0(), &NeovimApi0::neovimNotification, this, &TreeView::handleNeovimNotification);

	m_nvim->api0()->vim_subscribe("Dir");
	m_nvim->api0()->vim_subscribe("Gui");
}

void TreeView::open(const QModelIndex& index) noexcept
{
	const QFileInfo fileInfo{ m_model.fileInfo(index) };
	if (fileInfo.isFile() && fileInfo.isReadable()) {
		m_nvim->api0()->vim_call_function("GuiDrop", { fileInfo.filePath() });
	}
	focusNextChild();
}

void TreeView::handleNeovimNotification(const QByteArray& name, const QVariantList& args) noexcept
{
	if (args.size() <= 0) {
		return;
	}

	if (name == "Dir" && args.size() >= 0) {
		handleDirectoryChanged(args);
		return;
	}

	if (name == "Gui") {
		const QString guiEvName{ args.at(0).toByteArray() };

		if (guiEvName == "TreeView") {
			handleGuiTreeView(args);
			return;
		}
	}
}

void TreeView::handleDirectoryChanged(const QVariantList& args) noexcept
{
	if (args.size() < 1 || !args.at(0).canConvert<QString>()) {
		qWarning() << "Unexpected arguments for Dir:" << args;
		return;
	}

	const QString dir{ args.at(0).toString() };

	if (!QDir{ dir }.exists()) {
		return;
	}

	QDir::setCurrent(dir);
	m_model.setRootPath(dir);
	setRootIndex(m_model.index(dir));
}

void TreeView::handleGuiTreeView(const QVariantList& args) noexcept
{
	if (args.size() < 2 || !args.at(1).canConvert<QString>()) {
		qWarning() << "Unexpected arguments for Dir:" << args;
		return;
	}

	const QString action{ args.at(1).toString() };
	if (action == "Toggle") {
		updateVisibility(!isVisible());
		return;
	}

	if (action == "ShowHide" && args.size() == 3) {
		handleShowHide(args);
	}
}

void TreeView::handleShowHide(const QVariantList& args) noexcept
{
	if (args.size() < 3 || !args.at(2).canConvert<bool>()) {
		qWarning() << "Unexpected arguments for GuiTreeView ShowHide:" << args;
	}

	const bool isVisible{ args.at(2).toBool() };

	updateVisibility(isVisible);
}

void TreeView::updateVisibility(bool isVisible) noexcept
{
	QSettings settings;
	settings.setValue("Gui/TreeView", isVisible);
	setVisible(isVisible);
}

} // namespace NeovimQt
