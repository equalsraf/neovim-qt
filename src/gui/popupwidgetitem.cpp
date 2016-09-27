#include "popupwidgetitem.h"

#include <QDebug>

namespace NeovimQt {

PopupWidgetItem::PopupWidgetItem(const QString& text,
		const QString& kind, const QString& extra, const QString& info)
:QListWidgetItem(text)
{
	qDebug() << extra;
	if (extra.trimmed() == "[F]") {
		setIcon(QIcon(":/icons/inode-directory.png"));
	} else if (extra.trimmed() == "[B]") {
		setIcon(QIcon(":/icons/document-open-recent.png"));
	} else {
		setIcon(QIcon(":/icons/unknown.png"));
	}
}

} // Namespace
