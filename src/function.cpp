#include "function.h"
#include <QMetaMethod>
#include <QStringList>

namespace NeovimQt {

bool Function::operator==(const Function& other) const noexcept
{
	if (m_name != other.m_name) {
		return false;
	}

	if (m_returnType != other.m_returnType) {
		return false;
	}

	if (m_parameters.size() != other.m_parameters.size()) {
		return false;
	}

	return std::equal(m_parameters.begin(), m_parameters.end(), other.m_parameters.begin());
}

Function Function::fromVariant(const QVariant& fun) noexcept
{
	Function f;
	if (!fun.canConvert<QVariantMap>()) {
		qDebug() << "Found unexpected data type when unpacking function" << fun;
		return f;
	}

	const QVariantMap m{ fun.toMap() };
	for (auto it = m.begin(); it != m.end(); ++it) {
		if (it.key() == "return_type") {
			if (!it.value().canConvert<QByteArray>()) {
				qDebug() << "Found unexpected data type when unpacking function" << fun;
				return f;
			}
			f.m_returnType = QString::fromUtf8(it.value().toByteArray());
		}
		else if (it.key() == "name") {
			if (!it.value().canConvert<QByteArray>()) {
				qDebug() << "Found unexpected data type when unpacking function" << fun;
				return f;
			}
			f.m_name = QString::fromUtf8(it.value().toByteArray());
		}
		else if (it.key() == "can_fail") {
			if (!it.value().canConvert<bool>()) {
				qDebug() << "Found unexpected data type when unpacking function" << fun;
				return f;
			}
			f.m_canFail = it.value().toBool();
		}
		else if (it.key() == "parameters") {
			if (!it.value().canConvert<QVariantList>()) {
				qDebug() << "Found unexpected data type when unpacking function" << fun;
				return f;
			}
			f.m_parameters = parseParameters(it.value().toList());
		}
		else if (it.key() == "id") {
			// Deprecated
		}
		else if (it.key() == "receives_channel_id") {
			// Internal
		}
		else if (it.key() == "impl_name") {
			// Internal
		}
		else if (it.key() == "method") {
			// Internal
		}
		else if (it.key() == "noeval") {
			// API only function
		}
		else if (it.key() == "deferred" || it.key() == "async") {
			// Internal, "deferred" renamed "async" in neovim/ccdeb91
		}
		else if (it.key() == "deprecated_since" || it.key() == "since") {
			// Creation/Deprecation
		}
		else {
			qDebug() << "Unsupported function attribute"<< it.key() << it.value();
		}
	}

	f.m_valid = true;
	return f;
}

QVector<QStringPair> Function::parseParameters(const QVariantList& obj) noexcept
{
	QVector<QStringPair> result;

	for (const auto& val : obj) {
		const QVariantList& params = val.toList();
		if (params.size() % 2 != 0) {
			return {};
		}

		for (auto it = params.begin(); it != params.begin(); it += 2) {
			auto first{ it };
			auto second{ it + 1 };

			if (!first->canConvert<QByteArray>() || !second->canConvert<QByteArray>()) {
				return {};
			}

			QStringPair arg{ QString::fromUtf8(first->toByteArray()),
				QString::fromUtf8(second->toByteArray()) };

			result.append(arg);
		}
	}

	return result;
}

QString Function::signature() const noexcept
{
	QStringList sigparams;
	for (const auto& p : m_parameters) {
		sigparams.append(QStringLiteral("%1 %2").arg(p.first, p.second));
	}

	QString notes;
	if (m_canFail) {
		notes += " !fail";
	}

	return QStringLiteral("%1 %2(%3)%4").arg(m_returnType, m_name, sigparams.join(", "), notes);
}

} // Namespace

