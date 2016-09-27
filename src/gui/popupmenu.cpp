#include "popupmenu.h"
#include "popupwidgetitem.h"

#include <QScrollBar>

namespace NeovimQt {

PopupMenu::PopupMenu(QWidget *parent)
:QListWidget(parent)
{
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

QSize PopupMenu::sizeHint() const {
	int height = 0;
	for (int i=0; i<count(); i++) {
		height += sizeHintForRow(i);
	}
	return QSize(sizeHintForColumn(0) + 2*frameWidth(),
			height + 2*frameWidth());
}

void PopupMenu::addNeovimItem(const QString& text, const QString& kind,
								const QString& extra, const QString& info)
{
	addItem(new PopupWidgetItem(text, kind, extra, info));
}

} // Namespace
