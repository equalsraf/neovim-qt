
#include <QIcon>
#include "popupmenumodel.h"

namespace NeovimQt {

PopupMenuModel::PopupMenuModel(QList<PopupMenuItem> l)
:QAbstractTableModel(), m_data(l)
{
}

int PopupMenuModel::rowCount(const QModelIndex &parent) const {
	if (parent.isValid()) {
		return 0;
	} else {
		return m_data.size();
	}
}


int PopupMenuModel::columnCount(const QModelIndex &parent) const {
	return 4;
}

QVariant PopupMenuModel::data(const QModelIndex &index, int role) const {

	if (index.parent().isValid()) {
		return QVariant();
	}

	if (index.row() < 0 || m_data.size() <= index.row()) {
		return QVariant();
	}
	auto item = m_data.at(index.row());

	int column = index.column();

    if ( role == Qt::DisplayRole ) {
		if (column == 0) {
			return item.text;
		} else if (column == 1 && !item.kind.isEmpty()) {
			return item.kind;
		} else if (column == 2 && !item.extra.isEmpty()) {
			return item.extra;
		} else if (column == 3 && !item.info.isEmpty()) {
			return item.info;
		} else {
			return QVariant();
		}
	} else {
		return QVariant();
	}
}

} // namespace
