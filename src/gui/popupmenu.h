#ifndef NEOVIM_QT_POPUPMENU
#define NEOVIM_QT_POPUPMENU

#include <QListWidget>

namespace NeovimQt {

class PopupMenu: public QListView {
public:
	PopupMenu(QWidget *parent=0);
	QSize sizeHint() const Q_DECL_OVERRIDE;
};

} // Namespace
#endif
