#pragma once

#include <QByteArray>
#include <QDebug>
#include <QVector>
#include <QPair>
#include <QPoint>
#include <QStringList>
#include <QtGlobal>

namespace NeovimQt {

using QStringPair = QPair<QString, QString>;

/// Representation of a Neovim API function signature
class Function {
public:
	/// FIXME! Comment Invalid?
	Function() noexcept = default;

	/// Construct new function with the given return type, name, parameters and error
	Function(QString ret, QString name, QVector<QStringPair> params, bool can_fail) noexcept
		: m_canFail{ can_fail }
		, m_valid{ true }
		, m_name{ std::move(name) }
		, m_returnType{ std::move(ret) }
		, m_parameters{ params }
	{
	}

	/// Construct new function with the given return type, name, parameters and error
	Function(QString ret, QString name, QVector<QString> paramTypes, bool can_fail) noexcept
		: m_canFail{ can_fail }
		, m_valid{ true }
		, m_name{ std::move(name) }
		, m_returnType{ std::move(ret) }
	{
		for (const auto& type : paramTypes) {
			m_parameters.append({ type, {} });
		}
	}

	/// Returns true if this function has all the necessary attributes
	bool isValid() const noexcept { return m_valid; }

	/// FIXME Comment
	bool canFail() const noexcept { return m_canFail; }

	/// Function name
	const QString& name() const noexcept { return m_name; }

	/// Function parameter types and name
	const QVector<QStringPair>& parameters() const noexcept { return m_parameters; }

	/// Function return type
	const QString& returnType() const noexcept { return m_returnType; }

	QString signature() const noexcept;

	/// Two functions are considered identical if their names
	/// argument and return types, and error status are identical
	bool operator==(const Function& other) const noexcept;
	static Function fromVariant(const QVariant& fun) noexcept;

	/// Retrieve parameter types from a list of function parameters in the metadata
	/// object. Basically retrieves the even numbered elements of the array (types)
	/// i.e. [Type0 name0 Type1 name1 ... ] -> [Type0 Type1 ...]
	static QVector<QStringPair> parseParameters(const QVariantList& obj) noexcept;

	/// The static list **knowFunctions** holds a list of all the supported
	/// signature. The list is populated at compile time from a code generator.
	static QVector<Function> knownFunctions;

private:
	bool m_canFail{ false };
	bool m_valid{ false };
	QString m_name;
	QString m_returnType;
	QVector<QStringPair> m_parameters;
};

} // namespace NeovimQt
