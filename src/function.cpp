#include "function.h"
#include <QMetaMethod>
#include <QStringList>
#include "util.h"

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

/**
 * Deserialise a msgpack function description
 */
Function Function::fromMsgpack(const msgpack_object& fun)
{
	Function f;
	if ( fun.type != MSGPACK_OBJECT_MAP ) {
		qDebug() << "Found unexpected data type when unpacking function" << fun;
		return f;
	}

	for (uint32_t i=0; i<fun.via.map.size; i++) {
		QByteArray key;
		if (decodeMsgpack(fun.via.map.ptr[i].key, key)) {
			qDebug() << "Found unexpected data type when unpacking function" << fun;
			return f;
		}
		msgpack_object& val = fun.via.map.ptr[i].val;
		if ( key == "return_type" ) {
			if ( val.type != MSGPACK_OBJECT_BIN ) {
				qDebug() << "Found unexpected data type when unpacking function" << fun;
				return f;
			}
			f.return_type = QString::fromUtf8(val.via.bin.ptr, val.via.bin.size);
		} else if ( key == "name" ) {
			if ( val.type != MSGPACK_OBJECT_BIN ) {
				qDebug() << "Found unexpected data type when unpacking function" << fun;
				return f;
			}
			f.name = QString::fromUtf8(val.via.bin.ptr, val.via.bin.size);
		} else if ( key == "can_fail" ) {
			if ( val.type != MSGPACK_OBJECT_BOOLEAN ) {
				qDebug() << "Found unexpected data type when unpacking function" << fun;
				return f;
			}
			f.can_fail = val.via.boolean;
		} else if ( key == "parameters" ) {
			if ( val.type != MSGPACK_OBJECT_ARRAY ) {
				qDebug() << "Found unexpected data type when unpacking function" << fun;
				return f;
			}
			f.parameters = parseParameters(val);
		} else if ( key == "id" ) {
			// Deprecated
		} else if ( key == "receives_channel_id" ) {
			// Internal
		} else if ( key == "deferred" ) {
			// Internal
		} else {
			qWarning() << "Unsupported function attribute"<< key << val.type;
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
QList<QPair<QString,QString> > Function::parseParameters(const msgpack_object& obj)
{
	QList<QPair<QString,QString> > fail;
	if ( obj.type != MSGPACK_OBJECT_ARRAY ) {
		return fail;
	}

	QList<QPair<QString,QString> > res;
	for (uint32_t i=0; i<obj.via.array.size; i++) {
		msgpack_object& param = obj.via.array.ptr[i];
		if ( param.type != MSGPACK_OBJECT_ARRAY ) {
			return fail;
		}

		if ( param.via.array.size % 2 != 0 ) {
			return fail;
		}

		for (uint32_t j=0; j<param.via.array.size; j+=2) {
			QByteArray type, name;
			if (decodeMsgpack(param.via.array.ptr[j], type)) {
				return fail;
			}
			if (decodeMsgpack(param.via.array.ptr[j+1], name)) {
				return fail;
			}
			QPair<QString,QString> arg(type, name);
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

