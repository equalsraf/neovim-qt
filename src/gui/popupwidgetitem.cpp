#include "popupwidgetitem.h"

#include <QDebug>

namespace NeovimQt {

PopupWidgetItem::PopupWidgetItem(const QString& text,
		const QString& kind, const QString& extra, const QString& info)
:QListWidgetItem(text)
{
}

} // Namespace
