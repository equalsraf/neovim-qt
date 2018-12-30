#include "cmdwidget.h"

namespace NeovimQt {

CmdWidget::CmdWidget(QWidget *parent) {
    setParent(parent);
    setReadOnly(true);
}

void CmdWidget::keyPressEvent(QKeyEvent *ev)
{
    QWidget::keyPressEvent(ev);
}

}  // namespace NeovimQt
