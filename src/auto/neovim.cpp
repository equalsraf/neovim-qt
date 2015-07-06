// Auto generated 2015-07-06 14:02:18.187121
#include "neovim.h"
#include "neovimconnector.h"
#include "msgpackrequest.h"
#include "msgpackiodevice.h"
#include "util.h"

namespace NeovimQt {

/* Unpack Neovim EXT types Window, Buffer Tabpage which are all
 * uint64_t see Neovim:msgpack_rpc_to_
 */
QVariant unpackBuffer(MsgpackIODevice *dev, const char* in, quint32 size)
{
	msgpack_unpacked result;
	msgpack_unpacked_init(&result);
	msgpack_unpack_return ret = msgpack_unpack_next(&result, in, size, NULL);
	msgpack_unpacked_destroy(&result);

	if (ret != MSGPACK_UNPACK_SUCCESS) {
		return QVariant();
	}
	return QVariant((quint64)result.data.via.u64);
}
#define unpackWindow unpackBuffer
#define unpackTabpage unpackBuffer

Neovim::Neovim(NeovimConnector *c)
:m_c(c)
{
	// EXT types
		m_c->m_dev->registerExtType(0, unpackBuffer);
		m_c->m_dev->registerExtType(1, unpackWindow);
		m_c->m_dev->registerExtType(2, unpackTabpage);
		connect(m_c->m_dev, &MsgpackIODevice::notification,
			this, &Neovim::neovimNotification);
}


void Neovim::ui_try_resize(int64_t width, int64_t height)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("ui_try_resize", 2);
	m_c->m_dev->send(width);
	m_c->m_dev->send(height);
	connect(r, &MsgpackRequest::finished, this, &Neovim::on_ui_try_resize);
}


// Slots
MsgpackRequest* Neovim::tabpage_get_windows(int64_t tabpage)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("tabpage_get_windows", 1);
	r->setFunction(Function::NEOVIM_FN_TABPAGE_GET_WINDOWS);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(tabpage);
	return r;
}
MsgpackRequest* Neovim::tabpage_get_var(int64_t tabpage, QByteArray name)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("tabpage_get_var", 2);
	r->setFunction(Function::NEOVIM_FN_TABPAGE_GET_VAR);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(tabpage);
	m_c->m_dev->send(name);
	return r;
}
MsgpackRequest* Neovim::tabpage_set_var(int64_t tabpage, QByteArray name, QVariant value)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("tabpage_set_var", 3);
	r->setFunction(Function::NEOVIM_FN_TABPAGE_SET_VAR);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(tabpage);
	m_c->m_dev->send(name);
	m_c->m_dev->send(value);
	return r;
}
MsgpackRequest* Neovim::tabpage_get_window(int64_t tabpage)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("tabpage_get_window", 1);
	r->setFunction(Function::NEOVIM_FN_TABPAGE_GET_WINDOW);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(tabpage);
	return r;
}
MsgpackRequest* Neovim::tabpage_is_valid(int64_t tabpage)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("tabpage_is_valid", 1);
	r->setFunction(Function::NEOVIM_FN_TABPAGE_IS_VALID);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(tabpage);
	return r;
}
MsgpackRequest* Neovim::buffer_line_count(int64_t buffer)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("buffer_line_count", 1);
	r->setFunction(Function::NEOVIM_FN_BUFFER_LINE_COUNT);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(buffer);
	return r;
}
MsgpackRequest* Neovim::buffer_get_line(int64_t buffer, int64_t index)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("buffer_get_line", 2);
	r->setFunction(Function::NEOVIM_FN_BUFFER_GET_LINE);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(buffer);
	m_c->m_dev->send(index);
	return r;
}
MsgpackRequest* Neovim::buffer_set_line(int64_t buffer, int64_t index, QByteArray line)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("buffer_set_line", 3);
	r->setFunction(Function::NEOVIM_FN_BUFFER_SET_LINE);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(buffer);
	m_c->m_dev->send(index);
	m_c->m_dev->send(line);
	return r;
}
MsgpackRequest* Neovim::buffer_del_line(int64_t buffer, int64_t index)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("buffer_del_line", 2);
	r->setFunction(Function::NEOVIM_FN_BUFFER_DEL_LINE);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(buffer);
	m_c->m_dev->send(index);
	return r;
}
MsgpackRequest* Neovim::buffer_get_line_slice(int64_t buffer, int64_t start, int64_t end, bool include_start, bool include_end)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("buffer_get_line_slice", 5);
	r->setFunction(Function::NEOVIM_FN_BUFFER_GET_LINE_SLICE);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(buffer);
	m_c->m_dev->send(start);
	m_c->m_dev->send(end);
	m_c->m_dev->send(include_start);
	m_c->m_dev->send(include_end);
	return r;
}
MsgpackRequest* Neovim::buffer_set_line_slice(int64_t buffer, int64_t start, int64_t end, bool include_start, bool include_end, QList<QByteArray> replacement)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("buffer_set_line_slice", 6);
	r->setFunction(Function::NEOVIM_FN_BUFFER_SET_LINE_SLICE);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(buffer);
	m_c->m_dev->send(start);
	m_c->m_dev->send(end);
	m_c->m_dev->send(include_start);
	m_c->m_dev->send(include_end);
	m_c->m_dev->sendArrayOf(replacement);
	return r;
}
MsgpackRequest* Neovim::buffer_get_var(int64_t buffer, QByteArray name)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("buffer_get_var", 2);
	r->setFunction(Function::NEOVIM_FN_BUFFER_GET_VAR);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(buffer);
	m_c->m_dev->send(name);
	return r;
}
MsgpackRequest* Neovim::buffer_set_var(int64_t buffer, QByteArray name, QVariant value)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("buffer_set_var", 3);
	r->setFunction(Function::NEOVIM_FN_BUFFER_SET_VAR);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(buffer);
	m_c->m_dev->send(name);
	m_c->m_dev->send(value);
	return r;
}
MsgpackRequest* Neovim::buffer_get_option(int64_t buffer, QByteArray name)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("buffer_get_option", 2);
	r->setFunction(Function::NEOVIM_FN_BUFFER_GET_OPTION);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(buffer);
	m_c->m_dev->send(name);
	return r;
}
MsgpackRequest* Neovim::buffer_set_option(int64_t buffer, QByteArray name, QVariant value)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("buffer_set_option", 3);
	r->setFunction(Function::NEOVIM_FN_BUFFER_SET_OPTION);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(buffer);
	m_c->m_dev->send(name);
	m_c->m_dev->send(value);
	return r;
}
MsgpackRequest* Neovim::buffer_get_number(int64_t buffer)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("buffer_get_number", 1);
	r->setFunction(Function::NEOVIM_FN_BUFFER_GET_NUMBER);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(buffer);
	return r;
}
MsgpackRequest* Neovim::buffer_get_name(int64_t buffer)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("buffer_get_name", 1);
	r->setFunction(Function::NEOVIM_FN_BUFFER_GET_NAME);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(buffer);
	return r;
}
MsgpackRequest* Neovim::buffer_set_name(int64_t buffer, QByteArray name)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("buffer_set_name", 2);
	r->setFunction(Function::NEOVIM_FN_BUFFER_SET_NAME);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(buffer);
	m_c->m_dev->send(name);
	return r;
}
MsgpackRequest* Neovim::buffer_is_valid(int64_t buffer)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("buffer_is_valid", 1);
	r->setFunction(Function::NEOVIM_FN_BUFFER_IS_VALID);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(buffer);
	return r;
}
MsgpackRequest* Neovim::buffer_insert(int64_t buffer, int64_t lnum, QList<QByteArray> lines)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("buffer_insert", 3);
	r->setFunction(Function::NEOVIM_FN_BUFFER_INSERT);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(buffer);
	m_c->m_dev->send(lnum);
	m_c->m_dev->sendArrayOf(lines);
	return r;
}
MsgpackRequest* Neovim::buffer_get_mark(int64_t buffer, QByteArray name)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("buffer_get_mark", 2);
	r->setFunction(Function::NEOVIM_FN_BUFFER_GET_MARK);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(buffer);
	m_c->m_dev->send(name);
	return r;
}
MsgpackRequest* Neovim::window_get_buffer(int64_t window)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("window_get_buffer", 1);
	r->setFunction(Function::NEOVIM_FN_WINDOW_GET_BUFFER);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(window);
	return r;
}
MsgpackRequest* Neovim::window_get_cursor(int64_t window)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("window_get_cursor", 1);
	r->setFunction(Function::NEOVIM_FN_WINDOW_GET_CURSOR);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(window);
	return r;
}
MsgpackRequest* Neovim::window_set_cursor(int64_t window, QPoint pos)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("window_set_cursor", 2);
	r->setFunction(Function::NEOVIM_FN_WINDOW_SET_CURSOR);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(window);
	m_c->m_dev->send(pos);
	return r;
}
MsgpackRequest* Neovim::window_get_height(int64_t window)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("window_get_height", 1);
	r->setFunction(Function::NEOVIM_FN_WINDOW_GET_HEIGHT);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(window);
	return r;
}
MsgpackRequest* Neovim::window_set_height(int64_t window, int64_t height)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("window_set_height", 2);
	r->setFunction(Function::NEOVIM_FN_WINDOW_SET_HEIGHT);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(window);
	m_c->m_dev->send(height);
	return r;
}
MsgpackRequest* Neovim::window_get_width(int64_t window)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("window_get_width", 1);
	r->setFunction(Function::NEOVIM_FN_WINDOW_GET_WIDTH);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(window);
	return r;
}
MsgpackRequest* Neovim::window_set_width(int64_t window, int64_t width)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("window_set_width", 2);
	r->setFunction(Function::NEOVIM_FN_WINDOW_SET_WIDTH);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(window);
	m_c->m_dev->send(width);
	return r;
}
MsgpackRequest* Neovim::window_get_var(int64_t window, QByteArray name)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("window_get_var", 2);
	r->setFunction(Function::NEOVIM_FN_WINDOW_GET_VAR);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(window);
	m_c->m_dev->send(name);
	return r;
}
MsgpackRequest* Neovim::window_set_var(int64_t window, QByteArray name, QVariant value)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("window_set_var", 3);
	r->setFunction(Function::NEOVIM_FN_WINDOW_SET_VAR);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(window);
	m_c->m_dev->send(name);
	m_c->m_dev->send(value);
	return r;
}
MsgpackRequest* Neovim::window_get_option(int64_t window, QByteArray name)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("window_get_option", 2);
	r->setFunction(Function::NEOVIM_FN_WINDOW_GET_OPTION);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(window);
	m_c->m_dev->send(name);
	return r;
}
MsgpackRequest* Neovim::window_set_option(int64_t window, QByteArray name, QVariant value)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("window_set_option", 3);
	r->setFunction(Function::NEOVIM_FN_WINDOW_SET_OPTION);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(window);
	m_c->m_dev->send(name);
	m_c->m_dev->send(value);
	return r;
}
MsgpackRequest* Neovim::window_get_position(int64_t window)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("window_get_position", 1);
	r->setFunction(Function::NEOVIM_FN_WINDOW_GET_POSITION);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(window);
	return r;
}
MsgpackRequest* Neovim::window_get_tabpage(int64_t window)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("window_get_tabpage", 1);
	r->setFunction(Function::NEOVIM_FN_WINDOW_GET_TABPAGE);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(window);
	return r;
}
MsgpackRequest* Neovim::window_is_valid(int64_t window)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("window_is_valid", 1);
	r->setFunction(Function::NEOVIM_FN_WINDOW_IS_VALID);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(window);
	return r;
}
MsgpackRequest* Neovim::vim_command(QByteArray str)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("vim_command", 1);
	r->setFunction(Function::NEOVIM_FN_VIM_COMMAND);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(str);
	return r;
}
MsgpackRequest* Neovim::vim_feedkeys(QByteArray keys, QByteArray mode, bool escape_csi)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("vim_feedkeys", 3);
	r->setFunction(Function::NEOVIM_FN_VIM_FEEDKEYS);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(keys);
	m_c->m_dev->send(mode);
	m_c->m_dev->send(escape_csi);
	return r;
}
MsgpackRequest* Neovim::vim_input(QByteArray keys)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("vim_input", 1);
	r->setFunction(Function::NEOVIM_FN_VIM_INPUT);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(keys);
	return r;
}
MsgpackRequest* Neovim::vim_replace_termcodes(QByteArray str, bool from_part, bool do_lt, bool special)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("vim_replace_termcodes", 4);
	r->setFunction(Function::NEOVIM_FN_VIM_REPLACE_TERMCODES);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(str);
	m_c->m_dev->send(from_part);
	m_c->m_dev->send(do_lt);
	m_c->m_dev->send(special);
	return r;
}
MsgpackRequest* Neovim::vim_command_output(QByteArray str)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("vim_command_output", 1);
	r->setFunction(Function::NEOVIM_FN_VIM_COMMAND_OUTPUT);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(str);
	return r;
}
MsgpackRequest* Neovim::vim_eval(QByteArray str)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("vim_eval", 1);
	r->setFunction(Function::NEOVIM_FN_VIM_EVAL);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(str);
	return r;
}
MsgpackRequest* Neovim::vim_strwidth(QByteArray str)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("vim_strwidth", 1);
	r->setFunction(Function::NEOVIM_FN_VIM_STRWIDTH);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(str);
	return r;
}
MsgpackRequest* Neovim::vim_list_runtime_paths()
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("vim_list_runtime_paths", 0);
	r->setFunction(Function::NEOVIM_FN_VIM_LIST_RUNTIME_PATHS);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	return r;
}
MsgpackRequest* Neovim::vim_change_directory(QByteArray dir)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("vim_change_directory", 1);
	r->setFunction(Function::NEOVIM_FN_VIM_CHANGE_DIRECTORY);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(dir);
	return r;
}
MsgpackRequest* Neovim::vim_get_current_line()
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("vim_get_current_line", 0);
	r->setFunction(Function::NEOVIM_FN_VIM_GET_CURRENT_LINE);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	return r;
}
MsgpackRequest* Neovim::vim_set_current_line(QByteArray line)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("vim_set_current_line", 1);
	r->setFunction(Function::NEOVIM_FN_VIM_SET_CURRENT_LINE);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(line);
	return r;
}
MsgpackRequest* Neovim::vim_del_current_line()
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("vim_del_current_line", 0);
	r->setFunction(Function::NEOVIM_FN_VIM_DEL_CURRENT_LINE);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	return r;
}
MsgpackRequest* Neovim::vim_get_var(QByteArray name)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("vim_get_var", 1);
	r->setFunction(Function::NEOVIM_FN_VIM_GET_VAR);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(name);
	return r;
}
MsgpackRequest* Neovim::vim_set_var(QByteArray name, QVariant value)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("vim_set_var", 2);
	r->setFunction(Function::NEOVIM_FN_VIM_SET_VAR);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(name);
	m_c->m_dev->send(value);
	return r;
}
MsgpackRequest* Neovim::vim_get_vvar(QByteArray name)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("vim_get_vvar", 1);
	r->setFunction(Function::NEOVIM_FN_VIM_GET_VVAR);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(name);
	return r;
}
MsgpackRequest* Neovim::vim_get_option(QByteArray name)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("vim_get_option", 1);
	r->setFunction(Function::NEOVIM_FN_VIM_GET_OPTION);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(name);
	return r;
}
MsgpackRequest* Neovim::vim_set_option(QByteArray name, QVariant value)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("vim_set_option", 2);
	r->setFunction(Function::NEOVIM_FN_VIM_SET_OPTION);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(name);
	m_c->m_dev->send(value);
	return r;
}
MsgpackRequest* Neovim::vim_out_write(QByteArray str)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("vim_out_write", 1);
	r->setFunction(Function::NEOVIM_FN_VIM_OUT_WRITE);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(str);
	return r;
}
MsgpackRequest* Neovim::vim_err_write(QByteArray str)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("vim_err_write", 1);
	r->setFunction(Function::NEOVIM_FN_VIM_ERR_WRITE);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(str);
	return r;
}
MsgpackRequest* Neovim::vim_report_error(QByteArray str)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("vim_report_error", 1);
	r->setFunction(Function::NEOVIM_FN_VIM_REPORT_ERROR);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(str);
	return r;
}
MsgpackRequest* Neovim::vim_get_buffers()
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("vim_get_buffers", 0);
	r->setFunction(Function::NEOVIM_FN_VIM_GET_BUFFERS);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	return r;
}
MsgpackRequest* Neovim::vim_get_current_buffer()
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("vim_get_current_buffer", 0);
	r->setFunction(Function::NEOVIM_FN_VIM_GET_CURRENT_BUFFER);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	return r;
}
MsgpackRequest* Neovim::vim_set_current_buffer(int64_t buffer)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("vim_set_current_buffer", 1);
	r->setFunction(Function::NEOVIM_FN_VIM_SET_CURRENT_BUFFER);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(buffer);
	return r;
}
MsgpackRequest* Neovim::vim_get_windows()
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("vim_get_windows", 0);
	r->setFunction(Function::NEOVIM_FN_VIM_GET_WINDOWS);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	return r;
}
MsgpackRequest* Neovim::vim_get_current_window()
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("vim_get_current_window", 0);
	r->setFunction(Function::NEOVIM_FN_VIM_GET_CURRENT_WINDOW);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	return r;
}
MsgpackRequest* Neovim::vim_set_current_window(int64_t window)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("vim_set_current_window", 1);
	r->setFunction(Function::NEOVIM_FN_VIM_SET_CURRENT_WINDOW);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(window);
	return r;
}
MsgpackRequest* Neovim::vim_get_tabpages()
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("vim_get_tabpages", 0);
	r->setFunction(Function::NEOVIM_FN_VIM_GET_TABPAGES);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	return r;
}
MsgpackRequest* Neovim::vim_get_current_tabpage()
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("vim_get_current_tabpage", 0);
	r->setFunction(Function::NEOVIM_FN_VIM_GET_CURRENT_TABPAGE);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	return r;
}
MsgpackRequest* Neovim::vim_set_current_tabpage(int64_t tabpage)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("vim_set_current_tabpage", 1);
	r->setFunction(Function::NEOVIM_FN_VIM_SET_CURRENT_TABPAGE);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(tabpage);
	return r;
}
MsgpackRequest* Neovim::vim_subscribe(QByteArray event)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("vim_subscribe", 1);
	r->setFunction(Function::NEOVIM_FN_VIM_SUBSCRIBE);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(event);
	return r;
}
MsgpackRequest* Neovim::vim_unsubscribe(QByteArray event)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("vim_unsubscribe", 1);
	r->setFunction(Function::NEOVIM_FN_VIM_UNSUBSCRIBE);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(event);
	return r;
}
MsgpackRequest* Neovim::vim_name_to_color(QByteArray name)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("vim_name_to_color", 1);
	r->setFunction(Function::NEOVIM_FN_VIM_NAME_TO_COLOR);
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
	m_c->m_dev->send(name);
	return r;
}

// Handlers

void Neovim::handleResponseError(quint32 msgid, Function::FunctionId fun, const QVariant& res)
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

	switch(fun) {
	case Function::NEOVIM_FN_TABPAGE_GET_WINDOWS:
		emit err_tabpage_get_windows(errMsg, res);
		break;
	case Function::NEOVIM_FN_TABPAGE_GET_VAR:
		emit err_tabpage_get_var(errMsg, res);
		break;
	case Function::NEOVIM_FN_TABPAGE_SET_VAR:
		emit err_tabpage_set_var(errMsg, res);
		break;
	case Function::NEOVIM_FN_TABPAGE_GET_WINDOW:
		emit err_tabpage_get_window(errMsg, res);
		break;
	case Function::NEOVIM_FN_BUFFER_LINE_COUNT:
		emit err_buffer_line_count(errMsg, res);
		break;
	case Function::NEOVIM_FN_BUFFER_GET_LINE:
		emit err_buffer_get_line(errMsg, res);
		break;
	case Function::NEOVIM_FN_BUFFER_SET_LINE:
		emit err_buffer_set_line(errMsg, res);
		break;
	case Function::NEOVIM_FN_BUFFER_DEL_LINE:
		emit err_buffer_del_line(errMsg, res);
		break;
	case Function::NEOVIM_FN_BUFFER_GET_LINE_SLICE:
		emit err_buffer_get_line_slice(errMsg, res);
		break;
	case Function::NEOVIM_FN_BUFFER_SET_LINE_SLICE:
		emit err_buffer_set_line_slice(errMsg, res);
		break;
	case Function::NEOVIM_FN_BUFFER_GET_VAR:
		emit err_buffer_get_var(errMsg, res);
		break;
	case Function::NEOVIM_FN_BUFFER_SET_VAR:
		emit err_buffer_set_var(errMsg, res);
		break;
	case Function::NEOVIM_FN_BUFFER_GET_OPTION:
		emit err_buffer_get_option(errMsg, res);
		break;
	case Function::NEOVIM_FN_BUFFER_SET_OPTION:
		emit err_buffer_set_option(errMsg, res);
		break;
	case Function::NEOVIM_FN_BUFFER_GET_NUMBER:
		emit err_buffer_get_number(errMsg, res);
		break;
	case Function::NEOVIM_FN_BUFFER_GET_NAME:
		emit err_buffer_get_name(errMsg, res);
		break;
	case Function::NEOVIM_FN_BUFFER_SET_NAME:
		emit err_buffer_set_name(errMsg, res);
		break;
	case Function::NEOVIM_FN_BUFFER_INSERT:
		emit err_buffer_insert(errMsg, res);
		break;
	case Function::NEOVIM_FN_BUFFER_GET_MARK:
		emit err_buffer_get_mark(errMsg, res);
		break;
	case Function::NEOVIM_FN_WINDOW_GET_BUFFER:
		emit err_window_get_buffer(errMsg, res);
		break;
	case Function::NEOVIM_FN_WINDOW_GET_CURSOR:
		emit err_window_get_cursor(errMsg, res);
		break;
	case Function::NEOVIM_FN_WINDOW_SET_CURSOR:
		emit err_window_set_cursor(errMsg, res);
		break;
	case Function::NEOVIM_FN_WINDOW_GET_HEIGHT:
		emit err_window_get_height(errMsg, res);
		break;
	case Function::NEOVIM_FN_WINDOW_SET_HEIGHT:
		emit err_window_set_height(errMsg, res);
		break;
	case Function::NEOVIM_FN_WINDOW_GET_WIDTH:
		emit err_window_get_width(errMsg, res);
		break;
	case Function::NEOVIM_FN_WINDOW_SET_WIDTH:
		emit err_window_set_width(errMsg, res);
		break;
	case Function::NEOVIM_FN_WINDOW_GET_VAR:
		emit err_window_get_var(errMsg, res);
		break;
	case Function::NEOVIM_FN_WINDOW_SET_VAR:
		emit err_window_set_var(errMsg, res);
		break;
	case Function::NEOVIM_FN_WINDOW_GET_OPTION:
		emit err_window_get_option(errMsg, res);
		break;
	case Function::NEOVIM_FN_WINDOW_SET_OPTION:
		emit err_window_set_option(errMsg, res);
		break;
	case Function::NEOVIM_FN_WINDOW_GET_POSITION:
		emit err_window_get_position(errMsg, res);
		break;
	case Function::NEOVIM_FN_WINDOW_GET_TABPAGE:
		emit err_window_get_tabpage(errMsg, res);
		break;
	case Function::NEOVIM_FN_VIM_COMMAND:
		emit err_vim_command(errMsg, res);
		break;
	case Function::NEOVIM_FN_VIM_COMMAND_OUTPUT:
		emit err_vim_command_output(errMsg, res);
		break;
	case Function::NEOVIM_FN_VIM_EVAL:
		emit err_vim_eval(errMsg, res);
		break;
	case Function::NEOVIM_FN_VIM_STRWIDTH:
		emit err_vim_strwidth(errMsg, res);
		break;
	case Function::NEOVIM_FN_VIM_CHANGE_DIRECTORY:
		emit err_vim_change_directory(errMsg, res);
		break;
	case Function::NEOVIM_FN_VIM_GET_CURRENT_LINE:
		emit err_vim_get_current_line(errMsg, res);
		break;
	case Function::NEOVIM_FN_VIM_SET_CURRENT_LINE:
		emit err_vim_set_current_line(errMsg, res);
		break;
	case Function::NEOVIM_FN_VIM_DEL_CURRENT_LINE:
		emit err_vim_del_current_line(errMsg, res);
		break;
	case Function::NEOVIM_FN_VIM_GET_VAR:
		emit err_vim_get_var(errMsg, res);
		break;
	case Function::NEOVIM_FN_VIM_SET_VAR:
		emit err_vim_set_var(errMsg, res);
		break;
	case Function::NEOVIM_FN_VIM_GET_VVAR:
		emit err_vim_get_vvar(errMsg, res);
		break;
	case Function::NEOVIM_FN_VIM_GET_OPTION:
		emit err_vim_get_option(errMsg, res);
		break;
	case Function::NEOVIM_FN_VIM_SET_OPTION:
		emit err_vim_set_option(errMsg, res);
		break;
	case Function::NEOVIM_FN_VIM_SET_CURRENT_BUFFER:
		emit err_vim_set_current_buffer(errMsg, res);
		break;
	case Function::NEOVIM_FN_VIM_SET_CURRENT_WINDOW:
		emit err_vim_set_current_window(errMsg, res);
		break;
	case Function::NEOVIM_FN_VIM_SET_CURRENT_TABPAGE:
		emit err_vim_set_current_tabpage(errMsg, res);
		break;
	default:
		m_c->setError(NeovimConnector::RuntimeMsgpackError, QString("Received error for function that should not fail: %s").arg(fun));
	}
}

void Neovim::handleResponse(quint32 msgid, Function::FunctionId fun, const QVariant& res)
{
	switch(fun) {
	case Function::NEOVIM_FN_TABPAGE_GET_WINDOWS:
		{
			QList<int64_t> data;
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for tabpage_get_windows");
				return;
			} else {
				emit on_tabpage_get_windows(data);
			}

		}
		break;
	case Function::NEOVIM_FN_TABPAGE_GET_VAR:
		{
			QVariant data;
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for tabpage_get_var");
				return;
			} else {
				emit on_tabpage_get_var(data);
			}

		}
		break;
	case Function::NEOVIM_FN_TABPAGE_SET_VAR:
		{
			QVariant data;
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for tabpage_set_var");
				return;
			} else {
				emit on_tabpage_set_var(data);
			}

		}
		break;
	case Function::NEOVIM_FN_TABPAGE_GET_WINDOW:
		{
			int64_t data;
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for tabpage_get_window");
				return;
			} else {
				emit on_tabpage_get_window(data);
			}

		}
		break;
	case Function::NEOVIM_FN_TABPAGE_IS_VALID:
		{
			bool data;
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for tabpage_is_valid");
				return;
			} else {
				emit on_tabpage_is_valid(data);
			}

		}
		break;
	case Function::NEOVIM_FN_BUFFER_LINE_COUNT:
		{
			int64_t data;
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for buffer_line_count");
				return;
			} else {
				emit on_buffer_line_count(data);
			}

		}
		break;
	case Function::NEOVIM_FN_BUFFER_GET_LINE:
		{
			QByteArray data;
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for buffer_get_line");
				return;
			} else {
				emit on_buffer_get_line(data);
			}

		}
		break;
	case Function::NEOVIM_FN_BUFFER_SET_LINE:
		{
			emit on_buffer_set_line();

		}
		break;
	case Function::NEOVIM_FN_BUFFER_DEL_LINE:
		{
			emit on_buffer_del_line();

		}
		break;
	case Function::NEOVIM_FN_BUFFER_GET_LINE_SLICE:
		{
			QList<QByteArray> data;
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for buffer_get_line_slice");
				return;
			} else {
				emit on_buffer_get_line_slice(data);
			}

		}
		break;
	case Function::NEOVIM_FN_BUFFER_SET_LINE_SLICE:
		{
			emit on_buffer_set_line_slice();

		}
		break;
	case Function::NEOVIM_FN_BUFFER_GET_VAR:
		{
			QVariant data;
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for buffer_get_var");
				return;
			} else {
				emit on_buffer_get_var(data);
			}

		}
		break;
	case Function::NEOVIM_FN_BUFFER_SET_VAR:
		{
			QVariant data;
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for buffer_set_var");
				return;
			} else {
				emit on_buffer_set_var(data);
			}

		}
		break;
	case Function::NEOVIM_FN_BUFFER_GET_OPTION:
		{
			QVariant data;
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for buffer_get_option");
				return;
			} else {
				emit on_buffer_get_option(data);
			}

		}
		break;
	case Function::NEOVIM_FN_BUFFER_SET_OPTION:
		{
			emit on_buffer_set_option();

		}
		break;
	case Function::NEOVIM_FN_BUFFER_GET_NUMBER:
		{
			int64_t data;
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for buffer_get_number");
				return;
			} else {
				emit on_buffer_get_number(data);
			}

		}
		break;
	case Function::NEOVIM_FN_BUFFER_GET_NAME:
		{
			QByteArray data;
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for buffer_get_name");
				return;
			} else {
				emit on_buffer_get_name(data);
			}

		}
		break;
	case Function::NEOVIM_FN_BUFFER_SET_NAME:
		{
			emit on_buffer_set_name();

		}
		break;
	case Function::NEOVIM_FN_BUFFER_IS_VALID:
		{
			bool data;
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for buffer_is_valid");
				return;
			} else {
				emit on_buffer_is_valid(data);
			}

		}
		break;
	case Function::NEOVIM_FN_BUFFER_INSERT:
		{
			emit on_buffer_insert();

		}
		break;
	case Function::NEOVIM_FN_BUFFER_GET_MARK:
		{
			QPoint data;
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for buffer_get_mark");
				return;
			} else {
				emit on_buffer_get_mark(data);
			}

		}
		break;
	case Function::NEOVIM_FN_WINDOW_GET_BUFFER:
		{
			int64_t data;
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for window_get_buffer");
				return;
			} else {
				emit on_window_get_buffer(data);
			}

		}
		break;
	case Function::NEOVIM_FN_WINDOW_GET_CURSOR:
		{
			QPoint data;
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for window_get_cursor");
				return;
			} else {
				emit on_window_get_cursor(data);
			}

		}
		break;
	case Function::NEOVIM_FN_WINDOW_SET_CURSOR:
		{
			emit on_window_set_cursor();

		}
		break;
	case Function::NEOVIM_FN_WINDOW_GET_HEIGHT:
		{
			int64_t data;
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for window_get_height");
				return;
			} else {
				emit on_window_get_height(data);
			}

		}
		break;
	case Function::NEOVIM_FN_WINDOW_SET_HEIGHT:
		{
			emit on_window_set_height();

		}
		break;
	case Function::NEOVIM_FN_WINDOW_GET_WIDTH:
		{
			int64_t data;
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for window_get_width");
				return;
			} else {
				emit on_window_get_width(data);
			}

		}
		break;
	case Function::NEOVIM_FN_WINDOW_SET_WIDTH:
		{
			emit on_window_set_width();

		}
		break;
	case Function::NEOVIM_FN_WINDOW_GET_VAR:
		{
			QVariant data;
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for window_get_var");
				return;
			} else {
				emit on_window_get_var(data);
			}

		}
		break;
	case Function::NEOVIM_FN_WINDOW_SET_VAR:
		{
			QVariant data;
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for window_set_var");
				return;
			} else {
				emit on_window_set_var(data);
			}

		}
		break;
	case Function::NEOVIM_FN_WINDOW_GET_OPTION:
		{
			QVariant data;
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for window_get_option");
				return;
			} else {
				emit on_window_get_option(data);
			}

		}
		break;
	case Function::NEOVIM_FN_WINDOW_SET_OPTION:
		{
			emit on_window_set_option();

		}
		break;
	case Function::NEOVIM_FN_WINDOW_GET_POSITION:
		{
			QPoint data;
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for window_get_position");
				return;
			} else {
				emit on_window_get_position(data);
			}

		}
		break;
	case Function::NEOVIM_FN_WINDOW_GET_TABPAGE:
		{
			int64_t data;
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for window_get_tabpage");
				return;
			} else {
				emit on_window_get_tabpage(data);
			}

		}
		break;
	case Function::NEOVIM_FN_WINDOW_IS_VALID:
		{
			bool data;
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for window_is_valid");
				return;
			} else {
				emit on_window_is_valid(data);
			}

		}
		break;
	case Function::NEOVIM_FN_VIM_COMMAND:
		{
			emit on_vim_command();

		}
		break;
	case Function::NEOVIM_FN_VIM_FEEDKEYS:
		{
			emit on_vim_feedkeys();

		}
		break;
	case Function::NEOVIM_FN_VIM_INPUT:
		{
			int64_t data;
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_input");
				return;
			} else {
				emit on_vim_input(data);
			}

		}
		break;
	case Function::NEOVIM_FN_VIM_REPLACE_TERMCODES:
		{
			QByteArray data;
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_replace_termcodes");
				return;
			} else {
				emit on_vim_replace_termcodes(data);
			}

		}
		break;
	case Function::NEOVIM_FN_VIM_COMMAND_OUTPUT:
		{
			QByteArray data;
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_command_output");
				return;
			} else {
				emit on_vim_command_output(data);
			}

		}
		break;
	case Function::NEOVIM_FN_VIM_EVAL:
		{
			QVariant data;
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_eval");
				return;
			} else {
				emit on_vim_eval(data);
			}

		}
		break;
	case Function::NEOVIM_FN_VIM_STRWIDTH:
		{
			int64_t data;
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_strwidth");
				return;
			} else {
				emit on_vim_strwidth(data);
			}

		}
		break;
	case Function::NEOVIM_FN_VIM_LIST_RUNTIME_PATHS:
		{
			QList<QByteArray> data;
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_list_runtime_paths");
				return;
			} else {
				emit on_vim_list_runtime_paths(data);
			}

		}
		break;
	case Function::NEOVIM_FN_VIM_CHANGE_DIRECTORY:
		{
			emit on_vim_change_directory();

		}
		break;
	case Function::NEOVIM_FN_VIM_GET_CURRENT_LINE:
		{
			QByteArray data;
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_get_current_line");
				return;
			} else {
				emit on_vim_get_current_line(data);
			}

		}
		break;
	case Function::NEOVIM_FN_VIM_SET_CURRENT_LINE:
		{
			emit on_vim_set_current_line();

		}
		break;
	case Function::NEOVIM_FN_VIM_DEL_CURRENT_LINE:
		{
			emit on_vim_del_current_line();

		}
		break;
	case Function::NEOVIM_FN_VIM_GET_VAR:
		{
			QVariant data;
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_get_var");
				return;
			} else {
				emit on_vim_get_var(data);
			}

		}
		break;
	case Function::NEOVIM_FN_VIM_SET_VAR:
		{
			QVariant data;
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_set_var");
				return;
			} else {
				emit on_vim_set_var(data);
			}

		}
		break;
	case Function::NEOVIM_FN_VIM_GET_VVAR:
		{
			QVariant data;
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_get_vvar");
				return;
			} else {
				emit on_vim_get_vvar(data);
			}

		}
		break;
	case Function::NEOVIM_FN_VIM_GET_OPTION:
		{
			QVariant data;
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_get_option");
				return;
			} else {
				emit on_vim_get_option(data);
			}

		}
		break;
	case Function::NEOVIM_FN_VIM_SET_OPTION:
		{
			emit on_vim_set_option();

		}
		break;
	case Function::NEOVIM_FN_VIM_OUT_WRITE:
		{
			emit on_vim_out_write();

		}
		break;
	case Function::NEOVIM_FN_VIM_ERR_WRITE:
		{
			emit on_vim_err_write();

		}
		break;
	case Function::NEOVIM_FN_VIM_REPORT_ERROR:
		{
			emit on_vim_report_error();

		}
		break;
	case Function::NEOVIM_FN_VIM_GET_BUFFERS:
		{
			QList<int64_t> data;
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_get_buffers");
				return;
			} else {
				emit on_vim_get_buffers(data);
			}

		}
		break;
	case Function::NEOVIM_FN_VIM_GET_CURRENT_BUFFER:
		{
			int64_t data;
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_get_current_buffer");
				return;
			} else {
				emit on_vim_get_current_buffer(data);
			}

		}
		break;
	case Function::NEOVIM_FN_VIM_SET_CURRENT_BUFFER:
		{
			emit on_vim_set_current_buffer();

		}
		break;
	case Function::NEOVIM_FN_VIM_GET_WINDOWS:
		{
			QList<int64_t> data;
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_get_windows");
				return;
			} else {
				emit on_vim_get_windows(data);
			}

		}
		break;
	case Function::NEOVIM_FN_VIM_GET_CURRENT_WINDOW:
		{
			int64_t data;
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_get_current_window");
				return;
			} else {
				emit on_vim_get_current_window(data);
			}

		}
		break;
	case Function::NEOVIM_FN_VIM_SET_CURRENT_WINDOW:
		{
			emit on_vim_set_current_window();

		}
		break;
	case Function::NEOVIM_FN_VIM_GET_TABPAGES:
		{
			QList<int64_t> data;
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_get_tabpages");
				return;
			} else {
				emit on_vim_get_tabpages(data);
			}

		}
		break;
	case Function::NEOVIM_FN_VIM_GET_CURRENT_TABPAGE:
		{
			int64_t data;
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_get_current_tabpage");
				return;
			} else {
				emit on_vim_get_current_tabpage(data);
			}

		}
		break;
	case Function::NEOVIM_FN_VIM_SET_CURRENT_TABPAGE:
		{
			emit on_vim_set_current_tabpage();

		}
		break;
	case Function::NEOVIM_FN_VIM_SUBSCRIBE:
		{
			emit on_vim_subscribe();

		}
		break;
	case Function::NEOVIM_FN_VIM_UNSUBSCRIBE:
		{
			emit on_vim_unsubscribe();

		}
		break;
	case Function::NEOVIM_FN_VIM_NAME_TO_COLOR:
		{
			int64_t data;
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for vim_name_to_color");
				return;
			} else {
				emit on_vim_name_to_color(data);
			}

		}
		break;
	default:
		qWarning() << "Received unexpected response";
	}
}

} // Namespace