#include "cmdwidget.h"

namespace NeovimQt {

CmdWidget::CmdWidget(QWidget *parent) {
    setParent(parent);
}

void CmdWidget::keyPressEvent(QKeyEvent *ev)
{
    QWidget::keyPressEvent(ev);

	// QString inp = Input.convertKey(ev->text(), ev->key(), ev->modifiers());
	// if (inp.isEmpty()) {
	// 	QWidget::keyPressEvent(ev);
	// 	return;
	// }

	// m_nvim->api0()->vim_input(m_nvim->encode(inp));
	// FIXME: bytes might not be written, and need to be buffered
}
}  // namespace NeovimQt