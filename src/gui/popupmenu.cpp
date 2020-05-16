#include "popupmenu.h"
#include "popupwidgetitem.h"

#include <QDebug>
#include <QScrollBar>

namespace NeovimQt {

PopupMenu::PopupMenu(ShellWidget* parent) :
	QListView(parent),
	m_parentShellWidget{ parent }
{
	setFocusPolicy(Qt::NoFocus);
	setAttribute(Qt::WA_TransparentForMouseEvents);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setViewMode(QListView::ListMode);
}

QSize PopupMenu::sizeHint() const {
	if (!model()) {
		return {};
	}

	int height = 0;
	for (int i=0; i<model()->rowCount(); i++) {
		height += sizeHintForRow(i);
	}
	return QSize(sizeHintForColumn(0) + 2*frameWidth(),
			height + 2*frameWidth());
}

void PopupMenu::setAnchor(int64_t row, int64_t col)
{
	m_anchorRow = row;
	m_anchorCol = col;
}

void PopupMenu::setSelectedIndex(int64_t index)
{
	if (!model()) {
		return;
	}

	QModelIndex idx = model()->index(index, 0);
	setCurrentIndex(idx);
	scrollTo(idx);
}

void PopupMenu::updateGeometry()
{
	setGeometry(m_anchorRow, m_anchorCol);
	QListView::updateGeometry();
}

void PopupMenu::setGeometry(int64_t row, int64_t col)
{
	const QSize sizeHintContent = sizeHint();

	if (!m_parentShellWidget) {
		const int width = sizeHintContent.width();
		const int height = sizeHintContent.height();

		qDebug() << "Fallback: PUM anchored to 0,0 without parent shell_widget!";
		return QListView::setGeometry(0, 0, width, height);
	}

	const int cell_width = m_parentShellWidget->cellSize().width();
	const int min_width = 20 * cell_width;
	const int total_width = m_parentShellWidget->columns() * cell_width;

	// Compute default width properties (anchor_x, width)
	int width = sizeHintContent.width();
	int anchor_x = col * cell_width;

	// Scrollbar visibility depends on content, increase width when necessary.
	const QScrollBar* vScrollBar{ verticalScrollBar() };
	if (vScrollBar && vScrollBar->isVisible())
	{
		width += vScrollBar->size().width();
	}

	// PUM must fit within available space to the right of anchor_x
	if (anchor_x + width > total_width)
	{
		width = total_width - anchor_x;

		// PUM should never go below minimum width
		if (width < min_width)
		{
			anchor_x = 0;
			width = qMin(total_width, sizeHintContent.width());
		}
	}

	const int cell_height = m_parentShellWidget->cellSize().height();
	const int min_height = 15 * cell_height;
	const int space_above_row = row * cell_height + 1;
	const int space_below_row =
		(m_parentShellWidget->rows() - row - 2) * cell_height + 1;

	// Compute default height properties (anchor_y, height)
	int height = sizeHintContent.height();
	int anchor_y = (row + 1) * cell_height;

	if (height < space_below_row) {
		// PUM defaults work fine. Keep this case.
	}
	else if (space_below_row >= min_height) {
		// Truncate PUM to space available below anchor.
		height = space_below_row;
	}
	else if (height < space_above_row) {
		// Space available for PUM above anchor.
		anchor_y = (row - 1) * cell_height - height;
	}
	else if (space_above_row > space_below_row) {
		// Not enough space for min_height, more space above.
		anchor_y = 0;
		height = space_above_row;
	}
	else {
		// Not enough space for min_height, more space below.
		height = space_below_row;
		anchor_y = (row + 1) * cell_height;
	}

	return QListView::setGeometry(anchor_x, anchor_y, width, height);
}

} // Namespace
