#ifndef TREEVIEW
#define TREEVIEW

#include <QUrl>
#include <QTreeView>
#include <QFileSystemModel>
#include "neovimconnector.h"

namespace NeovimQt {

class TreeView : public QTreeView {
    Q_OBJECT
public:
	TreeView(NeovimConnector *, QWidget *parent = 0);

public slots:
	void open(const QModelIndex &);
	void setDirectory(const QString &, bool notify = true);
	void handleNeovimNotification(const QByteArray &, const QVariantList&);

protected:
	QFileSystemModel *model;
	NeovimConnector *m_nvim;
};

}

#endif // TREEVIEW
