#ifndef NEOVIM_QT_POPUPMENU
#define NEOVIM_QT_POPUPMENU

#include <QListView>
#include <QStyledItemDelegate>

namespace NeovimQt {

class PopupMenu: public QListView {
public:
	PopupMenu(QWidget *parent=0);
	QSize sizeHint() const Q_DECL_OVERRIDE;
};

class PopupMenuDelegate: public QStyledItemDelegate {
public:
    PopupMenuDelegate(QObject *parent = 0);
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
 
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index ) const;
};

} // Namespace
#endif
