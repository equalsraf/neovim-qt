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
	void setParentDirectory();
	void handleNeovimNotification(const QByteArray &, const QVariantList&);

protected:
	QFileSystemModel *model;
	NeovimConnector *m_nvim;

	virtual void keyPressEvent(QKeyEvent *ev) Q_DECL_OVERRIDE;
};

}

#endif // TREEVIEW
