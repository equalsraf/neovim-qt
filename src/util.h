#ifndef NEOVIM_QT_UTIL
#define NEOVIM_QT_UTIL

#include <QDebug>
#include <msgpack.h>
#include "function.h"

QDebug operator<<(QDebug dbg, const msgpack_object&);
QDebug operator<<(QDebug dbg, const NeovimQt::Function& f);

namespace NeovimQt {

// Methods used to turn QVariant into native types,
// used by neovim.cpp to decode QVariants
template <class T>
bool decode(const QVariant& in, QList<T>& out)
{
	out.clear();
	if ((QMetaType::Type)in.type() != QMetaType::QVariantList) {
		qWarning() << "Attempting to decode as QList<...> when type is" << in.type() << in;
		return true;
	}
	foreach(const QVariant val, in.toList()) {
		if (!val.canConvert<T>()) {
			return false;
		}
	}
	foreach(const QVariant val, in.toList()) {
		out.append(val.value<T>());
	}
	return false;
}
bool decode(const QVariant& in, QVariant& out);
template <class T>
bool decode(const QVariant& in, T& out) {
	if (!in.canConvert<T>()) {
		return true;
	}
	out = in.value<T>();
	return false;
}

}

#endif
