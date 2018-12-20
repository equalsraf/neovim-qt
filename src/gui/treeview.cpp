#include "treeview.h"

#include <QDir>
#include <QHeaderView>
#include <QKeyEvent>
#include <QProcess>
#include <QStandardPaths>

namespace NeovimQt {
TreeView::TreeView(NeovimConnector *nvim, QWidget *parent)
    : QTreeView(parent), m_nvim(nvim) {
  model = new QFileSystemModel(this);

  setModel(model);

  header()->hide();
  // FIXME: MAGIC NUMBER!!
  for (int i = 1; i < 5; i++) {
    hideColumn(i);
  }

  connect(m_nvim, &NeovimConnector::ready, this, &TreeView::connector_ready_cb);
}

void TreeView::connector_ready_cb() {
  setDirectory(QStandardPaths::writableLocation(QStandardPaths::HomeLocation));

  connect(this, &TreeView::doubleClicked, this, &TreeView::open);

  connect(m_nvim->neovimObject(), &NeovimApi1::neovimNotification, this,
          &TreeView::handleNeovimNotification);

  m_nvim->neovimObject()->vim_subscribe("Dir");
  m_nvim->neovimObject()->vim_subscribe("TreeView");
}

void TreeView::open(const QModelIndex &index) {
  QFileInfo info = model->fileInfo(index);
  if (info.isFile() and info.isReadable()) {
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
  if (name == "Dir" and args.size() >= 0) {
    setDirectory(m_nvim->decode(args.at(0).toByteArray()), false);
  } else if (name == "TreeView") {
    QByteArray action = args.at(0).toByteArray();
    if (action == "Toggle" and args.size() == 1) {
      if (isVisible())
        hide();
      else
        show();
    } else if (action == "ShowHide" and args.size() == 2) {
      args.at(1).toBool() ? show() : hide();
    }
  }
}

}  // namespace NeovimQt
