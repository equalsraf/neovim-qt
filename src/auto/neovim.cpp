// Auto generated Wed Oct 1 13:47:33 2014
#include "neovim.h"
#include "neovimconnector.h"
namespace NeovimQt {
Neovim::Neovim(NeovimConnector *c)
:m_c(c)
{
}

void Neovim::tabpage_get_windows(Tabpage tabpage)
{
	NeovimRequest *r = m_c->startRequestUnchecked("tabpage_get_windows", 1);
	r->setFunction(Function::NEOVIM_FN_TABPAGE_GET_WINDOWS);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(tabpage);
}

void Neovim::tabpage_get_var(Tabpage tabpage, String name)
{
	NeovimRequest *r = m_c->startRequestUnchecked("tabpage_get_var", 2);
	r->setFunction(Function::NEOVIM_FN_TABPAGE_GET_VAR);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(tabpage);
	m_c->send(name);
}

void Neovim::tabpage_set_var(Tabpage tabpage, String name, Object value)
{
	NeovimRequest *r = m_c->startRequestUnchecked("tabpage_set_var", 3);
	r->setFunction(Function::NEOVIM_FN_TABPAGE_SET_VAR);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(tabpage);
	m_c->send(name);
	m_c->send(value);
}

void Neovim::tabpage_get_window(Tabpage tabpage)
{
	NeovimRequest *r = m_c->startRequestUnchecked("tabpage_get_window", 1);
	r->setFunction(Function::NEOVIM_FN_TABPAGE_GET_WINDOW);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(tabpage);
}

void Neovim::tabpage_is_valid(Tabpage tabpage)
{
	NeovimRequest *r = m_c->startRequestUnchecked("tabpage_is_valid", 1);
	r->setFunction(Function::NEOVIM_FN_TABPAGE_IS_VALID);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(tabpage);
}

void Neovim::vim_push_keys(String str)
{
	NeovimRequest *r = m_c->startRequestUnchecked("vim_push_keys", 1);
	r->setFunction(Function::NEOVIM_FN_VIM_PUSH_KEYS);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(str);
}

void Neovim::vim_command(String str)
{
	NeovimRequest *r = m_c->startRequestUnchecked("vim_command", 1);
	r->setFunction(Function::NEOVIM_FN_VIM_COMMAND);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(str);
}

void Neovim::vim_feedkeys(String keys, String mode)
{
	NeovimRequest *r = m_c->startRequestUnchecked("vim_feedkeys", 2);
	r->setFunction(Function::NEOVIM_FN_VIM_FEEDKEYS);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(keys);
	m_c->send(mode);
}

void Neovim::vim_replace_termcodes(String str, Boolean from_part, Boolean do_lt, Boolean special)
{
	NeovimRequest *r = m_c->startRequestUnchecked("vim_replace_termcodes", 4);
	r->setFunction(Function::NEOVIM_FN_VIM_REPLACE_TERMCODES);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(str);
	m_c->send(from_part);
	m_c->send(do_lt);
	m_c->send(special);
}

void Neovim::vim_eval(String str)
{
	NeovimRequest *r = m_c->startRequestUnchecked("vim_eval", 1);
	r->setFunction(Function::NEOVIM_FN_VIM_EVAL);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(str);
}

void Neovim::vim_strwidth(String str)
{
	NeovimRequest *r = m_c->startRequestUnchecked("vim_strwidth", 1);
	r->setFunction(Function::NEOVIM_FN_VIM_STRWIDTH);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(str);
}

void Neovim::vim_list_runtime_paths()
{
	NeovimRequest *r = m_c->startRequestUnchecked("vim_list_runtime_paths", 0);
	r->setFunction(Function::NEOVIM_FN_VIM_LIST_RUNTIME_PATHS);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
}

void Neovim::vim_change_directory(String dir)
{
	NeovimRequest *r = m_c->startRequestUnchecked("vim_change_directory", 1);
	r->setFunction(Function::NEOVIM_FN_VIM_CHANGE_DIRECTORY);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(dir);
}

void Neovim::vim_get_current_line()
{
	NeovimRequest *r = m_c->startRequestUnchecked("vim_get_current_line", 0);
	r->setFunction(Function::NEOVIM_FN_VIM_GET_CURRENT_LINE);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
}

void Neovim::vim_set_current_line(String line)
{
	NeovimRequest *r = m_c->startRequestUnchecked("vim_set_current_line", 1);
	r->setFunction(Function::NEOVIM_FN_VIM_SET_CURRENT_LINE);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(line);
}

void Neovim::vim_del_current_line()
{
	NeovimRequest *r = m_c->startRequestUnchecked("vim_del_current_line", 0);
	r->setFunction(Function::NEOVIM_FN_VIM_DEL_CURRENT_LINE);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
}

void Neovim::vim_get_var(String name)
{
	NeovimRequest *r = m_c->startRequestUnchecked("vim_get_var", 1);
	r->setFunction(Function::NEOVIM_FN_VIM_GET_VAR);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(name);
}

void Neovim::vim_set_var(String name, Object value)
{
	NeovimRequest *r = m_c->startRequestUnchecked("vim_set_var", 2);
	r->setFunction(Function::NEOVIM_FN_VIM_SET_VAR);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(name);
	m_c->send(value);
}

void Neovim::vim_get_vvar(String name)
{
	NeovimRequest *r = m_c->startRequestUnchecked("vim_get_vvar", 1);
	r->setFunction(Function::NEOVIM_FN_VIM_GET_VVAR);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(name);
}

void Neovim::vim_get_option(String name)
{
	NeovimRequest *r = m_c->startRequestUnchecked("vim_get_option", 1);
	r->setFunction(Function::NEOVIM_FN_VIM_GET_OPTION);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(name);
}

void Neovim::vim_set_option(String name, Object value)
{
	NeovimRequest *r = m_c->startRequestUnchecked("vim_set_option", 2);
	r->setFunction(Function::NEOVIM_FN_VIM_SET_OPTION);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(name);
	m_c->send(value);
}

void Neovim::vim_out_write(String str)
{
	NeovimRequest *r = m_c->startRequestUnchecked("vim_out_write", 1);
	r->setFunction(Function::NEOVIM_FN_VIM_OUT_WRITE);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(str);
}

void Neovim::vim_err_write(String str)
{
	NeovimRequest *r = m_c->startRequestUnchecked("vim_err_write", 1);
	r->setFunction(Function::NEOVIM_FN_VIM_ERR_WRITE);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(str);
}

void Neovim::vim_report_error(String str)
{
	NeovimRequest *r = m_c->startRequestUnchecked("vim_report_error", 1);
	r->setFunction(Function::NEOVIM_FN_VIM_REPORT_ERROR);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(str);
}

void Neovim::vim_get_buffers()
{
	NeovimRequest *r = m_c->startRequestUnchecked("vim_get_buffers", 0);
	r->setFunction(Function::NEOVIM_FN_VIM_GET_BUFFERS);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
}

void Neovim::vim_get_current_buffer()
{
	NeovimRequest *r = m_c->startRequestUnchecked("vim_get_current_buffer", 0);
	r->setFunction(Function::NEOVIM_FN_VIM_GET_CURRENT_BUFFER);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
}

void Neovim::vim_set_current_buffer(Buffer buffer)
{
	NeovimRequest *r = m_c->startRequestUnchecked("vim_set_current_buffer", 1);
	r->setFunction(Function::NEOVIM_FN_VIM_SET_CURRENT_BUFFER);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(buffer);
}

void Neovim::vim_get_windows()
{
	NeovimRequest *r = m_c->startRequestUnchecked("vim_get_windows", 0);
	r->setFunction(Function::NEOVIM_FN_VIM_GET_WINDOWS);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
}

void Neovim::vim_get_current_window()
{
	NeovimRequest *r = m_c->startRequestUnchecked("vim_get_current_window", 0);
	r->setFunction(Function::NEOVIM_FN_VIM_GET_CURRENT_WINDOW);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
}

void Neovim::vim_set_current_window(Window window)
{
	NeovimRequest *r = m_c->startRequestUnchecked("vim_set_current_window", 1);
	r->setFunction(Function::NEOVIM_FN_VIM_SET_CURRENT_WINDOW);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(window);
}

void Neovim::vim_get_tabpages()
{
	NeovimRequest *r = m_c->startRequestUnchecked("vim_get_tabpages", 0);
	r->setFunction(Function::NEOVIM_FN_VIM_GET_TABPAGES);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
}

void Neovim::vim_get_current_tabpage()
{
	NeovimRequest *r = m_c->startRequestUnchecked("vim_get_current_tabpage", 0);
	r->setFunction(Function::NEOVIM_FN_VIM_GET_CURRENT_TABPAGE);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
}

void Neovim::vim_set_current_tabpage(Tabpage tabpage)
{
	NeovimRequest *r = m_c->startRequestUnchecked("vim_set_current_tabpage", 1);
	r->setFunction(Function::NEOVIM_FN_VIM_SET_CURRENT_TABPAGE);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(tabpage);
}

void Neovim::vim_subscribe(String event)
{
	NeovimRequest *r = m_c->startRequestUnchecked("vim_subscribe", 1);
	r->setFunction(Function::NEOVIM_FN_VIM_SUBSCRIBE);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(event);
}

void Neovim::vim_unsubscribe(String event)
{
	NeovimRequest *r = m_c->startRequestUnchecked("vim_unsubscribe", 1);
	r->setFunction(Function::NEOVIM_FN_VIM_UNSUBSCRIBE);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(event);
}

void Neovim::vim_register_provider(String feature)
{
	NeovimRequest *r = m_c->startRequestUnchecked("vim_register_provider", 1);
	r->setFunction(Function::NEOVIM_FN_VIM_REGISTER_PROVIDER);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(feature);
}

void Neovim::buffer_get_length(Buffer buffer)
{
	NeovimRequest *r = m_c->startRequestUnchecked("buffer_get_length", 1);
	r->setFunction(Function::NEOVIM_FN_BUFFER_GET_LENGTH);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(buffer);
}

void Neovim::buffer_get_line(Buffer buffer, Integer index)
{
	NeovimRequest *r = m_c->startRequestUnchecked("buffer_get_line", 2);
	r->setFunction(Function::NEOVIM_FN_BUFFER_GET_LINE);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(buffer);
	m_c->send(index);
}

void Neovim::buffer_set_line(Buffer buffer, Integer index, String line)
{
	NeovimRequest *r = m_c->startRequestUnchecked("buffer_set_line", 3);
	r->setFunction(Function::NEOVIM_FN_BUFFER_SET_LINE);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(buffer);
	m_c->send(index);
	m_c->send(line);
}

void Neovim::buffer_del_line(Buffer buffer, Integer index)
{
	NeovimRequest *r = m_c->startRequestUnchecked("buffer_del_line", 2);
	r->setFunction(Function::NEOVIM_FN_BUFFER_DEL_LINE);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(buffer);
	m_c->send(index);
}

void Neovim::buffer_get_slice(Buffer buffer, Integer start, Integer end, Boolean include_start, Boolean include_end)
{
	NeovimRequest *r = m_c->startRequestUnchecked("buffer_get_slice", 5);
	r->setFunction(Function::NEOVIM_FN_BUFFER_GET_SLICE);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(buffer);
	m_c->send(start);
	m_c->send(end);
	m_c->send(include_start);
	m_c->send(include_end);
}

void Neovim::buffer_set_slice(Buffer buffer, Integer start, Integer end, Boolean include_start, Boolean include_end, StringArray replacement)
{
	NeovimRequest *r = m_c->startRequestUnchecked("buffer_set_slice", 6);
	r->setFunction(Function::NEOVIM_FN_BUFFER_SET_SLICE);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(buffer);
	m_c->send(start);
	m_c->send(end);
	m_c->send(include_start);
	m_c->send(include_end);
	m_c->send(replacement);
}

void Neovim::buffer_get_var(Buffer buffer, String name)
{
	NeovimRequest *r = m_c->startRequestUnchecked("buffer_get_var", 2);
	r->setFunction(Function::NEOVIM_FN_BUFFER_GET_VAR);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(buffer);
	m_c->send(name);
}

void Neovim::buffer_set_var(Buffer buffer, String name, Object value)
{
	NeovimRequest *r = m_c->startRequestUnchecked("buffer_set_var", 3);
	r->setFunction(Function::NEOVIM_FN_BUFFER_SET_VAR);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(buffer);
	m_c->send(name);
	m_c->send(value);
}

void Neovim::buffer_get_option(Buffer buffer, String name)
{
	NeovimRequest *r = m_c->startRequestUnchecked("buffer_get_option", 2);
	r->setFunction(Function::NEOVIM_FN_BUFFER_GET_OPTION);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(buffer);
	m_c->send(name);
}

void Neovim::buffer_set_option(Buffer buffer, String name, Object value)
{
	NeovimRequest *r = m_c->startRequestUnchecked("buffer_set_option", 3);
	r->setFunction(Function::NEOVIM_FN_BUFFER_SET_OPTION);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(buffer);
	m_c->send(name);
	m_c->send(value);
}

void Neovim::buffer_get_number(Buffer buffer)
{
	NeovimRequest *r = m_c->startRequestUnchecked("buffer_get_number", 1);
	r->setFunction(Function::NEOVIM_FN_BUFFER_GET_NUMBER);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(buffer);
}

void Neovim::buffer_get_name(Buffer buffer)
{
	NeovimRequest *r = m_c->startRequestUnchecked("buffer_get_name", 1);
	r->setFunction(Function::NEOVIM_FN_BUFFER_GET_NAME);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(buffer);
}

void Neovim::buffer_set_name(Buffer buffer, String name)
{
	NeovimRequest *r = m_c->startRequestUnchecked("buffer_set_name", 2);
	r->setFunction(Function::NEOVIM_FN_BUFFER_SET_NAME);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(buffer);
	m_c->send(name);
}

void Neovim::buffer_is_valid(Buffer buffer)
{
	NeovimRequest *r = m_c->startRequestUnchecked("buffer_is_valid", 1);
	r->setFunction(Function::NEOVIM_FN_BUFFER_IS_VALID);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(buffer);
}

void Neovim::buffer_insert(Buffer buffer, Integer lnum, StringArray lines)
{
	NeovimRequest *r = m_c->startRequestUnchecked("buffer_insert", 3);
	r->setFunction(Function::NEOVIM_FN_BUFFER_INSERT);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(buffer);
	m_c->send(lnum);
	m_c->send(lines);
}

void Neovim::buffer_get_mark(Buffer buffer, String name)
{
	NeovimRequest *r = m_c->startRequestUnchecked("buffer_get_mark", 2);
	r->setFunction(Function::NEOVIM_FN_BUFFER_GET_MARK);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(buffer);
	m_c->send(name);
}

void Neovim::window_get_buffer(Window window)
{
	NeovimRequest *r = m_c->startRequestUnchecked("window_get_buffer", 1);
	r->setFunction(Function::NEOVIM_FN_WINDOW_GET_BUFFER);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(window);
}

void Neovim::window_get_cursor(Window window)
{
	NeovimRequest *r = m_c->startRequestUnchecked("window_get_cursor", 1);
	r->setFunction(Function::NEOVIM_FN_WINDOW_GET_CURSOR);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(window);
}

void Neovim::window_set_cursor(Window window, Position pos)
{
	NeovimRequest *r = m_c->startRequestUnchecked("window_set_cursor", 2);
	r->setFunction(Function::NEOVIM_FN_WINDOW_SET_CURSOR);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(window);
	m_c->send(pos);
}

void Neovim::window_get_height(Window window)
{
	NeovimRequest *r = m_c->startRequestUnchecked("window_get_height", 1);
	r->setFunction(Function::NEOVIM_FN_WINDOW_GET_HEIGHT);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(window);
}

void Neovim::window_set_height(Window window, Integer height)
{
	NeovimRequest *r = m_c->startRequestUnchecked("window_set_height", 2);
	r->setFunction(Function::NEOVIM_FN_WINDOW_SET_HEIGHT);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(window);
	m_c->send(height);
}

void Neovim::window_get_width(Window window)
{
	NeovimRequest *r = m_c->startRequestUnchecked("window_get_width", 1);
	r->setFunction(Function::NEOVIM_FN_WINDOW_GET_WIDTH);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(window);
}

void Neovim::window_set_width(Window window, Integer width)
{
	NeovimRequest *r = m_c->startRequestUnchecked("window_set_width", 2);
	r->setFunction(Function::NEOVIM_FN_WINDOW_SET_WIDTH);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(window);
	m_c->send(width);
}

void Neovim::window_get_var(Window window, String name)
{
	NeovimRequest *r = m_c->startRequestUnchecked("window_get_var", 2);
	r->setFunction(Function::NEOVIM_FN_WINDOW_GET_VAR);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(window);
	m_c->send(name);
}

void Neovim::window_set_var(Window window, String name, Object value)
{
	NeovimRequest *r = m_c->startRequestUnchecked("window_set_var", 3);
	r->setFunction(Function::NEOVIM_FN_WINDOW_SET_VAR);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(window);
	m_c->send(name);
	m_c->send(value);
}

void Neovim::window_get_option(Window window, String name)
{
	NeovimRequest *r = m_c->startRequestUnchecked("window_get_option", 2);
	r->setFunction(Function::NEOVIM_FN_WINDOW_GET_OPTION);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(window);
	m_c->send(name);
}

void Neovim::window_set_option(Window window, String name, Object value)
{
	NeovimRequest *r = m_c->startRequestUnchecked("window_set_option", 3);
	r->setFunction(Function::NEOVIM_FN_WINDOW_SET_OPTION);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(window);
	m_c->send(name);
	m_c->send(value);
}

void Neovim::window_get_position(Window window)
{
	NeovimRequest *r = m_c->startRequestUnchecked("window_get_position", 1);
	r->setFunction(Function::NEOVIM_FN_WINDOW_GET_POSITION);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(window);
}

void Neovim::window_get_tabpage(Window window)
{
	NeovimRequest *r = m_c->startRequestUnchecked("window_get_tabpage", 1);
	r->setFunction(Function::NEOVIM_FN_WINDOW_GET_TABPAGE);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(window);
}

void Neovim::window_is_valid(Window window)
{
	NeovimRequest *r = m_c->startRequestUnchecked("window_is_valid", 1);
	r->setFunction(Function::NEOVIM_FN_WINDOW_IS_VALID);
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
	m_c->send(window);
}

void Neovim::handleResponseError(uint32_t msgid, Function::FunctionId fun, const QString& msg, const msgpack_object& res)
{
	emit error(msg);
	qDebug() << msg;
	}
void Neovim::handleResponse(uint32_t msgid, Function::FunctionId fun, const msgpack_object& res)
{
	bool convfail=true;
	switch(fun) {
	case Function::NEOVIM_FN_TABPAGE_GET_WINDOWS:
		{
			WindowArray data = m_c->to_WindowArray(res, &convfail);
			if (convfail) {
				qWarning() << "Error unpacking data for signal tabpage_get_windows";
			} else {
				qDebug() << "tabpage_get_windows ->" << data;
				emit on_tabpage_get_windows(data);
			}
		}
		break;
	case Function::NEOVIM_FN_TABPAGE_GET_VAR:
		{
			Object data = m_c->to_Object(res, &convfail);
			if (convfail) {
				qWarning() << "Error unpacking data for signal tabpage_get_var";
			} else {
				qDebug() << "tabpage_get_var ->" << data;
				emit on_tabpage_get_var(data);
			}
		}
		break;
	case Function::NEOVIM_FN_TABPAGE_SET_VAR:
		{
			Object data = m_c->to_Object(res, &convfail);
			if (convfail) {
				qWarning() << "Error unpacking data for signal tabpage_set_var";
			} else {
				qDebug() << "tabpage_set_var ->" << data;
				emit on_tabpage_set_var(data);
			}
		}
		break;
	case Function::NEOVIM_FN_TABPAGE_GET_WINDOW:
		{
			Window data = m_c->to_Window(res, &convfail);
			if (convfail) {
				qWarning() << "Error unpacking data for signal tabpage_get_window";
			} else {
				qDebug() << "tabpage_get_window ->" << data;
				emit on_tabpage_get_window(data);
			}
		}
		break;
	case Function::NEOVIM_FN_TABPAGE_IS_VALID:
		{
			Boolean data = m_c->to_Boolean(res, &convfail);
			if (convfail) {
				qWarning() << "Error unpacking data for signal tabpage_is_valid";
			} else {
				qDebug() << "tabpage_is_valid ->" << data;
				emit on_tabpage_is_valid(data);
			}
		}
		break;
	case Function::NEOVIM_FN_VIM_PUSH_KEYS:
		{
			qDebug() << "on_vim_push_keys";
			emit on_vim_push_keys();
		}
		break;
	case Function::NEOVIM_FN_VIM_COMMAND:
		{
			qDebug() << "on_vim_command";
			emit on_vim_command();
		}
		break;
	case Function::NEOVIM_FN_VIM_FEEDKEYS:
		{
			qDebug() << "on_vim_feedkeys";
			emit on_vim_feedkeys();
		}
		break;
	case Function::NEOVIM_FN_VIM_REPLACE_TERMCODES:
		{
			String data = m_c->to_String(res, &convfail);
			if (convfail) {
				qWarning() << "Error unpacking data for signal vim_replace_termcodes";
			} else {
				qDebug() << "vim_replace_termcodes ->" << data;
				emit on_vim_replace_termcodes(data);
			}
		}
		break;
	case Function::NEOVIM_FN_VIM_EVAL:
		{
			Object data = m_c->to_Object(res, &convfail);
			if (convfail) {
				qWarning() << "Error unpacking data for signal vim_eval";
			} else {
				qDebug() << "vim_eval ->" << data;
				emit on_vim_eval(data);
			}
		}
		break;
	case Function::NEOVIM_FN_VIM_STRWIDTH:
		{
			Integer data = m_c->to_Integer(res, &convfail);
			if (convfail) {
				qWarning() << "Error unpacking data for signal vim_strwidth";
			} else {
				qDebug() << "vim_strwidth ->" << data;
				emit on_vim_strwidth(data);
			}
		}
		break;
	case Function::NEOVIM_FN_VIM_LIST_RUNTIME_PATHS:
		{
			StringArray data = m_c->to_StringArray(res, &convfail);
			if (convfail) {
				qWarning() << "Error unpacking data for signal vim_list_runtime_paths";
			} else {
				qDebug() << "vim_list_runtime_paths ->" << data;
				emit on_vim_list_runtime_paths(data);
			}
		}
		break;
	case Function::NEOVIM_FN_VIM_CHANGE_DIRECTORY:
		{
			qDebug() << "on_vim_change_directory";
			emit on_vim_change_directory();
		}
		break;
	case Function::NEOVIM_FN_VIM_GET_CURRENT_LINE:
		{
			String data = m_c->to_String(res, &convfail);
			if (convfail) {
				qWarning() << "Error unpacking data for signal vim_get_current_line";
			} else {
				qDebug() << "vim_get_current_line ->" << data;
				emit on_vim_get_current_line(data);
			}
		}
		break;
	case Function::NEOVIM_FN_VIM_SET_CURRENT_LINE:
		{
			qDebug() << "on_vim_set_current_line";
			emit on_vim_set_current_line();
		}
		break;
	case Function::NEOVIM_FN_VIM_DEL_CURRENT_LINE:
		{
			qDebug() << "on_vim_del_current_line";
			emit on_vim_del_current_line();
		}
		break;
	case Function::NEOVIM_FN_VIM_GET_VAR:
		{
			Object data = m_c->to_Object(res, &convfail);
			if (convfail) {
				qWarning() << "Error unpacking data for signal vim_get_var";
			} else {
				qDebug() << "vim_get_var ->" << data;
				emit on_vim_get_var(data);
			}
		}
		break;
	case Function::NEOVIM_FN_VIM_SET_VAR:
		{
			Object data = m_c->to_Object(res, &convfail);
			if (convfail) {
				qWarning() << "Error unpacking data for signal vim_set_var";
			} else {
				qDebug() << "vim_set_var ->" << data;
				emit on_vim_set_var(data);
			}
		}
		break;
	case Function::NEOVIM_FN_VIM_GET_VVAR:
		{
			Object data = m_c->to_Object(res, &convfail);
			if (convfail) {
				qWarning() << "Error unpacking data for signal vim_get_vvar";
			} else {
				qDebug() << "vim_get_vvar ->" << data;
				emit on_vim_get_vvar(data);
			}
		}
		break;
	case Function::NEOVIM_FN_VIM_GET_OPTION:
		{
			Object data = m_c->to_Object(res, &convfail);
			if (convfail) {
				qWarning() << "Error unpacking data for signal vim_get_option";
			} else {
				qDebug() << "vim_get_option ->" << data;
				emit on_vim_get_option(data);
			}
		}
		break;
	case Function::NEOVIM_FN_VIM_SET_OPTION:
		{
			qDebug() << "on_vim_set_option";
			emit on_vim_set_option();
		}
		break;
	case Function::NEOVIM_FN_VIM_OUT_WRITE:
		{
			qDebug() << "on_vim_out_write";
			emit on_vim_out_write();
		}
		break;
	case Function::NEOVIM_FN_VIM_ERR_WRITE:
		{
			qDebug() << "on_vim_err_write";
			emit on_vim_err_write();
		}
		break;
	case Function::NEOVIM_FN_VIM_REPORT_ERROR:
		{
			qDebug() << "on_vim_report_error";
			emit on_vim_report_error();
		}
		break;
	case Function::NEOVIM_FN_VIM_GET_BUFFERS:
		{
			BufferArray data = m_c->to_BufferArray(res, &convfail);
			if (convfail) {
				qWarning() << "Error unpacking data for signal vim_get_buffers";
			} else {
				qDebug() << "vim_get_buffers ->" << data;
				emit on_vim_get_buffers(data);
			}
		}
		break;
	case Function::NEOVIM_FN_VIM_GET_CURRENT_BUFFER:
		{
			Buffer data = m_c->to_Buffer(res, &convfail);
			if (convfail) {
				qWarning() << "Error unpacking data for signal vim_get_current_buffer";
			} else {
				qDebug() << "vim_get_current_buffer ->" << data;
				emit on_vim_get_current_buffer(data);
			}
		}
		break;
	case Function::NEOVIM_FN_VIM_SET_CURRENT_BUFFER:
		{
			qDebug() << "on_vim_set_current_buffer";
			emit on_vim_set_current_buffer();
		}
		break;
	case Function::NEOVIM_FN_VIM_GET_WINDOWS:
		{
			WindowArray data = m_c->to_WindowArray(res, &convfail);
			if (convfail) {
				qWarning() << "Error unpacking data for signal vim_get_windows";
			} else {
				qDebug() << "vim_get_windows ->" << data;
				emit on_vim_get_windows(data);
			}
		}
		break;
	case Function::NEOVIM_FN_VIM_GET_CURRENT_WINDOW:
		{
			Window data = m_c->to_Window(res, &convfail);
			if (convfail) {
				qWarning() << "Error unpacking data for signal vim_get_current_window";
			} else {
				qDebug() << "vim_get_current_window ->" << data;
				emit on_vim_get_current_window(data);
			}
		}
		break;
	case Function::NEOVIM_FN_VIM_SET_CURRENT_WINDOW:
		{
			qDebug() << "on_vim_set_current_window";
			emit on_vim_set_current_window();
		}
		break;
	case Function::NEOVIM_FN_VIM_GET_TABPAGES:
		{
			TabpageArray data = m_c->to_TabpageArray(res, &convfail);
			if (convfail) {
				qWarning() << "Error unpacking data for signal vim_get_tabpages";
			} else {
				qDebug() << "vim_get_tabpages ->" << data;
				emit on_vim_get_tabpages(data);
			}
		}
		break;
	case Function::NEOVIM_FN_VIM_GET_CURRENT_TABPAGE:
		{
			Tabpage data = m_c->to_Tabpage(res, &convfail);
			if (convfail) {
				qWarning() << "Error unpacking data for signal vim_get_current_tabpage";
			} else {
				qDebug() << "vim_get_current_tabpage ->" << data;
				emit on_vim_get_current_tabpage(data);
			}
		}
		break;
	case Function::NEOVIM_FN_VIM_SET_CURRENT_TABPAGE:
		{
			qDebug() << "on_vim_set_current_tabpage";
			emit on_vim_set_current_tabpage();
		}
		break;
	case Function::NEOVIM_FN_VIM_SUBSCRIBE:
		{
			qDebug() << "on_vim_subscribe";
			emit on_vim_subscribe();
		}
		break;
	case Function::NEOVIM_FN_VIM_UNSUBSCRIBE:
		{
			qDebug() << "on_vim_unsubscribe";
			emit on_vim_unsubscribe();
		}
		break;
	case Function::NEOVIM_FN_VIM_REGISTER_PROVIDER:
		{
			qDebug() << "on_vim_register_provider";
			emit on_vim_register_provider();
		}
		break;
	case Function::NEOVIM_FN_BUFFER_GET_LENGTH:
		{
			Integer data = m_c->to_Integer(res, &convfail);
			if (convfail) {
				qWarning() << "Error unpacking data for signal buffer_get_length";
			} else {
				qDebug() << "buffer_get_length ->" << data;
				emit on_buffer_get_length(data);
			}
		}
		break;
	case Function::NEOVIM_FN_BUFFER_GET_LINE:
		{
			String data = m_c->to_String(res, &convfail);
			if (convfail) {
				qWarning() << "Error unpacking data for signal buffer_get_line";
			} else {
				qDebug() << "buffer_get_line ->" << data;
				emit on_buffer_get_line(data);
			}
		}
		break;
	case Function::NEOVIM_FN_BUFFER_SET_LINE:
		{
			qDebug() << "on_buffer_set_line";
			emit on_buffer_set_line();
		}
		break;
	case Function::NEOVIM_FN_BUFFER_DEL_LINE:
		{
			qDebug() << "on_buffer_del_line";
			emit on_buffer_del_line();
		}
		break;
	case Function::NEOVIM_FN_BUFFER_GET_SLICE:
		{
			StringArray data = m_c->to_StringArray(res, &convfail);
			if (convfail) {
				qWarning() << "Error unpacking data for signal buffer_get_slice";
			} else {
				qDebug() << "buffer_get_slice ->" << data;
				emit on_buffer_get_slice(data);
			}
		}
		break;
	case Function::NEOVIM_FN_BUFFER_SET_SLICE:
		{
			qDebug() << "on_buffer_set_slice";
			emit on_buffer_set_slice();
		}
		break;
	case Function::NEOVIM_FN_BUFFER_GET_VAR:
		{
			Object data = m_c->to_Object(res, &convfail);
			if (convfail) {
				qWarning() << "Error unpacking data for signal buffer_get_var";
			} else {
				qDebug() << "buffer_get_var ->" << data;
				emit on_buffer_get_var(data);
			}
		}
		break;
	case Function::NEOVIM_FN_BUFFER_SET_VAR:
		{
			Object data = m_c->to_Object(res, &convfail);
			if (convfail) {
				qWarning() << "Error unpacking data for signal buffer_set_var";
			} else {
				qDebug() << "buffer_set_var ->" << data;
				emit on_buffer_set_var(data);
			}
		}
		break;
	case Function::NEOVIM_FN_BUFFER_GET_OPTION:
		{
			Object data = m_c->to_Object(res, &convfail);
			if (convfail) {
				qWarning() << "Error unpacking data for signal buffer_get_option";
			} else {
				qDebug() << "buffer_get_option ->" << data;
				emit on_buffer_get_option(data);
			}
		}
		break;
	case Function::NEOVIM_FN_BUFFER_SET_OPTION:
		{
			qDebug() << "on_buffer_set_option";
			emit on_buffer_set_option();
		}
		break;
	case Function::NEOVIM_FN_BUFFER_GET_NUMBER:
		{
			Integer data = m_c->to_Integer(res, &convfail);
			if (convfail) {
				qWarning() << "Error unpacking data for signal buffer_get_number";
			} else {
				qDebug() << "buffer_get_number ->" << data;
				emit on_buffer_get_number(data);
			}
		}
		break;
	case Function::NEOVIM_FN_BUFFER_GET_NAME:
		{
			String data = m_c->to_String(res, &convfail);
			if (convfail) {
				qWarning() << "Error unpacking data for signal buffer_get_name";
			} else {
				qDebug() << "buffer_get_name ->" << data;
				emit on_buffer_get_name(data);
			}
		}
		break;
	case Function::NEOVIM_FN_BUFFER_SET_NAME:
		{
			qDebug() << "on_buffer_set_name";
			emit on_buffer_set_name();
		}
		break;
	case Function::NEOVIM_FN_BUFFER_IS_VALID:
		{
			Boolean data = m_c->to_Boolean(res, &convfail);
			if (convfail) {
				qWarning() << "Error unpacking data for signal buffer_is_valid";
			} else {
				qDebug() << "buffer_is_valid ->" << data;
				emit on_buffer_is_valid(data);
			}
		}
		break;
	case Function::NEOVIM_FN_BUFFER_INSERT:
		{
			qDebug() << "on_buffer_insert";
			emit on_buffer_insert();
		}
		break;
	case Function::NEOVIM_FN_BUFFER_GET_MARK:
		{
			Position data = m_c->to_Position(res, &convfail);
			if (convfail) {
				qWarning() << "Error unpacking data for signal buffer_get_mark";
			} else {
				qDebug() << "buffer_get_mark ->" << data;
				emit on_buffer_get_mark(data);
			}
		}
		break;
	case Function::NEOVIM_FN_WINDOW_GET_BUFFER:
		{
			Buffer data = m_c->to_Buffer(res, &convfail);
			if (convfail) {
				qWarning() << "Error unpacking data for signal window_get_buffer";
			} else {
				qDebug() << "window_get_buffer ->" << data;
				emit on_window_get_buffer(data);
			}
		}
		break;
	case Function::NEOVIM_FN_WINDOW_GET_CURSOR:
		{
			Position data = m_c->to_Position(res, &convfail);
			if (convfail) {
				qWarning() << "Error unpacking data for signal window_get_cursor";
			} else {
				qDebug() << "window_get_cursor ->" << data;
				emit on_window_get_cursor(data);
			}
		}
		break;
	case Function::NEOVIM_FN_WINDOW_SET_CURSOR:
		{
			qDebug() << "on_window_set_cursor";
			emit on_window_set_cursor();
		}
		break;
	case Function::NEOVIM_FN_WINDOW_GET_HEIGHT:
		{
			Integer data = m_c->to_Integer(res, &convfail);
			if (convfail) {
				qWarning() << "Error unpacking data for signal window_get_height";
			} else {
				qDebug() << "window_get_height ->" << data;
				emit on_window_get_height(data);
			}
		}
		break;
	case Function::NEOVIM_FN_WINDOW_SET_HEIGHT:
		{
			qDebug() << "on_window_set_height";
			emit on_window_set_height();
		}
		break;
	case Function::NEOVIM_FN_WINDOW_GET_WIDTH:
		{
			Integer data = m_c->to_Integer(res, &convfail);
			if (convfail) {
				qWarning() << "Error unpacking data for signal window_get_width";
			} else {
				qDebug() << "window_get_width ->" << data;
				emit on_window_get_width(data);
			}
		}
		break;
	case Function::NEOVIM_FN_WINDOW_SET_WIDTH:
		{
			qDebug() << "on_window_set_width";
			emit on_window_set_width();
		}
		break;
	case Function::NEOVIM_FN_WINDOW_GET_VAR:
		{
			Object data = m_c->to_Object(res, &convfail);
			if (convfail) {
				qWarning() << "Error unpacking data for signal window_get_var";
			} else {
				qDebug() << "window_get_var ->" << data;
				emit on_window_get_var(data);
			}
		}
		break;
	case Function::NEOVIM_FN_WINDOW_SET_VAR:
		{
			Object data = m_c->to_Object(res, &convfail);
			if (convfail) {
				qWarning() << "Error unpacking data for signal window_set_var";
			} else {
				qDebug() << "window_set_var ->" << data;
				emit on_window_set_var(data);
			}
		}
		break;
	case Function::NEOVIM_FN_WINDOW_GET_OPTION:
		{
			Object data = m_c->to_Object(res, &convfail);
			if (convfail) {
				qWarning() << "Error unpacking data for signal window_get_option";
			} else {
				qDebug() << "window_get_option ->" << data;
				emit on_window_get_option(data);
			}
		}
		break;
	case Function::NEOVIM_FN_WINDOW_SET_OPTION:
		{
			qDebug() << "on_window_set_option";
			emit on_window_set_option();
		}
		break;
	case Function::NEOVIM_FN_WINDOW_GET_POSITION:
		{
			Position data = m_c->to_Position(res, &convfail);
			if (convfail) {
				qWarning() << "Error unpacking data for signal window_get_position";
			} else {
				qDebug() << "window_get_position ->" << data;
				emit on_window_get_position(data);
			}
		}
		break;
	case Function::NEOVIM_FN_WINDOW_GET_TABPAGE:
		{
			Tabpage data = m_c->to_Tabpage(res, &convfail);
			if (convfail) {
				qWarning() << "Error unpacking data for signal window_get_tabpage";
			} else {
				qDebug() << "window_get_tabpage ->" << data;
				emit on_window_get_tabpage(data);
			}
		}
		break;
	case Function::NEOVIM_FN_WINDOW_IS_VALID:
		{
			Boolean data = m_c->to_Boolean(res, &convfail);
			if (convfail) {
				qWarning() << "Error unpacking data for signal window_is_valid";
			} else {
				qDebug() << "window_is_valid ->" << data;
				emit on_window_is_valid(data);
			}
		}
		break;
	default:
		qWarning() << "Received unexpected response";
	}
}
} // namespace
