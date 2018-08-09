#ifndef NEOVIM_QT_POPUPMODEL
#define NEOVIM_QT_POPUPMODEL

#include <QAbstractListModel>

namespace NeovimQt {

struct PopupMenuItem {
	QString text;
	QString kind;
	QString extra;
	QString info;
};

class PopupMenuModel: public QAbstractListModel {
public:
	enum Roles{
		Text=Qt::UserRole,
		Kind,
		Extra,
		Info,
	};


	PopupMenuModel(QList<PopupMenuItem>);
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
private:
	QList<PopupMenuItem> m_data;
};

} // namespace

#endif
