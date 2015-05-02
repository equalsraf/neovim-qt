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
bool decode(const QVariant& in, QList<int64_t>& out);
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
