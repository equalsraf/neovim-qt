#include "treeview.h"

#include <QHeaderView>
#include <QProcess>
#include <QDir>
#include <QStandardPaths>
#include <QKeyEvent>

namespace NeovimQt {
TreeView::TreeView(NeovimConnector *nvim, QWidget *parent)
:QTreeView(parent), m_nvim(nvim)
{

	model = new QFileSystemModel(this);

	setModel(model);

	header()->hide();
	// FIXME: MAGIC NUMBER!!
	for (int i = 1; i < 5; i++) {
		hideColumn(i);
	}

	setDirectory(QStandardPaths::writableLocation(QStandardPaths::HomeLocation));

	connect(this, &TreeView::doubleClicked,
			this, &TreeView::open);

	connect(m_nvim->neovimObject(), &Neovim::neovimNotification,
			this, &TreeView::handleNeovimNotification);

	m_nvim->neovimObject()->vim_subscribe("Dir");
	m_nvim->neovimObject()->vim_subscribe("TreeView");
}

void TreeView::open(const QModelIndex &index)
{
	QFileInfo info = model->fileInfo(index);
	if (info.isFile() and info.isReadable()) {
		QVariantList args;
		args << info.filePath();
		m_nvim->neovimObject()->vim_call_function("GuiDrop", args);
	}
	else if (info.isDir()) {
		setDirectory(info.absoluteFilePath());
	}
}

void TreeView::setDirectory(const QString &dir, bool notify)
{
	if (QDir(dir).exists()) {
		QDir::setCurrent(dir);
		model->setRootPath(dir);
		setRootIndex(model->index(dir));
		if (notify) {
			m_nvim->neovimObject()->vim_change_directory(QByteArray::fromStdString(dir.toStdString()));
		}
	}
}

void TreeView::keyPressEvent(QKeyEvent *ev)
{
	if (ev->key() == Qt::Key_Backspace) {
		setParentDirectory();
	}
}

void TreeView::setParentDirectory()
{
	QDir dir = QDir::current();
	if (dir.cdUp()) {
		setDirectory(dir.absolutePath());
	}
}

void TreeView::handleNeovimNotification(const QByteArray &name, const QVariantList& args)
{
	if (name == "Dir" and args.size() >= 0) {
		setDirectory(m_nvim->decode(args.at(0).toByteArray()), false);
	}
	else if (name == "TreeView") {
		QByteArray action = args.at(0).toByteArray();
		if (action == "Toggle" and args.size() == 1) {
		    if (isVisible()) hide();
		    else show();
		}
		else if (action == "ShowHide" and args.size() == 2) {
			args.at(1).toBool() ? show() : hide();
		}
	}
}

}

