#include "popupmenu.h"
#include "popupmenumodel.h"

#include <QScrollBar>
#include <QPainter>

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

PopupMenuDelegate::PopupMenuDelegate(QObject *parent)
:QStyledItemDelegate(parent)
{
}

QSize PopupMenuDelegate::sizeHint(const QStyleOptionViewItem &  option ,
                                        const QModelIndex & index) const
{
    if(!index.isValid())
        return QSize();
 
    QString text = index.data(PopupMenuModel::Text).toString();
    QFontMetrics fm(option.font);
 
    QRect rect = fm.boundingRect(0, 0,
								   option.rect.width(), 0,
								   Qt::AlignLeft|Qt::AlignTop,
								   text);
    QSize size(rect.width(), rect.height() + 4);
    return size;
}
 
void PopupMenuDelegate::paint(QPainter *painter,
                                    const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const
{
    if(!index.isValid())
        return;
 
    painter->save();
 
    if (option.state & QStyle::State_Selected)
        painter->fillRect(option.rect, option.palette.highlight());
 
    QString text = index.data(PopupMenuModel::Text).toString();
    painter->drawText(option.rect, Qt::AlignLeft|Qt::AlignTop, text);
 
    painter->restore();
}


} // Namespace
