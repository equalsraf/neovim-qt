// Auto generated 2020-09-09 15:16:43.068844 from nvim API level:6
#include "auto/neovimapi6.h"
#include "msgpackiodevice.h"
#include "msgpackrequest.h"
#include "neovimconnector.h"
#include "util.h"

namespace NeovimQt {
/* Unpack Neovim EXT types Window, Buffer Tabpage which are all
 * uint64_t see Neovim:msgpack_rpc_to_
 */
static QVariant unpackBufferApi6(MsgpackIODevice *dev, const char* in, uint32_t size) noexcept
{
	msgpack_unpacked result;
	msgpack_unpacked_init(&result);
	msgpack_unpack_return ret = msgpack_unpack_next(&result, in, size, NULL);

	QVariant variant;

	if (ret == MSGPACK_UNPACK_SUCCESS) {
		switch (result.data.type) {
			case MSGPACK_OBJECT_NEGATIVE_INTEGER:
				variant.setValue<int64_t>(result.data.via.i64);
				break;
			case MSGPACK_OBJECT_POSITIVE_INTEGER:
				variant.setValue<uint64_t>(result.data.via.u64);
				break;
			default:
				// TODO it would be nice if we could call back MsgpackIoDevice method or primitive types here
				qWarning() << "Unsupported type found for EXT type" << result.data.type << result.data;
		}
	}

	msgpack_unpacked_destroy(&result);
	return variant;
}

static QVariant unpackWindowApi6(MsgpackIODevice *dev, const char* in, uint32_t size) noexcept
{
	return unpackBufferApi6(dev, in, size);
}

static QVariant unpackTabpageApi6(MsgpackIODevice *dev, const char* in, uint32_t size) noexcept
{
	return unpackBufferApi6(dev, in, size);
}

NeovimApi6::NeovimApi6(NeovimConnector *c) noexcept
	: m_connector{ c }
{
	// EXT types
	m_connector->m_dev->registerExtType(0, unpackBufferApi6);
	m_connector->m_dev->registerExtType(1, unpackWindowApi6);
	m_connector->m_dev->registerExtType(2, unpackTabpageApi6);
	connect(m_connector->m_dev, &MsgpackIODevice::notification,
		this, &NeovimApi6::neovimNotification);
}

// Slots
MsgpackRequest* NeovimApi6::nvim_buf_line_count(int64_t buffer)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_buf_line_count", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_BUF_LINE_COUNT);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	return r;
}

MsgpackRequest* NeovimApi6::buffer_get_line(int64_t buffer, int64_t index)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("buffer_get_line", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_BUFFER_GET_LINE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	m_connector->m_dev->send(index);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_buf_attach(int64_t buffer, bool send_buffer, QVariantMap opts)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_buf_attach", 3) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_BUF_ATTACH);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	m_connector->m_dev->send(send_buffer);
	m_connector->m_dev->send(opts);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_buf_detach(int64_t buffer)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_buf_detach", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_BUF_DETACH);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	return r;
}

MsgpackRequest* NeovimApi6::buffer_set_line(int64_t buffer, int64_t index, QByteArray line)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("buffer_set_line", 3) };
	r->setFunction(NeovimApi6::NEOVIM_FN_BUFFER_SET_LINE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	m_connector->m_dev->send(index);
	m_connector->m_dev->send(line);
	return r;
}

MsgpackRequest* NeovimApi6::buffer_del_line(int64_t buffer, int64_t index)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("buffer_del_line", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_BUFFER_DEL_LINE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	m_connector->m_dev->send(index);
	return r;
}

MsgpackRequest* NeovimApi6::buffer_get_line_slice(int64_t buffer, int64_t start, int64_t end, bool include_start, bool include_end)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("buffer_get_line_slice", 5) };
	r->setFunction(NeovimApi6::NEOVIM_FN_BUFFER_GET_LINE_SLICE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	m_connector->m_dev->send(start);
	m_connector->m_dev->send(end);
	m_connector->m_dev->send(include_start);
	m_connector->m_dev->send(include_end);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_buf_get_lines(int64_t buffer, int64_t start, int64_t end, bool strict_indexing)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_buf_get_lines", 4) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_BUF_GET_LINES);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	m_connector->m_dev->send(start);
	m_connector->m_dev->send(end);
	m_connector->m_dev->send(strict_indexing);
	return r;
}

MsgpackRequest* NeovimApi6::buffer_set_line_slice(int64_t buffer, int64_t start, int64_t end, bool include_start, bool include_end, QList<QByteArray> replacement)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("buffer_set_line_slice", 6) };
	r->setFunction(NeovimApi6::NEOVIM_FN_BUFFER_SET_LINE_SLICE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	m_connector->m_dev->send(start);
	m_connector->m_dev->send(end);
	m_connector->m_dev->send(include_start);
	m_connector->m_dev->send(include_end);
	m_connector->m_dev->sendArrayOf(replacement);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_buf_set_lines(int64_t buffer, int64_t start, int64_t end, bool strict_indexing, QList<QByteArray> replacement)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_buf_set_lines", 5) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_BUF_SET_LINES);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	m_connector->m_dev->send(start);
	m_connector->m_dev->send(end);
	m_connector->m_dev->send(strict_indexing);
	m_connector->m_dev->sendArrayOf(replacement);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_buf_get_offset(int64_t buffer, int64_t index)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_buf_get_offset", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_BUF_GET_OFFSET);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	m_connector->m_dev->send(index);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_buf_get_var(int64_t buffer, QByteArray name)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_buf_get_var", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_BUF_GET_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	m_connector->m_dev->send(name);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_buf_get_changedtick(int64_t buffer)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_buf_get_changedtick", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_BUF_GET_CHANGEDTICK);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_buf_get_keymap(int64_t buffer, QByteArray mode)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_buf_get_keymap", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_BUF_GET_KEYMAP);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	m_connector->m_dev->send(mode);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_buf_set_keymap(int64_t buffer, QByteArray mode, QByteArray lhs, QByteArray rhs, QVariantMap opts)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_buf_set_keymap", 5) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_BUF_SET_KEYMAP);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	m_connector->m_dev->send(mode);
	m_connector->m_dev->send(lhs);
	m_connector->m_dev->send(rhs);
	m_connector->m_dev->send(opts);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_buf_del_keymap(int64_t buffer, QByteArray mode, QByteArray lhs)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_buf_del_keymap", 3) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_BUF_DEL_KEYMAP);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	m_connector->m_dev->send(mode);
	m_connector->m_dev->send(lhs);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_buf_get_commands(int64_t buffer, QVariantMap opts)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_buf_get_commands", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_BUF_GET_COMMANDS);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	m_connector->m_dev->send(opts);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_buf_set_var(int64_t buffer, QByteArray name, QVariant value)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_buf_set_var", 3) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_BUF_SET_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	m_connector->m_dev->send(name);
	m_connector->m_dev->send(value);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_buf_del_var(int64_t buffer, QByteArray name)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_buf_del_var", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_BUF_DEL_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	m_connector->m_dev->send(name);
	return r;
}

MsgpackRequest* NeovimApi6::buffer_set_var(int64_t buffer, QByteArray name, QVariant value)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("buffer_set_var", 3) };
	r->setFunction(NeovimApi6::NEOVIM_FN_BUFFER_SET_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	m_connector->m_dev->send(name);
	m_connector->m_dev->send(value);
	return r;
}

MsgpackRequest* NeovimApi6::buffer_del_var(int64_t buffer, QByteArray name)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("buffer_del_var", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_BUFFER_DEL_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	m_connector->m_dev->send(name);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_buf_get_option(int64_t buffer, QByteArray name)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_buf_get_option", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_BUF_GET_OPTION);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	m_connector->m_dev->send(name);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_buf_set_option(int64_t buffer, QByteArray name, QVariant value)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_buf_set_option", 3) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_BUF_SET_OPTION);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	m_connector->m_dev->send(name);
	m_connector->m_dev->send(value);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_buf_get_number(int64_t buffer)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_buf_get_number", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_BUF_GET_NUMBER);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_buf_get_name(int64_t buffer)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_buf_get_name", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_BUF_GET_NAME);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_buf_set_name(int64_t buffer, QByteArray name)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_buf_set_name", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_BUF_SET_NAME);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	m_connector->m_dev->send(name);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_buf_is_loaded(int64_t buffer)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_buf_is_loaded", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_BUF_IS_LOADED);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_buf_is_valid(int64_t buffer)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_buf_is_valid", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_BUF_IS_VALID);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	return r;
}

MsgpackRequest* NeovimApi6::buffer_insert(int64_t buffer, int64_t lnum, QList<QByteArray> lines)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("buffer_insert", 3) };
	r->setFunction(NeovimApi6::NEOVIM_FN_BUFFER_INSERT);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	m_connector->m_dev->send(lnum);
	m_connector->m_dev->sendArrayOf(lines);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_buf_get_mark(int64_t buffer, QByteArray name)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_buf_get_mark", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_BUF_GET_MARK);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	m_connector->m_dev->send(name);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_buf_add_highlight(int64_t buffer, int64_t ns_id, QByteArray hl_group, int64_t line, int64_t col_start, int64_t col_end)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_buf_add_highlight", 6) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_BUF_ADD_HIGHLIGHT);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	m_connector->m_dev->send(ns_id);
	m_connector->m_dev->send(hl_group);
	m_connector->m_dev->send(line);
	m_connector->m_dev->send(col_start);
	m_connector->m_dev->send(col_end);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_buf_clear_namespace(int64_t buffer, int64_t ns_id, int64_t line_start, int64_t line_end)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_buf_clear_namespace", 4) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_BUF_CLEAR_NAMESPACE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	m_connector->m_dev->send(ns_id);
	m_connector->m_dev->send(line_start);
	m_connector->m_dev->send(line_end);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_buf_clear_highlight(int64_t buffer, int64_t ns_id, int64_t line_start, int64_t line_end)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_buf_clear_highlight", 4) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_BUF_CLEAR_HIGHLIGHT);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	m_connector->m_dev->send(ns_id);
	m_connector->m_dev->send(line_start);
	m_connector->m_dev->send(line_end);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_buf_set_virtual_text(int64_t buffer, int64_t ns_id, int64_t line, QVariantList chunks, QVariantMap opts)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_buf_set_virtual_text", 5) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_BUF_SET_VIRTUAL_TEXT);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	m_connector->m_dev->send(ns_id);
	m_connector->m_dev->send(line);
	m_connector->m_dev->send(chunks);
	m_connector->m_dev->send(opts);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_tabpage_list_wins(int64_t tabpage)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_tabpage_list_wins", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_TABPAGE_LIST_WINS);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(tabpage);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_tabpage_get_var(int64_t tabpage, QByteArray name)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_tabpage_get_var", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_TABPAGE_GET_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(tabpage);
	m_connector->m_dev->send(name);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_tabpage_set_var(int64_t tabpage, QByteArray name, QVariant value)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_tabpage_set_var", 3) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_TABPAGE_SET_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(tabpage);
	m_connector->m_dev->send(name);
	m_connector->m_dev->send(value);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_tabpage_del_var(int64_t tabpage, QByteArray name)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_tabpage_del_var", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_TABPAGE_DEL_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(tabpage);
	m_connector->m_dev->send(name);
	return r;
}

MsgpackRequest* NeovimApi6::tabpage_set_var(int64_t tabpage, QByteArray name, QVariant value)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("tabpage_set_var", 3) };
	r->setFunction(NeovimApi6::NEOVIM_FN_TABPAGE_SET_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(tabpage);
	m_connector->m_dev->send(name);
	m_connector->m_dev->send(value);
	return r;
}

MsgpackRequest* NeovimApi6::tabpage_del_var(int64_t tabpage, QByteArray name)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("tabpage_del_var", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_TABPAGE_DEL_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(tabpage);
	m_connector->m_dev->send(name);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_tabpage_get_win(int64_t tabpage)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_tabpage_get_win", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_TABPAGE_GET_WIN);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(tabpage);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_tabpage_get_number(int64_t tabpage)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_tabpage_get_number", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_TABPAGE_GET_NUMBER);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(tabpage);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_tabpage_is_valid(int64_t tabpage)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_tabpage_is_valid", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_TABPAGE_IS_VALID);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(tabpage);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_ui_attach(int64_t width, int64_t height, QVariantMap options)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_ui_attach", 3) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_UI_ATTACH);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(width);
	m_connector->m_dev->send(height);
	m_connector->m_dev->send(options);
	return r;
}

MsgpackRequest* NeovimApi6::ui_attach(int64_t width, int64_t height, bool enable_rgb)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("ui_attach", 3) };
	r->setFunction(NeovimApi6::NEOVIM_FN_UI_ATTACH);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(width);
	m_connector->m_dev->send(height);
	m_connector->m_dev->send(enable_rgb);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_ui_detach()
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_ui_detach", 0) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_UI_DETACH);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_ui_try_resize(int64_t width, int64_t height)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_ui_try_resize", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_UI_TRY_RESIZE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(width);
	m_connector->m_dev->send(height);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_ui_set_option(QByteArray name, QVariant value)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_ui_set_option", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_UI_SET_OPTION);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(name);
	m_connector->m_dev->send(value);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_ui_try_resize_grid(int64_t grid, int64_t width, int64_t height)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_ui_try_resize_grid", 3) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_UI_TRY_RESIZE_GRID);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(grid);
	m_connector->m_dev->send(width);
	m_connector->m_dev->send(height);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_ui_pum_set_height(int64_t height)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_ui_pum_set_height", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_UI_PUM_SET_HEIGHT);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(height);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_command(QByteArray command)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_command", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_COMMAND);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(command);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_get_hl_by_name(QByteArray name, bool rgb)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_get_hl_by_name", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_GET_HL_BY_NAME);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(name);
	m_connector->m_dev->send(rgb);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_get_hl_by_id(int64_t hl_id, bool rgb)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_get_hl_by_id", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_GET_HL_BY_ID);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(hl_id);
	m_connector->m_dev->send(rgb);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_feedkeys(QByteArray keys, QByteArray mode, bool escape_csi)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_feedkeys", 3) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_FEEDKEYS);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(keys);
	m_connector->m_dev->send(mode);
	m_connector->m_dev->send(escape_csi);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_input(QByteArray keys)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_input", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_INPUT);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(keys);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_input_mouse(QByteArray button, QByteArray action, QByteArray modifier, int64_t grid, int64_t row, int64_t col)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_input_mouse", 6) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_INPUT_MOUSE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(button);
	m_connector->m_dev->send(action);
	m_connector->m_dev->send(modifier);
	m_connector->m_dev->send(grid);
	m_connector->m_dev->send(row);
	m_connector->m_dev->send(col);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_replace_termcodes(QByteArray str, bool from_part, bool do_lt, bool special)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_replace_termcodes", 4) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_REPLACE_TERMCODES);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(str);
	m_connector->m_dev->send(from_part);
	m_connector->m_dev->send(do_lt);
	m_connector->m_dev->send(special);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_command_output(QByteArray command)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_command_output", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_COMMAND_OUTPUT);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(command);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_eval(QByteArray expr)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_eval", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_EVAL);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(expr);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_execute_lua(QByteArray code, QVariantList args)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_execute_lua", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_EXECUTE_LUA);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(code);
	m_connector->m_dev->send(args);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_call_function(QByteArray fn, QVariantList args)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_call_function", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_CALL_FUNCTION);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(fn);
	m_connector->m_dev->send(args);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_call_dict_function(QVariant dict, QByteArray fn, QVariantList args)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_call_dict_function", 3) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_CALL_DICT_FUNCTION);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(dict);
	m_connector->m_dev->send(fn);
	m_connector->m_dev->send(args);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_strwidth(QByteArray text)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_strwidth", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_STRWIDTH);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(text);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_list_runtime_paths()
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_list_runtime_paths", 0) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_LIST_RUNTIME_PATHS);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_set_current_dir(QByteArray dir)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_set_current_dir", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_SET_CURRENT_DIR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(dir);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_get_current_line()
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_get_current_line", 0) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_GET_CURRENT_LINE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_set_current_line(QByteArray line)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_set_current_line", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_SET_CURRENT_LINE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(line);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_del_current_line()
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_del_current_line", 0) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_DEL_CURRENT_LINE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_get_var(QByteArray name)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_get_var", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_GET_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(name);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_set_var(QByteArray name, QVariant value)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_set_var", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_SET_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(name);
	m_connector->m_dev->send(value);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_del_var(QByteArray name)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_del_var", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_DEL_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(name);
	return r;
}

MsgpackRequest* NeovimApi6::vim_set_var(QByteArray name, QVariant value)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("vim_set_var", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_VIM_SET_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(name);
	m_connector->m_dev->send(value);
	return r;
}

MsgpackRequest* NeovimApi6::vim_del_var(QByteArray name)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("vim_del_var", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_VIM_DEL_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(name);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_get_vvar(QByteArray name)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_get_vvar", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_GET_VVAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(name);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_set_vvar(QByteArray name, QVariant value)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_set_vvar", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_SET_VVAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(name);
	m_connector->m_dev->send(value);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_get_option(QByteArray name)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_get_option", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_GET_OPTION);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(name);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_set_option(QByteArray name, QVariant value)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_set_option", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_SET_OPTION);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(name);
	m_connector->m_dev->send(value);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_out_write(QByteArray str)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_out_write", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_OUT_WRITE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(str);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_err_write(QByteArray str)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_err_write", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_ERR_WRITE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(str);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_err_writeln(QByteArray str)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_err_writeln", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_ERR_WRITELN);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(str);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_list_bufs()
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_list_bufs", 0) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_LIST_BUFS);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_get_current_buf()
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_get_current_buf", 0) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_GET_CURRENT_BUF);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_set_current_buf(int64_t buffer)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_set_current_buf", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_SET_CURRENT_BUF);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_list_wins()
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_list_wins", 0) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_LIST_WINS);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_get_current_win()
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_get_current_win", 0) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_GET_CURRENT_WIN);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_set_current_win(int64_t window)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_set_current_win", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_SET_CURRENT_WIN);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(window);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_create_buf(bool listed, bool scratch)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_create_buf", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_CREATE_BUF);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(listed);
	m_connector->m_dev->send(scratch);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_open_win(int64_t buffer, bool enter, QVariantMap config)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_open_win", 3) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_OPEN_WIN);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	m_connector->m_dev->send(enter);
	m_connector->m_dev->send(config);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_list_tabpages()
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_list_tabpages", 0) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_LIST_TABPAGES);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_get_current_tabpage()
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_get_current_tabpage", 0) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_GET_CURRENT_TABPAGE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_set_current_tabpage(int64_t tabpage)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_set_current_tabpage", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_SET_CURRENT_TABPAGE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(tabpage);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_create_namespace(QByteArray name)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_create_namespace", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_CREATE_NAMESPACE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(name);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_get_namespaces()
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_get_namespaces", 0) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_GET_NAMESPACES);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_paste(QByteArray data, bool crlf, int64_t phase)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_paste", 3) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_PASTE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(data);
	m_connector->m_dev->send(crlf);
	m_connector->m_dev->send(phase);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_put(QList<QByteArray> lines, QByteArray type, bool after, bool follow)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_put", 4) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_PUT);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->sendArrayOf(lines);
	m_connector->m_dev->send(type);
	m_connector->m_dev->send(after);
	m_connector->m_dev->send(follow);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_subscribe(QByteArray event)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_subscribe", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_SUBSCRIBE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(event);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_unsubscribe(QByteArray event)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_unsubscribe", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_UNSUBSCRIBE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(event);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_get_color_by_name(QByteArray name)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_get_color_by_name", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_GET_COLOR_BY_NAME);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(name);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_get_color_map()
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_get_color_map", 0) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_GET_COLOR_MAP);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_get_context(QVariantMap opts)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_get_context", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_GET_CONTEXT);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(opts);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_load_context(QVariantMap dict)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_load_context", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_LOAD_CONTEXT);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(dict);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_get_mode()
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_get_mode", 0) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_GET_MODE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_get_keymap(QByteArray mode)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_get_keymap", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_GET_KEYMAP);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(mode);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_set_keymap(QByteArray mode, QByteArray lhs, QByteArray rhs, QVariantMap opts)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_set_keymap", 4) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_SET_KEYMAP);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(mode);
	m_connector->m_dev->send(lhs);
	m_connector->m_dev->send(rhs);
	m_connector->m_dev->send(opts);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_del_keymap(QByteArray mode, QByteArray lhs)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_del_keymap", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_DEL_KEYMAP);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(mode);
	m_connector->m_dev->send(lhs);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_get_commands(QVariantMap opts)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_get_commands", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_GET_COMMANDS);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(opts);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_get_api_info()
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_get_api_info", 0) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_GET_API_INFO);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_set_client_info(QByteArray name, QVariantMap version, QByteArray type, QVariantMap methods, QVariantMap attributes)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_set_client_info", 5) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_SET_CLIENT_INFO);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(name);
	m_connector->m_dev->send(version);
	m_connector->m_dev->send(type);
	m_connector->m_dev->send(methods);
	m_connector->m_dev->send(attributes);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_get_chan_info(int64_t chan)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_get_chan_info", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_GET_CHAN_INFO);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(chan);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_list_chans()
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_list_chans", 0) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_LIST_CHANS);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_call_atomic(QVariantList calls)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_call_atomic", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_CALL_ATOMIC);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(calls);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_parse_expression(QByteArray expr, QByteArray flags, bool highlight)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_parse_expression", 3) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_PARSE_EXPRESSION);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(expr);
	m_connector->m_dev->send(flags);
	m_connector->m_dev->send(highlight);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_list_uis()
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_list_uis", 0) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_LIST_UIS);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_get_proc_children(int64_t pid)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_get_proc_children", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_GET_PROC_CHILDREN);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(pid);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_get_proc(int64_t pid)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_get_proc", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_GET_PROC);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(pid);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_select_popupmenu_item(int64_t item, bool insert, bool finish, QVariantMap opts)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_select_popupmenu_item", 4) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_SELECT_POPUPMENU_ITEM);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(item);
	m_connector->m_dev->send(insert);
	m_connector->m_dev->send(finish);
	m_connector->m_dev->send(opts);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_win_get_buf(int64_t window)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_win_get_buf", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_WIN_GET_BUF);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(window);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_win_set_buf(int64_t window, int64_t buffer)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_win_set_buf", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_WIN_SET_BUF);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(window);
	m_connector->m_dev->send(buffer);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_win_get_cursor(int64_t window)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_win_get_cursor", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_WIN_GET_CURSOR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(window);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_win_set_cursor(int64_t window, QPoint pos)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_win_set_cursor", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_WIN_SET_CURSOR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(window);
	m_connector->m_dev->send(pos);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_win_get_height(int64_t window)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_win_get_height", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_WIN_GET_HEIGHT);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(window);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_win_set_height(int64_t window, int64_t height)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_win_set_height", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_WIN_SET_HEIGHT);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(window);
	m_connector->m_dev->send(height);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_win_get_width(int64_t window)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_win_get_width", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_WIN_GET_WIDTH);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(window);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_win_set_width(int64_t window, int64_t width)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_win_set_width", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_WIN_SET_WIDTH);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(window);
	m_connector->m_dev->send(width);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_win_get_var(int64_t window, QByteArray name)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_win_get_var", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_WIN_GET_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(window);
	m_connector->m_dev->send(name);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_win_set_var(int64_t window, QByteArray name, QVariant value)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_win_set_var", 3) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_WIN_SET_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(window);
	m_connector->m_dev->send(name);
	m_connector->m_dev->send(value);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_win_del_var(int64_t window, QByteArray name)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_win_del_var", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_WIN_DEL_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(window);
	m_connector->m_dev->send(name);
	return r;
}

MsgpackRequest* NeovimApi6::window_set_var(int64_t window, QByteArray name, QVariant value)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("window_set_var", 3) };
	r->setFunction(NeovimApi6::NEOVIM_FN_WINDOW_SET_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(window);
	m_connector->m_dev->send(name);
	m_connector->m_dev->send(value);
	return r;
}

MsgpackRequest* NeovimApi6::window_del_var(int64_t window, QByteArray name)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("window_del_var", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_WINDOW_DEL_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(window);
	m_connector->m_dev->send(name);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_win_get_option(int64_t window, QByteArray name)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_win_get_option", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_WIN_GET_OPTION);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(window);
	m_connector->m_dev->send(name);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_win_set_option(int64_t window, QByteArray name, QVariant value)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_win_set_option", 3) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_WIN_SET_OPTION);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(window);
	m_connector->m_dev->send(name);
	m_connector->m_dev->send(value);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_win_get_position(int64_t window)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_win_get_position", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_WIN_GET_POSITION);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(window);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_win_get_tabpage(int64_t window)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_win_get_tabpage", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_WIN_GET_TABPAGE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(window);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_win_get_number(int64_t window)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_win_get_number", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_WIN_GET_NUMBER);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(window);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_win_is_valid(int64_t window)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_win_is_valid", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_WIN_IS_VALID);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(window);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_win_set_config(int64_t window, QVariantMap config)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_win_set_config", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_WIN_SET_CONFIG);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(window);
	m_connector->m_dev->send(config);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_win_get_config(int64_t window)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_win_get_config", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_WIN_GET_CONFIG);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(window);
	return r;
}

MsgpackRequest* NeovimApi6::nvim_win_close(int64_t window, bool force)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("nvim_win_close", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_NVIM_WIN_CLOSE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(window);
	m_connector->m_dev->send(force);
	return r;
}

MsgpackRequest* NeovimApi6::buffer_line_count(int64_t buffer)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("buffer_line_count", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_BUFFER_LINE_COUNT);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	return r;
}

MsgpackRequest* NeovimApi6::buffer_get_lines(int64_t buffer, int64_t start, int64_t end, bool strict_indexing)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("buffer_get_lines", 4) };
	r->setFunction(NeovimApi6::NEOVIM_FN_BUFFER_GET_LINES);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	m_connector->m_dev->send(start);
	m_connector->m_dev->send(end);
	m_connector->m_dev->send(strict_indexing);
	return r;
}

MsgpackRequest* NeovimApi6::buffer_set_lines(int64_t buffer, int64_t start, int64_t end, bool strict_indexing, QList<QByteArray> replacement)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("buffer_set_lines", 5) };
	r->setFunction(NeovimApi6::NEOVIM_FN_BUFFER_SET_LINES);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	m_connector->m_dev->send(start);
	m_connector->m_dev->send(end);
	m_connector->m_dev->send(strict_indexing);
	m_connector->m_dev->sendArrayOf(replacement);
	return r;
}

MsgpackRequest* NeovimApi6::buffer_get_var(int64_t buffer, QByteArray name)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("buffer_get_var", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_BUFFER_GET_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	m_connector->m_dev->send(name);
	return r;
}

MsgpackRequest* NeovimApi6::buffer_get_option(int64_t buffer, QByteArray name)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("buffer_get_option", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_BUFFER_GET_OPTION);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	m_connector->m_dev->send(name);
	return r;
}

MsgpackRequest* NeovimApi6::buffer_set_option(int64_t buffer, QByteArray name, QVariant value)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("buffer_set_option", 3) };
	r->setFunction(NeovimApi6::NEOVIM_FN_BUFFER_SET_OPTION);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	m_connector->m_dev->send(name);
	m_connector->m_dev->send(value);
	return r;
}

MsgpackRequest* NeovimApi6::buffer_get_number(int64_t buffer)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("buffer_get_number", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_BUFFER_GET_NUMBER);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	return r;
}

MsgpackRequest* NeovimApi6::buffer_get_name(int64_t buffer)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("buffer_get_name", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_BUFFER_GET_NAME);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	return r;
}

MsgpackRequest* NeovimApi6::buffer_set_name(int64_t buffer, QByteArray name)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("buffer_set_name", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_BUFFER_SET_NAME);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	m_connector->m_dev->send(name);
	return r;
}

MsgpackRequest* NeovimApi6::buffer_is_valid(int64_t buffer)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("buffer_is_valid", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_BUFFER_IS_VALID);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	return r;
}

MsgpackRequest* NeovimApi6::buffer_get_mark(int64_t buffer, QByteArray name)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("buffer_get_mark", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_BUFFER_GET_MARK);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	m_connector->m_dev->send(name);
	return r;
}

MsgpackRequest* NeovimApi6::buffer_add_highlight(int64_t buffer, int64_t ns_id, QByteArray hl_group, int64_t line, int64_t col_start, int64_t col_end)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("buffer_add_highlight", 6) };
	r->setFunction(NeovimApi6::NEOVIM_FN_BUFFER_ADD_HIGHLIGHT);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	m_connector->m_dev->send(ns_id);
	m_connector->m_dev->send(hl_group);
	m_connector->m_dev->send(line);
	m_connector->m_dev->send(col_start);
	m_connector->m_dev->send(col_end);
	return r;
}

MsgpackRequest* NeovimApi6::buffer_clear_highlight(int64_t buffer, int64_t ns_id, int64_t line_start, int64_t line_end)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("buffer_clear_highlight", 4) };
	r->setFunction(NeovimApi6::NEOVIM_FN_BUFFER_CLEAR_HIGHLIGHT);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	m_connector->m_dev->send(ns_id);
	m_connector->m_dev->send(line_start);
	m_connector->m_dev->send(line_end);
	return r;
}

MsgpackRequest* NeovimApi6::tabpage_get_windows(int64_t tabpage)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("tabpage_get_windows", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_TABPAGE_GET_WINDOWS);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(tabpage);
	return r;
}

MsgpackRequest* NeovimApi6::tabpage_get_var(int64_t tabpage, QByteArray name)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("tabpage_get_var", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_TABPAGE_GET_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(tabpage);
	m_connector->m_dev->send(name);
	return r;
}

MsgpackRequest* NeovimApi6::tabpage_get_window(int64_t tabpage)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("tabpage_get_window", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_TABPAGE_GET_WINDOW);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(tabpage);
	return r;
}

MsgpackRequest* NeovimApi6::tabpage_is_valid(int64_t tabpage)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("tabpage_is_valid", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_TABPAGE_IS_VALID);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(tabpage);
	return r;
}

MsgpackRequest* NeovimApi6::ui_detach()
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("ui_detach", 0) };
	r->setFunction(NeovimApi6::NEOVIM_FN_UI_DETACH);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	return r;
}

MsgpackRequest* NeovimApi6::ui_try_resize(int64_t width, int64_t height)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("ui_try_resize", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_UI_TRY_RESIZE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(width);
	m_connector->m_dev->send(height);
	return r;
}

MsgpackRequest* NeovimApi6::vim_command(QByteArray command)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("vim_command", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_VIM_COMMAND);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(command);
	return r;
}

MsgpackRequest* NeovimApi6::vim_feedkeys(QByteArray keys, QByteArray mode, bool escape_csi)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("vim_feedkeys", 3) };
	r->setFunction(NeovimApi6::NEOVIM_FN_VIM_FEEDKEYS);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(keys);
	m_connector->m_dev->send(mode);
	m_connector->m_dev->send(escape_csi);
	return r;
}

MsgpackRequest* NeovimApi6::vim_input(QByteArray keys)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("vim_input", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_VIM_INPUT);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(keys);
	return r;
}

MsgpackRequest* NeovimApi6::vim_replace_termcodes(QByteArray str, bool from_part, bool do_lt, bool special)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("vim_replace_termcodes", 4) };
	r->setFunction(NeovimApi6::NEOVIM_FN_VIM_REPLACE_TERMCODES);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(str);
	m_connector->m_dev->send(from_part);
	m_connector->m_dev->send(do_lt);
	m_connector->m_dev->send(special);
	return r;
}

MsgpackRequest* NeovimApi6::vim_command_output(QByteArray command)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("vim_command_output", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_VIM_COMMAND_OUTPUT);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(command);
	return r;
}

MsgpackRequest* NeovimApi6::vim_eval(QByteArray expr)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("vim_eval", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_VIM_EVAL);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(expr);
	return r;
}

MsgpackRequest* NeovimApi6::vim_call_function(QByteArray fn, QVariantList args)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("vim_call_function", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_VIM_CALL_FUNCTION);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(fn);
	m_connector->m_dev->send(args);
	return r;
}

MsgpackRequest* NeovimApi6::vim_strwidth(QByteArray text)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("vim_strwidth", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_VIM_STRWIDTH);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(text);
	return r;
}

MsgpackRequest* NeovimApi6::vim_list_runtime_paths()
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("vim_list_runtime_paths", 0) };
	r->setFunction(NeovimApi6::NEOVIM_FN_VIM_LIST_RUNTIME_PATHS);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	return r;
}

MsgpackRequest* NeovimApi6::vim_change_directory(QByteArray dir)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("vim_change_directory", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_VIM_CHANGE_DIRECTORY);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(dir);
	return r;
}

MsgpackRequest* NeovimApi6::vim_get_current_line()
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("vim_get_current_line", 0) };
	r->setFunction(NeovimApi6::NEOVIM_FN_VIM_GET_CURRENT_LINE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	return r;
}

MsgpackRequest* NeovimApi6::vim_set_current_line(QByteArray line)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("vim_set_current_line", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_VIM_SET_CURRENT_LINE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(line);
	return r;
}

MsgpackRequest* NeovimApi6::vim_del_current_line()
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("vim_del_current_line", 0) };
	r->setFunction(NeovimApi6::NEOVIM_FN_VIM_DEL_CURRENT_LINE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	return r;
}

MsgpackRequest* NeovimApi6::vim_get_var(QByteArray name)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("vim_get_var", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_VIM_GET_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(name);
	return r;
}

MsgpackRequest* NeovimApi6::vim_get_vvar(QByteArray name)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("vim_get_vvar", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_VIM_GET_VVAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(name);
	return r;
}

MsgpackRequest* NeovimApi6::vim_get_option(QByteArray name)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("vim_get_option", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_VIM_GET_OPTION);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(name);
	return r;
}

MsgpackRequest* NeovimApi6::vim_set_option(QByteArray name, QVariant value)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("vim_set_option", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_VIM_SET_OPTION);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(name);
	m_connector->m_dev->send(value);
	return r;
}

MsgpackRequest* NeovimApi6::vim_out_write(QByteArray str)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("vim_out_write", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_VIM_OUT_WRITE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(str);
	return r;
}

MsgpackRequest* NeovimApi6::vim_err_write(QByteArray str)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("vim_err_write", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_VIM_ERR_WRITE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(str);
	return r;
}

MsgpackRequest* NeovimApi6::vim_report_error(QByteArray str)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("vim_report_error", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_VIM_REPORT_ERROR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(str);
	return r;
}

MsgpackRequest* NeovimApi6::vim_get_buffers()
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("vim_get_buffers", 0) };
	r->setFunction(NeovimApi6::NEOVIM_FN_VIM_GET_BUFFERS);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	return r;
}

MsgpackRequest* NeovimApi6::vim_get_current_buffer()
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("vim_get_current_buffer", 0) };
	r->setFunction(NeovimApi6::NEOVIM_FN_VIM_GET_CURRENT_BUFFER);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	return r;
}

MsgpackRequest* NeovimApi6::vim_set_current_buffer(int64_t buffer)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("vim_set_current_buffer", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_VIM_SET_CURRENT_BUFFER);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(buffer);
	return r;
}

MsgpackRequest* NeovimApi6::vim_get_windows()
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("vim_get_windows", 0) };
	r->setFunction(NeovimApi6::NEOVIM_FN_VIM_GET_WINDOWS);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	return r;
}

MsgpackRequest* NeovimApi6::vim_get_current_window()
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("vim_get_current_window", 0) };
	r->setFunction(NeovimApi6::NEOVIM_FN_VIM_GET_CURRENT_WINDOW);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	return r;
}

MsgpackRequest* NeovimApi6::vim_set_current_window(int64_t window)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("vim_set_current_window", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_VIM_SET_CURRENT_WINDOW);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(window);
	return r;
}

MsgpackRequest* NeovimApi6::vim_get_tabpages()
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("vim_get_tabpages", 0) };
	r->setFunction(NeovimApi6::NEOVIM_FN_VIM_GET_TABPAGES);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	return r;
}

MsgpackRequest* NeovimApi6::vim_get_current_tabpage()
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("vim_get_current_tabpage", 0) };
	r->setFunction(NeovimApi6::NEOVIM_FN_VIM_GET_CURRENT_TABPAGE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	return r;
}

MsgpackRequest* NeovimApi6::vim_set_current_tabpage(int64_t tabpage)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("vim_set_current_tabpage", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_VIM_SET_CURRENT_TABPAGE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(tabpage);
	return r;
}

MsgpackRequest* NeovimApi6::vim_subscribe(QByteArray event)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("vim_subscribe", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_VIM_SUBSCRIBE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(event);
	return r;
}

MsgpackRequest* NeovimApi6::vim_unsubscribe(QByteArray event)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("vim_unsubscribe", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_VIM_UNSUBSCRIBE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(event);
	return r;
}

MsgpackRequest* NeovimApi6::vim_name_to_color(QByteArray name)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("vim_name_to_color", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_VIM_NAME_TO_COLOR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(name);
	return r;
}

MsgpackRequest* NeovimApi6::vim_get_color_map()
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("vim_get_color_map", 0) };
	r->setFunction(NeovimApi6::NEOVIM_FN_VIM_GET_COLOR_MAP);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	return r;
}

MsgpackRequest* NeovimApi6::window_get_buffer(int64_t window)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("window_get_buffer", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_WINDOW_GET_BUFFER);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(window);
	return r;
}

MsgpackRequest* NeovimApi6::window_get_cursor(int64_t window)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("window_get_cursor", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_WINDOW_GET_CURSOR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(window);
	return r;
}

MsgpackRequest* NeovimApi6::window_set_cursor(int64_t window, QPoint pos)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("window_set_cursor", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_WINDOW_SET_CURSOR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(window);
	m_connector->m_dev->send(pos);
	return r;
}

MsgpackRequest* NeovimApi6::window_get_height(int64_t window)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("window_get_height", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_WINDOW_GET_HEIGHT);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(window);
	return r;
}

MsgpackRequest* NeovimApi6::window_set_height(int64_t window, int64_t height)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("window_set_height", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_WINDOW_SET_HEIGHT);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(window);
	m_connector->m_dev->send(height);
	return r;
}

MsgpackRequest* NeovimApi6::window_get_width(int64_t window)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("window_get_width", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_WINDOW_GET_WIDTH);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(window);
	return r;
}

MsgpackRequest* NeovimApi6::window_set_width(int64_t window, int64_t width)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("window_set_width", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_WINDOW_SET_WIDTH);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(window);
	m_connector->m_dev->send(width);
	return r;
}

MsgpackRequest* NeovimApi6::window_get_var(int64_t window, QByteArray name)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("window_get_var", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_WINDOW_GET_VAR);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(window);
	m_connector->m_dev->send(name);
	return r;
}

MsgpackRequest* NeovimApi6::window_get_option(int64_t window, QByteArray name)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("window_get_option", 2) };
	r->setFunction(NeovimApi6::NEOVIM_FN_WINDOW_GET_OPTION);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(window);
	m_connector->m_dev->send(name);
	return r;
}

MsgpackRequest* NeovimApi6::window_set_option(int64_t window, QByteArray name, QVariant value)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("window_set_option", 3) };
	r->setFunction(NeovimApi6::NEOVIM_FN_WINDOW_SET_OPTION);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(window);
	m_connector->m_dev->send(name);
	m_connector->m_dev->send(value);
	return r;
}

MsgpackRequest* NeovimApi6::window_get_position(int64_t window)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("window_get_position", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_WINDOW_GET_POSITION);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(window);
	return r;
}

MsgpackRequest* NeovimApi6::window_get_tabpage(int64_t window)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("window_get_tabpage", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_WINDOW_GET_TABPAGE);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(window);
	return r;
}

MsgpackRequest* NeovimApi6::window_is_valid(int64_t window)
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("window_is_valid", 1) };
	r->setFunction(NeovimApi6::NEOVIM_FN_WINDOW_IS_VALID);
	connect(r, &MsgpackRequest::finished, this, &NeovimApi6::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi6::handleResponseError);
	m_connector->m_dev->send(window);
	return r;
}


// Handlers

void NeovimApi6::handleResponseError(uint32_t msgid, uint64_t fun, const QVariant& res)
{

	// TODO: support Neovim error types Exception/Validation/etc
	QString errMsg;
	const QVariantList asList = res.toList();
	if (asList.size() >= 2) {
		if (asList.at(1).canConvert<QByteArray>()) {
			errMsg = m_connector->m_dev->decode(asList.at(1).toByteArray());
		} else {
			errMsg = tr("Received unsupported Neovim error type");
		}
	}

	switch(fun) {
	case NeovimApi6::NEOVIM_FN_NVIM_BUF_LINE_COUNT:
		emit err_nvim_buf_line_count(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_BUFFER_GET_LINE:
		emit err_buffer_get_line(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_BUF_ATTACH:
		emit err_nvim_buf_attach(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_BUF_DETACH:
		emit err_nvim_buf_detach(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_BUFFER_SET_LINE:
		emit err_buffer_set_line(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_BUFFER_DEL_LINE:
		emit err_buffer_del_line(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_BUFFER_GET_LINE_SLICE:
		emit err_buffer_get_line_slice(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_BUF_GET_LINES:
		emit err_nvim_buf_get_lines(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_BUFFER_SET_LINE_SLICE:
		emit err_buffer_set_line_slice(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_BUF_SET_LINES:
		emit err_nvim_buf_set_lines(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_BUF_GET_OFFSET:
		emit err_nvim_buf_get_offset(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_BUF_GET_VAR:
		emit err_nvim_buf_get_var(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_BUF_GET_CHANGEDTICK:
		emit err_nvim_buf_get_changedtick(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_BUF_GET_KEYMAP:
		emit err_nvim_buf_get_keymap(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_BUF_SET_KEYMAP:
		emit err_nvim_buf_set_keymap(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_BUF_DEL_KEYMAP:
		emit err_nvim_buf_del_keymap(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_BUF_GET_COMMANDS:
		emit err_nvim_buf_get_commands(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_BUF_SET_VAR:
		emit err_nvim_buf_set_var(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_BUF_DEL_VAR:
		emit err_nvim_buf_del_var(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_BUFFER_SET_VAR:
		emit err_buffer_set_var(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_BUFFER_DEL_VAR:
		emit err_buffer_del_var(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_BUF_GET_OPTION:
		emit err_nvim_buf_get_option(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_BUF_SET_OPTION:
		emit err_nvim_buf_set_option(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_BUF_GET_NUMBER:
		emit err_nvim_buf_get_number(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_BUF_GET_NAME:
		emit err_nvim_buf_get_name(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_BUF_SET_NAME:
		emit err_nvim_buf_set_name(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_BUF_IS_LOADED:
		emit err_nvim_buf_is_loaded(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_BUF_IS_VALID:
		emit err_nvim_buf_is_valid(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_BUFFER_INSERT:
		emit err_buffer_insert(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_BUF_GET_MARK:
		emit err_nvim_buf_get_mark(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_BUF_ADD_HIGHLIGHT:
		emit err_nvim_buf_add_highlight(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_BUF_CLEAR_NAMESPACE:
		emit err_nvim_buf_clear_namespace(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_BUF_CLEAR_HIGHLIGHT:
		emit err_nvim_buf_clear_highlight(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_BUF_SET_VIRTUAL_TEXT:
		emit err_nvim_buf_set_virtual_text(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_TABPAGE_LIST_WINS:
		emit err_nvim_tabpage_list_wins(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_TABPAGE_GET_VAR:
		emit err_nvim_tabpage_get_var(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_TABPAGE_SET_VAR:
		emit err_nvim_tabpage_set_var(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_TABPAGE_DEL_VAR:
		emit err_nvim_tabpage_del_var(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_TABPAGE_SET_VAR:
		emit err_tabpage_set_var(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_TABPAGE_DEL_VAR:
		emit err_tabpage_del_var(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_TABPAGE_GET_WIN:
		emit err_nvim_tabpage_get_win(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_TABPAGE_GET_NUMBER:
		emit err_nvim_tabpage_get_number(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_TABPAGE_IS_VALID:
		emit err_nvim_tabpage_is_valid(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_UI_ATTACH:
		emit err_nvim_ui_attach(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_UI_ATTACH:
		emit err_ui_attach(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_UI_DETACH:
		emit err_nvim_ui_detach(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_UI_TRY_RESIZE:
		emit err_nvim_ui_try_resize(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_UI_SET_OPTION:
		emit err_nvim_ui_set_option(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_UI_TRY_RESIZE_GRID:
		emit err_nvim_ui_try_resize_grid(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_UI_PUM_SET_HEIGHT:
		emit err_nvim_ui_pum_set_height(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_COMMAND:
		emit err_nvim_command(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_GET_HL_BY_NAME:
		emit err_nvim_get_hl_by_name(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_GET_HL_BY_ID:
		emit err_nvim_get_hl_by_id(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_FEEDKEYS:
		emit err_nvim_feedkeys(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_INPUT:
		emit err_nvim_input(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_INPUT_MOUSE:
		emit err_nvim_input_mouse(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_REPLACE_TERMCODES:
		emit err_nvim_replace_termcodes(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_COMMAND_OUTPUT:
		emit err_nvim_command_output(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_EVAL:
		emit err_nvim_eval(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_EXECUTE_LUA:
		emit err_nvim_execute_lua(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_CALL_FUNCTION:
		emit err_nvim_call_function(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_CALL_DICT_FUNCTION:
		emit err_nvim_call_dict_function(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_STRWIDTH:
		emit err_nvim_strwidth(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_LIST_RUNTIME_PATHS:
		emit err_nvim_list_runtime_paths(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_SET_CURRENT_DIR:
		emit err_nvim_set_current_dir(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_GET_CURRENT_LINE:
		emit err_nvim_get_current_line(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_SET_CURRENT_LINE:
		emit err_nvim_set_current_line(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_DEL_CURRENT_LINE:
		emit err_nvim_del_current_line(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_GET_VAR:
		emit err_nvim_get_var(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_SET_VAR:
		emit err_nvim_set_var(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_DEL_VAR:
		emit err_nvim_del_var(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_VIM_SET_VAR:
		emit err_vim_set_var(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_VIM_DEL_VAR:
		emit err_vim_del_var(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_GET_VVAR:
		emit err_nvim_get_vvar(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_SET_VVAR:
		emit err_nvim_set_vvar(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_GET_OPTION:
		emit err_nvim_get_option(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_SET_OPTION:
		emit err_nvim_set_option(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_OUT_WRITE:
		emit err_nvim_out_write(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_ERR_WRITE:
		emit err_nvim_err_write(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_ERR_WRITELN:
		emit err_nvim_err_writeln(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_LIST_BUFS:
		emit err_nvim_list_bufs(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_GET_CURRENT_BUF:
		emit err_nvim_get_current_buf(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_SET_CURRENT_BUF:
		emit err_nvim_set_current_buf(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_LIST_WINS:
		emit err_nvim_list_wins(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_GET_CURRENT_WIN:
		emit err_nvim_get_current_win(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_SET_CURRENT_WIN:
		emit err_nvim_set_current_win(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_CREATE_BUF:
		emit err_nvim_create_buf(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_OPEN_WIN:
		emit err_nvim_open_win(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_LIST_TABPAGES:
		emit err_nvim_list_tabpages(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_GET_CURRENT_TABPAGE:
		emit err_nvim_get_current_tabpage(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_SET_CURRENT_TABPAGE:
		emit err_nvim_set_current_tabpage(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_CREATE_NAMESPACE:
		emit err_nvim_create_namespace(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_GET_NAMESPACES:
		emit err_nvim_get_namespaces(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_PASTE:
		emit err_nvim_paste(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_PUT:
		emit err_nvim_put(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_SUBSCRIBE:
		emit err_nvim_subscribe(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_UNSUBSCRIBE:
		emit err_nvim_unsubscribe(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_GET_COLOR_BY_NAME:
		emit err_nvim_get_color_by_name(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_GET_COLOR_MAP:
		emit err_nvim_get_color_map(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_GET_CONTEXT:
		emit err_nvim_get_context(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_LOAD_CONTEXT:
		emit err_nvim_load_context(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_GET_MODE:
		emit err_nvim_get_mode(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_GET_KEYMAP:
		emit err_nvim_get_keymap(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_SET_KEYMAP:
		emit err_nvim_set_keymap(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_DEL_KEYMAP:
		emit err_nvim_del_keymap(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_GET_COMMANDS:
		emit err_nvim_get_commands(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_GET_API_INFO:
		emit err_nvim_get_api_info(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_SET_CLIENT_INFO:
		emit err_nvim_set_client_info(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_GET_CHAN_INFO:
		emit err_nvim_get_chan_info(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_LIST_CHANS:
		emit err_nvim_list_chans(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_CALL_ATOMIC:
		emit err_nvim_call_atomic(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_PARSE_EXPRESSION:
		emit err_nvim_parse_expression(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_LIST_UIS:
		emit err_nvim_list_uis(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_GET_PROC_CHILDREN:
		emit err_nvim_get_proc_children(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_GET_PROC:
		emit err_nvim_get_proc(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_SELECT_POPUPMENU_ITEM:
		emit err_nvim_select_popupmenu_item(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_WIN_GET_BUF:
		emit err_nvim_win_get_buf(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_WIN_SET_BUF:
		emit err_nvim_win_set_buf(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_WIN_GET_CURSOR:
		emit err_nvim_win_get_cursor(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_WIN_SET_CURSOR:
		emit err_nvim_win_set_cursor(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_WIN_GET_HEIGHT:
		emit err_nvim_win_get_height(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_WIN_SET_HEIGHT:
		emit err_nvim_win_set_height(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_WIN_GET_WIDTH:
		emit err_nvim_win_get_width(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_WIN_SET_WIDTH:
		emit err_nvim_win_set_width(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_WIN_GET_VAR:
		emit err_nvim_win_get_var(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_WIN_SET_VAR:
		emit err_nvim_win_set_var(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_WIN_DEL_VAR:
		emit err_nvim_win_del_var(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_WINDOW_SET_VAR:
		emit err_window_set_var(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_WINDOW_DEL_VAR:
		emit err_window_del_var(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_WIN_GET_OPTION:
		emit err_nvim_win_get_option(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_WIN_SET_OPTION:
		emit err_nvim_win_set_option(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_WIN_GET_POSITION:
		emit err_nvim_win_get_position(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_WIN_GET_TABPAGE:
		emit err_nvim_win_get_tabpage(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_WIN_GET_NUMBER:
		emit err_nvim_win_get_number(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_WIN_IS_VALID:
		emit err_nvim_win_is_valid(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_WIN_SET_CONFIG:
		emit err_nvim_win_set_config(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_WIN_GET_CONFIG:
		emit err_nvim_win_get_config(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_NVIM_WIN_CLOSE:
		emit err_nvim_win_close(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_BUFFER_LINE_COUNT:
		emit err_buffer_line_count(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_BUFFER_GET_LINES:
		emit err_buffer_get_lines(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_BUFFER_SET_LINES:
		emit err_buffer_set_lines(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_BUFFER_GET_VAR:
		emit err_buffer_get_var(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_BUFFER_GET_OPTION:
		emit err_buffer_get_option(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_BUFFER_SET_OPTION:
		emit err_buffer_set_option(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_BUFFER_GET_NUMBER:
		emit err_buffer_get_number(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_BUFFER_GET_NAME:
		emit err_buffer_get_name(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_BUFFER_SET_NAME:
		emit err_buffer_set_name(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_BUFFER_IS_VALID:
		emit err_buffer_is_valid(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_BUFFER_GET_MARK:
		emit err_buffer_get_mark(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_BUFFER_ADD_HIGHLIGHT:
		emit err_buffer_add_highlight(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_BUFFER_CLEAR_HIGHLIGHT:
		emit err_buffer_clear_highlight(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_TABPAGE_GET_WINDOWS:
		emit err_tabpage_get_windows(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_TABPAGE_GET_VAR:
		emit err_tabpage_get_var(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_TABPAGE_GET_WINDOW:
		emit err_tabpage_get_window(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_TABPAGE_IS_VALID:
		emit err_tabpage_is_valid(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_UI_DETACH:
		emit err_ui_detach(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_UI_TRY_RESIZE:
		emit err_ui_try_resize(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_VIM_COMMAND:
		emit err_vim_command(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_VIM_FEEDKEYS:
		emit err_vim_feedkeys(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_VIM_INPUT:
		emit err_vim_input(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_VIM_REPLACE_TERMCODES:
		emit err_vim_replace_termcodes(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_VIM_COMMAND_OUTPUT:
		emit err_vim_command_output(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_VIM_EVAL:
		emit err_vim_eval(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_VIM_CALL_FUNCTION:
		emit err_vim_call_function(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_VIM_STRWIDTH:
		emit err_vim_strwidth(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_VIM_LIST_RUNTIME_PATHS:
		emit err_vim_list_runtime_paths(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_VIM_CHANGE_DIRECTORY:
		emit err_vim_change_directory(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_VIM_GET_CURRENT_LINE:
		emit err_vim_get_current_line(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_VIM_SET_CURRENT_LINE:
		emit err_vim_set_current_line(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_VIM_DEL_CURRENT_LINE:
		emit err_vim_del_current_line(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_VIM_GET_VAR:
		emit err_vim_get_var(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_VIM_GET_VVAR:
		emit err_vim_get_vvar(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_VIM_GET_OPTION:
		emit err_vim_get_option(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_VIM_SET_OPTION:
		emit err_vim_set_option(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_VIM_OUT_WRITE:
		emit err_vim_out_write(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_VIM_ERR_WRITE:
		emit err_vim_err_write(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_VIM_REPORT_ERROR:
		emit err_vim_report_error(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_VIM_GET_BUFFERS:
		emit err_vim_get_buffers(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_VIM_GET_CURRENT_BUFFER:
		emit err_vim_get_current_buffer(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_VIM_SET_CURRENT_BUFFER:
		emit err_vim_set_current_buffer(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_VIM_GET_WINDOWS:
		emit err_vim_get_windows(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_VIM_GET_CURRENT_WINDOW:
		emit err_vim_get_current_window(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_VIM_SET_CURRENT_WINDOW:
		emit err_vim_set_current_window(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_VIM_GET_TABPAGES:
		emit err_vim_get_tabpages(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_VIM_GET_CURRENT_TABPAGE:
		emit err_vim_get_current_tabpage(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_VIM_SET_CURRENT_TABPAGE:
		emit err_vim_set_current_tabpage(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_VIM_SUBSCRIBE:
		emit err_vim_subscribe(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_VIM_UNSUBSCRIBE:
		emit err_vim_unsubscribe(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_VIM_NAME_TO_COLOR:
		emit err_vim_name_to_color(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_VIM_GET_COLOR_MAP:
		emit err_vim_get_color_map(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_WINDOW_GET_BUFFER:
		emit err_window_get_buffer(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_WINDOW_GET_CURSOR:
		emit err_window_get_cursor(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_WINDOW_SET_CURSOR:
		emit err_window_set_cursor(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_WINDOW_GET_HEIGHT:
		emit err_window_get_height(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_WINDOW_SET_HEIGHT:
		emit err_window_set_height(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_WINDOW_GET_WIDTH:
		emit err_window_get_width(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_WINDOW_SET_WIDTH:
		emit err_window_set_width(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_WINDOW_GET_VAR:
		emit err_window_get_var(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_WINDOW_GET_OPTION:
		emit err_window_get_option(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_WINDOW_SET_OPTION:
		emit err_window_set_option(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_WINDOW_GET_POSITION:
		emit err_window_get_position(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_WINDOW_GET_TABPAGE:
		emit err_window_get_tabpage(errMsg, res);
		break;
	case NeovimApi6::NEOVIM_FN_WINDOW_IS_VALID:
		emit err_window_is_valid(errMsg, res);
		break;
	default:
		m_connector->setError(NeovimConnector::RuntimeMsgpackError, QString("Received error for function that should not fail: %s").arg(fun));
	}
}

void NeovimApi6::handleResponse(uint32_t msgid, uint64_t fun, const QVariant& res)
{
	switch(fun) {
		case NeovimApi6::NEOVIM_FN_NVIM_BUF_LINE_COUNT:
		{
			int64_t data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_buf_line_count");
				return;
			}

			emit on_nvim_buf_line_count(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_BUFFER_GET_LINE:
		{
			QByteArray data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for buffer_get_line");
				return;
			}

			emit on_buffer_get_line(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_BUF_ATTACH:
		{
			bool data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_buf_attach");
				return;
			}

			emit on_nvim_buf_attach(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_BUF_DETACH:
		{
			bool data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_buf_detach");
				return;
			}

			emit on_nvim_buf_detach(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_BUFFER_SET_LINE:
		{
			emit on_buffer_set_line();
		}
		break;

		case NeovimApi6::NEOVIM_FN_BUFFER_DEL_LINE:
		{
			emit on_buffer_del_line();
		}
		break;

		case NeovimApi6::NEOVIM_FN_BUFFER_GET_LINE_SLICE:
		{
			QList<QByteArray> data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for buffer_get_line_slice");
				return;
			}

			emit on_buffer_get_line_slice(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_BUF_GET_LINES:
		{
			QList<QByteArray> data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_buf_get_lines");
				return;
			}

			emit on_nvim_buf_get_lines(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_BUFFER_SET_LINE_SLICE:
		{
			emit on_buffer_set_line_slice();
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_BUF_SET_LINES:
		{
			emit on_nvim_buf_set_lines();
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_BUF_GET_OFFSET:
		{
			int64_t data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_buf_get_offset");
				return;
			}

			emit on_nvim_buf_get_offset(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_BUF_GET_VAR:
		{
			QVariant data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_buf_get_var");
				return;
			}

			emit on_nvim_buf_get_var(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_BUF_GET_CHANGEDTICK:
		{
			int64_t data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_buf_get_changedtick");
				return;
			}

			emit on_nvim_buf_get_changedtick(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_BUF_GET_KEYMAP:
		{
			QList<QVariantMap> data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_buf_get_keymap");
				return;
			}

			emit on_nvim_buf_get_keymap(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_BUF_SET_KEYMAP:
		{
			emit on_nvim_buf_set_keymap();
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_BUF_DEL_KEYMAP:
		{
			emit on_nvim_buf_del_keymap();
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_BUF_GET_COMMANDS:
		{
			QVariantMap data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_buf_get_commands");
				return;
			}

			emit on_nvim_buf_get_commands(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_BUF_SET_VAR:
		{
			emit on_nvim_buf_set_var();
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_BUF_DEL_VAR:
		{
			emit on_nvim_buf_del_var();
		}
		break;

		case NeovimApi6::NEOVIM_FN_BUFFER_SET_VAR:
		{
			QVariant data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for buffer_set_var");
				return;
			}

			emit on_buffer_set_var(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_BUFFER_DEL_VAR:
		{
			QVariant data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for buffer_del_var");
				return;
			}

			emit on_buffer_del_var(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_BUF_GET_OPTION:
		{
			QVariant data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_buf_get_option");
				return;
			}

			emit on_nvim_buf_get_option(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_BUF_SET_OPTION:
		{
			emit on_nvim_buf_set_option();
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_BUF_GET_NUMBER:
		{
			int64_t data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_buf_get_number");
				return;
			}

			emit on_nvim_buf_get_number(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_BUF_GET_NAME:
		{
			QByteArray data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_buf_get_name");
				return;
			}

			emit on_nvim_buf_get_name(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_BUF_SET_NAME:
		{
			emit on_nvim_buf_set_name();
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_BUF_IS_LOADED:
		{
			bool data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_buf_is_loaded");
				return;
			}

			emit on_nvim_buf_is_loaded(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_BUF_IS_VALID:
		{
			bool data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_buf_is_valid");
				return;
			}

			emit on_nvim_buf_is_valid(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_BUFFER_INSERT:
		{
			emit on_buffer_insert();
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_BUF_GET_MARK:
		{
			QPoint data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_buf_get_mark");
				return;
			}

			emit on_nvim_buf_get_mark(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_BUF_ADD_HIGHLIGHT:
		{
			int64_t data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_buf_add_highlight");
				return;
			}

			emit on_nvim_buf_add_highlight(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_BUF_CLEAR_NAMESPACE:
		{
			emit on_nvim_buf_clear_namespace();
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_BUF_CLEAR_HIGHLIGHT:
		{
			emit on_nvim_buf_clear_highlight();
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_BUF_SET_VIRTUAL_TEXT:
		{
			int64_t data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_buf_set_virtual_text");
				return;
			}

			emit on_nvim_buf_set_virtual_text(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_TABPAGE_LIST_WINS:
		{
			QList<int64_t> data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_tabpage_list_wins");
				return;
			}

			emit on_nvim_tabpage_list_wins(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_TABPAGE_GET_VAR:
		{
			QVariant data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_tabpage_get_var");
				return;
			}

			emit on_nvim_tabpage_get_var(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_TABPAGE_SET_VAR:
		{
			emit on_nvim_tabpage_set_var();
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_TABPAGE_DEL_VAR:
		{
			emit on_nvim_tabpage_del_var();
		}
		break;

		case NeovimApi6::NEOVIM_FN_TABPAGE_SET_VAR:
		{
			QVariant data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for tabpage_set_var");
				return;
			}

			emit on_tabpage_set_var(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_TABPAGE_DEL_VAR:
		{
			QVariant data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for tabpage_del_var");
				return;
			}

			emit on_tabpage_del_var(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_TABPAGE_GET_WIN:
		{
			int64_t data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_tabpage_get_win");
				return;
			}

			emit on_nvim_tabpage_get_win(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_TABPAGE_GET_NUMBER:
		{
			int64_t data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_tabpage_get_number");
				return;
			}

			emit on_nvim_tabpage_get_number(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_TABPAGE_IS_VALID:
		{
			bool data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_tabpage_is_valid");
				return;
			}

			emit on_nvim_tabpage_is_valid(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_UI_ATTACH:
		{
			emit on_nvim_ui_attach();
		}
		break;

		case NeovimApi6::NEOVIM_FN_UI_ATTACH:
		{
			emit on_ui_attach();
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_UI_DETACH:
		{
			emit on_nvim_ui_detach();
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_UI_TRY_RESIZE:
		{
			emit on_nvim_ui_try_resize();
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_UI_SET_OPTION:
		{
			emit on_nvim_ui_set_option();
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_UI_TRY_RESIZE_GRID:
		{
			emit on_nvim_ui_try_resize_grid();
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_UI_PUM_SET_HEIGHT:
		{
			emit on_nvim_ui_pum_set_height();
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_COMMAND:
		{
			emit on_nvim_command();
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_GET_HL_BY_NAME:
		{
			QVariantMap data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_get_hl_by_name");
				return;
			}

			emit on_nvim_get_hl_by_name(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_GET_HL_BY_ID:
		{
			QVariantMap data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_get_hl_by_id");
				return;
			}

			emit on_nvim_get_hl_by_id(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_FEEDKEYS:
		{
			emit on_nvim_feedkeys();
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_INPUT:
		{
			int64_t data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_input");
				return;
			}

			emit on_nvim_input(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_INPUT_MOUSE:
		{
			emit on_nvim_input_mouse();
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_REPLACE_TERMCODES:
		{
			QByteArray data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_replace_termcodes");
				return;
			}

			emit on_nvim_replace_termcodes(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_COMMAND_OUTPUT:
		{
			QByteArray data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_command_output");
				return;
			}

			emit on_nvim_command_output(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_EVAL:
		{
			QVariant data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_eval");
				return;
			}

			emit on_nvim_eval(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_EXECUTE_LUA:
		{
			QVariant data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_execute_lua");
				return;
			}

			emit on_nvim_execute_lua(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_CALL_FUNCTION:
		{
			QVariant data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_call_function");
				return;
			}

			emit on_nvim_call_function(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_CALL_DICT_FUNCTION:
		{
			QVariant data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_call_dict_function");
				return;
			}

			emit on_nvim_call_dict_function(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_STRWIDTH:
		{
			int64_t data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_strwidth");
				return;
			}

			emit on_nvim_strwidth(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_LIST_RUNTIME_PATHS:
		{
			QList<QByteArray> data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_list_runtime_paths");
				return;
			}

			emit on_nvim_list_runtime_paths(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_SET_CURRENT_DIR:
		{
			emit on_nvim_set_current_dir();
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_GET_CURRENT_LINE:
		{
			QByteArray data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_get_current_line");
				return;
			}

			emit on_nvim_get_current_line(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_SET_CURRENT_LINE:
		{
			emit on_nvim_set_current_line();
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_DEL_CURRENT_LINE:
		{
			emit on_nvim_del_current_line();
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_GET_VAR:
		{
			QVariant data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_get_var");
				return;
			}

			emit on_nvim_get_var(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_SET_VAR:
		{
			emit on_nvim_set_var();
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_DEL_VAR:
		{
			emit on_nvim_del_var();
		}
		break;

		case NeovimApi6::NEOVIM_FN_VIM_SET_VAR:
		{
			QVariant data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_set_var");
				return;
			}

			emit on_vim_set_var(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_VIM_DEL_VAR:
		{
			QVariant data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_del_var");
				return;
			}

			emit on_vim_del_var(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_GET_VVAR:
		{
			QVariant data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_get_vvar");
				return;
			}

			emit on_nvim_get_vvar(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_SET_VVAR:
		{
			emit on_nvim_set_vvar();
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_GET_OPTION:
		{
			QVariant data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_get_option");
				return;
			}

			emit on_nvim_get_option(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_SET_OPTION:
		{
			emit on_nvim_set_option();
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_OUT_WRITE:
		{
			emit on_nvim_out_write();
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_ERR_WRITE:
		{
			emit on_nvim_err_write();
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_ERR_WRITELN:
		{
			emit on_nvim_err_writeln();
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_LIST_BUFS:
		{
			QList<int64_t> data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_list_bufs");
				return;
			}

			emit on_nvim_list_bufs(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_GET_CURRENT_BUF:
		{
			int64_t data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_get_current_buf");
				return;
			}

			emit on_nvim_get_current_buf(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_SET_CURRENT_BUF:
		{
			emit on_nvim_set_current_buf();
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_LIST_WINS:
		{
			QList<int64_t> data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_list_wins");
				return;
			}

			emit on_nvim_list_wins(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_GET_CURRENT_WIN:
		{
			int64_t data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_get_current_win");
				return;
			}

			emit on_nvim_get_current_win(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_SET_CURRENT_WIN:
		{
			emit on_nvim_set_current_win();
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_CREATE_BUF:
		{
			int64_t data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_create_buf");
				return;
			}

			emit on_nvim_create_buf(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_OPEN_WIN:
		{
			int64_t data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_open_win");
				return;
			}

			emit on_nvim_open_win(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_LIST_TABPAGES:
		{
			QList<int64_t> data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_list_tabpages");
				return;
			}

			emit on_nvim_list_tabpages(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_GET_CURRENT_TABPAGE:
		{
			int64_t data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_get_current_tabpage");
				return;
			}

			emit on_nvim_get_current_tabpage(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_SET_CURRENT_TABPAGE:
		{
			emit on_nvim_set_current_tabpage();
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_CREATE_NAMESPACE:
		{
			int64_t data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_create_namespace");
				return;
			}

			emit on_nvim_create_namespace(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_GET_NAMESPACES:
		{
			QVariantMap data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_get_namespaces");
				return;
			}

			emit on_nvim_get_namespaces(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_PASTE:
		{
			bool data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_paste");
				return;
			}

			emit on_nvim_paste(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_PUT:
		{
			emit on_nvim_put();
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_SUBSCRIBE:
		{
			emit on_nvim_subscribe();
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_UNSUBSCRIBE:
		{
			emit on_nvim_unsubscribe();
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_GET_COLOR_BY_NAME:
		{
			int64_t data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_get_color_by_name");
				return;
			}

			emit on_nvim_get_color_by_name(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_GET_COLOR_MAP:
		{
			QVariantMap data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_get_color_map");
				return;
			}

			emit on_nvim_get_color_map(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_GET_CONTEXT:
		{
			QVariantMap data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_get_context");
				return;
			}

			emit on_nvim_get_context(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_LOAD_CONTEXT:
		{
			QVariant data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_load_context");
				return;
			}

			emit on_nvim_load_context(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_GET_MODE:
		{
			QVariantMap data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_get_mode");
				return;
			}

			emit on_nvim_get_mode(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_GET_KEYMAP:
		{
			QList<QVariantMap> data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_get_keymap");
				return;
			}

			emit on_nvim_get_keymap(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_SET_KEYMAP:
		{
			emit on_nvim_set_keymap();
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_DEL_KEYMAP:
		{
			emit on_nvim_del_keymap();
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_GET_COMMANDS:
		{
			QVariantMap data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_get_commands");
				return;
			}

			emit on_nvim_get_commands(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_GET_API_INFO:
		{
			QVariantList data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_get_api_info");
				return;
			}

			emit on_nvim_get_api_info(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_SET_CLIENT_INFO:
		{
			emit on_nvim_set_client_info();
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_GET_CHAN_INFO:
		{
			QVariantMap data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_get_chan_info");
				return;
			}

			emit on_nvim_get_chan_info(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_LIST_CHANS:
		{
			QVariantList data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_list_chans");
				return;
			}

			emit on_nvim_list_chans(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_CALL_ATOMIC:
		{
			QVariantList data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_call_atomic");
				return;
			}

			emit on_nvim_call_atomic(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_PARSE_EXPRESSION:
		{
			QVariantMap data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_parse_expression");
				return;
			}

			emit on_nvim_parse_expression(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_LIST_UIS:
		{
			QVariantList data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_list_uis");
				return;
			}

			emit on_nvim_list_uis(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_GET_PROC_CHILDREN:
		{
			QVariantList data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_get_proc_children");
				return;
			}

			emit on_nvim_get_proc_children(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_GET_PROC:
		{
			QVariant data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_get_proc");
				return;
			}

			emit on_nvim_get_proc(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_SELECT_POPUPMENU_ITEM:
		{
			emit on_nvim_select_popupmenu_item();
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_WIN_GET_BUF:
		{
			int64_t data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_win_get_buf");
				return;
			}

			emit on_nvim_win_get_buf(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_WIN_SET_BUF:
		{
			emit on_nvim_win_set_buf();
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_WIN_GET_CURSOR:
		{
			QPoint data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_win_get_cursor");
				return;
			}

			emit on_nvim_win_get_cursor(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_WIN_SET_CURSOR:
		{
			emit on_nvim_win_set_cursor();
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_WIN_GET_HEIGHT:
		{
			int64_t data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_win_get_height");
				return;
			}

			emit on_nvim_win_get_height(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_WIN_SET_HEIGHT:
		{
			emit on_nvim_win_set_height();
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_WIN_GET_WIDTH:
		{
			int64_t data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_win_get_width");
				return;
			}

			emit on_nvim_win_get_width(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_WIN_SET_WIDTH:
		{
			emit on_nvim_win_set_width();
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_WIN_GET_VAR:
		{
			QVariant data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_win_get_var");
				return;
			}

			emit on_nvim_win_get_var(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_WIN_SET_VAR:
		{
			emit on_nvim_win_set_var();
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_WIN_DEL_VAR:
		{
			emit on_nvim_win_del_var();
		}
		break;

		case NeovimApi6::NEOVIM_FN_WINDOW_SET_VAR:
		{
			QVariant data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for window_set_var");
				return;
			}

			emit on_window_set_var(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_WINDOW_DEL_VAR:
		{
			QVariant data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for window_del_var");
				return;
			}

			emit on_window_del_var(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_WIN_GET_OPTION:
		{
			QVariant data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_win_get_option");
				return;
			}

			emit on_nvim_win_get_option(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_WIN_SET_OPTION:
		{
			emit on_nvim_win_set_option();
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_WIN_GET_POSITION:
		{
			QPoint data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_win_get_position");
				return;
			}

			emit on_nvim_win_get_position(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_WIN_GET_TABPAGE:
		{
			int64_t data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_win_get_tabpage");
				return;
			}

			emit on_nvim_win_get_tabpage(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_WIN_GET_NUMBER:
		{
			int64_t data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_win_get_number");
				return;
			}

			emit on_nvim_win_get_number(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_WIN_IS_VALID:
		{
			bool data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_win_is_valid");
				return;
			}

			emit on_nvim_win_is_valid(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_WIN_SET_CONFIG:
		{
			emit on_nvim_win_set_config();
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_WIN_GET_CONFIG:
		{
			QVariantMap data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for nvim_win_get_config");
				return;
			}

			emit on_nvim_win_get_config(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_NVIM_WIN_CLOSE:
		{
			emit on_nvim_win_close();
		}
		break;

		case NeovimApi6::NEOVIM_FN_BUFFER_LINE_COUNT:
		{
			int64_t data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for buffer_line_count");
				return;
			}

			emit on_buffer_line_count(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_BUFFER_GET_LINES:
		{
			QList<QByteArray> data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for buffer_get_lines");
				return;
			}

			emit on_buffer_get_lines(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_BUFFER_SET_LINES:
		{
			emit on_buffer_set_lines();
		}
		break;

		case NeovimApi6::NEOVIM_FN_BUFFER_GET_VAR:
		{
			QVariant data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for buffer_get_var");
				return;
			}

			emit on_buffer_get_var(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_BUFFER_GET_OPTION:
		{
			QVariant data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for buffer_get_option");
				return;
			}

			emit on_buffer_get_option(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_BUFFER_SET_OPTION:
		{
			emit on_buffer_set_option();
		}
		break;

		case NeovimApi6::NEOVIM_FN_BUFFER_GET_NUMBER:
		{
			int64_t data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for buffer_get_number");
				return;
			}

			emit on_buffer_get_number(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_BUFFER_GET_NAME:
		{
			QByteArray data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for buffer_get_name");
				return;
			}

			emit on_buffer_get_name(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_BUFFER_SET_NAME:
		{
			emit on_buffer_set_name();
		}
		break;

		case NeovimApi6::NEOVIM_FN_BUFFER_IS_VALID:
		{
			bool data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for buffer_is_valid");
				return;
			}

			emit on_buffer_is_valid(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_BUFFER_GET_MARK:
		{
			QPoint data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for buffer_get_mark");
				return;
			}

			emit on_buffer_get_mark(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_BUFFER_ADD_HIGHLIGHT:
		{
			int64_t data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for buffer_add_highlight");
				return;
			}

			emit on_buffer_add_highlight(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_BUFFER_CLEAR_HIGHLIGHT:
		{
			emit on_buffer_clear_highlight();
		}
		break;

		case NeovimApi6::NEOVIM_FN_TABPAGE_GET_WINDOWS:
		{
			QList<int64_t> data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for tabpage_get_windows");
				return;
			}

			emit on_tabpage_get_windows(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_TABPAGE_GET_VAR:
		{
			QVariant data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for tabpage_get_var");
				return;
			}

			emit on_tabpage_get_var(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_TABPAGE_GET_WINDOW:
		{
			int64_t data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for tabpage_get_window");
				return;
			}

			emit on_tabpage_get_window(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_TABPAGE_IS_VALID:
		{
			bool data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for tabpage_is_valid");
				return;
			}

			emit on_tabpage_is_valid(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_UI_DETACH:
		{
			emit on_ui_detach();
		}
		break;

		case NeovimApi6::NEOVIM_FN_UI_TRY_RESIZE:
		{
			QVariant data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for ui_try_resize");
				return;
			}

			emit on_ui_try_resize(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_VIM_COMMAND:
		{
			emit on_vim_command();
		}
		break;

		case NeovimApi6::NEOVIM_FN_VIM_FEEDKEYS:
		{
			emit on_vim_feedkeys();
		}
		break;

		case NeovimApi6::NEOVIM_FN_VIM_INPUT:
		{
			int64_t data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_input");
				return;
			}

			emit on_vim_input(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_VIM_REPLACE_TERMCODES:
		{
			QByteArray data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_replace_termcodes");
				return;
			}

			emit on_vim_replace_termcodes(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_VIM_COMMAND_OUTPUT:
		{
			QByteArray data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_command_output");
				return;
			}

			emit on_vim_command_output(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_VIM_EVAL:
		{
			QVariant data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_eval");
				return;
			}

			emit on_vim_eval(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_VIM_CALL_FUNCTION:
		{
			QVariant data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_call_function");
				return;
			}

			emit on_vim_call_function(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_VIM_STRWIDTH:
		{
			int64_t data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_strwidth");
				return;
			}

			emit on_vim_strwidth(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_VIM_LIST_RUNTIME_PATHS:
		{
			QList<QByteArray> data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_list_runtime_paths");
				return;
			}

			emit on_vim_list_runtime_paths(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_VIM_CHANGE_DIRECTORY:
		{
			emit on_vim_change_directory();
		}
		break;

		case NeovimApi6::NEOVIM_FN_VIM_GET_CURRENT_LINE:
		{
			QByteArray data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_get_current_line");
				return;
			}

			emit on_vim_get_current_line(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_VIM_SET_CURRENT_LINE:
		{
			emit on_vim_set_current_line();
		}
		break;

		case NeovimApi6::NEOVIM_FN_VIM_DEL_CURRENT_LINE:
		{
			emit on_vim_del_current_line();
		}
		break;

		case NeovimApi6::NEOVIM_FN_VIM_GET_VAR:
		{
			QVariant data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_get_var");
				return;
			}

			emit on_vim_get_var(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_VIM_GET_VVAR:
		{
			QVariant data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_get_vvar");
				return;
			}

			emit on_vim_get_vvar(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_VIM_GET_OPTION:
		{
			QVariant data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_get_option");
				return;
			}

			emit on_vim_get_option(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_VIM_SET_OPTION:
		{
			emit on_vim_set_option();
		}
		break;

		case NeovimApi6::NEOVIM_FN_VIM_OUT_WRITE:
		{
			emit on_vim_out_write();
		}
		break;

		case NeovimApi6::NEOVIM_FN_VIM_ERR_WRITE:
		{
			emit on_vim_err_write();
		}
		break;

		case NeovimApi6::NEOVIM_FN_VIM_REPORT_ERROR:
		{
			emit on_vim_report_error();
		}
		break;

		case NeovimApi6::NEOVIM_FN_VIM_GET_BUFFERS:
		{
			QList<int64_t> data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_get_buffers");
				return;
			}

			emit on_vim_get_buffers(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_VIM_GET_CURRENT_BUFFER:
		{
			int64_t data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_get_current_buffer");
				return;
			}

			emit on_vim_get_current_buffer(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_VIM_SET_CURRENT_BUFFER:
		{
			emit on_vim_set_current_buffer();
		}
		break;

		case NeovimApi6::NEOVIM_FN_VIM_GET_WINDOWS:
		{
			QList<int64_t> data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_get_windows");
				return;
			}

			emit on_vim_get_windows(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_VIM_GET_CURRENT_WINDOW:
		{
			int64_t data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_get_current_window");
				return;
			}

			emit on_vim_get_current_window(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_VIM_SET_CURRENT_WINDOW:
		{
			emit on_vim_set_current_window();
		}
		break;

		case NeovimApi6::NEOVIM_FN_VIM_GET_TABPAGES:
		{
			QList<int64_t> data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_get_tabpages");
				return;
			}

			emit on_vim_get_tabpages(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_VIM_GET_CURRENT_TABPAGE:
		{
			int64_t data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_get_current_tabpage");
				return;
			}

			emit on_vim_get_current_tabpage(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_VIM_SET_CURRENT_TABPAGE:
		{
			emit on_vim_set_current_tabpage();
		}
		break;

		case NeovimApi6::NEOVIM_FN_VIM_SUBSCRIBE:
		{
			emit on_vim_subscribe();
		}
		break;

		case NeovimApi6::NEOVIM_FN_VIM_UNSUBSCRIBE:
		{
			emit on_vim_unsubscribe();
		}
		break;

		case NeovimApi6::NEOVIM_FN_VIM_NAME_TO_COLOR:
		{
			int64_t data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_name_to_color");
				return;
			}

			emit on_vim_name_to_color(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_VIM_GET_COLOR_MAP:
		{
			QVariantMap data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_get_color_map");
				return;
			}

			emit on_vim_get_color_map(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_WINDOW_GET_BUFFER:
		{
			int64_t data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for window_get_buffer");
				return;
			}

			emit on_window_get_buffer(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_WINDOW_GET_CURSOR:
		{
			QPoint data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for window_get_cursor");
				return;
			}

			emit on_window_get_cursor(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_WINDOW_SET_CURSOR:
		{
			emit on_window_set_cursor();
		}
		break;

		case NeovimApi6::NEOVIM_FN_WINDOW_GET_HEIGHT:
		{
			int64_t data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for window_get_height");
				return;
			}

			emit on_window_get_height(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_WINDOW_SET_HEIGHT:
		{
			emit on_window_set_height();
		}
		break;

		case NeovimApi6::NEOVIM_FN_WINDOW_GET_WIDTH:
		{
			int64_t data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for window_get_width");
				return;
			}

			emit on_window_get_width(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_WINDOW_SET_WIDTH:
		{
			emit on_window_set_width();
		}
		break;

		case NeovimApi6::NEOVIM_FN_WINDOW_GET_VAR:
		{
			QVariant data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for window_get_var");
				return;
			}

			emit on_window_get_var(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_WINDOW_GET_OPTION:
		{
			QVariant data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for window_get_option");
				return;
			}

			emit on_window_get_option(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_WINDOW_SET_OPTION:
		{
			emit on_window_set_option();
		}
		break;

		case NeovimApi6::NEOVIM_FN_WINDOW_GET_POSITION:
		{
			QPoint data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for window_get_position");
				return;
			}

			emit on_window_get_position(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_WINDOW_GET_TABPAGE:
		{
			int64_t data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for window_get_tabpage");
				return;
			}

			emit on_window_get_tabpage(data);
		}
		break;

		case NeovimApi6::NEOVIM_FN_WINDOW_IS_VALID:
		{
			bool data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for window_is_valid");
				return;
			}

			emit on_window_is_valid(data);
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
/*static*/ bool NeovimApi6::checkFunctions(const QVariantList& ftable) noexcept
{
	static const QList<Function> required{
			Function{
				QStringLiteral("Integer"),
				QStringLiteral("nvim_buf_line_count"),
				QStringList{
					QStringLiteral("Buffer"),
					}
				, false },
			Function{
				QStringLiteral("String"),
				QStringLiteral("buffer_get_line"),
				QStringList{
					QStringLiteral("Buffer"),
					QStringLiteral("Integer"),
					}
				, false },
			Function{
				QStringLiteral("Boolean"),
				QStringLiteral("nvim_buf_attach"),
				QStringList{
					QStringLiteral("Buffer"),
					QStringLiteral("Boolean"),
					QStringLiteral("Dictionary"),
					}
				, false },
			Function{
				QStringLiteral("Boolean"),
				QStringLiteral("nvim_buf_detach"),
				QStringList{
					QStringLiteral("Buffer"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("buffer_set_line"),
				QStringList{
					QStringLiteral("Buffer"),
					QStringLiteral("Integer"),
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("buffer_del_line"),
				QStringList{
					QStringLiteral("Buffer"),
					QStringLiteral("Integer"),
					}
				, false },
			Function{
				QStringLiteral("ArrayOf(String)"),
				QStringLiteral("buffer_get_line_slice"),
				QStringList{
					QStringLiteral("Buffer"),
					QStringLiteral("Integer"),
					QStringLiteral("Integer"),
					QStringLiteral("Boolean"),
					QStringLiteral("Boolean"),
					}
				, false },
			Function{
				QStringLiteral("ArrayOf(String)"),
				QStringLiteral("nvim_buf_get_lines"),
				QStringList{
					QStringLiteral("Buffer"),
					QStringLiteral("Integer"),
					QStringLiteral("Integer"),
					QStringLiteral("Boolean"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("buffer_set_line_slice"),
				QStringList{
					QStringLiteral("Buffer"),
					QStringLiteral("Integer"),
					QStringLiteral("Integer"),
					QStringLiteral("Boolean"),
					QStringLiteral("Boolean"),
					QStringLiteral("ArrayOf(String)"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_buf_set_lines"),
				QStringList{
					QStringLiteral("Buffer"),
					QStringLiteral("Integer"),
					QStringLiteral("Integer"),
					QStringLiteral("Boolean"),
					QStringLiteral("ArrayOf(String)"),
					}
				, false },
			Function{
				QStringLiteral("Integer"),
				QStringLiteral("nvim_buf_get_offset"),
				QStringList{
					QStringLiteral("Buffer"),
					QStringLiteral("Integer"),
					}
				, false },
			Function{
				QStringLiteral("Object"),
				QStringLiteral("nvim_buf_get_var"),
				QStringList{
					QStringLiteral("Buffer"),
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("Integer"),
				QStringLiteral("nvim_buf_get_changedtick"),
				QStringList{
					QStringLiteral("Buffer"),
					}
				, false },
			Function{
				QStringLiteral("ArrayOf(Dictionary)"),
				QStringLiteral("nvim_buf_get_keymap"),
				QStringList{
					QStringLiteral("Buffer"),
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_buf_set_keymap"),
				QStringList{
					QStringLiteral("Buffer"),
					QStringLiteral("String"),
					QStringLiteral("String"),
					QStringLiteral("String"),
					QStringLiteral("Dictionary"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_buf_del_keymap"),
				QStringList{
					QStringLiteral("Buffer"),
					QStringLiteral("String"),
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("Dictionary"),
				QStringLiteral("nvim_buf_get_commands"),
				QStringList{
					QStringLiteral("Buffer"),
					QStringLiteral("Dictionary"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_buf_set_var"),
				QStringList{
					QStringLiteral("Buffer"),
					QStringLiteral("String"),
					QStringLiteral("Object"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_buf_del_var"),
				QStringList{
					QStringLiteral("Buffer"),
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("Object"),
				QStringLiteral("buffer_set_var"),
				QStringList{
					QStringLiteral("Buffer"),
					QStringLiteral("String"),
					QStringLiteral("Object"),
					}
				, false },
			Function{
				QStringLiteral("Object"),
				QStringLiteral("buffer_del_var"),
				QStringList{
					QStringLiteral("Buffer"),
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("Object"),
				QStringLiteral("nvim_buf_get_option"),
				QStringList{
					QStringLiteral("Buffer"),
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_buf_set_option"),
				QStringList{
					QStringLiteral("Buffer"),
					QStringLiteral("String"),
					QStringLiteral("Object"),
					}
				, false },
			Function{
				QStringLiteral("Integer"),
				QStringLiteral("nvim_buf_get_number"),
				QStringList{
					QStringLiteral("Buffer"),
					}
				, false },
			Function{
				QStringLiteral("String"),
				QStringLiteral("nvim_buf_get_name"),
				QStringList{
					QStringLiteral("Buffer"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_buf_set_name"),
				QStringList{
					QStringLiteral("Buffer"),
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("Boolean"),
				QStringLiteral("nvim_buf_is_loaded"),
				QStringList{
					QStringLiteral("Buffer"),
					}
				, false },
			Function{
				QStringLiteral("Boolean"),
				QStringLiteral("nvim_buf_is_valid"),
				QStringList{
					QStringLiteral("Buffer"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("buffer_insert"),
				QStringList{
					QStringLiteral("Buffer"),
					QStringLiteral("Integer"),
					QStringLiteral("ArrayOf(String)"),
					}
				, false },
			Function{
				QStringLiteral("ArrayOf(Integer, 2)"),
				QStringLiteral("nvim_buf_get_mark"),
				QStringList{
					QStringLiteral("Buffer"),
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("Integer"),
				QStringLiteral("nvim_buf_add_highlight"),
				QStringList{
					QStringLiteral("Buffer"),
					QStringLiteral("Integer"),
					QStringLiteral("String"),
					QStringLiteral("Integer"),
					QStringLiteral("Integer"),
					QStringLiteral("Integer"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_buf_clear_namespace"),
				QStringList{
					QStringLiteral("Buffer"),
					QStringLiteral("Integer"),
					QStringLiteral("Integer"),
					QStringLiteral("Integer"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_buf_clear_highlight"),
				QStringList{
					QStringLiteral("Buffer"),
					QStringLiteral("Integer"),
					QStringLiteral("Integer"),
					QStringLiteral("Integer"),
					}
				, false },
			Function{
				QStringLiteral("Integer"),
				QStringLiteral("nvim_buf_set_virtual_text"),
				QStringList{
					QStringLiteral("Buffer"),
					QStringLiteral("Integer"),
					QStringLiteral("Integer"),
					QStringLiteral("Array"),
					QStringLiteral("Dictionary"),
					}
				, false },
			Function{
				QStringLiteral("ArrayOf(Window)"),
				QStringLiteral("nvim_tabpage_list_wins"),
				QStringList{
					QStringLiteral("Tabpage"),
					}
				, false },
			Function{
				QStringLiteral("Object"),
				QStringLiteral("nvim_tabpage_get_var"),
				QStringList{
					QStringLiteral("Tabpage"),
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_tabpage_set_var"),
				QStringList{
					QStringLiteral("Tabpage"),
					QStringLiteral("String"),
					QStringLiteral("Object"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_tabpage_del_var"),
				QStringList{
					QStringLiteral("Tabpage"),
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("Object"),
				QStringLiteral("tabpage_set_var"),
				QStringList{
					QStringLiteral("Tabpage"),
					QStringLiteral("String"),
					QStringLiteral("Object"),
					}
				, false },
			Function{
				QStringLiteral("Object"),
				QStringLiteral("tabpage_del_var"),
				QStringList{
					QStringLiteral("Tabpage"),
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("Window"),
				QStringLiteral("nvim_tabpage_get_win"),
				QStringList{
					QStringLiteral("Tabpage"),
					}
				, false },
			Function{
				QStringLiteral("Integer"),
				QStringLiteral("nvim_tabpage_get_number"),
				QStringList{
					QStringLiteral("Tabpage"),
					}
				, false },
			Function{
				QStringLiteral("Boolean"),
				QStringLiteral("nvim_tabpage_is_valid"),
				QStringList{
					QStringLiteral("Tabpage"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_ui_attach"),
				QStringList{
					QStringLiteral("Integer"),
					QStringLiteral("Integer"),
					QStringLiteral("Dictionary"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("ui_attach"),
				QStringList{
					QStringLiteral("Integer"),
					QStringLiteral("Integer"),
					QStringLiteral("Boolean"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_ui_detach"),
				QStringList{
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_ui_try_resize"),
				QStringList{
					QStringLiteral("Integer"),
					QStringLiteral("Integer"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_ui_set_option"),
				QStringList{
					QStringLiteral("String"),
					QStringLiteral("Object"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_ui_try_resize_grid"),
				QStringList{
					QStringLiteral("Integer"),
					QStringLiteral("Integer"),
					QStringLiteral("Integer"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_ui_pum_set_height"),
				QStringList{
					QStringLiteral("Integer"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_command"),
				QStringList{
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("Dictionary"),
				QStringLiteral("nvim_get_hl_by_name"),
				QStringList{
					QStringLiteral("String"),
					QStringLiteral("Boolean"),
					}
				, false },
			Function{
				QStringLiteral("Dictionary"),
				QStringLiteral("nvim_get_hl_by_id"),
				QStringList{
					QStringLiteral("Integer"),
					QStringLiteral("Boolean"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_feedkeys"),
				QStringList{
					QStringLiteral("String"),
					QStringLiteral("String"),
					QStringLiteral("Boolean"),
					}
				, false },
			Function{
				QStringLiteral("Integer"),
				QStringLiteral("nvim_input"),
				QStringList{
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_input_mouse"),
				QStringList{
					QStringLiteral("String"),
					QStringLiteral("String"),
					QStringLiteral("String"),
					QStringLiteral("Integer"),
					QStringLiteral("Integer"),
					QStringLiteral("Integer"),
					}
				, false },
			Function{
				QStringLiteral("String"),
				QStringLiteral("nvim_replace_termcodes"),
				QStringList{
					QStringLiteral("String"),
					QStringLiteral("Boolean"),
					QStringLiteral("Boolean"),
					QStringLiteral("Boolean"),
					}
				, false },
			Function{
				QStringLiteral("String"),
				QStringLiteral("nvim_command_output"),
				QStringList{
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("Object"),
				QStringLiteral("nvim_eval"),
				QStringList{
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("Object"),
				QStringLiteral("nvim_execute_lua"),
				QStringList{
					QStringLiteral("String"),
					QStringLiteral("Array"),
					}
				, false },
			Function{
				QStringLiteral("Object"),
				QStringLiteral("nvim_call_function"),
				QStringList{
					QStringLiteral("String"),
					QStringLiteral("Array"),
					}
				, false },
			Function{
				QStringLiteral("Object"),
				QStringLiteral("nvim_call_dict_function"),
				QStringList{
					QStringLiteral("Object"),
					QStringLiteral("String"),
					QStringLiteral("Array"),
					}
				, false },
			Function{
				QStringLiteral("Integer"),
				QStringLiteral("nvim_strwidth"),
				QStringList{
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("ArrayOf(String)"),
				QStringLiteral("nvim_list_runtime_paths"),
				QStringList{
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_set_current_dir"),
				QStringList{
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("String"),
				QStringLiteral("nvim_get_current_line"),
				QStringList{
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_set_current_line"),
				QStringList{
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_del_current_line"),
				QStringList{
					}
				, false },
			Function{
				QStringLiteral("Object"),
				QStringLiteral("nvim_get_var"),
				QStringList{
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_set_var"),
				QStringList{
					QStringLiteral("String"),
					QStringLiteral("Object"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_del_var"),
				QStringList{
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("Object"),
				QStringLiteral("vim_set_var"),
				QStringList{
					QStringLiteral("String"),
					QStringLiteral("Object"),
					}
				, false },
			Function{
				QStringLiteral("Object"),
				QStringLiteral("vim_del_var"),
				QStringList{
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("Object"),
				QStringLiteral("nvim_get_vvar"),
				QStringList{
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_set_vvar"),
				QStringList{
					QStringLiteral("String"),
					QStringLiteral("Object"),
					}
				, false },
			Function{
				QStringLiteral("Object"),
				QStringLiteral("nvim_get_option"),
				QStringList{
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_set_option"),
				QStringList{
					QStringLiteral("String"),
					QStringLiteral("Object"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_out_write"),
				QStringList{
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_err_write"),
				QStringList{
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_err_writeln"),
				QStringList{
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("ArrayOf(Buffer)"),
				QStringLiteral("nvim_list_bufs"),
				QStringList{
					}
				, false },
			Function{
				QStringLiteral("Buffer"),
				QStringLiteral("nvim_get_current_buf"),
				QStringList{
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_set_current_buf"),
				QStringList{
					QStringLiteral("Buffer"),
					}
				, false },
			Function{
				QStringLiteral("ArrayOf(Window)"),
				QStringLiteral("nvim_list_wins"),
				QStringList{
					}
				, false },
			Function{
				QStringLiteral("Window"),
				QStringLiteral("nvim_get_current_win"),
				QStringList{
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_set_current_win"),
				QStringList{
					QStringLiteral("Window"),
					}
				, false },
			Function{
				QStringLiteral("Buffer"),
				QStringLiteral("nvim_create_buf"),
				QStringList{
					QStringLiteral("Boolean"),
					QStringLiteral("Boolean"),
					}
				, false },
			Function{
				QStringLiteral("Window"),
				QStringLiteral("nvim_open_win"),
				QStringList{
					QStringLiteral("Buffer"),
					QStringLiteral("Boolean"),
					QStringLiteral("Dictionary"),
					}
				, false },
			Function{
				QStringLiteral("ArrayOf(Tabpage)"),
				QStringLiteral("nvim_list_tabpages"),
				QStringList{
					}
				, false },
			Function{
				QStringLiteral("Tabpage"),
				QStringLiteral("nvim_get_current_tabpage"),
				QStringList{
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_set_current_tabpage"),
				QStringList{
					QStringLiteral("Tabpage"),
					}
				, false },
			Function{
				QStringLiteral("Integer"),
				QStringLiteral("nvim_create_namespace"),
				QStringList{
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("Dictionary"),
				QStringLiteral("nvim_get_namespaces"),
				QStringList{
					}
				, false },
			Function{
				QStringLiteral("Boolean"),
				QStringLiteral("nvim_paste"),
				QStringList{
					QStringLiteral("String"),
					QStringLiteral("Boolean"),
					QStringLiteral("Integer"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_put"),
				QStringList{
					QStringLiteral("ArrayOf(String)"),
					QStringLiteral("String"),
					QStringLiteral("Boolean"),
					QStringLiteral("Boolean"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_subscribe"),
				QStringList{
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_unsubscribe"),
				QStringList{
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("Integer"),
				QStringLiteral("nvim_get_color_by_name"),
				QStringList{
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("Dictionary"),
				QStringLiteral("nvim_get_color_map"),
				QStringList{
					}
				, false },
			Function{
				QStringLiteral("Dictionary"),
				QStringLiteral("nvim_get_context"),
				QStringList{
					QStringLiteral("Dictionary"),
					}
				, false },
			Function{
				QStringLiteral("Object"),
				QStringLiteral("nvim_load_context"),
				QStringList{
					QStringLiteral("Dictionary"),
					}
				, false },
			Function{
				QStringLiteral("Dictionary"),
				QStringLiteral("nvim_get_mode"),
				QStringList{
					}
				, false },
			Function{
				QStringLiteral("ArrayOf(Dictionary)"),
				QStringLiteral("nvim_get_keymap"),
				QStringList{
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_set_keymap"),
				QStringList{
					QStringLiteral("String"),
					QStringLiteral("String"),
					QStringLiteral("String"),
					QStringLiteral("Dictionary"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_del_keymap"),
				QStringList{
					QStringLiteral("String"),
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("Dictionary"),
				QStringLiteral("nvim_get_commands"),
				QStringList{
					QStringLiteral("Dictionary"),
					}
				, false },
			Function{
				QStringLiteral("Array"),
				QStringLiteral("nvim_get_api_info"),
				QStringList{
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_set_client_info"),
				QStringList{
					QStringLiteral("String"),
					QStringLiteral("Dictionary"),
					QStringLiteral("String"),
					QStringLiteral("Dictionary"),
					QStringLiteral("Dictionary"),
					}
				, false },
			Function{
				QStringLiteral("Dictionary"),
				QStringLiteral("nvim_get_chan_info"),
				QStringList{
					QStringLiteral("Integer"),
					}
				, false },
			Function{
				QStringLiteral("Array"),
				QStringLiteral("nvim_list_chans"),
				QStringList{
					}
				, false },
			Function{
				QStringLiteral("Array"),
				QStringLiteral("nvim_call_atomic"),
				QStringList{
					QStringLiteral("Array"),
					}
				, false },
			Function{
				QStringLiteral("Dictionary"),
				QStringLiteral("nvim_parse_expression"),
				QStringList{
					QStringLiteral("String"),
					QStringLiteral("String"),
					QStringLiteral("Boolean"),
					}
				, false },
			Function{
				QStringLiteral("Array"),
				QStringLiteral("nvim_list_uis"),
				QStringList{
					}
				, false },
			Function{
				QStringLiteral("Array"),
				QStringLiteral("nvim_get_proc_children"),
				QStringList{
					QStringLiteral("Integer"),
					}
				, false },
			Function{
				QStringLiteral("Object"),
				QStringLiteral("nvim_get_proc"),
				QStringList{
					QStringLiteral("Integer"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_select_popupmenu_item"),
				QStringList{
					QStringLiteral("Integer"),
					QStringLiteral("Boolean"),
					QStringLiteral("Boolean"),
					QStringLiteral("Dictionary"),
					}
				, false },
			Function{
				QStringLiteral("Buffer"),
				QStringLiteral("nvim_win_get_buf"),
				QStringList{
					QStringLiteral("Window"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_win_set_buf"),
				QStringList{
					QStringLiteral("Window"),
					QStringLiteral("Buffer"),
					}
				, false },
			Function{
				QStringLiteral("ArrayOf(Integer, 2)"),
				QStringLiteral("nvim_win_get_cursor"),
				QStringList{
					QStringLiteral("Window"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_win_set_cursor"),
				QStringList{
					QStringLiteral("Window"),
					QStringLiteral("ArrayOf(Integer, 2)"),
					}
				, false },
			Function{
				QStringLiteral("Integer"),
				QStringLiteral("nvim_win_get_height"),
				QStringList{
					QStringLiteral("Window"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_win_set_height"),
				QStringList{
					QStringLiteral("Window"),
					QStringLiteral("Integer"),
					}
				, false },
			Function{
				QStringLiteral("Integer"),
				QStringLiteral("nvim_win_get_width"),
				QStringList{
					QStringLiteral("Window"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_win_set_width"),
				QStringList{
					QStringLiteral("Window"),
					QStringLiteral("Integer"),
					}
				, false },
			Function{
				QStringLiteral("Object"),
				QStringLiteral("nvim_win_get_var"),
				QStringList{
					QStringLiteral("Window"),
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_win_set_var"),
				QStringList{
					QStringLiteral("Window"),
					QStringLiteral("String"),
					QStringLiteral("Object"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_win_del_var"),
				QStringList{
					QStringLiteral("Window"),
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("Object"),
				QStringLiteral("window_set_var"),
				QStringList{
					QStringLiteral("Window"),
					QStringLiteral("String"),
					QStringLiteral("Object"),
					}
				, false },
			Function{
				QStringLiteral("Object"),
				QStringLiteral("window_del_var"),
				QStringList{
					QStringLiteral("Window"),
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("Object"),
				QStringLiteral("nvim_win_get_option"),
				QStringList{
					QStringLiteral("Window"),
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_win_set_option"),
				QStringList{
					QStringLiteral("Window"),
					QStringLiteral("String"),
					QStringLiteral("Object"),
					}
				, false },
			Function{
				QStringLiteral("ArrayOf(Integer, 2)"),
				QStringLiteral("nvim_win_get_position"),
				QStringList{
					QStringLiteral("Window"),
					}
				, false },
			Function{
				QStringLiteral("Tabpage"),
				QStringLiteral("nvim_win_get_tabpage"),
				QStringList{
					QStringLiteral("Window"),
					}
				, false },
			Function{
				QStringLiteral("Integer"),
				QStringLiteral("nvim_win_get_number"),
				QStringList{
					QStringLiteral("Window"),
					}
				, false },
			Function{
				QStringLiteral("Boolean"),
				QStringLiteral("nvim_win_is_valid"),
				QStringList{
					QStringLiteral("Window"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_win_set_config"),
				QStringList{
					QStringLiteral("Window"),
					QStringLiteral("Dictionary"),
					}
				, false },
			Function{
				QStringLiteral("Dictionary"),
				QStringLiteral("nvim_win_get_config"),
				QStringList{
					QStringLiteral("Window"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("nvim_win_close"),
				QStringList{
					QStringLiteral("Window"),
					QStringLiteral("Boolean"),
					}
				, false },
			Function{
				QStringLiteral("Integer"),
				QStringLiteral("buffer_line_count"),
				QStringList{
					QStringLiteral("Buffer"),
					}
				, false },
			Function{
				QStringLiteral("ArrayOf(String)"),
				QStringLiteral("buffer_get_lines"),
				QStringList{
					QStringLiteral("Buffer"),
					QStringLiteral("Integer"),
					QStringLiteral("Integer"),
					QStringLiteral("Boolean"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("buffer_set_lines"),
				QStringList{
					QStringLiteral("Buffer"),
					QStringLiteral("Integer"),
					QStringLiteral("Integer"),
					QStringLiteral("Boolean"),
					QStringLiteral("ArrayOf(String)"),
					}
				, false },
			Function{
				QStringLiteral("Object"),
				QStringLiteral("buffer_get_var"),
				QStringList{
					QStringLiteral("Buffer"),
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("Object"),
				QStringLiteral("buffer_get_option"),
				QStringList{
					QStringLiteral("Buffer"),
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("buffer_set_option"),
				QStringList{
					QStringLiteral("Buffer"),
					QStringLiteral("String"),
					QStringLiteral("Object"),
					}
				, false },
			Function{
				QStringLiteral("Integer"),
				QStringLiteral("buffer_get_number"),
				QStringList{
					QStringLiteral("Buffer"),
					}
				, false },
			Function{
				QStringLiteral("String"),
				QStringLiteral("buffer_get_name"),
				QStringList{
					QStringLiteral("Buffer"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("buffer_set_name"),
				QStringList{
					QStringLiteral("Buffer"),
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("Boolean"),
				QStringLiteral("buffer_is_valid"),
				QStringList{
					QStringLiteral("Buffer"),
					}
				, false },
			Function{
				QStringLiteral("ArrayOf(Integer, 2)"),
				QStringLiteral("buffer_get_mark"),
				QStringList{
					QStringLiteral("Buffer"),
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("Integer"),
				QStringLiteral("buffer_add_highlight"),
				QStringList{
					QStringLiteral("Buffer"),
					QStringLiteral("Integer"),
					QStringLiteral("String"),
					QStringLiteral("Integer"),
					QStringLiteral("Integer"),
					QStringLiteral("Integer"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("buffer_clear_highlight"),
				QStringList{
					QStringLiteral("Buffer"),
					QStringLiteral("Integer"),
					QStringLiteral("Integer"),
					QStringLiteral("Integer"),
					}
				, false },
			Function{
				QStringLiteral("ArrayOf(Window)"),
				QStringLiteral("tabpage_get_windows"),
				QStringList{
					QStringLiteral("Tabpage"),
					}
				, false },
			Function{
				QStringLiteral("Object"),
				QStringLiteral("tabpage_get_var"),
				QStringList{
					QStringLiteral("Tabpage"),
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("Window"),
				QStringLiteral("tabpage_get_window"),
				QStringList{
					QStringLiteral("Tabpage"),
					}
				, false },
			Function{
				QStringLiteral("Boolean"),
				QStringLiteral("tabpage_is_valid"),
				QStringList{
					QStringLiteral("Tabpage"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("ui_detach"),
				QStringList{
					}
				, false },
			Function{
				QStringLiteral("Object"),
				QStringLiteral("ui_try_resize"),
				QStringList{
					QStringLiteral("Integer"),
					QStringLiteral("Integer"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("vim_command"),
				QStringList{
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("vim_feedkeys"),
				QStringList{
					QStringLiteral("String"),
					QStringLiteral("String"),
					QStringLiteral("Boolean"),
					}
				, false },
			Function{
				QStringLiteral("Integer"),
				QStringLiteral("vim_input"),
				QStringList{
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("String"),
				QStringLiteral("vim_replace_termcodes"),
				QStringList{
					QStringLiteral("String"),
					QStringLiteral("Boolean"),
					QStringLiteral("Boolean"),
					QStringLiteral("Boolean"),
					}
				, false },
			Function{
				QStringLiteral("String"),
				QStringLiteral("vim_command_output"),
				QStringList{
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("Object"),
				QStringLiteral("vim_eval"),
				QStringList{
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("Object"),
				QStringLiteral("vim_call_function"),
				QStringList{
					QStringLiteral("String"),
					QStringLiteral("Array"),
					}
				, false },
			Function{
				QStringLiteral("Integer"),
				QStringLiteral("vim_strwidth"),
				QStringList{
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("ArrayOf(String)"),
				QStringLiteral("vim_list_runtime_paths"),
				QStringList{
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("vim_change_directory"),
				QStringList{
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("String"),
				QStringLiteral("vim_get_current_line"),
				QStringList{
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("vim_set_current_line"),
				QStringList{
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("vim_del_current_line"),
				QStringList{
					}
				, false },
			Function{
				QStringLiteral("Object"),
				QStringLiteral("vim_get_var"),
				QStringList{
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("Object"),
				QStringLiteral("vim_get_vvar"),
				QStringList{
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("Object"),
				QStringLiteral("vim_get_option"),
				QStringList{
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("vim_set_option"),
				QStringList{
					QStringLiteral("String"),
					QStringLiteral("Object"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("vim_out_write"),
				QStringList{
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("vim_err_write"),
				QStringList{
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("vim_report_error"),
				QStringList{
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("ArrayOf(Buffer)"),
				QStringLiteral("vim_get_buffers"),
				QStringList{
					}
				, false },
			Function{
				QStringLiteral("Buffer"),
				QStringLiteral("vim_get_current_buffer"),
				QStringList{
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("vim_set_current_buffer"),
				QStringList{
					QStringLiteral("Buffer"),
					}
				, false },
			Function{
				QStringLiteral("ArrayOf(Window)"),
				QStringLiteral("vim_get_windows"),
				QStringList{
					}
				, false },
			Function{
				QStringLiteral("Window"),
				QStringLiteral("vim_get_current_window"),
				QStringList{
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("vim_set_current_window"),
				QStringList{
					QStringLiteral("Window"),
					}
				, false },
			Function{
				QStringLiteral("ArrayOf(Tabpage)"),
				QStringLiteral("vim_get_tabpages"),
				QStringList{
					}
				, false },
			Function{
				QStringLiteral("Tabpage"),
				QStringLiteral("vim_get_current_tabpage"),
				QStringList{
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("vim_set_current_tabpage"),
				QStringList{
					QStringLiteral("Tabpage"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("vim_subscribe"),
				QStringList{
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("vim_unsubscribe"),
				QStringList{
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("Integer"),
				QStringLiteral("vim_name_to_color"),
				QStringList{
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("Dictionary"),
				QStringLiteral("vim_get_color_map"),
				QStringList{
					}
				, false },
			Function{
				QStringLiteral("Buffer"),
				QStringLiteral("window_get_buffer"),
				QStringList{
					QStringLiteral("Window"),
					}
				, false },
			Function{
				QStringLiteral("ArrayOf(Integer, 2)"),
				QStringLiteral("window_get_cursor"),
				QStringList{
					QStringLiteral("Window"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("window_set_cursor"),
				QStringList{
					QStringLiteral("Window"),
					QStringLiteral("ArrayOf(Integer, 2)"),
					}
				, false },
			Function{
				QStringLiteral("Integer"),
				QStringLiteral("window_get_height"),
				QStringList{
					QStringLiteral("Window"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("window_set_height"),
				QStringList{
					QStringLiteral("Window"),
					QStringLiteral("Integer"),
					}
				, false },
			Function{
				QStringLiteral("Integer"),
				QStringLiteral("window_get_width"),
				QStringList{
					QStringLiteral("Window"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("window_set_width"),
				QStringList{
					QStringLiteral("Window"),
					QStringLiteral("Integer"),
					}
				, false },
			Function{
				QStringLiteral("Object"),
				QStringLiteral("window_get_var"),
				QStringList{
					QStringLiteral("Window"),
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("Object"),
				QStringLiteral("window_get_option"),
				QStringList{
					QStringLiteral("Window"),
					QStringLiteral("String"),
					}
				, false },
			Function{
				QStringLiteral("void"),
				QStringLiteral("window_set_option"),
				QStringList{
					QStringLiteral("Window"),
					QStringLiteral("String"),
					QStringLiteral("Object"),
					}
				, false },
			Function{
				QStringLiteral("ArrayOf(Integer, 2)"),
				QStringLiteral("window_get_position"),
				QStringList{
					QStringLiteral("Window"),
					}
				, false },
			Function{
				QStringLiteral("Tabpage"),
				QStringLiteral("window_get_tabpage"),
				QStringList{
					QStringLiteral("Window"),
					}
				, false },
			Function{
				QStringLiteral("Boolean"),
				QStringLiteral("window_is_valid"),
				QStringList{
					QStringLiteral("Window"),
					}
				, false },
		};

	QVector<Function> supported;
	supported.reserve(ftable.size());
	for(const auto& val : ftable) {
		auto f = Function::fromVariant(val);
		if (!f.isValid()) {
			qDebug() << "Invalid function in metadata" << f;
			continue;
		}
		supported.append(f);

		if (!required.contains(f)) {
			qDebug() << "Unknown function(api 6)" << f;
		}
	}

	bool ok = true;
	for(const auto& f : required) {
		if (!supported.contains(f)) {
			qDebug() << "- instance DOES NOT support API6:" << f;
			ok = false;
		}
	}
	return ok;
}

} // Namespace