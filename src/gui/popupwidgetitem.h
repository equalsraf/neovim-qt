#ifndef NEOVIM_QT_POPUPWIDGETITEM
#define NEOVIM_QT_POPUPWIDGETITEM

#include <QListWidgetItem>

namespace NeovimQt {

class PopupWidgetItem: public QListWidgetItem {
public:
	PopupWidgetItem(const QString& text, const QString& kind=QString(),
			const QString& extra=QString(), const QString& info=QString());
private:
};

} // Namespace
#endif
