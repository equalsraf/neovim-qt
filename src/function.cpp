#include "function.h"
#include <QMetaMethod>
#include <QStringList>

namespace NeovimQt {

typedef QPair<QString,QString> StringPair;

/**
 * \class NeovimQt::Function
 *
 * \brief Representation of a Neovim API function signature
 */

/**
 * \enum Function::FunctionId
 *
 * Neovim API function identifiers, the list NeovimQt::Function::knownFunctions
 * is indexed with this enum.
 */

#ifndef NEOVIMQT_NO_AUTO
#include "auto/function_static.cpp"
#endif

/**
 * Construct invalid function
 */
Function::Function()
:can_fail(false), m_valid(false)
{
}

/**
 * Construct new function with the given return type, name, parameters and error
 */
Function::Function(const QString& ret, const QString& name, QList<QPair<QString,QString> > params, bool can_fail)
:m_valid(true)
{
	this->return_type = ret;
	this->name = name;
	this->parameters = params;
	this->can_fail = can_fail;
}

/**
 * Construct new function with the given return type, name, parameters and error
 */
Function::Function(const QString& ret, const QString& name, QList<QString> paramTypes, bool can_fail)
:m_valid(true)
{
	this->return_type = ret;
	this->name = name;
	foreach (QString type, paramTypes) {
		this->parameters .append(QPair<QString,QString>(type, ""));
	}
	this->can_fail = can_fail;
}

/**
 * Returns true if this function has all the necessary attributes
 */
bool Function::isValid() const
{
	return m_valid;
}

/**
 * Two functions are considered identical if their names
 * argument and return types, and error status are identical
 */
bool Function::operator==(const Function& other)
{
	if ( this->name != other.name ) {
		return false;
	}

	if ( this->can_fail != other.can_fail ) {
		return false;
	}

	if ( this->return_type != other.return_type ) {
		return false;
	}
	if (this->parameters.size() != other.parameters.size()) {
		return false;
	}
	for (int i=0; i<this->parameters.size(); i++) {
		if ( this->parameters.at(i).first != other.parameters.at(i).first ) {
			return false;
		}
	}
	return true;
}

Function Function::fromVariant(const QVariant& fun)
{
	Function f;
	if (!fun.canConvert<QVariantMap>()) {
		qDebug() << "Found unexpected data type when unpacking function" << fun;
		return f;
	}

	const QVariantMap& m = fun.toMap();
	QMapIterator<QString,QVariant> it(m);
	while(it.hasNext()) {
		it.next();

		if ( it.key() == "return_type" ) {
			if (!it.value().canConvert<QByteArray>()) {
				qDebug() << "Found unexpected data type when unpacking function" << fun;
				return f;
			}
			f.return_type = QString::fromUtf8(it.value().toByteArray());
		} else if ( it.key() == "name" ) {
			if (!it.value().canConvert<QByteArray>()) {
				qDebug() << "Found unexpected data type when unpacking function" << fun;
				return f;
			}
			f.name = QString::fromUtf8(it.value().toByteArray());
		} else if ( it.key() == "can_fail" ) {
			if (!it.value().canConvert<bool>()) {
				qDebug() << "Found unexpected data type when unpacking function" << fun;
				return f;
			}
			f.can_fail = it.value().toBool();
		} else if ( it.key() == "parameters" ) {
			if (!it.value().canConvert<QVariantList>()) {
				qDebug() << "Found unexpected data type when unpacking function" << fun;
				return f;
			}
			f.parameters = parseParameters(it.value().toList());
		} else if ( it.key() == "id" ) {
			// Deprecated
		} else if ( it.key() == "receives_channel_id" ) {
			// Internal
		} else if ( it.key() == "deferred" || it.key() == "async" ) {
			// Internal, "deferred" renamed "async" in neovim/ccdeb91
		} else {
			qWarning() << "Unsupported function attribute"<< it.key() << it.value();
		}
	}

	f.m_valid = true;
	return f;
}

/**
 * Retrieve parameter types from a list of function parameters in the metadata
 * object. Basically retrieves the even numbered elements of the array (types)
 * i.e. [Type0 name0 Type1 name1 ... ] -> [Type0 Type1 ...]
 *
 */
QList<QPair<QString,QString> > Function::parseParameters(const QVariantList& obj)
{
	QList<QPair<QString,QString> > fail;
	QList<QPair<QString,QString> > res;
	foreach(const QVariant& val, obj) {

		const QVariantList& params = val.toList();
		if ( params.size() % 2 != 0 ) {
			return fail;
		}

		for (int j=0; j<params.size(); j+=2) {
			QByteArray type, name;
			if (!params.at(j).canConvert<QByteArray>() || 
					!params.at(j+1).canConvert<QByteArray>()) {
				return fail;
			}
			QPair<QString,QString> arg(
					QString::fromUtf8(params.at(j).toByteArray()),
					QString::fromUtf8(params.at(j+1).toByteArray()));
			res.append(arg);
		}
	}
	return res;
}

QString Function::signature() const
{
	QStringList sigparams;
	foreach(const StringPair p, parameters) {
		sigparams.append(QString("%1 %2").arg(p.first).arg(p.second));
	}

	QString notes;
	if (can_fail) {
		notes += " !fail";
	}
	return  QString("%1 %2(%3)%4").arg(return_type).arg(name).arg(sigparams.join(", ")).arg(notes);
}

/**
 * return the FunctionId or NEOVIM_FN_NULL if the
 * function is uknown
 */
Function::FunctionId Function::functionId(const Function& f)
{
	if ( !f.isValid() ) {
		return Function::NEOVIM_FN_NULL;
	}
	int index = Function::knownFunctions.indexOf(f);
	if ( index != -1 ) {
		return Function::FunctionId(index);
	}
	qDebug() << "Unknown Neovim function" << f.signature();
	return Function::NEOVIM_FN_NULL;
}


} // Namespace

