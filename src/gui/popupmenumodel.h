#ifndef NEOVIM_QT_POPUPMODEL
#define NEOVIM_QT_POPUPMODEL

#include <QAbstractTableModel>

namespace NeovimQt {

struct PopupMenuItem {
	QString text;
	QString kind;
	QString extra;
	QString info;
};

class PopupMenuModel: public QAbstractTableModel {
public:
	PopupMenuModel(QList<PopupMenuItem>);
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	// TODO QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
private:
	QList<PopupMenuItem> m_data;
};

} // namespace

#endif
