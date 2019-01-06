
#include <QIcon>
#include "popupmenumodel.h"

namespace NeovimQt {

PopupMenuModel::PopupMenuModel(QList<PopupMenuItem> l)
:QAbstractListModel(), m_data(l)
{
}

int PopupMenuModel::rowCount(const QModelIndex &parent) const {
	if (parent.isValid()) {
		return 0;
	} else {
		return m_data.size();
	}
}

QVariant PopupMenuModel::data(const QModelIndex &index, int role) const {

	if (index.parent().isValid()) {
		return QVariant();
	}

	if (index.row() < 0 || m_data.size() <= index.row()) {
		return QVariant();
	}
	auto item = m_data.at(index.row());

    if ( role == Qt::DisplayRole ) {
		QString text = item.text;

		if (!item.kind.isEmpty()) {
			text = text + " " + item.kind;
		}
		if (!item.extra.isEmpty()) {
			text = text + " " + item.extra;
		}
		if (!item.info.isEmpty()) {
			text = text + " " + item.info;
		}
		return text;
	} else if (role == PopupMenuModel::Text) {
		return item.text;
	} else if (role == PopupMenuModel::Kind) {
		if (item.kind.isEmpty()) {
			return item.kind;
		} else {
			return QVariant();
		}
	} else if (role == PopupMenuModel::Extra) {
		if (item.extra.isEmpty()) {
			return item.extra;
		} else {
			return QVariant();
		}
	} else if (role == PopupMenuModel::Info) {
		if (item.info.isEmpty()) {
			return item.info;
		} else {
			return QVariant();
		}
	} else {
		return QVariant();
	}
}

} // namespace
