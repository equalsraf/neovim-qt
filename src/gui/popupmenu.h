#ifndef NEOVIM_QT_POPUPMENU
#define NEOVIM_QT_POPUPMENU

#include <QListWidget>
#include "shellwidget/shellwidget.h"

namespace NeovimQt {

class PopupMenu: public QListView {
public:
	PopupMenu(ShellWidget* parent = nullptr);
	QSize sizeHint() const Q_DECL_OVERRIDE;
	void setAnchor(int64_t row, int64_t col);
	void setSelectedIndex(int64_t index);
	void updateGeometry();

private:
	int64_t m_anchorRow{ 0 };
	int64_t m_anchorCol{ 0 };
	ShellWidget* m_parentShellWidget{ nullptr };

	void setGeometry(int64_t row, int64_t col);
};

} // Namespace
#endif
