#include "treeview.h"

#include <QDir>
#include <QHeaderView>
#include <QKeyEvent>
#include <QProcess>
#include <QStandardPaths>

static const int MAX_COLUMNS_ON_INIT = 10;

namespace NeovimQt {
TreeView::TreeView(NeovimConnector *nvim, QWidget *parent)
: QTreeView(parent), m_nvim(nvim) {
	model = new QFileSystemModel(this);

	setModel(model);

	header()->hide();

	for (int i = 1; i < MAX_COLUMNS_ON_INIT; i++) {
		hideColumn(i);
	}

	if (m_nvim->isReady()) {
		connector_ready_cb();
	}
	connect(m_nvim, &NeovimConnector::ready, this, &TreeView::connector_ready_cb);
}

void TreeView::connector_ready_cb() {
	setDirectory(QStandardPaths::writableLocation(QStandardPaths::HomeLocation));

	connect(this, &TreeView::doubleClicked, this, &TreeView::open);

	connect(m_nvim->neovimObject(), &NeovimApi1::neovimNotification, this,
			&TreeView::handleNeovimNotification);

	m_nvim->neovimObject()->vim_subscribe("Dir");
	m_nvim->neovimObject()->vim_subscribe("GuiTreeView");
}

void TreeView::open(const QModelIndex &index) {
	QFileInfo info = model->fileInfo(index);
	if (info.isFile() && info.isReadable()) {
		QVariantList args;
		args << info.filePath();
		m_nvim->neovimObject()->vim_call_function("GuiDrop", args);
	}
	focusNextChild();
}

void TreeView::setDirectory(const QString &dir, bool notify) {
	if (QDir(dir).exists()) {
		QDir::setCurrent(dir);
		model->setRootPath(dir);
		setRootIndex(model->index(dir));
		if (notify) {
			m_nvim->neovimObject()->vim_change_directory(
				QByteArray::fromStdString(dir.toStdString()));
		}
	}
}

void TreeView::handleNeovimNotification(const QByteArray &name,
					const QVariantList &args) {
	if (name == "Dir" && args.size() >= 0) {
		setDirectory(m_nvim->decode(args.at(0).toByteArray()), false);
	} else if (name == "GuiTreeView") {
		QByteArray action = args.at(0).toByteArray();
		if (action == "Toggle" && args.size() == 1) {
			if (isVisible())
				hide();
			else
				show();
		} else if (action == "ShowHide" && args.size() == 2) {
			args.at(1).toBool() ? show() : hide();
		}
	}
}

}  // namespace NeovimQt
