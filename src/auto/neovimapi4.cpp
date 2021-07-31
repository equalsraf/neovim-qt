// Auto generated 2021-07-31 13:13:49.628227 from nvim API level:4
#include "auto/neovimapi4.h"

#include "msgpackiodevice.h"
#include "msgpackrequest.h"
#include "neovimconnector.h"
#include "util.h"

namespace NeovimQt {

// Unpack Neovim EXT types Window, Buffer Tabpage which are all uint64_t see Neovim:msgpack_rpc_to_
static QVariant unpackBufferApi4(MsgpackIODevice *dev, const char* in, quint32 size) noexcept
{
	msgpack_unpacked result;
	msgpack_unpacked_init(&result);
	msgpack_unpack_return ret = msgpack_unpack_next(&result, in, size, NULL);

	QVariant variant;

	if (ret == MSGPACK_UNPACK_SUCCESS) {
		switch (result.data.type) {
			case MSGPACK_OBJECT_NEGATIVE_INTEGER:
				variant = (qint64)result.data.via.i64;
				break;
			case MSGPACK_OBJECT_POSITIVE_INTEGER:
				variant = (quint64)result.data.via.u64;
				break;
			default:
				// TODO it would be nice if we could call back MsgpackIoDevice method or primitive types here
				qWarning() << "Unsupported type found for EXT type" << result.data.type << result.data;
		}
	}

	msgpack_unpacked_destroy(&result);
	return variant;
}

static QVariant unpackWindowApi4(MsgpackIODevice *dev, const char* in, quint32 size) noexcept
{
	return unpackBufferApi4(dev, in, size);
}

static QVariant unpackTabpageApi4(MsgpackIODevice* dev, const char* in, quint32 size) noexcept
{
	return unpackBufferApi4(dev, in, size);
}

NeovimApi4::NeovimApi4(NeovimConnector* c) noexcept
	: m_c{ c }
{
	// EXT types
	m_c->m_dev->registerExtType(0, unpackBufferApi4);
	m_c->m_dev->registerExtType(1, unpackWindowApi4);
	m_c->m_dev->registerExtType(2, unpackTabpageApi4);

	connect(m_c->m_dev, &MsgpackIODevice::notification,
		this, &NeovimApi4::neovimNotification);
}

// Slots
MsgpackRequest* NeovimApi4::nvim_buf_line_count(int64_t buffer) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_buf_line_count", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_BUF_LINE_COUNT);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(buffer);
	return r;
}
MsgpackRequest* NeovimApi4::buffer_get_line(int64_t buffer, int64_t index) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("buffer_get_line", 2) };
	r->setFunction(NeovimApi4::NEOVIM_FN_BUFFER_GET_LINE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(buffer);
	m_c->m_dev->send(index);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_buf_attach(int64_t buffer, bool send_buffer, QVariantMap opts) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_buf_attach", 3) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_BUF_ATTACH);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(buffer);
	m_c->m_dev->send(send_buffer);
	m_c->m_dev->send(opts);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_buf_detach(int64_t buffer) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_buf_detach", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_BUF_DETACH);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(buffer);
	return r;
}
MsgpackRequest* NeovimApi4::buffer_set_line(int64_t buffer, int64_t index, QByteArray line) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("buffer_set_line", 3) };
	r->setFunction(NeovimApi4::NEOVIM_FN_BUFFER_SET_LINE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(buffer);
	m_c->m_dev->send(index);
	m_c->m_dev->send(line);
	return r;
}
MsgpackRequest* NeovimApi4::buffer_del_line(int64_t buffer, int64_t index) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("buffer_del_line", 2) };
	r->setFunction(NeovimApi4::NEOVIM_FN_BUFFER_DEL_LINE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(buffer);
	m_c->m_dev->send(index);
	return r;
}
MsgpackRequest* NeovimApi4::buffer_get_line_slice(int64_t buffer, int64_t start, int64_t end, bool include_start, bool include_end) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("buffer_get_line_slice", 5) };
	r->setFunction(NeovimApi4::NEOVIM_FN_BUFFER_GET_LINE_SLICE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(buffer);
	m_c->m_dev->send(start);
	m_c->m_dev->send(end);
	m_c->m_dev->send(include_start);
	m_c->m_dev->send(include_end);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_buf_get_lines(int64_t buffer, int64_t start, int64_t end, bool strict_indexing) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_buf_get_lines", 4) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_BUF_GET_LINES);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(buffer);
	m_c->m_dev->send(start);
	m_c->m_dev->send(end);
	m_c->m_dev->send(strict_indexing);
	return r;
}
MsgpackRequest* NeovimApi4::buffer_set_line_slice(int64_t buffer, int64_t start, int64_t end, bool include_start, bool include_end, QList<QByteArray> replacement) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("buffer_set_line_slice", 6) };
	r->setFunction(NeovimApi4::NEOVIM_FN_BUFFER_SET_LINE_SLICE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(buffer);
	m_c->m_dev->send(start);
	m_c->m_dev->send(end);
	m_c->m_dev->send(include_start);
	m_c->m_dev->send(include_end);
	m_c->m_dev->sendArrayOf(replacement);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_buf_set_lines(int64_t buffer, int64_t start, int64_t end, bool strict_indexing, QList<QByteArray> replacement) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_buf_set_lines", 5) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_BUF_SET_LINES);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(buffer);
	m_c->m_dev->send(start);
	m_c->m_dev->send(end);
	m_c->m_dev->send(strict_indexing);
	m_c->m_dev->sendArrayOf(replacement);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_buf_get_var(int64_t buffer, QByteArray name) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_buf_get_var", 2) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_BUF_GET_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(buffer);
	m_c->m_dev->send(name);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_buf_get_changedtick(int64_t buffer) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_buf_get_changedtick", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_BUF_GET_CHANGEDTICK);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(buffer);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_buf_get_keymap(int64_t buffer, QByteArray mode) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_buf_get_keymap", 2) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_BUF_GET_KEYMAP);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(buffer);
	m_c->m_dev->send(mode);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_buf_get_commands(int64_t buffer, QVariantMap opts) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_buf_get_commands", 2) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_BUF_GET_COMMANDS);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(buffer);
	m_c->m_dev->send(opts);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_buf_set_var(int64_t buffer, QByteArray name, QVariant value) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_buf_set_var", 3) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_BUF_SET_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(buffer);
	m_c->m_dev->send(name);
	m_c->m_dev->send(value);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_buf_del_var(int64_t buffer, QByteArray name) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_buf_del_var", 2) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_BUF_DEL_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(buffer);
	m_c->m_dev->send(name);
	return r;
}
MsgpackRequest* NeovimApi4::buffer_set_var(int64_t buffer, QByteArray name, QVariant value) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("buffer_set_var", 3) };
	r->setFunction(NeovimApi4::NEOVIM_FN_BUFFER_SET_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(buffer);
	m_c->m_dev->send(name);
	m_c->m_dev->send(value);
	return r;
}
MsgpackRequest* NeovimApi4::buffer_del_var(int64_t buffer, QByteArray name) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("buffer_del_var", 2) };
	r->setFunction(NeovimApi4::NEOVIM_FN_BUFFER_DEL_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(buffer);
	m_c->m_dev->send(name);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_buf_get_option(int64_t buffer, QByteArray name) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_buf_get_option", 2) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_BUF_GET_OPTION);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(buffer);
	m_c->m_dev->send(name);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_buf_set_option(int64_t buffer, QByteArray name, QVariant value) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_buf_set_option", 3) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_BUF_SET_OPTION);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(buffer);
	m_c->m_dev->send(name);
	m_c->m_dev->send(value);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_buf_get_number(int64_t buffer) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_buf_get_number", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_BUF_GET_NUMBER);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(buffer);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_buf_get_name(int64_t buffer) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_buf_get_name", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_BUF_GET_NAME);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(buffer);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_buf_set_name(int64_t buffer, QByteArray name) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_buf_set_name", 2) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_BUF_SET_NAME);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(buffer);
	m_c->m_dev->send(name);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_buf_is_valid(int64_t buffer) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_buf_is_valid", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_BUF_IS_VALID);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(buffer);
	return r;
}
MsgpackRequest* NeovimApi4::buffer_insert(int64_t buffer, int64_t lnum, QList<QByteArray> lines) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("buffer_insert", 3) };
	r->setFunction(NeovimApi4::NEOVIM_FN_BUFFER_INSERT);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(buffer);
	m_c->m_dev->send(lnum);
	m_c->m_dev->sendArrayOf(lines);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_buf_get_mark(int64_t buffer, QByteArray name) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_buf_get_mark", 2) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_BUF_GET_MARK);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(buffer);
	m_c->m_dev->send(name);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_buf_add_highlight(int64_t buffer, int64_t src_id, QByteArray hl_group, int64_t line, int64_t col_start, int64_t col_end) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_buf_add_highlight", 6) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_BUF_ADD_HIGHLIGHT);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(buffer);
	m_c->m_dev->send(src_id);
	m_c->m_dev->send(hl_group);
	m_c->m_dev->send(line);
	m_c->m_dev->send(col_start);
	m_c->m_dev->send(col_end);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_buf_clear_highlight(int64_t buffer, int64_t src_id, int64_t line_start, int64_t line_end) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_buf_clear_highlight", 4) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_BUF_CLEAR_HIGHLIGHT);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(buffer);
	m_c->m_dev->send(src_id);
	m_c->m_dev->send(line_start);
	m_c->m_dev->send(line_end);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_tabpage_list_wins(int64_t tabpage) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_tabpage_list_wins", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_TABPAGE_LIST_WINS);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(tabpage);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_tabpage_get_var(int64_t tabpage, QByteArray name) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_tabpage_get_var", 2) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_TABPAGE_GET_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(tabpage);
	m_c->m_dev->send(name);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_tabpage_set_var(int64_t tabpage, QByteArray name, QVariant value) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_tabpage_set_var", 3) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_TABPAGE_SET_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(tabpage);
	m_c->m_dev->send(name);
	m_c->m_dev->send(value);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_tabpage_del_var(int64_t tabpage, QByteArray name) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_tabpage_del_var", 2) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_TABPAGE_DEL_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(tabpage);
	m_c->m_dev->send(name);
	return r;
}
MsgpackRequest* NeovimApi4::tabpage_set_var(int64_t tabpage, QByteArray name, QVariant value) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("tabpage_set_var", 3) };
	r->setFunction(NeovimApi4::NEOVIM_FN_TABPAGE_SET_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(tabpage);
	m_c->m_dev->send(name);
	m_c->m_dev->send(value);
	return r;
}
MsgpackRequest* NeovimApi4::tabpage_del_var(int64_t tabpage, QByteArray name) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("tabpage_del_var", 2) };
	r->setFunction(NeovimApi4::NEOVIM_FN_TABPAGE_DEL_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(tabpage);
	m_c->m_dev->send(name);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_tabpage_get_win(int64_t tabpage) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_tabpage_get_win", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_TABPAGE_GET_WIN);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(tabpage);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_tabpage_get_number(int64_t tabpage) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_tabpage_get_number", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_TABPAGE_GET_NUMBER);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(tabpage);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_tabpage_is_valid(int64_t tabpage) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_tabpage_is_valid", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_TABPAGE_IS_VALID);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(tabpage);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_ui_attach(int64_t width, int64_t height, QVariantMap options) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_ui_attach", 3) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_UI_ATTACH);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(width);
	m_c->m_dev->send(height);
	m_c->m_dev->send(options);
	return r;
}
MsgpackRequest* NeovimApi4::ui_attach(int64_t width, int64_t height, bool enable_rgb) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("ui_attach", 3) };
	r->setFunction(NeovimApi4::NEOVIM_FN_UI_ATTACH);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(width);
	m_c->m_dev->send(height);
	m_c->m_dev->send(enable_rgb);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_ui_detach() noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_ui_detach", 0) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_UI_DETACH);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_ui_try_resize(int64_t width, int64_t height) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_ui_try_resize", 2) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_UI_TRY_RESIZE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(width);
	m_c->m_dev->send(height);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_ui_set_option(QByteArray name, QVariant value) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_ui_set_option", 2) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_UI_SET_OPTION);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(name);
	m_c->m_dev->send(value);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_command(QByteArray command) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_command", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_COMMAND);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(command);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_get_hl_by_name(QByteArray name, bool rgb) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_get_hl_by_name", 2) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_GET_HL_BY_NAME);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(name);
	m_c->m_dev->send(rgb);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_get_hl_by_id(int64_t hl_id, bool rgb) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_get_hl_by_id", 2) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_GET_HL_BY_ID);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(hl_id);
	m_c->m_dev->send(rgb);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_feedkeys(QByteArray keys, QByteArray mode, bool escape_csi) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_feedkeys", 3) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_FEEDKEYS);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(keys);
	m_c->m_dev->send(mode);
	m_c->m_dev->send(escape_csi);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_input(QByteArray keys) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_input", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_INPUT);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(keys);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_replace_termcodes(QByteArray str, bool from_part, bool do_lt, bool special) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_replace_termcodes", 4) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_REPLACE_TERMCODES);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(str);
	m_c->m_dev->send(from_part);
	m_c->m_dev->send(do_lt);
	m_c->m_dev->send(special);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_command_output(QByteArray command) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_command_output", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_COMMAND_OUTPUT);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(command);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_eval(QByteArray expr) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_eval", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_EVAL);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(expr);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_execute_lua(QByteArray code, QVariantList args) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_execute_lua", 2) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_EXECUTE_LUA);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(code);
	m_c->m_dev->send(args);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_call_function(QByteArray fn, QVariantList args) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_call_function", 2) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_CALL_FUNCTION);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(fn);
	m_c->m_dev->send(args);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_call_dict_function(QVariant dict, QByteArray fn, QVariantList args) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_call_dict_function", 3) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_CALL_DICT_FUNCTION);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(dict);
	m_c->m_dev->send(fn);
	m_c->m_dev->send(args);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_strwidth(QByteArray text) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_strwidth", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_STRWIDTH);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(text);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_list_runtime_paths() noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_list_runtime_paths", 0) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_LIST_RUNTIME_PATHS);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_set_current_dir(QByteArray dir) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_set_current_dir", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_SET_CURRENT_DIR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(dir);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_get_current_line() noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_get_current_line", 0) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_GET_CURRENT_LINE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_set_current_line(QByteArray line) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_set_current_line", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_SET_CURRENT_LINE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(line);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_del_current_line() noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_del_current_line", 0) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_DEL_CURRENT_LINE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_get_var(QByteArray name) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_get_var", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_GET_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(name);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_set_var(QByteArray name, QVariant value) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_set_var", 2) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_SET_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(name);
	m_c->m_dev->send(value);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_del_var(QByteArray name) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_del_var", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_DEL_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(name);
	return r;
}
MsgpackRequest* NeovimApi4::vim_set_var(QByteArray name, QVariant value) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("vim_set_var", 2) };
	r->setFunction(NeovimApi4::NEOVIM_FN_VIM_SET_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(name);
	m_c->m_dev->send(value);
	return r;
}
MsgpackRequest* NeovimApi4::vim_del_var(QByteArray name) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("vim_del_var", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_VIM_DEL_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(name);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_get_vvar(QByteArray name) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_get_vvar", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_GET_VVAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(name);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_get_option(QByteArray name) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_get_option", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_GET_OPTION);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(name);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_set_option(QByteArray name, QVariant value) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_set_option", 2) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_SET_OPTION);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(name);
	m_c->m_dev->send(value);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_out_write(QByteArray str) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_out_write", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_OUT_WRITE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(str);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_err_write(QByteArray str) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_err_write", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_ERR_WRITE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(str);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_err_writeln(QByteArray str) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_err_writeln", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_ERR_WRITELN);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(str);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_list_bufs() noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_list_bufs", 0) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_LIST_BUFS);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_get_current_buf() noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_get_current_buf", 0) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_GET_CURRENT_BUF);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_set_current_buf(int64_t buffer) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_set_current_buf", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_SET_CURRENT_BUF);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(buffer);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_list_wins() noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_list_wins", 0) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_LIST_WINS);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_get_current_win() noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_get_current_win", 0) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_GET_CURRENT_WIN);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_set_current_win(int64_t window) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_set_current_win", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_SET_CURRENT_WIN);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(window);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_list_tabpages() noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_list_tabpages", 0) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_LIST_TABPAGES);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_get_current_tabpage() noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_get_current_tabpage", 0) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_GET_CURRENT_TABPAGE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_set_current_tabpage(int64_t tabpage) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_set_current_tabpage", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_SET_CURRENT_TABPAGE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(tabpage);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_subscribe(QByteArray event) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_subscribe", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_SUBSCRIBE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(event);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_unsubscribe(QByteArray event) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_unsubscribe", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_UNSUBSCRIBE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(event);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_get_color_by_name(QByteArray name) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_get_color_by_name", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_GET_COLOR_BY_NAME);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(name);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_get_color_map() noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_get_color_map", 0) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_GET_COLOR_MAP);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_get_mode() noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_get_mode", 0) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_GET_MODE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_get_keymap(QByteArray mode) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_get_keymap", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_GET_KEYMAP);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(mode);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_get_commands(QVariantMap opts) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_get_commands", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_GET_COMMANDS);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(opts);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_get_api_info() noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_get_api_info", 0) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_GET_API_INFO);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_set_client_info(QByteArray name, QVariantMap version, QByteArray type, QVariantMap methods, QVariantMap attributes) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_set_client_info", 5) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_SET_CLIENT_INFO);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(name);
	m_c->m_dev->send(version);
	m_c->m_dev->send(type);
	m_c->m_dev->send(methods);
	m_c->m_dev->send(attributes);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_get_chan_info(int64_t chan) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_get_chan_info", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_GET_CHAN_INFO);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(chan);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_list_chans() noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_list_chans", 0) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_LIST_CHANS);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_call_atomic(QVariantList calls) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_call_atomic", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_CALL_ATOMIC);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(calls);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_parse_expression(QByteArray expr, QByteArray flags, bool highlight) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_parse_expression", 3) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_PARSE_EXPRESSION);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(expr);
	m_c->m_dev->send(flags);
	m_c->m_dev->send(highlight);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_list_uis() noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_list_uis", 0) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_LIST_UIS);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_get_proc_children(int64_t pid) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_get_proc_children", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_GET_PROC_CHILDREN);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(pid);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_get_proc(int64_t pid) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_get_proc", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_GET_PROC);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(pid);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_win_get_buf(int64_t window) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_win_get_buf", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_WIN_GET_BUF);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(window);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_win_get_cursor(int64_t window) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_win_get_cursor", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_WIN_GET_CURSOR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(window);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_win_set_cursor(int64_t window, QPoint pos) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_win_set_cursor", 2) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_WIN_SET_CURSOR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(window);
	m_c->m_dev->send(pos);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_win_get_height(int64_t window) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_win_get_height", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_WIN_GET_HEIGHT);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(window);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_win_set_height(int64_t window, int64_t height) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_win_set_height", 2) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_WIN_SET_HEIGHT);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(window);
	m_c->m_dev->send(height);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_win_get_width(int64_t window) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_win_get_width", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_WIN_GET_WIDTH);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(window);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_win_set_width(int64_t window, int64_t width) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_win_set_width", 2) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_WIN_SET_WIDTH);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(window);
	m_c->m_dev->send(width);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_win_get_var(int64_t window, QByteArray name) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_win_get_var", 2) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_WIN_GET_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(window);
	m_c->m_dev->send(name);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_win_set_var(int64_t window, QByteArray name, QVariant value) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_win_set_var", 3) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_WIN_SET_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(window);
	m_c->m_dev->send(name);
	m_c->m_dev->send(value);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_win_del_var(int64_t window, QByteArray name) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_win_del_var", 2) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_WIN_DEL_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(window);
	m_c->m_dev->send(name);
	return r;
}
MsgpackRequest* NeovimApi4::window_set_var(int64_t window, QByteArray name, QVariant value) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("window_set_var", 3) };
	r->setFunction(NeovimApi4::NEOVIM_FN_WINDOW_SET_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(window);
	m_c->m_dev->send(name);
	m_c->m_dev->send(value);
	return r;
}
MsgpackRequest* NeovimApi4::window_del_var(int64_t window, QByteArray name) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("window_del_var", 2) };
	r->setFunction(NeovimApi4::NEOVIM_FN_WINDOW_DEL_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(window);
	m_c->m_dev->send(name);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_win_get_option(int64_t window, QByteArray name) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_win_get_option", 2) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_WIN_GET_OPTION);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(window);
	m_c->m_dev->send(name);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_win_set_option(int64_t window, QByteArray name, QVariant value) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_win_set_option", 3) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_WIN_SET_OPTION);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(window);
	m_c->m_dev->send(name);
	m_c->m_dev->send(value);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_win_get_position(int64_t window) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_win_get_position", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_WIN_GET_POSITION);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(window);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_win_get_tabpage(int64_t window) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_win_get_tabpage", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_WIN_GET_TABPAGE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(window);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_win_get_number(int64_t window) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_win_get_number", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_WIN_GET_NUMBER);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(window);
	return r;
}
MsgpackRequest* NeovimApi4::nvim_win_is_valid(int64_t window) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("nvim_win_is_valid", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_NVIM_WIN_IS_VALID);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(window);
	return r;
}
MsgpackRequest* NeovimApi4::buffer_line_count(int64_t buffer) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("buffer_line_count", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_BUFFER_LINE_COUNT);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(buffer);
	return r;
}
MsgpackRequest* NeovimApi4::buffer_get_lines(int64_t buffer, int64_t start, int64_t end, bool strict_indexing) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("buffer_get_lines", 4) };
	r->setFunction(NeovimApi4::NEOVIM_FN_BUFFER_GET_LINES);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(buffer);
	m_c->m_dev->send(start);
	m_c->m_dev->send(end);
	m_c->m_dev->send(strict_indexing);
	return r;
}
MsgpackRequest* NeovimApi4::buffer_set_lines(int64_t buffer, int64_t start, int64_t end, bool strict_indexing, QList<QByteArray> replacement) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("buffer_set_lines", 5) };
	r->setFunction(NeovimApi4::NEOVIM_FN_BUFFER_SET_LINES);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(buffer);
	m_c->m_dev->send(start);
	m_c->m_dev->send(end);
	m_c->m_dev->send(strict_indexing);
	m_c->m_dev->sendArrayOf(replacement);
	return r;
}
MsgpackRequest* NeovimApi4::buffer_get_var(int64_t buffer, QByteArray name) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("buffer_get_var", 2) };
	r->setFunction(NeovimApi4::NEOVIM_FN_BUFFER_GET_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(buffer);
	m_c->m_dev->send(name);
	return r;
}
MsgpackRequest* NeovimApi4::buffer_get_option(int64_t buffer, QByteArray name) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("buffer_get_option", 2) };
	r->setFunction(NeovimApi4::NEOVIM_FN_BUFFER_GET_OPTION);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(buffer);
	m_c->m_dev->send(name);
	return r;
}
MsgpackRequest* NeovimApi4::buffer_set_option(int64_t buffer, QByteArray name, QVariant value) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("buffer_set_option", 3) };
	r->setFunction(NeovimApi4::NEOVIM_FN_BUFFER_SET_OPTION);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(buffer);
	m_c->m_dev->send(name);
	m_c->m_dev->send(value);
	return r;
}
MsgpackRequest* NeovimApi4::buffer_get_number(int64_t buffer) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("buffer_get_number", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_BUFFER_GET_NUMBER);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(buffer);
	return r;
}
MsgpackRequest* NeovimApi4::buffer_get_name(int64_t buffer) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("buffer_get_name", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_BUFFER_GET_NAME);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(buffer);
	return r;
}
MsgpackRequest* NeovimApi4::buffer_set_name(int64_t buffer, QByteArray name) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("buffer_set_name", 2) };
	r->setFunction(NeovimApi4::NEOVIM_FN_BUFFER_SET_NAME);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(buffer);
	m_c->m_dev->send(name);
	return r;
}
MsgpackRequest* NeovimApi4::buffer_is_valid(int64_t buffer) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("buffer_is_valid", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_BUFFER_IS_VALID);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(buffer);
	return r;
}
MsgpackRequest* NeovimApi4::buffer_get_mark(int64_t buffer, QByteArray name) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("buffer_get_mark", 2) };
	r->setFunction(NeovimApi4::NEOVIM_FN_BUFFER_GET_MARK);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(buffer);
	m_c->m_dev->send(name);
	return r;
}
MsgpackRequest* NeovimApi4::buffer_add_highlight(int64_t buffer, int64_t src_id, QByteArray hl_group, int64_t line, int64_t col_start, int64_t col_end) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("buffer_add_highlight", 6) };
	r->setFunction(NeovimApi4::NEOVIM_FN_BUFFER_ADD_HIGHLIGHT);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(buffer);
	m_c->m_dev->send(src_id);
	m_c->m_dev->send(hl_group);
	m_c->m_dev->send(line);
	m_c->m_dev->send(col_start);
	m_c->m_dev->send(col_end);
	return r;
}
MsgpackRequest* NeovimApi4::buffer_clear_highlight(int64_t buffer, int64_t src_id, int64_t line_start, int64_t line_end) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("buffer_clear_highlight", 4) };
	r->setFunction(NeovimApi4::NEOVIM_FN_BUFFER_CLEAR_HIGHLIGHT);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(buffer);
	m_c->m_dev->send(src_id);
	m_c->m_dev->send(line_start);
	m_c->m_dev->send(line_end);
	return r;
}
MsgpackRequest* NeovimApi4::tabpage_get_windows(int64_t tabpage) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("tabpage_get_windows", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_TABPAGE_GET_WINDOWS);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(tabpage);
	return r;
}
MsgpackRequest* NeovimApi4::tabpage_get_var(int64_t tabpage, QByteArray name) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("tabpage_get_var", 2) };
	r->setFunction(NeovimApi4::NEOVIM_FN_TABPAGE_GET_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(tabpage);
	m_c->m_dev->send(name);
	return r;
}
MsgpackRequest* NeovimApi4::tabpage_get_window(int64_t tabpage) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("tabpage_get_window", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_TABPAGE_GET_WINDOW);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(tabpage);
	return r;
}
MsgpackRequest* NeovimApi4::tabpage_is_valid(int64_t tabpage) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("tabpage_is_valid", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_TABPAGE_IS_VALID);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(tabpage);
	return r;
}
MsgpackRequest* NeovimApi4::ui_detach() noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("ui_detach", 0) };
	r->setFunction(NeovimApi4::NEOVIM_FN_UI_DETACH);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	return r;
}
MsgpackRequest* NeovimApi4::ui_try_resize(int64_t width, int64_t height) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("ui_try_resize", 2) };
	r->setFunction(NeovimApi4::NEOVIM_FN_UI_TRY_RESIZE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(width);
	m_c->m_dev->send(height);
	return r;
}
MsgpackRequest* NeovimApi4::vim_command(QByteArray command) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("vim_command", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_VIM_COMMAND);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(command);
	return r;
}
MsgpackRequest* NeovimApi4::vim_feedkeys(QByteArray keys, QByteArray mode, bool escape_csi) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("vim_feedkeys", 3) };
	r->setFunction(NeovimApi4::NEOVIM_FN_VIM_FEEDKEYS);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(keys);
	m_c->m_dev->send(mode);
	m_c->m_dev->send(escape_csi);
	return r;
}
MsgpackRequest* NeovimApi4::vim_input(QByteArray keys) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("vim_input", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_VIM_INPUT);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(keys);
	return r;
}
MsgpackRequest* NeovimApi4::vim_replace_termcodes(QByteArray str, bool from_part, bool do_lt, bool special) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("vim_replace_termcodes", 4) };
	r->setFunction(NeovimApi4::NEOVIM_FN_VIM_REPLACE_TERMCODES);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(str);
	m_c->m_dev->send(from_part);
	m_c->m_dev->send(do_lt);
	m_c->m_dev->send(special);
	return r;
}
MsgpackRequest* NeovimApi4::vim_command_output(QByteArray command) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("vim_command_output", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_VIM_COMMAND_OUTPUT);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(command);
	return r;
}
MsgpackRequest* NeovimApi4::vim_eval(QByteArray expr) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("vim_eval", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_VIM_EVAL);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(expr);
	return r;
}
MsgpackRequest* NeovimApi4::vim_call_function(QByteArray fn, QVariantList args) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("vim_call_function", 2) };
	r->setFunction(NeovimApi4::NEOVIM_FN_VIM_CALL_FUNCTION);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(fn);
	m_c->m_dev->send(args);
	return r;
}
MsgpackRequest* NeovimApi4::vim_strwidth(QByteArray text) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("vim_strwidth", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_VIM_STRWIDTH);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(text);
	return r;
}
MsgpackRequest* NeovimApi4::vim_list_runtime_paths() noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("vim_list_runtime_paths", 0) };
	r->setFunction(NeovimApi4::NEOVIM_FN_VIM_LIST_RUNTIME_PATHS);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	return r;
}
MsgpackRequest* NeovimApi4::vim_change_directory(QByteArray dir) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("vim_change_directory", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_VIM_CHANGE_DIRECTORY);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(dir);
	return r;
}
MsgpackRequest* NeovimApi4::vim_get_current_line() noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("vim_get_current_line", 0) };
	r->setFunction(NeovimApi4::NEOVIM_FN_VIM_GET_CURRENT_LINE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	return r;
}
MsgpackRequest* NeovimApi4::vim_set_current_line(QByteArray line) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("vim_set_current_line", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_VIM_SET_CURRENT_LINE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(line);
	return r;
}
MsgpackRequest* NeovimApi4::vim_del_current_line() noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("vim_del_current_line", 0) };
	r->setFunction(NeovimApi4::NEOVIM_FN_VIM_DEL_CURRENT_LINE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	return r;
}
MsgpackRequest* NeovimApi4::vim_get_var(QByteArray name) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("vim_get_var", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_VIM_GET_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(name);
	return r;
}
MsgpackRequest* NeovimApi4::vim_get_vvar(QByteArray name) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("vim_get_vvar", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_VIM_GET_VVAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(name);
	return r;
}
MsgpackRequest* NeovimApi4::vim_get_option(QByteArray name) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("vim_get_option", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_VIM_GET_OPTION);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(name);
	return r;
}
MsgpackRequest* NeovimApi4::vim_set_option(QByteArray name, QVariant value) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("vim_set_option", 2) };
	r->setFunction(NeovimApi4::NEOVIM_FN_VIM_SET_OPTION);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(name);
	m_c->m_dev->send(value);
	return r;
}
MsgpackRequest* NeovimApi4::vim_out_write(QByteArray str) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("vim_out_write", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_VIM_OUT_WRITE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(str);
	return r;
}
MsgpackRequest* NeovimApi4::vim_err_write(QByteArray str) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("vim_err_write", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_VIM_ERR_WRITE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(str);
	return r;
}
MsgpackRequest* NeovimApi4::vim_report_error(QByteArray str) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("vim_report_error", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_VIM_REPORT_ERROR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(str);
	return r;
}
MsgpackRequest* NeovimApi4::vim_get_buffers() noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("vim_get_buffers", 0) };
	r->setFunction(NeovimApi4::NEOVIM_FN_VIM_GET_BUFFERS);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	return r;
}
MsgpackRequest* NeovimApi4::vim_get_current_buffer() noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("vim_get_current_buffer", 0) };
	r->setFunction(NeovimApi4::NEOVIM_FN_VIM_GET_CURRENT_BUFFER);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	return r;
}
MsgpackRequest* NeovimApi4::vim_set_current_buffer(int64_t buffer) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("vim_set_current_buffer", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_VIM_SET_CURRENT_BUFFER);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(buffer);
	return r;
}
MsgpackRequest* NeovimApi4::vim_get_windows() noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("vim_get_windows", 0) };
	r->setFunction(NeovimApi4::NEOVIM_FN_VIM_GET_WINDOWS);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	return r;
}
MsgpackRequest* NeovimApi4::vim_get_current_window() noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("vim_get_current_window", 0) };
	r->setFunction(NeovimApi4::NEOVIM_FN_VIM_GET_CURRENT_WINDOW);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	return r;
}
MsgpackRequest* NeovimApi4::vim_set_current_window(int64_t window) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("vim_set_current_window", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_VIM_SET_CURRENT_WINDOW);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(window);
	return r;
}
MsgpackRequest* NeovimApi4::vim_get_tabpages() noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("vim_get_tabpages", 0) };
	r->setFunction(NeovimApi4::NEOVIM_FN_VIM_GET_TABPAGES);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	return r;
}
MsgpackRequest* NeovimApi4::vim_get_current_tabpage() noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("vim_get_current_tabpage", 0) };
	r->setFunction(NeovimApi4::NEOVIM_FN_VIM_GET_CURRENT_TABPAGE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	return r;
}
MsgpackRequest* NeovimApi4::vim_set_current_tabpage(int64_t tabpage) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("vim_set_current_tabpage", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_VIM_SET_CURRENT_TABPAGE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(tabpage);
	return r;
}
MsgpackRequest* NeovimApi4::vim_subscribe(QByteArray event) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("vim_subscribe", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_VIM_SUBSCRIBE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(event);
	return r;
}
MsgpackRequest* NeovimApi4::vim_unsubscribe(QByteArray event) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("vim_unsubscribe", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_VIM_UNSUBSCRIBE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(event);
	return r;
}
MsgpackRequest* NeovimApi4::vim_name_to_color(QByteArray name) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("vim_name_to_color", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_VIM_NAME_TO_COLOR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(name);
	return r;
}
MsgpackRequest* NeovimApi4::vim_get_color_map() noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("vim_get_color_map", 0) };
	r->setFunction(NeovimApi4::NEOVIM_FN_VIM_GET_COLOR_MAP);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	return r;
}
MsgpackRequest* NeovimApi4::window_get_buffer(int64_t window) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("window_get_buffer", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_WINDOW_GET_BUFFER);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(window);
	return r;
}
MsgpackRequest* NeovimApi4::window_get_cursor(int64_t window) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("window_get_cursor", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_WINDOW_GET_CURSOR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(window);
	return r;
}
MsgpackRequest* NeovimApi4::window_set_cursor(int64_t window, QPoint pos) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("window_set_cursor", 2) };
	r->setFunction(NeovimApi4::NEOVIM_FN_WINDOW_SET_CURSOR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(window);
	m_c->m_dev->send(pos);
	return r;
}
MsgpackRequest* NeovimApi4::window_get_height(int64_t window) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("window_get_height", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_WINDOW_GET_HEIGHT);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(window);
	return r;
}
MsgpackRequest* NeovimApi4::window_set_height(int64_t window, int64_t height) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("window_set_height", 2) };
	r->setFunction(NeovimApi4::NEOVIM_FN_WINDOW_SET_HEIGHT);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(window);
	m_c->m_dev->send(height);
	return r;
}
MsgpackRequest* NeovimApi4::window_get_width(int64_t window) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("window_get_width", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_WINDOW_GET_WIDTH);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(window);
	return r;
}
MsgpackRequest* NeovimApi4::window_set_width(int64_t window, int64_t width) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("window_set_width", 2) };
	r->setFunction(NeovimApi4::NEOVIM_FN_WINDOW_SET_WIDTH);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(window);
	m_c->m_dev->send(width);
	return r;
}
MsgpackRequest* NeovimApi4::window_get_var(int64_t window, QByteArray name) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("window_get_var", 2) };
	r->setFunction(NeovimApi4::NEOVIM_FN_WINDOW_GET_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(window);
	m_c->m_dev->send(name);
	return r;
}
MsgpackRequest* NeovimApi4::window_get_option(int64_t window, QByteArray name) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("window_get_option", 2) };
	r->setFunction(NeovimApi4::NEOVIM_FN_WINDOW_GET_OPTION);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(window);
	m_c->m_dev->send(name);
	return r;
}
MsgpackRequest* NeovimApi4::window_set_option(int64_t window, QByteArray name, QVariant value) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("window_set_option", 3) };
	r->setFunction(NeovimApi4::NEOVIM_FN_WINDOW_SET_OPTION);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(window);
	m_c->m_dev->send(name);
	m_c->m_dev->send(value);
	return r;
}
MsgpackRequest* NeovimApi4::window_get_position(int64_t window) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("window_get_position", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_WINDOW_GET_POSITION);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(window);
	return r;
}
MsgpackRequest* NeovimApi4::window_get_tabpage(int64_t window) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("window_get_tabpage", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_WINDOW_GET_TABPAGE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(window);
	return r;
}
MsgpackRequest* NeovimApi4::window_is_valid(int64_t window) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("window_is_valid", 1) };
	r->setFunction(NeovimApi4::NEOVIM_FN_WINDOW_IS_VALID);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi4::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi4::handleResponseError);
	m_c->m_dev->send(window);
	return r;
}

// Handlers
void NeovimApi4::handleResponseError(quint32 msgid, quint64 fun, const QVariant& res) noexcept
{
	// TODO: support Neovim error types Exception/Validation/etc
	QString errMsg;
	const QVariantList asList = res.toList();
	if (asList.size() >= 2) {
		if (asList.at(1).canConvert<QByteArray>()) {
			errMsg = m_c->m_dev->decode(asList.at(1).toByteArray());
		} else {
			errMsg = tr("Received unsupported Neovim error type");
		}
	}

	switch(fun)
	{
		case NeovimApi4::NEOVIM_FN_NVIM_BUF_LINE_COUNT:
			emit err_nvim_buf_line_count(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_BUFFER_GET_LINE:
			emit err_buffer_get_line(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_BUF_ATTACH:
			emit err_nvim_buf_attach(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_BUF_DETACH:
			emit err_nvim_buf_detach(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_BUFFER_SET_LINE:
			emit err_buffer_set_line(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_BUFFER_DEL_LINE:
			emit err_buffer_del_line(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_BUFFER_GET_LINE_SLICE:
			emit err_buffer_get_line_slice(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_BUF_GET_LINES:
			emit err_nvim_buf_get_lines(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_BUFFER_SET_LINE_SLICE:
			emit err_buffer_set_line_slice(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_BUF_SET_LINES:
			emit err_nvim_buf_set_lines(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_BUF_GET_VAR:
			emit err_nvim_buf_get_var(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_BUF_GET_CHANGEDTICK:
			emit err_nvim_buf_get_changedtick(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_BUF_GET_KEYMAP:
			emit err_nvim_buf_get_keymap(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_BUF_GET_COMMANDS:
			emit err_nvim_buf_get_commands(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_BUF_SET_VAR:
			emit err_nvim_buf_set_var(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_BUF_DEL_VAR:
			emit err_nvim_buf_del_var(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_BUFFER_SET_VAR:
			emit err_buffer_set_var(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_BUFFER_DEL_VAR:
			emit err_buffer_del_var(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_BUF_GET_OPTION:
			emit err_nvim_buf_get_option(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_BUF_SET_OPTION:
			emit err_nvim_buf_set_option(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_BUF_GET_NUMBER:
			emit err_nvim_buf_get_number(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_BUF_GET_NAME:
			emit err_nvim_buf_get_name(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_BUF_SET_NAME:
			emit err_nvim_buf_set_name(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_BUF_IS_VALID:
			emit err_nvim_buf_is_valid(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_BUFFER_INSERT:
			emit err_buffer_insert(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_BUF_GET_MARK:
			emit err_nvim_buf_get_mark(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_BUF_ADD_HIGHLIGHT:
			emit err_nvim_buf_add_highlight(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_BUF_CLEAR_HIGHLIGHT:
			emit err_nvim_buf_clear_highlight(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_TABPAGE_LIST_WINS:
			emit err_nvim_tabpage_list_wins(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_TABPAGE_GET_VAR:
			emit err_nvim_tabpage_get_var(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_TABPAGE_SET_VAR:
			emit err_nvim_tabpage_set_var(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_TABPAGE_DEL_VAR:
			emit err_nvim_tabpage_del_var(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_TABPAGE_SET_VAR:
			emit err_tabpage_set_var(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_TABPAGE_DEL_VAR:
			emit err_tabpage_del_var(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_TABPAGE_GET_WIN:
			emit err_nvim_tabpage_get_win(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_TABPAGE_GET_NUMBER:
			emit err_nvim_tabpage_get_number(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_TABPAGE_IS_VALID:
			emit err_nvim_tabpage_is_valid(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_UI_ATTACH:
			emit err_nvim_ui_attach(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_UI_ATTACH:
			emit err_ui_attach(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_UI_DETACH:
			emit err_nvim_ui_detach(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_UI_TRY_RESIZE:
			emit err_nvim_ui_try_resize(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_UI_SET_OPTION:
			emit err_nvim_ui_set_option(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_COMMAND:
			emit err_nvim_command(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_GET_HL_BY_NAME:
			emit err_nvim_get_hl_by_name(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_GET_HL_BY_ID:
			emit err_nvim_get_hl_by_id(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_FEEDKEYS:
			emit err_nvim_feedkeys(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_INPUT:
			emit err_nvim_input(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_REPLACE_TERMCODES:
			emit err_nvim_replace_termcodes(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_COMMAND_OUTPUT:
			emit err_nvim_command_output(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_EVAL:
			emit err_nvim_eval(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_EXECUTE_LUA:
			emit err_nvim_execute_lua(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_CALL_FUNCTION:
			emit err_nvim_call_function(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_CALL_DICT_FUNCTION:
			emit err_nvim_call_dict_function(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_STRWIDTH:
			emit err_nvim_strwidth(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_LIST_RUNTIME_PATHS:
			emit err_nvim_list_runtime_paths(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_SET_CURRENT_DIR:
			emit err_nvim_set_current_dir(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_GET_CURRENT_LINE:
			emit err_nvim_get_current_line(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_SET_CURRENT_LINE:
			emit err_nvim_set_current_line(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_DEL_CURRENT_LINE:
			emit err_nvim_del_current_line(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_GET_VAR:
			emit err_nvim_get_var(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_SET_VAR:
			emit err_nvim_set_var(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_DEL_VAR:
			emit err_nvim_del_var(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_VIM_SET_VAR:
			emit err_vim_set_var(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_VIM_DEL_VAR:
			emit err_vim_del_var(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_GET_VVAR:
			emit err_nvim_get_vvar(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_GET_OPTION:
			emit err_nvim_get_option(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_SET_OPTION:
			emit err_nvim_set_option(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_OUT_WRITE:
			emit err_nvim_out_write(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_ERR_WRITE:
			emit err_nvim_err_write(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_ERR_WRITELN:
			emit err_nvim_err_writeln(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_LIST_BUFS:
			emit err_nvim_list_bufs(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_GET_CURRENT_BUF:
			emit err_nvim_get_current_buf(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_SET_CURRENT_BUF:
			emit err_nvim_set_current_buf(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_LIST_WINS:
			emit err_nvim_list_wins(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_GET_CURRENT_WIN:
			emit err_nvim_get_current_win(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_SET_CURRENT_WIN:
			emit err_nvim_set_current_win(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_LIST_TABPAGES:
			emit err_nvim_list_tabpages(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_GET_CURRENT_TABPAGE:
			emit err_nvim_get_current_tabpage(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_SET_CURRENT_TABPAGE:
			emit err_nvim_set_current_tabpage(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_SUBSCRIBE:
			emit err_nvim_subscribe(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_UNSUBSCRIBE:
			emit err_nvim_unsubscribe(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_GET_COLOR_BY_NAME:
			emit err_nvim_get_color_by_name(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_GET_COLOR_MAP:
			emit err_nvim_get_color_map(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_GET_MODE:
			emit err_nvim_get_mode(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_GET_KEYMAP:
			emit err_nvim_get_keymap(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_GET_COMMANDS:
			emit err_nvim_get_commands(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_GET_API_INFO:
			emit err_nvim_get_api_info(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_SET_CLIENT_INFO:
			emit err_nvim_set_client_info(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_GET_CHAN_INFO:
			emit err_nvim_get_chan_info(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_LIST_CHANS:
			emit err_nvim_list_chans(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_CALL_ATOMIC:
			emit err_nvim_call_atomic(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_PARSE_EXPRESSION:
			emit err_nvim_parse_expression(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_LIST_UIS:
			emit err_nvim_list_uis(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_GET_PROC_CHILDREN:
			emit err_nvim_get_proc_children(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_GET_PROC:
			emit err_nvim_get_proc(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_WIN_GET_BUF:
			emit err_nvim_win_get_buf(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_WIN_GET_CURSOR:
			emit err_nvim_win_get_cursor(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_WIN_SET_CURSOR:
			emit err_nvim_win_set_cursor(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_WIN_GET_HEIGHT:
			emit err_nvim_win_get_height(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_WIN_SET_HEIGHT:
			emit err_nvim_win_set_height(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_WIN_GET_WIDTH:
			emit err_nvim_win_get_width(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_WIN_SET_WIDTH:
			emit err_nvim_win_set_width(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_WIN_GET_VAR:
			emit err_nvim_win_get_var(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_WIN_SET_VAR:
			emit err_nvim_win_set_var(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_WIN_DEL_VAR:
			emit err_nvim_win_del_var(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_WINDOW_SET_VAR:
			emit err_window_set_var(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_WINDOW_DEL_VAR:
			emit err_window_del_var(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_WIN_GET_OPTION:
			emit err_nvim_win_get_option(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_WIN_SET_OPTION:
			emit err_nvim_win_set_option(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_WIN_GET_POSITION:
			emit err_nvim_win_get_position(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_WIN_GET_TABPAGE:
			emit err_nvim_win_get_tabpage(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_WIN_GET_NUMBER:
			emit err_nvim_win_get_number(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_NVIM_WIN_IS_VALID:
			emit err_nvim_win_is_valid(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_BUFFER_LINE_COUNT:
			emit err_buffer_line_count(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_BUFFER_GET_LINES:
			emit err_buffer_get_lines(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_BUFFER_SET_LINES:
			emit err_buffer_set_lines(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_BUFFER_GET_VAR:
			emit err_buffer_get_var(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_BUFFER_GET_OPTION:
			emit err_buffer_get_option(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_BUFFER_SET_OPTION:
			emit err_buffer_set_option(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_BUFFER_GET_NUMBER:
			emit err_buffer_get_number(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_BUFFER_GET_NAME:
			emit err_buffer_get_name(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_BUFFER_SET_NAME:
			emit err_buffer_set_name(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_BUFFER_IS_VALID:
			emit err_buffer_is_valid(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_BUFFER_GET_MARK:
			emit err_buffer_get_mark(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_BUFFER_ADD_HIGHLIGHT:
			emit err_buffer_add_highlight(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_BUFFER_CLEAR_HIGHLIGHT:
			emit err_buffer_clear_highlight(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_TABPAGE_GET_WINDOWS:
			emit err_tabpage_get_windows(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_TABPAGE_GET_VAR:
			emit err_tabpage_get_var(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_TABPAGE_GET_WINDOW:
			emit err_tabpage_get_window(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_TABPAGE_IS_VALID:
			emit err_tabpage_is_valid(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_UI_DETACH:
			emit err_ui_detach(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_UI_TRY_RESIZE:
			emit err_ui_try_resize(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_VIM_COMMAND:
			emit err_vim_command(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_VIM_FEEDKEYS:
			emit err_vim_feedkeys(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_VIM_INPUT:
			emit err_vim_input(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_VIM_REPLACE_TERMCODES:
			emit err_vim_replace_termcodes(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_VIM_COMMAND_OUTPUT:
			emit err_vim_command_output(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_VIM_EVAL:
			emit err_vim_eval(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_VIM_CALL_FUNCTION:
			emit err_vim_call_function(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_VIM_STRWIDTH:
			emit err_vim_strwidth(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_VIM_LIST_RUNTIME_PATHS:
			emit err_vim_list_runtime_paths(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_VIM_CHANGE_DIRECTORY:
			emit err_vim_change_directory(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_VIM_GET_CURRENT_LINE:
			emit err_vim_get_current_line(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_VIM_SET_CURRENT_LINE:
			emit err_vim_set_current_line(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_VIM_DEL_CURRENT_LINE:
			emit err_vim_del_current_line(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_VIM_GET_VAR:
			emit err_vim_get_var(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_VIM_GET_VVAR:
			emit err_vim_get_vvar(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_VIM_GET_OPTION:
			emit err_vim_get_option(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_VIM_SET_OPTION:
			emit err_vim_set_option(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_VIM_OUT_WRITE:
			emit err_vim_out_write(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_VIM_ERR_WRITE:
			emit err_vim_err_write(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_VIM_REPORT_ERROR:
			emit err_vim_report_error(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_VIM_GET_BUFFERS:
			emit err_vim_get_buffers(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_VIM_GET_CURRENT_BUFFER:
			emit err_vim_get_current_buffer(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_VIM_SET_CURRENT_BUFFER:
			emit err_vim_set_current_buffer(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_VIM_GET_WINDOWS:
			emit err_vim_get_windows(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_VIM_GET_CURRENT_WINDOW:
			emit err_vim_get_current_window(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_VIM_SET_CURRENT_WINDOW:
			emit err_vim_set_current_window(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_VIM_GET_TABPAGES:
			emit err_vim_get_tabpages(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_VIM_GET_CURRENT_TABPAGE:
			emit err_vim_get_current_tabpage(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_VIM_SET_CURRENT_TABPAGE:
			emit err_vim_set_current_tabpage(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_VIM_SUBSCRIBE:
			emit err_vim_subscribe(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_VIM_UNSUBSCRIBE:
			emit err_vim_unsubscribe(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_VIM_NAME_TO_COLOR:
			emit err_vim_name_to_color(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_VIM_GET_COLOR_MAP:
			emit err_vim_get_color_map(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_WINDOW_GET_BUFFER:
			emit err_window_get_buffer(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_WINDOW_GET_CURSOR:
			emit err_window_get_cursor(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_WINDOW_SET_CURSOR:
			emit err_window_set_cursor(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_WINDOW_GET_HEIGHT:
			emit err_window_get_height(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_WINDOW_SET_HEIGHT:
			emit err_window_set_height(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_WINDOW_GET_WIDTH:
			emit err_window_get_width(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_WINDOW_SET_WIDTH:
			emit err_window_set_width(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_WINDOW_GET_VAR:
			emit err_window_get_var(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_WINDOW_GET_OPTION:
			emit err_window_get_option(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_WINDOW_SET_OPTION:
			emit err_window_set_option(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_WINDOW_GET_POSITION:
			emit err_window_get_position(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_WINDOW_GET_TABPAGE:
			emit err_window_get_tabpage(errMsg, res);
			break;

		case NeovimApi4::NEOVIM_FN_WINDOW_IS_VALID:
			emit err_window_is_valid(errMsg, res);
			break;

	default:
		m_c->setError(NeovimConnector::RuntimeMsgpackError, QString("Received error for function that should not fail: %s").arg(fun));
	}
}

void NeovimApi4::handleResponse(quint32 msgid, quint64 fun, const QVariant& res) noexcept
{
	switch(fun)
	{
		case NeovimApi4::NEOVIM_FN_NVIM_BUF_LINE_COUNT:
		{
			int64_t data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_buf_line_count");
				return;
			} else {
				emit on_nvim_buf_line_count(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_BUFFER_GET_LINE:
		{
			QByteArray data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for buffer_get_line");
				return;
			} else {
				emit on_buffer_get_line(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_BUF_ATTACH:
		{
			bool data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_buf_attach");
				return;
			} else {
				emit on_nvim_buf_attach(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_BUF_DETACH:
		{
			bool data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_buf_detach");
				return;
			} else {
				emit on_nvim_buf_detach(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_BUFFER_SET_LINE:
		{
			emit on_buffer_set_line();
		}
		break;

		case NeovimApi4::NEOVIM_FN_BUFFER_DEL_LINE:
		{
			emit on_buffer_del_line();
		}
		break;

		case NeovimApi4::NEOVIM_FN_BUFFER_GET_LINE_SLICE:
		{
			QList<QByteArray> data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for buffer_get_line_slice");
				return;
			} else {
				emit on_buffer_get_line_slice(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_BUF_GET_LINES:
		{
			QList<QByteArray> data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_buf_get_lines");
				return;
			} else {
				emit on_nvim_buf_get_lines(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_BUFFER_SET_LINE_SLICE:
		{
			emit on_buffer_set_line_slice();
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_BUF_SET_LINES:
		{
			emit on_nvim_buf_set_lines();
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_BUF_GET_VAR:
		{
			QVariant data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_buf_get_var");
				return;
			} else {
				emit on_nvim_buf_get_var(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_BUF_GET_CHANGEDTICK:
		{
			int64_t data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_buf_get_changedtick");
				return;
			} else {
				emit on_nvim_buf_get_changedtick(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_BUF_GET_KEYMAP:
		{
			QList<QVariantMap> data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_buf_get_keymap");
				return;
			} else {
				emit on_nvim_buf_get_keymap(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_BUF_GET_COMMANDS:
		{
			QVariantMap data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_buf_get_commands");
				return;
			} else {
				emit on_nvim_buf_get_commands(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_BUF_SET_VAR:
		{
			emit on_nvim_buf_set_var();
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_BUF_DEL_VAR:
		{
			emit on_nvim_buf_del_var();
		}
		break;

		case NeovimApi4::NEOVIM_FN_BUFFER_SET_VAR:
		{
			QVariant data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for buffer_set_var");
				return;
			} else {
				emit on_buffer_set_var(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_BUFFER_DEL_VAR:
		{
			QVariant data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for buffer_del_var");
				return;
			} else {
				emit on_buffer_del_var(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_BUF_GET_OPTION:
		{
			QVariant data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_buf_get_option");
				return;
			} else {
				emit on_nvim_buf_get_option(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_BUF_SET_OPTION:
		{
			emit on_nvim_buf_set_option();
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_BUF_GET_NUMBER:
		{
			int64_t data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_buf_get_number");
				return;
			} else {
				emit on_nvim_buf_get_number(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_BUF_GET_NAME:
		{
			QByteArray data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_buf_get_name");
				return;
			} else {
				emit on_nvim_buf_get_name(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_BUF_SET_NAME:
		{
			emit on_nvim_buf_set_name();
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_BUF_IS_VALID:
		{
			bool data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_buf_is_valid");
				return;
			} else {
				emit on_nvim_buf_is_valid(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_BUFFER_INSERT:
		{
			emit on_buffer_insert();
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_BUF_GET_MARK:
		{
			QPoint data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_buf_get_mark");
				return;
			} else {
				emit on_nvim_buf_get_mark(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_BUF_ADD_HIGHLIGHT:
		{
			int64_t data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_buf_add_highlight");
				return;
			} else {
				emit on_nvim_buf_add_highlight(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_BUF_CLEAR_HIGHLIGHT:
		{
			emit on_nvim_buf_clear_highlight();
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_TABPAGE_LIST_WINS:
		{
			QList<int64_t> data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_tabpage_list_wins");
				return;
			} else {
				emit on_nvim_tabpage_list_wins(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_TABPAGE_GET_VAR:
		{
			QVariant data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_tabpage_get_var");
				return;
			} else {
				emit on_nvim_tabpage_get_var(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_TABPAGE_SET_VAR:
		{
			emit on_nvim_tabpage_set_var();
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_TABPAGE_DEL_VAR:
		{
			emit on_nvim_tabpage_del_var();
		}
		break;

		case NeovimApi4::NEOVIM_FN_TABPAGE_SET_VAR:
		{
			QVariant data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for tabpage_set_var");
				return;
			} else {
				emit on_tabpage_set_var(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_TABPAGE_DEL_VAR:
		{
			QVariant data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for tabpage_del_var");
				return;
			} else {
				emit on_tabpage_del_var(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_TABPAGE_GET_WIN:
		{
			int64_t data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_tabpage_get_win");
				return;
			} else {
				emit on_nvim_tabpage_get_win(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_TABPAGE_GET_NUMBER:
		{
			int64_t data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_tabpage_get_number");
				return;
			} else {
				emit on_nvim_tabpage_get_number(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_TABPAGE_IS_VALID:
		{
			bool data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_tabpage_is_valid");
				return;
			} else {
				emit on_nvim_tabpage_is_valid(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_UI_ATTACH:
		{
			emit on_nvim_ui_attach();
		}
		break;

		case NeovimApi4::NEOVIM_FN_UI_ATTACH:
		{
			emit on_ui_attach();
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_UI_DETACH:
		{
			emit on_nvim_ui_detach();
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_UI_TRY_RESIZE:
		{
			emit on_nvim_ui_try_resize();
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_UI_SET_OPTION:
		{
			emit on_nvim_ui_set_option();
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_COMMAND:
		{
			emit on_nvim_command();
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_GET_HL_BY_NAME:
		{
			QVariantMap data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_get_hl_by_name");
				return;
			} else {
				emit on_nvim_get_hl_by_name(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_GET_HL_BY_ID:
		{
			QVariantMap data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_get_hl_by_id");
				return;
			} else {
				emit on_nvim_get_hl_by_id(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_FEEDKEYS:
		{
			emit on_nvim_feedkeys();
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_INPUT:
		{
			int64_t data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_input");
				return;
			} else {
				emit on_nvim_input(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_REPLACE_TERMCODES:
		{
			QByteArray data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_replace_termcodes");
				return;
			} else {
				emit on_nvim_replace_termcodes(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_COMMAND_OUTPUT:
		{
			QByteArray data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_command_output");
				return;
			} else {
				emit on_nvim_command_output(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_EVAL:
		{
			QVariant data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_eval");
				return;
			} else {
				emit on_nvim_eval(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_EXECUTE_LUA:
		{
			QVariant data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_execute_lua");
				return;
			} else {
				emit on_nvim_execute_lua(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_CALL_FUNCTION:
		{
			QVariant data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_call_function");
				return;
			} else {
				emit on_nvim_call_function(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_CALL_DICT_FUNCTION:
		{
			QVariant data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_call_dict_function");
				return;
			} else {
				emit on_nvim_call_dict_function(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_STRWIDTH:
		{
			int64_t data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_strwidth");
				return;
			} else {
				emit on_nvim_strwidth(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_LIST_RUNTIME_PATHS:
		{
			QList<QByteArray> data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_list_runtime_paths");
				return;
			} else {
				emit on_nvim_list_runtime_paths(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_SET_CURRENT_DIR:
		{
			emit on_nvim_set_current_dir();
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_GET_CURRENT_LINE:
		{
			QByteArray data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_get_current_line");
				return;
			} else {
				emit on_nvim_get_current_line(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_SET_CURRENT_LINE:
		{
			emit on_nvim_set_current_line();
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_DEL_CURRENT_LINE:
		{
			emit on_nvim_del_current_line();
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_GET_VAR:
		{
			QVariant data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_get_var");
				return;
			} else {
				emit on_nvim_get_var(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_SET_VAR:
		{
			emit on_nvim_set_var();
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_DEL_VAR:
		{
			emit on_nvim_del_var();
		}
		break;

		case NeovimApi4::NEOVIM_FN_VIM_SET_VAR:
		{
			QVariant data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_set_var");
				return;
			} else {
				emit on_vim_set_var(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_VIM_DEL_VAR:
		{
			QVariant data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_del_var");
				return;
			} else {
				emit on_vim_del_var(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_GET_VVAR:
		{
			QVariant data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_get_vvar");
				return;
			} else {
				emit on_nvim_get_vvar(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_GET_OPTION:
		{
			QVariant data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_get_option");
				return;
			} else {
				emit on_nvim_get_option(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_SET_OPTION:
		{
			emit on_nvim_set_option();
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_OUT_WRITE:
		{
			emit on_nvim_out_write();
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_ERR_WRITE:
		{
			emit on_nvim_err_write();
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_ERR_WRITELN:
		{
			emit on_nvim_err_writeln();
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_LIST_BUFS:
		{
			QList<int64_t> data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_list_bufs");
				return;
			} else {
				emit on_nvim_list_bufs(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_GET_CURRENT_BUF:
		{
			int64_t data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_get_current_buf");
				return;
			} else {
				emit on_nvim_get_current_buf(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_SET_CURRENT_BUF:
		{
			emit on_nvim_set_current_buf();
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_LIST_WINS:
		{
			QList<int64_t> data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_list_wins");
				return;
			} else {
				emit on_nvim_list_wins(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_GET_CURRENT_WIN:
		{
			int64_t data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_get_current_win");
				return;
			} else {
				emit on_nvim_get_current_win(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_SET_CURRENT_WIN:
		{
			emit on_nvim_set_current_win();
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_LIST_TABPAGES:
		{
			QList<int64_t> data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_list_tabpages");
				return;
			} else {
				emit on_nvim_list_tabpages(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_GET_CURRENT_TABPAGE:
		{
			int64_t data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_get_current_tabpage");
				return;
			} else {
				emit on_nvim_get_current_tabpage(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_SET_CURRENT_TABPAGE:
		{
			emit on_nvim_set_current_tabpage();
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_SUBSCRIBE:
		{
			emit on_nvim_subscribe();
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_UNSUBSCRIBE:
		{
			emit on_nvim_unsubscribe();
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_GET_COLOR_BY_NAME:
		{
			int64_t data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_get_color_by_name");
				return;
			} else {
				emit on_nvim_get_color_by_name(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_GET_COLOR_MAP:
		{
			QVariantMap data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_get_color_map");
				return;
			} else {
				emit on_nvim_get_color_map(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_GET_MODE:
		{
			QVariantMap data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_get_mode");
				return;
			} else {
				emit on_nvim_get_mode(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_GET_KEYMAP:
		{
			QList<QVariantMap> data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_get_keymap");
				return;
			} else {
				emit on_nvim_get_keymap(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_GET_COMMANDS:
		{
			QVariantMap data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_get_commands");
				return;
			} else {
				emit on_nvim_get_commands(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_GET_API_INFO:
		{
			QVariantList data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_get_api_info");
				return;
			} else {
				emit on_nvim_get_api_info(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_SET_CLIENT_INFO:
		{
			emit on_nvim_set_client_info();
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_GET_CHAN_INFO:
		{
			QVariantMap data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_get_chan_info");
				return;
			} else {
				emit on_nvim_get_chan_info(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_LIST_CHANS:
		{
			QVariantList data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_list_chans");
				return;
			} else {
				emit on_nvim_list_chans(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_CALL_ATOMIC:
		{
			QVariantList data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_call_atomic");
				return;
			} else {
				emit on_nvim_call_atomic(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_PARSE_EXPRESSION:
		{
			QVariantMap data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_parse_expression");
				return;
			} else {
				emit on_nvim_parse_expression(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_LIST_UIS:
		{
			QVariantList data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_list_uis");
				return;
			} else {
				emit on_nvim_list_uis(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_GET_PROC_CHILDREN:
		{
			QVariantList data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_get_proc_children");
				return;
			} else {
				emit on_nvim_get_proc_children(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_GET_PROC:
		{
			QVariant data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_get_proc");
				return;
			} else {
				emit on_nvim_get_proc(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_WIN_GET_BUF:
		{
			int64_t data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_win_get_buf");
				return;
			} else {
				emit on_nvim_win_get_buf(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_WIN_GET_CURSOR:
		{
			QPoint data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_win_get_cursor");
				return;
			} else {
				emit on_nvim_win_get_cursor(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_WIN_SET_CURSOR:
		{
			emit on_nvim_win_set_cursor();
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_WIN_GET_HEIGHT:
		{
			int64_t data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_win_get_height");
				return;
			} else {
				emit on_nvim_win_get_height(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_WIN_SET_HEIGHT:
		{
			emit on_nvim_win_set_height();
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_WIN_GET_WIDTH:
		{
			int64_t data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_win_get_width");
				return;
			} else {
				emit on_nvim_win_get_width(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_WIN_SET_WIDTH:
		{
			emit on_nvim_win_set_width();
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_WIN_GET_VAR:
		{
			QVariant data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_win_get_var");
				return;
			} else {
				emit on_nvim_win_get_var(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_WIN_SET_VAR:
		{
			emit on_nvim_win_set_var();
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_WIN_DEL_VAR:
		{
			emit on_nvim_win_del_var();
		}
		break;

		case NeovimApi4::NEOVIM_FN_WINDOW_SET_VAR:
		{
			QVariant data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for window_set_var");
				return;
			} else {
				emit on_window_set_var(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_WINDOW_DEL_VAR:
		{
			QVariant data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for window_del_var");
				return;
			} else {
				emit on_window_del_var(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_WIN_GET_OPTION:
		{
			QVariant data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_win_get_option");
				return;
			} else {
				emit on_nvim_win_get_option(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_WIN_SET_OPTION:
		{
			emit on_nvim_win_set_option();
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_WIN_GET_POSITION:
		{
			QPoint data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_win_get_position");
				return;
			} else {
				emit on_nvim_win_get_position(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_WIN_GET_TABPAGE:
		{
			int64_t data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_win_get_tabpage");
				return;
			} else {
				emit on_nvim_win_get_tabpage(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_WIN_GET_NUMBER:
		{
			int64_t data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_win_get_number");
				return;
			} else {
				emit on_nvim_win_get_number(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_NVIM_WIN_IS_VALID:
		{
			bool data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_win_is_valid");
				return;
			} else {
				emit on_nvim_win_is_valid(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_BUFFER_LINE_COUNT:
		{
			int64_t data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for buffer_line_count");
				return;
			} else {
				emit on_buffer_line_count(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_BUFFER_GET_LINES:
		{
			QList<QByteArray> data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for buffer_get_lines");
				return;
			} else {
				emit on_buffer_get_lines(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_BUFFER_SET_LINES:
		{
			emit on_buffer_set_lines();
		}
		break;

		case NeovimApi4::NEOVIM_FN_BUFFER_GET_VAR:
		{
			QVariant data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for buffer_get_var");
				return;
			} else {
				emit on_buffer_get_var(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_BUFFER_GET_OPTION:
		{
			QVariant data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for buffer_get_option");
				return;
			} else {
				emit on_buffer_get_option(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_BUFFER_SET_OPTION:
		{
			emit on_buffer_set_option();
		}
		break;

		case NeovimApi4::NEOVIM_FN_BUFFER_GET_NUMBER:
		{
			int64_t data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for buffer_get_number");
				return;
			} else {
				emit on_buffer_get_number(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_BUFFER_GET_NAME:
		{
			QByteArray data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for buffer_get_name");
				return;
			} else {
				emit on_buffer_get_name(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_BUFFER_SET_NAME:
		{
			emit on_buffer_set_name();
		}
		break;

		case NeovimApi4::NEOVIM_FN_BUFFER_IS_VALID:
		{
			bool data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for buffer_is_valid");
				return;
			} else {
				emit on_buffer_is_valid(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_BUFFER_GET_MARK:
		{
			QPoint data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for buffer_get_mark");
				return;
			} else {
				emit on_buffer_get_mark(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_BUFFER_ADD_HIGHLIGHT:
		{
			int64_t data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for buffer_add_highlight");
				return;
			} else {
				emit on_buffer_add_highlight(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_BUFFER_CLEAR_HIGHLIGHT:
		{
			emit on_buffer_clear_highlight();
		}
		break;

		case NeovimApi4::NEOVIM_FN_TABPAGE_GET_WINDOWS:
		{
			QList<int64_t> data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for tabpage_get_windows");
				return;
			} else {
				emit on_tabpage_get_windows(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_TABPAGE_GET_VAR:
		{
			QVariant data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for tabpage_get_var");
				return;
			} else {
				emit on_tabpage_get_var(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_TABPAGE_GET_WINDOW:
		{
			int64_t data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for tabpage_get_window");
				return;
			} else {
				emit on_tabpage_get_window(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_TABPAGE_IS_VALID:
		{
			bool data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for tabpage_is_valid");
				return;
			} else {
				emit on_tabpage_is_valid(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_UI_DETACH:
		{
			emit on_ui_detach();
		}
		break;

		case NeovimApi4::NEOVIM_FN_UI_TRY_RESIZE:
		{
			QVariant data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for ui_try_resize");
				return;
			} else {
				emit on_ui_try_resize(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_VIM_COMMAND:
		{
			emit on_vim_command();
		}
		break;

		case NeovimApi4::NEOVIM_FN_VIM_FEEDKEYS:
		{
			emit on_vim_feedkeys();
		}
		break;

		case NeovimApi4::NEOVIM_FN_VIM_INPUT:
		{
			int64_t data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_input");
				return;
			} else {
				emit on_vim_input(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_VIM_REPLACE_TERMCODES:
		{
			QByteArray data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_replace_termcodes");
				return;
			} else {
				emit on_vim_replace_termcodes(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_VIM_COMMAND_OUTPUT:
		{
			QByteArray data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_command_output");
				return;
			} else {
				emit on_vim_command_output(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_VIM_EVAL:
		{
			QVariant data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_eval");
				return;
			} else {
				emit on_vim_eval(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_VIM_CALL_FUNCTION:
		{
			QVariant data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_call_function");
				return;
			} else {
				emit on_vim_call_function(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_VIM_STRWIDTH:
		{
			int64_t data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_strwidth");
				return;
			} else {
				emit on_vim_strwidth(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_VIM_LIST_RUNTIME_PATHS:
		{
			QList<QByteArray> data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_list_runtime_paths");
				return;
			} else {
				emit on_vim_list_runtime_paths(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_VIM_CHANGE_DIRECTORY:
		{
			emit on_vim_change_directory();
		}
		break;

		case NeovimApi4::NEOVIM_FN_VIM_GET_CURRENT_LINE:
		{
			QByteArray data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_get_current_line");
				return;
			} else {
				emit on_vim_get_current_line(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_VIM_SET_CURRENT_LINE:
		{
			emit on_vim_set_current_line();
		}
		break;

		case NeovimApi4::NEOVIM_FN_VIM_DEL_CURRENT_LINE:
		{
			emit on_vim_del_current_line();
		}
		break;

		case NeovimApi4::NEOVIM_FN_VIM_GET_VAR:
		{
			QVariant data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_get_var");
				return;
			} else {
				emit on_vim_get_var(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_VIM_GET_VVAR:
		{
			QVariant data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_get_vvar");
				return;
			} else {
				emit on_vim_get_vvar(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_VIM_GET_OPTION:
		{
			QVariant data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_get_option");
				return;
			} else {
				emit on_vim_get_option(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_VIM_SET_OPTION:
		{
			emit on_vim_set_option();
		}
		break;

		case NeovimApi4::NEOVIM_FN_VIM_OUT_WRITE:
		{
			emit on_vim_out_write();
		}
		break;

		case NeovimApi4::NEOVIM_FN_VIM_ERR_WRITE:
		{
			emit on_vim_err_write();
		}
		break;

		case NeovimApi4::NEOVIM_FN_VIM_REPORT_ERROR:
		{
			emit on_vim_report_error();
		}
		break;

		case NeovimApi4::NEOVIM_FN_VIM_GET_BUFFERS:
		{
			QList<int64_t> data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_get_buffers");
				return;
			} else {
				emit on_vim_get_buffers(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_VIM_GET_CURRENT_BUFFER:
		{
			int64_t data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_get_current_buffer");
				return;
			} else {
				emit on_vim_get_current_buffer(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_VIM_SET_CURRENT_BUFFER:
		{
			emit on_vim_set_current_buffer();
		}
		break;

		case NeovimApi4::NEOVIM_FN_VIM_GET_WINDOWS:
		{
			QList<int64_t> data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_get_windows");
				return;
			} else {
				emit on_vim_get_windows(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_VIM_GET_CURRENT_WINDOW:
		{
			int64_t data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_get_current_window");
				return;
			} else {
				emit on_vim_get_current_window(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_VIM_SET_CURRENT_WINDOW:
		{
			emit on_vim_set_current_window();
		}
		break;

		case NeovimApi4::NEOVIM_FN_VIM_GET_TABPAGES:
		{
			QList<int64_t> data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_get_tabpages");
				return;
			} else {
				emit on_vim_get_tabpages(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_VIM_GET_CURRENT_TABPAGE:
		{
			int64_t data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_get_current_tabpage");
				return;
			} else {
				emit on_vim_get_current_tabpage(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_VIM_SET_CURRENT_TABPAGE:
		{
			emit on_vim_set_current_tabpage();
		}
		break;

		case NeovimApi4::NEOVIM_FN_VIM_SUBSCRIBE:
		{
			emit on_vim_subscribe();
		}
		break;

		case NeovimApi4::NEOVIM_FN_VIM_UNSUBSCRIBE:
		{
			emit on_vim_unsubscribe();
		}
		break;

		case NeovimApi4::NEOVIM_FN_VIM_NAME_TO_COLOR:
		{
			int64_t data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_name_to_color");
				return;
			} else {
				emit on_vim_name_to_color(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_VIM_GET_COLOR_MAP:
		{
			QVariantMap data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_get_color_map");
				return;
			} else {
				emit on_vim_get_color_map(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_WINDOW_GET_BUFFER:
		{
			int64_t data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for window_get_buffer");
				return;
			} else {
				emit on_window_get_buffer(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_WINDOW_GET_CURSOR:
		{
			QPoint data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for window_get_cursor");
				return;
			} else {
				emit on_window_get_cursor(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_WINDOW_SET_CURSOR:
		{
			emit on_window_set_cursor();
		}
		break;

		case NeovimApi4::NEOVIM_FN_WINDOW_GET_HEIGHT:
		{
			int64_t data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for window_get_height");
				return;
			} else {
				emit on_window_get_height(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_WINDOW_SET_HEIGHT:
		{
			emit on_window_set_height();
		}
		break;

		case NeovimApi4::NEOVIM_FN_WINDOW_GET_WIDTH:
		{
			int64_t data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for window_get_width");
				return;
			} else {
				emit on_window_get_width(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_WINDOW_SET_WIDTH:
		{
			emit on_window_set_width();
		}
		break;

		case NeovimApi4::NEOVIM_FN_WINDOW_GET_VAR:
		{
			QVariant data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for window_get_var");
				return;
			} else {
				emit on_window_get_var(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_WINDOW_GET_OPTION:
		{
			QVariant data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for window_get_option");
				return;
			} else {
				emit on_window_get_option(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_WINDOW_SET_OPTION:
		{
			emit on_window_set_option();
		}
		break;

		case NeovimApi4::NEOVIM_FN_WINDOW_GET_POSITION:
		{
			QPoint data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for window_get_position");
				return;
			} else {
				emit on_window_get_position(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_WINDOW_GET_TABPAGE:
		{
			int64_t data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for window_get_tabpage");
				return;
			} else {
				emit on_window_get_tabpage(data);
			}
		}
		break;

		case NeovimApi4::NEOVIM_FN_WINDOW_IS_VALID:
		{
			bool data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for window_is_valid");
				return;
			} else {
				emit on_window_is_valid(data);
			}
		}
		break;

	default:
		qWarning() << "Received unexpected response";
	}
}

/**
 * Check function table from api_metadata[1]
 *
 * This checks the API metadata build from the bindings against the metadata
 * passed as argument.
 *
 * Returns false if there is an API mismatch
 */
static QVector<Function> GetSupportedList(const QVariantList& ftable,
	const QVector<Function>& required) noexcept
{
	QVector<Function> supported;

	for (const auto& val : ftable) {
		Function f{ Function::fromVariant(val) };
		if (!f.isValid()) {
			qDebug() << "Invalid function in metadata" << f;
			continue;
		}
		supported.append(f);

		if (!required.contains(f)) {
			qDebug() << "Unknown function(api 4)" << f;
		}
	}

	return supported;
}

bool NeovimApi4::checkFunctions(const QVariantList& ftable) noexcept
{
	static const QVector<Function> cs_required {
		Function{ "Integer", "nvim_buf_line_count",
			QVector<QString>{
				"Buffer",
			}
			, false },
		Function{ "String", "buffer_get_line",
			QVector<QString>{
				"Buffer",
				"Integer",
			}
			, false },
		Function{ "Boolean", "nvim_buf_attach",
			QVector<QString>{
				"Buffer",
				"Boolean",
				"Dictionary",
			}
			, false },
		Function{ "Boolean", "nvim_buf_detach",
			QVector<QString>{
				"Buffer",
			}
			, false },
		Function{ "void", "buffer_set_line",
			QVector<QString>{
				"Buffer",
				"Integer",
				"String",
			}
			, false },
		Function{ "void", "buffer_del_line",
			QVector<QString>{
				"Buffer",
				"Integer",
			}
			, false },
		Function{ "ArrayOf(String)", "buffer_get_line_slice",
			QVector<QString>{
				"Buffer",
				"Integer",
				"Integer",
				"Boolean",
				"Boolean",
			}
			, false },
		Function{ "ArrayOf(String)", "nvim_buf_get_lines",
			QVector<QString>{
				"Buffer",
				"Integer",
				"Integer",
				"Boolean",
			}
			, false },
		Function{ "void", "buffer_set_line_slice",
			QVector<QString>{
				"Buffer",
				"Integer",
				"Integer",
				"Boolean",
				"Boolean",
				"ArrayOf(String)",
			}
			, false },
		Function{ "void", "nvim_buf_set_lines",
			QVector<QString>{
				"Buffer",
				"Integer",
				"Integer",
				"Boolean",
				"ArrayOf(String)",
			}
			, false },
		Function{ "Object", "nvim_buf_get_var",
			QVector<QString>{
				"Buffer",
				"String",
			}
			, false },
		Function{ "Integer", "nvim_buf_get_changedtick",
			QVector<QString>{
				"Buffer",
			}
			, false },
		Function{ "ArrayOf(Dictionary)", "nvim_buf_get_keymap",
			QVector<QString>{
				"Buffer",
				"String",
			}
			, false },
		Function{ "Dictionary", "nvim_buf_get_commands",
			QVector<QString>{
				"Buffer",
				"Dictionary",
			}
			, false },
		Function{ "void", "nvim_buf_set_var",
			QVector<QString>{
				"Buffer",
				"String",
				"Object",
			}
			, false },
		Function{ "void", "nvim_buf_del_var",
			QVector<QString>{
				"Buffer",
				"String",
			}
			, false },
		Function{ "Object", "buffer_set_var",
			QVector<QString>{
				"Buffer",
				"String",
				"Object",
			}
			, false },
		Function{ "Object", "buffer_del_var",
			QVector<QString>{
				"Buffer",
				"String",
			}
			, false },
		Function{ "Object", "nvim_buf_get_option",
			QVector<QString>{
				"Buffer",
				"String",
			}
			, false },
		Function{ "void", "nvim_buf_set_option",
			QVector<QString>{
				"Buffer",
				"String",
				"Object",
			}
			, false },
		Function{ "Integer", "nvim_buf_get_number",
			QVector<QString>{
				"Buffer",
			}
			, false },
		Function{ "String", "nvim_buf_get_name",
			QVector<QString>{
				"Buffer",
			}
			, false },
		Function{ "void", "nvim_buf_set_name",
			QVector<QString>{
				"Buffer",
				"String",
			}
			, false },
		Function{ "Boolean", "nvim_buf_is_valid",
			QVector<QString>{
				"Buffer",
			}
			, false },
		Function{ "void", "buffer_insert",
			QVector<QString>{
				"Buffer",
				"Integer",
				"ArrayOf(String)",
			}
			, false },
		Function{ "ArrayOf(Integer, 2)", "nvim_buf_get_mark",
			QVector<QString>{
				"Buffer",
				"String",
			}
			, false },
		Function{ "Integer", "nvim_buf_add_highlight",
			QVector<QString>{
				"Buffer",
				"Integer",
				"String",
				"Integer",
				"Integer",
				"Integer",
			}
			, false },
		Function{ "void", "nvim_buf_clear_highlight",
			QVector<QString>{
				"Buffer",
				"Integer",
				"Integer",
				"Integer",
			}
			, false },
		Function{ "ArrayOf(Window)", "nvim_tabpage_list_wins",
			QVector<QString>{
				"Tabpage",
			}
			, false },
		Function{ "Object", "nvim_tabpage_get_var",
			QVector<QString>{
				"Tabpage",
				"String",
			}
			, false },
		Function{ "void", "nvim_tabpage_set_var",
			QVector<QString>{
				"Tabpage",
				"String",
				"Object",
			}
			, false },
		Function{ "void", "nvim_tabpage_del_var",
			QVector<QString>{
				"Tabpage",
				"String",
			}
			, false },
		Function{ "Object", "tabpage_set_var",
			QVector<QString>{
				"Tabpage",
				"String",
				"Object",
			}
			, false },
		Function{ "Object", "tabpage_del_var",
			QVector<QString>{
				"Tabpage",
				"String",
			}
			, false },
		Function{ "Window", "nvim_tabpage_get_win",
			QVector<QString>{
				"Tabpage",
			}
			, false },
		Function{ "Integer", "nvim_tabpage_get_number",
			QVector<QString>{
				"Tabpage",
			}
			, false },
		Function{ "Boolean", "nvim_tabpage_is_valid",
			QVector<QString>{
				"Tabpage",
			}
			, false },
		Function{ "void", "nvim_ui_attach",
			QVector<QString>{
				"Integer",
				"Integer",
				"Dictionary",
			}
			, false },
		Function{ "void", "ui_attach",
			QVector<QString>{
				"Integer",
				"Integer",
				"Boolean",
			}
			, false },
		Function{ "void", "nvim_ui_detach",
			QVector<QString>{
			}
			, false },
		Function{ "void", "nvim_ui_try_resize",
			QVector<QString>{
				"Integer",
				"Integer",
			}
			, false },
		Function{ "void", "nvim_ui_set_option",
			QVector<QString>{
				"String",
				"Object",
			}
			, false },
		Function{ "void", "nvim_command",
			QVector<QString>{
				"String",
			}
			, false },
		Function{ "Dictionary", "nvim_get_hl_by_name",
			QVector<QString>{
				"String",
				"Boolean",
			}
			, false },
		Function{ "Dictionary", "nvim_get_hl_by_id",
			QVector<QString>{
				"Integer",
				"Boolean",
			}
			, false },
		Function{ "void", "nvim_feedkeys",
			QVector<QString>{
				"String",
				"String",
				"Boolean",
			}
			, false },
		Function{ "Integer", "nvim_input",
			QVector<QString>{
				"String",
			}
			, false },
		Function{ "String", "nvim_replace_termcodes",
			QVector<QString>{
				"String",
				"Boolean",
				"Boolean",
				"Boolean",
			}
			, false },
		Function{ "String", "nvim_command_output",
			QVector<QString>{
				"String",
			}
			, false },
		Function{ "Object", "nvim_eval",
			QVector<QString>{
				"String",
			}
			, false },
		Function{ "Object", "nvim_execute_lua",
			QVector<QString>{
				"String",
				"Array",
			}
			, false },
		Function{ "Object", "nvim_call_function",
			QVector<QString>{
				"String",
				"Array",
			}
			, false },
		Function{ "Object", "nvim_call_dict_function",
			QVector<QString>{
				"Object",
				"String",
				"Array",
			}
			, false },
		Function{ "Integer", "nvim_strwidth",
			QVector<QString>{
				"String",
			}
			, false },
		Function{ "ArrayOf(String)", "nvim_list_runtime_paths",
			QVector<QString>{
			}
			, false },
		Function{ "void", "nvim_set_current_dir",
			QVector<QString>{
				"String",
			}
			, false },
		Function{ "String", "nvim_get_current_line",
			QVector<QString>{
			}
			, false },
		Function{ "void", "nvim_set_current_line",
			QVector<QString>{
				"String",
			}
			, false },
		Function{ "void", "nvim_del_current_line",
			QVector<QString>{
			}
			, false },
		Function{ "Object", "nvim_get_var",
			QVector<QString>{
				"String",
			}
			, false },
		Function{ "void", "nvim_set_var",
			QVector<QString>{
				"String",
				"Object",
			}
			, false },
		Function{ "void", "nvim_del_var",
			QVector<QString>{
				"String",
			}
			, false },
		Function{ "Object", "vim_set_var",
			QVector<QString>{
				"String",
				"Object",
			}
			, false },
		Function{ "Object", "vim_del_var",
			QVector<QString>{
				"String",
			}
			, false },
		Function{ "Object", "nvim_get_vvar",
			QVector<QString>{
				"String",
			}
			, false },
		Function{ "Object", "nvim_get_option",
			QVector<QString>{
				"String",
			}
			, false },
		Function{ "void", "nvim_set_option",
			QVector<QString>{
				"String",
				"Object",
			}
			, false },
		Function{ "void", "nvim_out_write",
			QVector<QString>{
				"String",
			}
			, false },
		Function{ "void", "nvim_err_write",
			QVector<QString>{
				"String",
			}
			, false },
		Function{ "void", "nvim_err_writeln",
			QVector<QString>{
				"String",
			}
			, false },
		Function{ "ArrayOf(Buffer)", "nvim_list_bufs",
			QVector<QString>{
			}
			, false },
		Function{ "Buffer", "nvim_get_current_buf",
			QVector<QString>{
			}
			, false },
		Function{ "void", "nvim_set_current_buf",
			QVector<QString>{
				"Buffer",
			}
			, false },
		Function{ "ArrayOf(Window)", "nvim_list_wins",
			QVector<QString>{
			}
			, false },
		Function{ "Window", "nvim_get_current_win",
			QVector<QString>{
			}
			, false },
		Function{ "void", "nvim_set_current_win",
			QVector<QString>{
				"Window",
			}
			, false },
		Function{ "ArrayOf(Tabpage)", "nvim_list_tabpages",
			QVector<QString>{
			}
			, false },
		Function{ "Tabpage", "nvim_get_current_tabpage",
			QVector<QString>{
			}
			, false },
		Function{ "void", "nvim_set_current_tabpage",
			QVector<QString>{
				"Tabpage",
			}
			, false },
		Function{ "void", "nvim_subscribe",
			QVector<QString>{
				"String",
			}
			, false },
		Function{ "void", "nvim_unsubscribe",
			QVector<QString>{
				"String",
			}
			, false },
		Function{ "Integer", "nvim_get_color_by_name",
			QVector<QString>{
				"String",
			}
			, false },
		Function{ "Dictionary", "nvim_get_color_map",
			QVector<QString>{
			}
			, false },
		Function{ "Dictionary", "nvim_get_mode",
			QVector<QString>{
			}
			, false },
		Function{ "ArrayOf(Dictionary)", "nvim_get_keymap",
			QVector<QString>{
				"String",
			}
			, false },
		Function{ "Dictionary", "nvim_get_commands",
			QVector<QString>{
				"Dictionary",
			}
			, false },
		Function{ "Array", "nvim_get_api_info",
			QVector<QString>{
			}
			, false },
		Function{ "void", "nvim_set_client_info",
			QVector<QString>{
				"String",
				"Dictionary",
				"String",
				"Dictionary",
				"Dictionary",
			}
			, false },
		Function{ "Dictionary", "nvim_get_chan_info",
			QVector<QString>{
				"Integer",
			}
			, false },
		Function{ "Array", "nvim_list_chans",
			QVector<QString>{
			}
			, false },
		Function{ "Array", "nvim_call_atomic",
			QVector<QString>{
				"Array",
			}
			, false },
		Function{ "Dictionary", "nvim_parse_expression",
			QVector<QString>{
				"String",
				"String",
				"Boolean",
			}
			, false },
		Function{ "Array", "nvim_list_uis",
			QVector<QString>{
			}
			, false },
		Function{ "Array", "nvim_get_proc_children",
			QVector<QString>{
				"Integer",
			}
			, false },
		Function{ "Object", "nvim_get_proc",
			QVector<QString>{
				"Integer",
			}
			, false },
		Function{ "Buffer", "nvim_win_get_buf",
			QVector<QString>{
				"Window",
			}
			, false },
		Function{ "ArrayOf(Integer, 2)", "nvim_win_get_cursor",
			QVector<QString>{
				"Window",
			}
			, false },
		Function{ "void", "nvim_win_set_cursor",
			QVector<QString>{
				"Window",
				"ArrayOf(Integer, 2)",
			}
			, false },
		Function{ "Integer", "nvim_win_get_height",
			QVector<QString>{
				"Window",
			}
			, false },
		Function{ "void", "nvim_win_set_height",
			QVector<QString>{
				"Window",
				"Integer",
			}
			, false },
		Function{ "Integer", "nvim_win_get_width",
			QVector<QString>{
				"Window",
			}
			, false },
		Function{ "void", "nvim_win_set_width",
			QVector<QString>{
				"Window",
				"Integer",
			}
			, false },
		Function{ "Object", "nvim_win_get_var",
			QVector<QString>{
				"Window",
				"String",
			}
			, false },
		Function{ "void", "nvim_win_set_var",
			QVector<QString>{
				"Window",
				"String",
				"Object",
			}
			, false },
		Function{ "void", "nvim_win_del_var",
			QVector<QString>{
				"Window",
				"String",
			}
			, false },
		Function{ "Object", "window_set_var",
			QVector<QString>{
				"Window",
				"String",
				"Object",
			}
			, false },
		Function{ "Object", "window_del_var",
			QVector<QString>{
				"Window",
				"String",
			}
			, false },
		Function{ "Object", "nvim_win_get_option",
			QVector<QString>{
				"Window",
				"String",
			}
			, false },
		Function{ "void", "nvim_win_set_option",
			QVector<QString>{
				"Window",
				"String",
				"Object",
			}
			, false },
		Function{ "ArrayOf(Integer, 2)", "nvim_win_get_position",
			QVector<QString>{
				"Window",
			}
			, false },
		Function{ "Tabpage", "nvim_win_get_tabpage",
			QVector<QString>{
				"Window",
			}
			, false },
		Function{ "Integer", "nvim_win_get_number",
			QVector<QString>{
				"Window",
			}
			, false },
		Function{ "Boolean", "nvim_win_is_valid",
			QVector<QString>{
				"Window",
			}
			, false },
		Function{ "Integer", "buffer_line_count",
			QVector<QString>{
				"Buffer",
			}
			, false },
		Function{ "ArrayOf(String)", "buffer_get_lines",
			QVector<QString>{
				"Buffer",
				"Integer",
				"Integer",
				"Boolean",
			}
			, false },
		Function{ "void", "buffer_set_lines",
			QVector<QString>{
				"Buffer",
				"Integer",
				"Integer",
				"Boolean",
				"ArrayOf(String)",
			}
			, false },
		Function{ "Object", "buffer_get_var",
			QVector<QString>{
				"Buffer",
				"String",
			}
			, false },
		Function{ "Object", "buffer_get_option",
			QVector<QString>{
				"Buffer",
				"String",
			}
			, false },
		Function{ "void", "buffer_set_option",
			QVector<QString>{
				"Buffer",
				"String",
				"Object",
			}
			, false },
		Function{ "Integer", "buffer_get_number",
			QVector<QString>{
				"Buffer",
			}
			, false },
		Function{ "String", "buffer_get_name",
			QVector<QString>{
				"Buffer",
			}
			, false },
		Function{ "void", "buffer_set_name",
			QVector<QString>{
				"Buffer",
				"String",
			}
			, false },
		Function{ "Boolean", "buffer_is_valid",
			QVector<QString>{
				"Buffer",
			}
			, false },
		Function{ "ArrayOf(Integer, 2)", "buffer_get_mark",
			QVector<QString>{
				"Buffer",
				"String",
			}
			, false },
		Function{ "Integer", "buffer_add_highlight",
			QVector<QString>{
				"Buffer",
				"Integer",
				"String",
				"Integer",
				"Integer",
				"Integer",
			}
			, false },
		Function{ "void", "buffer_clear_highlight",
			QVector<QString>{
				"Buffer",
				"Integer",
				"Integer",
				"Integer",
			}
			, false },
		Function{ "ArrayOf(Window)", "tabpage_get_windows",
			QVector<QString>{
				"Tabpage",
			}
			, false },
		Function{ "Object", "tabpage_get_var",
			QVector<QString>{
				"Tabpage",
				"String",
			}
			, false },
		Function{ "Window", "tabpage_get_window",
			QVector<QString>{
				"Tabpage",
			}
			, false },
		Function{ "Boolean", "tabpage_is_valid",
			QVector<QString>{
				"Tabpage",
			}
			, false },
		Function{ "void", "ui_detach",
			QVector<QString>{
			}
			, false },
		Function{ "Object", "ui_try_resize",
			QVector<QString>{
				"Integer",
				"Integer",
			}
			, false },
		Function{ "void", "vim_command",
			QVector<QString>{
				"String",
			}
			, false },
		Function{ "void", "vim_feedkeys",
			QVector<QString>{
				"String",
				"String",
				"Boolean",
			}
			, false },
		Function{ "Integer", "vim_input",
			QVector<QString>{
				"String",
			}
			, false },
		Function{ "String", "vim_replace_termcodes",
			QVector<QString>{
				"String",
				"Boolean",
				"Boolean",
				"Boolean",
			}
			, false },
		Function{ "String", "vim_command_output",
			QVector<QString>{
				"String",
			}
			, false },
		Function{ "Object", "vim_eval",
			QVector<QString>{
				"String",
			}
			, false },
		Function{ "Object", "vim_call_function",
			QVector<QString>{
				"String",
				"Array",
			}
			, false },
		Function{ "Integer", "vim_strwidth",
			QVector<QString>{
				"String",
			}
			, false },
		Function{ "ArrayOf(String)", "vim_list_runtime_paths",
			QVector<QString>{
			}
			, false },
		Function{ "void", "vim_change_directory",
			QVector<QString>{
				"String",
			}
			, false },
		Function{ "String", "vim_get_current_line",
			QVector<QString>{
			}
			, false },
		Function{ "void", "vim_set_current_line",
			QVector<QString>{
				"String",
			}
			, false },
		Function{ "void", "vim_del_current_line",
			QVector<QString>{
			}
			, false },
		Function{ "Object", "vim_get_var",
			QVector<QString>{
				"String",
			}
			, false },
		Function{ "Object", "vim_get_vvar",
			QVector<QString>{
				"String",
			}
			, false },
		Function{ "Object", "vim_get_option",
			QVector<QString>{
				"String",
			}
			, false },
		Function{ "void", "vim_set_option",
			QVector<QString>{
				"String",
				"Object",
			}
			, false },
		Function{ "void", "vim_out_write",
			QVector<QString>{
				"String",
			}
			, false },
		Function{ "void", "vim_err_write",
			QVector<QString>{
				"String",
			}
			, false },
		Function{ "void", "vim_report_error",
			QVector<QString>{
				"String",
			}
			, false },
		Function{ "ArrayOf(Buffer)", "vim_get_buffers",
			QVector<QString>{
			}
			, false },
		Function{ "Buffer", "vim_get_current_buffer",
			QVector<QString>{
			}
			, false },
		Function{ "void", "vim_set_current_buffer",
			QVector<QString>{
				"Buffer",
			}
			, false },
		Function{ "ArrayOf(Window)", "vim_get_windows",
			QVector<QString>{
			}
			, false },
		Function{ "Window", "vim_get_current_window",
			QVector<QString>{
			}
			, false },
		Function{ "void", "vim_set_current_window",
			QVector<QString>{
				"Window",
			}
			, false },
		Function{ "ArrayOf(Tabpage)", "vim_get_tabpages",
			QVector<QString>{
			}
			, false },
		Function{ "Tabpage", "vim_get_current_tabpage",
			QVector<QString>{
			}
			, false },
		Function{ "void", "vim_set_current_tabpage",
			QVector<QString>{
				"Tabpage",
			}
			, false },
		Function{ "void", "vim_subscribe",
			QVector<QString>{
				"String",
			}
			, false },
		Function{ "void", "vim_unsubscribe",
			QVector<QString>{
				"String",
			}
			, false },
		Function{ "Integer", "vim_name_to_color",
			QVector<QString>{
				"String",
			}
			, false },
		Function{ "Dictionary", "vim_get_color_map",
			QVector<QString>{
			}
			, false },
		Function{ "Buffer", "window_get_buffer",
			QVector<QString>{
				"Window",
			}
			, false },
		Function{ "ArrayOf(Integer, 2)", "window_get_cursor",
			QVector<QString>{
				"Window",
			}
			, false },
		Function{ "void", "window_set_cursor",
			QVector<QString>{
				"Window",
				"ArrayOf(Integer, 2)",
			}
			, false },
		Function{ "Integer", "window_get_height",
			QVector<QString>{
				"Window",
			}
			, false },
		Function{ "void", "window_set_height",
			QVector<QString>{
				"Window",
				"Integer",
			}
			, false },
		Function{ "Integer", "window_get_width",
			QVector<QString>{
				"Window",
			}
			, false },
		Function{ "void", "window_set_width",
			QVector<QString>{
				"Window",
				"Integer",
			}
			, false },
		Function{ "Object", "window_get_var",
			QVector<QString>{
				"Window",
				"String",
			}
			, false },
		Function{ "Object", "window_get_option",
			QVector<QString>{
				"Window",
				"String",
			}
			, false },
		Function{ "void", "window_set_option",
			QVector<QString>{
				"Window",
				"String",
				"Object",
			}
			, false },
		Function{ "ArrayOf(Integer, 2)", "window_get_position",
			QVector<QString>{
				"Window",
			}
			, false },
		Function{ "Tabpage", "window_get_tabpage",
			QVector<QString>{
				"Window",
			}
			, false },
		Function{ "Boolean", "window_is_valid",
			QVector<QString>{
				"Window",
			}
			, false },
		};

	static const QVector<Function> cs_supported{ GetSupportedList(ftable, cs_required) };

	bool ok{ true };
	for (const auto& f : cs_required) {
		if (!cs_supported.contains(f)) {
			qDebug() << "- instance DOES NOT support API4:" << f;
			ok = false;
		}
	}
	return ok;
}

} // namespace NeovimQt
