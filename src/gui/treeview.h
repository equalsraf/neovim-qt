#ifndef TREEVIEW
#define TREEVIEW

#include <QFileSystemModel>
#include <QTreeView>
#include <QUrl>
#include "neovimconnector.h"

namespace NeovimQt {

class TreeView : public QTreeView {
    Q_OBJECT
 public:
	TreeView(NeovimConnector *, QWidget *parent = 0);

 public slots:
	void open(const QModelIndex &);
	void setDirectory(const QString &, bool notify = true);
	void handleNeovimNotification(const QByteArray &, const QVariantList &);
	void connector_ready_cb();

 protected:
	QFileSystemModel *model;
	NeovimConnector *m_nvim;
};

}  // namespace NeovimQt

#endif  // TREEVIEW
