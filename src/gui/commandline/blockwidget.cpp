#include "blockwidget.h"

#include <QList>
#include <QDebug>
#include <QStringList>

namespace NeovimQt { namespace Commandline {

BlockWidget::BlockWidget()
{
	setVisible(false);
	setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
	setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
	setReadOnly(true);
	setFocusPolicy(Qt::NoFocus);
}

QSize BlockWidget::sizeHint() const
{
	QSize sizeHint{ Super::sizeHint() };

	int padding{ contentsMargins().top() + contentsMargins().bottom() };
	sizeHint.setHeight(document()->size().height() + padding);
	return sizeHint;
}

} } // namespace NeovimQt::Commandline
