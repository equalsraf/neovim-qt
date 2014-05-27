#ifndef NEOVIM_QT_UTIL
#define NEOVIM_QT_UTIL

#include <QDebug>
#include <msgpack.h>
#include "function.h"

QDebug operator<<(QDebug dbg, const msgpack_object&);
QDebug operator<<(QDebug dbg, const NeoVimQt::Function& f);

namespace NeoVimQt {
bool checkVariant(const QVariant&);
};

#endif
