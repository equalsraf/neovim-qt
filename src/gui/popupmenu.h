#ifndef NEOVIM_QT_POPUPMENU
#define NEOVIM_QT_POPUPMENU

#include <QListWidget>

namespace NeovimQt {

class PopupMenu: public QListWidget {
public:
	PopupMenu(QWidget *parent=0);
	QSize sizeHint() const Q_DECL_OVERRIDE;
	void addNeovimItem(const QString& text, const QString& kind,
						const QString& extra, const QString& info);
};

} // Namespace
#endif
