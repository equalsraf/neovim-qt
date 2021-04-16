#pragma once

#include <QTextEdit>

namespace NeovimQt { namespace Commandline {

class BlockDisplay : public QTextEdit {
	using Super = QTextEdit;

public:
	BlockDisplay();

	virtual QSize sizeHint() const override;
	virtual int GetMaxLineLength() const;
};

} } // namespace NeovimQt::Commandline
