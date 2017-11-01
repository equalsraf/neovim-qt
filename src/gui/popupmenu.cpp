#include "popupmenu.h"
#include "popupwidgetitem.h"

#include <QScrollBar>

namespace NeovimQt {

PopupMenu::PopupMenu(QWidget *parent)
:QListView(parent)
{
	setFocusPolicy(Qt::NoFocus);
	setAttribute(Qt::WA_TransparentForMouseEvents);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setViewMode(QListView::ListMode);
}

QSize PopupMenu::sizeHint() const {
	int height = 0;
	for (int i=0; i<model()->rowCount(); i++) {
		height += sizeHintForRow(i);
	}
	return QSize(sizeHintForColumn(0) + 2*frameWidth(),
			height + 2*frameWidth());
}

} // Namespace
