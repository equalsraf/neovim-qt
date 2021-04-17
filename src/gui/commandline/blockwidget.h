#pragma once

#include <QTextEdit>

namespace NeovimQt { namespace Commandline {

class BlockWidget : public QTextEdit {
	using Super = QTextEdit;

public:
	BlockWidget();

	virtual QSize sizeHint() const override;
};

} } // namespace NeovimQt::Commandline
