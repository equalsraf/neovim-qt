#pragma once

#include <QString>
#include <QTextLayout>
#include <QVariantList>

namespace NeovimQt { namespace Commandline {

class LineModel final {
public:
	LineModel(
		const QVariantList& content,
		int pos,
		const QString& firstc,
		const QString& prompt,
		int indent,
		int level) noexcept;

	QString getPromptText() const noexcept;

	int getIndent() const noexcept
	{
		return m_indent;
	}

private:
	QString m_firstc;
	QString m_prompt;
	QString m_content;
	int m_indent{ 0 };
	int m_position{ 0 };
	int m_level{ 0 };
};

} } // namespace NeovimQt::Commandline
