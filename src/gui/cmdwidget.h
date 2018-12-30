#ifndef NEOVIM_QT_CMDWIDGET
#define NEOVIM_QT_CMDWIDGET

#include <QTextEdit>

#include "input.h"
#include "neovimconnector.h"

namespace NeovimQt {

// A Widget that complies to Neovim ui-cmdline API
class CmdWidget: public QTextEdit {
	Q_OBJECT
public:
	CmdWidget(QWidget *parent=nullptr);

public slots:

signals:
	void reconnectNeovim();

protected:
	virtual void keyPressEvent(QKeyEvent *ev) Q_DECL_OVERRIDE;

private:
};

} // Namespace

#endif
