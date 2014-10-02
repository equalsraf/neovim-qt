// Auto generated 2014-10-02 13:43:11.577786
#ifndef NEOVIM_QT_NEOVIMOBJ
#define NEOVIM_QT_NEOVIMOBJ
#include "function.h"
#include <msgpack.h>
namespace NeovimQt {
class NeovimConnector;
class Neovim: public QObject
{

	Q_OBJECT
public:
	Neovim(NeovimConnector *);
protected slots:
	void handleResponse(uint32_t id, Function::FunctionId fun, const msgpack_object&);
	void handleResponseError(uint32_t id, Function::FunctionId fun, const QString& msg, const msgpack_object&);
signals:
	void error(const QString& errmsg);
private:
	NeovimConnector *m_c;
public slots:
	void tabpage_get_windows(int64_t tabpage);
	void tabpage_get_var(int64_t tabpage, QByteArray name);
	void tabpage_set_var(int64_t tabpage, QByteArray name, QVariant value);
	void tabpage_get_window(int64_t tabpage);
	void tabpage_is_valid(int64_t tabpage);
	void vim_push_keys(QByteArray str);
	void vim_command(QByteArray str);
	void vim_feedkeys(QByteArray keys, QByteArray mode);
	void vim_replace_termcodes(QByteArray str, bool from_part, bool do_lt, bool special);
	void vim_eval(QByteArray str);
	void vim_strwidth(QByteArray str);
	void vim_list_runtime_paths();
	void vim_change_directory(QByteArray dir);
	void vim_get_current_line();
	void vim_set_current_line(QByteArray line);
	void vim_del_current_line();
	void vim_get_var(QByteArray name);
	void vim_set_var(QByteArray name, QVariant value);
	void vim_get_vvar(QByteArray name);
	void vim_get_option(QByteArray name);
	void vim_set_option(QByteArray name, QVariant value);
	void vim_out_write(QByteArray str);
	void vim_err_write(QByteArray str);
	void vim_report_error(QByteArray str);
	void vim_get_buffers();
	void vim_get_current_buffer();
	void vim_set_current_buffer(int64_t buffer);
	void vim_get_windows();
	void vim_get_current_window();
	void vim_set_current_window(int64_t window);
	void vim_get_tabpages();
	void vim_get_current_tabpage();
	void vim_set_current_tabpage(int64_t tabpage);
	void vim_subscribe(QByteArray event);
	void vim_unsubscribe(QByteArray event);
	void vim_register_provider(QByteArray feature);
	void buffer_get_length(int64_t buffer);
	void buffer_get_line(int64_t buffer, int64_t index);
	void buffer_set_line(int64_t buffer, int64_t index, QByteArray line);
	void buffer_del_line(int64_t buffer, int64_t index);
	void buffer_get_slice(int64_t buffer, int64_t start, int64_t end, bool include_start, bool include_end);
	void buffer_set_slice(int64_t buffer, int64_t start, int64_t end, bool include_start, bool include_end, QList<QByteArray> replacement);
	void buffer_get_var(int64_t buffer, QByteArray name);
	void buffer_set_var(int64_t buffer, QByteArray name, QVariant value);
	void buffer_get_option(int64_t buffer, QByteArray name);
	void buffer_set_option(int64_t buffer, QByteArray name, QVariant value);
	void buffer_get_number(int64_t buffer);
	void buffer_get_name(int64_t buffer);
	void buffer_set_name(int64_t buffer, QByteArray name);
	void buffer_is_valid(int64_t buffer);
	void buffer_insert(int64_t buffer, int64_t lnum, QList<QByteArray> lines);
	void buffer_get_mark(int64_t buffer, QByteArray name);
	void window_get_buffer(int64_t window);
	void window_get_cursor(int64_t window);
	void window_set_cursor(int64_t window, QPoint pos);
	void window_get_height(int64_t window);
	void window_set_height(int64_t window, int64_t height);
	void window_get_width(int64_t window);
	void window_set_width(int64_t window, int64_t width);
	void window_get_var(int64_t window, QByteArray name);
	void window_set_var(int64_t window, QByteArray name, QVariant value);
	void window_get_option(int64_t window, QByteArray name);
	void window_set_option(int64_t window, QByteArray name, QVariant value);
	void window_get_position(int64_t window);
	void window_get_tabpage(int64_t window);
	void window_is_valid(int64_t window);

signals:
	void on_tabpage_get_windows(QList<int64_t>);
	void on_tabpage_get_var(QVariant);
	void on_tabpage_set_var(QVariant);
	void on_tabpage_get_window(int64_t);
	void on_tabpage_is_valid(bool);
	void on_vim_push_keys(void);
	void on_vim_command(void);
	void on_vim_feedkeys(void);
	void on_vim_replace_termcodes(QByteArray);
	void on_vim_eval(QVariant);
	void on_vim_strwidth(int64_t);
	void on_vim_list_runtime_paths(QList<QByteArray>);
	void on_vim_change_directory(void);
	void on_vim_get_current_line(QByteArray);
	void on_vim_set_current_line(void);
	void on_vim_del_current_line(void);
	void on_vim_get_var(QVariant);
	void on_vim_set_var(QVariant);
	void on_vim_get_vvar(QVariant);
	void on_vim_get_option(QVariant);
	void on_vim_set_option(void);
	void on_vim_out_write(void);
	void on_vim_err_write(void);
	void on_vim_report_error(void);
	void on_vim_get_buffers(QList<int64_t>);
	void on_vim_get_current_buffer(int64_t);
	void on_vim_set_current_buffer(void);
	void on_vim_get_windows(QList<int64_t>);
	void on_vim_get_current_window(int64_t);
	void on_vim_set_current_window(void);
	void on_vim_get_tabpages(QList<int64_t>);
	void on_vim_get_current_tabpage(int64_t);
	void on_vim_set_current_tabpage(void);
	void on_vim_subscribe(void);
	void on_vim_unsubscribe(void);
	void on_vim_register_provider(void);
	void on_buffer_get_length(int64_t);
	void on_buffer_get_line(QByteArray);
	void on_buffer_set_line(void);
	void on_buffer_del_line(void);
	void on_buffer_get_slice(QList<QByteArray>);
	void on_buffer_set_slice(void);
	void on_buffer_get_var(QVariant);
	void on_buffer_set_var(QVariant);
	void on_buffer_get_option(QVariant);
	void on_buffer_set_option(void);
	void on_buffer_get_number(int64_t);
	void on_buffer_get_name(QByteArray);
	void on_buffer_set_name(void);
	void on_buffer_is_valid(bool);
	void on_buffer_insert(void);
	void on_buffer_get_mark(QPoint);
	void on_window_get_buffer(int64_t);
	void on_window_get_cursor(QPoint);
	void on_window_set_cursor(void);
	void on_window_get_height(int64_t);
	void on_window_set_height(void);
	void on_window_get_width(int64_t);
	void on_window_set_width(void);
	void on_window_get_var(QVariant);
	void on_window_set_var(QVariant);
	void on_window_get_option(QVariant);
	void on_window_set_option(void);
	void on_window_get_position(QPoint);
	void on_window_get_tabpage(int64_t);
	void on_window_is_valid(bool);
};
} // namespace
#endif