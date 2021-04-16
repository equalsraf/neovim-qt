#include "blockdisplay.h"

#include <QList>
#include <QDebug>
#include <QStringList>

namespace NeovimQt { namespace Commandline {

BlockDisplay::BlockDisplay()
{
	setVisible(false);
	setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
	setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
	setReadOnly(true);
	setFocusPolicy(Qt::NoFocus);
}

QSize BlockDisplay::sizeHint() const
{
	QSize parentSizeHint = Super::sizeHint();
	int padding = contentsMargins().top() + contentsMargins().bottom();
	parentSizeHint.setHeight(document()->size().height() + padding);
	return parentSizeHint;
}

int BlockDisplay::GetMaxLineLength() const
{
	QStringList allLines = toPlainText().split('\n');

	int maxChars = 0;
	for (const auto& line : allLines) {
		maxChars = qMax(line.size(), maxChars);
	}

	return maxChars;
}

} } // namespace NeovimQt::Commandline
