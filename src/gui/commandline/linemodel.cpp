#include "linemodel.h"

#include <QDebug>

namespace NeovimQt { namespace Commandline {

LineModel::LineModel(
	const QVariantList& content,
	int pos,
	const QString& firstc,
	const QString& prompt,
	int indent,
	int level) noexcept
	: m_firstc{ firstc }
	, m_indent{ indent }
	, m_position{ pos }
	, m_level{ level }
{
	for (const auto& varLine : content) {
		if (static_cast<QMetaType::Type>(varLine.type()) != QMetaType::QVariantList) {
			qDebug() << "Unexpected item in 'content'" << varLine;
		}

		const QVariantList line = varLine.toList();
		if (line.size() < 2
			|| !line.at(0).canConvert<uint64_t>()
			|| !line.at(1).canConvert<QString>()) {
			qWarning() << "Unexpected format for item in list 'content':" << line;
		}

		const QString content_text = line.at(1).toString();

		m_content += content_text;
	}
}

QString LineModel::getPromptText() const noexcept
{
	const QString indentString{ static_cast<int>(m_indent), ' ' };
	const QString spaceForCursor{ QStringLiteral(" ") };

	return { m_firstc + indentString + m_content + spaceForCursor };
}

}  } // namespace NeovimQt::Commandline
