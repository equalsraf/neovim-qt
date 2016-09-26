#include "compat.h"

namespace NeovimQt {

MsgpackRequest* Neovim015::nvim_ui_attach(Neovim *nvim, int64_t width, int64_t height, const QVariantMap& options) {
	bool rgb = options.value("rgb", false).toBool();
	return nvim->ui_attach(width, height, rgb);
}

}; // Namespace

