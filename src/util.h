#ifndef NEOVIM_QT_UTIL
#define NEOVIM_QT_UTIL

#include <QDebug>
#include <msgpack.h>
#include "function.h"

QDebug operator<<(QDebug dbg, const msgpack_object&);
QDebug operator<<(QDebug dbg, const NeovimQt::Function& f);

namespace NeovimQt {

bool checkVariant(const QVariant&);
bool decodeMsgpack(const msgpack_object& in, bool& out);
bool decodeMsgpack(const msgpack_object& in, QByteArray& out);
bool decodeMsgpack(const msgpack_object& in, QPoint& out);
bool decodeMsgpack(const msgpack_object& in, int64_t& out);
bool decodeMsgpack(const msgpack_object& in, QList<int64_t>& out);
bool decodeMsgpack(const msgpack_object& in, QList<QByteArray>& out);
bool decodeMsgpack(const msgpack_object& in, QVariant& out);

}

#endif
