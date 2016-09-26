#ifndef NEOVIM_QT_COMPAT
#define NEOVIM_QT_COMPAT

#include "auto/neovim.h"

namespace NeovimQt {

class Neovim015 {
public:
	MsgpackRequest* nvim_ui_attach(Neovim *nvim, int64_t width, int64_t height, const QVariantMap& options);
};

}; // Namespace
#endif
