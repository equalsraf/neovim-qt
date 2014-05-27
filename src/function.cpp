#include "function.h"
#include <QMetaMethod>
#include <QStringList>
#include "util.h"

namespace NeoVimQt {

#include "auto/function_static.cpp"


Function::Function()
:can_fail(false)
{

}

Function::Function(const QByteArray& ret, const QByteArray& name, QList<QByteArray> params, bool can_fail)
{
	this->return_type = ret;
	this->name = name;
	this->parameterTypes = params;
	this->can_fail = can_fail;
}

bool Function::operator==(const Function& other)
{
	if ( this->name != other.name ) {
		return false;
	}

	if ( this->can_fail != other.can_fail ) {
		qDebug() << __func__ << *this << other;
		return false;
	}

	if ( this->return_type != other.return_type ) {
		return false;
	}
	if ( this->parameterTypes != other.parameterTypes ) {
		return false;
	}
	return true;
}

} // Namespace

