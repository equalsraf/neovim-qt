// Auto generated 2015-04-02 14:51:10.959077
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
	void error(const QString& errmsg, const QVariant& errObj);
	void neovimNotification(const QByteArray &name, const QVariantList& args);
private:
	NeovimConnector *m_c;
public slots:
	void ui_try_resize(int64_t width, int64_t height);

	// ArrayOf(Window) tabpage_get_windows(Tabpage tabpage, ) !fails
	void tabpage_get_windows(int64_t tabpage);
	// Object tabpage_get_var(Tabpage tabpage, String name, ) !fails
	void tabpage_get_var(int64_t tabpage, QByteArray name);
	// Object tabpage_set_var(Tabpage tabpage, String name, Object value, ) !fails
	void tabpage_set_var(int64_t tabpage, QByteArray name, QVariant value);
	// Window tabpage_get_window(Tabpage tabpage, ) !fails
	void tabpage_get_window(int64_t tabpage);
	// Boolean tabpage_is_valid(Tabpage tabpage, ) 
	void tabpage_is_valid(int64_t tabpage);
	// Integer buffer_line_count(Buffer buffer, ) !fails
	void buffer_line_count(int64_t buffer);
	// String buffer_get_line(Buffer buffer, Integer index, ) !fails
	void buffer_get_line(int64_t buffer, int64_t index);
	// void buffer_set_line(Buffer buffer, Integer index, String line, ) !fails
	void buffer_set_line(int64_t buffer, int64_t index, QByteArray line);
	// void buffer_del_line(Buffer buffer, Integer index, ) !fails
	void buffer_del_line(int64_t buffer, int64_t index);
	// ArrayOf(String) buffer_get_line_slice(Buffer buffer, Integer start, Integer end, Boolean include_start, Boolean include_end, ) !fails
	void buffer_get_line_slice(int64_t buffer, int64_t start, int64_t end, bool include_start, bool include_end);
	// void buffer_set_line_slice(Buffer buffer, Integer start, Integer end, Boolean include_start, Boolean include_end, ArrayOf(String) replacement, ) !fails
	void buffer_set_line_slice(int64_t buffer, int64_t start, int64_t end, bool include_start, bool include_end, QList<QByteArray> replacement);
	// Object buffer_get_var(Buffer buffer, String name, ) !fails
	void buffer_get_var(int64_t buffer, QByteArray name);
	// Object buffer_set_var(Buffer buffer, String name, Object value, ) !fails
	void buffer_set_var(int64_t buffer, QByteArray name, QVariant value);
	// Object buffer_get_option(Buffer buffer, String name, ) !fails
	void buffer_get_option(int64_t buffer, QByteArray name);
	// void buffer_set_option(Buffer buffer, String name, Object value, ) !fails
	void buffer_set_option(int64_t buffer, QByteArray name, QVariant value);
	// Integer buffer_get_number(Buffer buffer, ) !fails
	void buffer_get_number(int64_t buffer);
	// String buffer_get_name(Buffer buffer, ) !fails
	void buffer_get_name(int64_t buffer);
	// void buffer_set_name(Buffer buffer, String name, ) !fails
	void buffer_set_name(int64_t buffer, QByteArray name);
	// Boolean buffer_is_valid(Buffer buffer, ) 
	void buffer_is_valid(int64_t buffer);
	// void buffer_insert(Buffer buffer, Integer lnum, ArrayOf(String) lines, ) !fails
	void buffer_insert(int64_t buffer, int64_t lnum, QList<QByteArray> lines);
	// ArrayOf(Integer, 2) buffer_get_mark(Buffer buffer, String name, ) !fails
	void buffer_get_mark(int64_t buffer, QByteArray name);
	// Buffer window_get_buffer(Window window, ) !fails
	void window_get_buffer(int64_t window);
	// ArrayOf(Integer, 2) window_get_cursor(Window window, ) !fails
	void window_get_cursor(int64_t window);
	// void window_set_cursor(Window window, ArrayOf(Integer, 2) pos, ) !fails
	void window_set_cursor(int64_t window, QPoint pos);
	// Integer window_get_height(Window window, ) !fails
	void window_get_height(int64_t window);
	// void window_set_height(Window window, Integer height, ) !fails
	void window_set_height(int64_t window, int64_t height);
	// Integer window_get_width(Window window, ) !fails
	void window_get_width(int64_t window);
	// void window_set_width(Window window, Integer width, ) !fails
	void window_set_width(int64_t window, int64_t width);
	// Object window_get_var(Window window, String name, ) !fails
	void window_get_var(int64_t window, QByteArray name);
	// Object window_set_var(Window window, String name, Object value, ) !fails
	void window_set_var(int64_t window, QByteArray name, QVariant value);
	// Object window_get_option(Window window, String name, ) !fails
	void window_get_option(int64_t window, QByteArray name);
	// void window_set_option(Window window, String name, Object value, ) !fails
	void window_set_option(int64_t window, QByteArray name, QVariant value);
	// ArrayOf(Integer, 2) window_get_position(Window window, ) !fails
	void window_get_position(int64_t window);
	// Tabpage window_get_tabpage(Window window, ) !fails
	void window_get_tabpage(int64_t window);
	// Boolean window_is_valid(Window window, ) 
	void window_is_valid(int64_t window);
	// void vim_command(String str, ) !fails
	void vim_command(QByteArray str);
	// void vim_feedkeys(String keys, String mode, Boolean escape_csi, ) 
	void vim_feedkeys(QByteArray keys, QByteArray mode, bool escape_csi);
	// Integer vim_input(String keys, ) 
	void vim_input(QByteArray keys);
	// String vim_replace_termcodes(String str, Boolean from_part, Boolean do_lt, Boolean special, ) 
	void vim_replace_termcodes(QByteArray str, bool from_part, bool do_lt, bool special);
	// String vim_command_output(String str, ) !fails
	void vim_command_output(QByteArray str);
	// Object vim_eval(String str, ) !fails
	void vim_eval(QByteArray str);
	// Integer vim_strwidth(String str, ) !fails
	void vim_strwidth(QByteArray str);
	// ArrayOf(String) vim_list_runtime_paths() 
	void vim_list_runtime_paths();
	// void vim_change_directory(String dir, ) !fails
	void vim_change_directory(QByteArray dir);
	// String vim_get_current_line() !fails
	void vim_get_current_line();
	// void vim_set_current_line(String line, ) !fails
	void vim_set_current_line(QByteArray line);
	// void vim_del_current_line() !fails
	void vim_del_current_line();
	// Object vim_get_var(String name, ) !fails
	void vim_get_var(QByteArray name);
	// Object vim_set_var(String name, Object value, ) !fails
	void vim_set_var(QByteArray name, QVariant value);
	// Object vim_get_vvar(String name, ) !fails
	void vim_get_vvar(QByteArray name);
	// Object vim_get_option(String name, ) !fails
	void vim_get_option(QByteArray name);
	// void vim_set_option(String name, Object value, ) !fails
	void vim_set_option(QByteArray name, QVariant value);
	// void vim_out_write(String str, ) 
	void vim_out_write(QByteArray str);
	// void vim_err_write(String str, ) 
	void vim_err_write(QByteArray str);
	// void vim_report_error(String str, ) 
	void vim_report_error(QByteArray str);
	// ArrayOf(Buffer) vim_get_buffers() 
	void vim_get_buffers();
	// Buffer vim_get_current_buffer() 
	void vim_get_current_buffer();
	// void vim_set_current_buffer(Buffer buffer, ) !fails
	void vim_set_current_buffer(int64_t buffer);
	// ArrayOf(Window) vim_get_windows() 
	void vim_get_windows();
	// Window vim_get_current_window() 
	void vim_get_current_window();
	// void vim_set_current_window(Window window, ) !fails
	void vim_set_current_window(int64_t window);
	// ArrayOf(Tabpage) vim_get_tabpages() 
	void vim_get_tabpages();
	// Tabpage vim_get_current_tabpage() 
	void vim_get_current_tabpage();
	// void vim_set_current_tabpage(Tabpage tabpage, ) !fails
	void vim_set_current_tabpage(int64_t tabpage);
	// void vim_subscribe(String event, ) 
	void vim_subscribe(QByteArray event);
	// void vim_unsubscribe(String event, ) 
	void vim_unsubscribe(QByteArray event);
	// Integer vim_name_to_color(String name, ) 
	void vim_name_to_color(QByteArray name);

signals:
	void on_ui_try_resize();

	void on_tabpage_get_windows(QList<int64_t>);
	void err_tabpage_get_windows(const QString&, const QVariant&);

	void on_tabpage_get_var(QVariant);
	void err_tabpage_get_var(const QString&, const QVariant&);

	void on_tabpage_set_var(QVariant);
	void err_tabpage_set_var(const QString&, const QVariant&);

	void on_tabpage_get_window(int64_t);
	void err_tabpage_get_window(const QString&, const QVariant&);

	void on_tabpage_is_valid(bool);

	void on_buffer_line_count(int64_t);
	void err_buffer_line_count(const QString&, const QVariant&);

	void on_buffer_get_line(QByteArray);
	void err_buffer_get_line(const QString&, const QVariant&);

	void on_buffer_set_line(void);
	void err_buffer_set_line(const QString&, const QVariant&);

	void on_buffer_del_line(void);
	void err_buffer_del_line(const QString&, const QVariant&);

	void on_buffer_get_line_slice(QList<QByteArray>);
	void err_buffer_get_line_slice(const QString&, const QVariant&);

	void on_buffer_set_line_slice(void);
	void err_buffer_set_line_slice(const QString&, const QVariant&);

	void on_buffer_get_var(QVariant);
	void err_buffer_get_var(const QString&, const QVariant&);

	void on_buffer_set_var(QVariant);
	void err_buffer_set_var(const QString&, const QVariant&);

	void on_buffer_get_option(QVariant);
	void err_buffer_get_option(const QString&, const QVariant&);

	void on_buffer_set_option(void);
	void err_buffer_set_option(const QString&, const QVariant&);

	void on_buffer_get_number(int64_t);
	void err_buffer_get_number(const QString&, const QVariant&);

	void on_buffer_get_name(QByteArray);
	void err_buffer_get_name(const QString&, const QVariant&);

	void on_buffer_set_name(void);
	void err_buffer_set_name(const QString&, const QVariant&);

	void on_buffer_is_valid(bool);

	void on_buffer_insert(void);
	void err_buffer_insert(const QString&, const QVariant&);

	void on_buffer_get_mark(QPoint);
	void err_buffer_get_mark(const QString&, const QVariant&);

	void on_window_get_buffer(int64_t);
	void err_window_get_buffer(const QString&, const QVariant&);

	void on_window_get_cursor(QPoint);
	void err_window_get_cursor(const QString&, const QVariant&);

	void on_window_set_cursor(void);
	void err_window_set_cursor(const QString&, const QVariant&);

	void on_window_get_height(int64_t);
	void err_window_get_height(const QString&, const QVariant&);

	void on_window_set_height(void);
	void err_window_set_height(const QString&, const QVariant&);

	void on_window_get_width(int64_t);
	void err_window_get_width(const QString&, const QVariant&);

	void on_window_set_width(void);
	void err_window_set_width(const QString&, const QVariant&);

	void on_window_get_var(QVariant);
	void err_window_get_var(const QString&, const QVariant&);

	void on_window_set_var(QVariant);
	void err_window_set_var(const QString&, const QVariant&);

	void on_window_get_option(QVariant);
	void err_window_get_option(const QString&, const QVariant&);

	void on_window_set_option(void);
	void err_window_set_option(const QString&, const QVariant&);

	void on_window_get_position(QPoint);
	void err_window_get_position(const QString&, const QVariant&);

	void on_window_get_tabpage(int64_t);
	void err_window_get_tabpage(const QString&, const QVariant&);

	void on_window_is_valid(bool);

	void on_vim_command(void);
	void err_vim_command(const QString&, const QVariant&);

	void on_vim_feedkeys(void);

	void on_vim_input(int64_t);

	void on_vim_replace_termcodes(QByteArray);

	void on_vim_command_output(QByteArray);
	void err_vim_command_output(const QString&, const QVariant&);

	void on_vim_eval(QVariant);
	void err_vim_eval(const QString&, const QVariant&);

	void on_vim_strwidth(int64_t);
	void err_vim_strwidth(const QString&, const QVariant&);

	void on_vim_list_runtime_paths(QList<QByteArray>);

	void on_vim_change_directory(void);
	void err_vim_change_directory(const QString&, const QVariant&);

	void on_vim_get_current_line(QByteArray);
	void err_vim_get_current_line(const QString&, const QVariant&);

	void on_vim_set_current_line(void);
	void err_vim_set_current_line(const QString&, const QVariant&);

	void on_vim_del_current_line(void);
	void err_vim_del_current_line(const QString&, const QVariant&);

	void on_vim_get_var(QVariant);
	void err_vim_get_var(const QString&, const QVariant&);

	void on_vim_set_var(QVariant);
	void err_vim_set_var(const QString&, const QVariant&);

	void on_vim_get_vvar(QVariant);
	void err_vim_get_vvar(const QString&, const QVariant&);

	void on_vim_get_option(QVariant);
	void err_vim_get_option(const QString&, const QVariant&);

	void on_vim_set_option(void);
	void err_vim_set_option(const QString&, const QVariant&);

	void on_vim_out_write(void);

	void on_vim_err_write(void);

	void on_vim_report_error(void);

	void on_vim_get_buffers(QList<int64_t>);

	void on_vim_get_current_buffer(int64_t);

	void on_vim_set_current_buffer(void);
	void err_vim_set_current_buffer(const QString&, const QVariant&);

	void on_vim_get_windows(QList<int64_t>);

	void on_vim_get_current_window(int64_t);

	void on_vim_set_current_window(void);
	void err_vim_set_current_window(const QString&, const QVariant&);

	void on_vim_get_tabpages(QList<int64_t>);

	void on_vim_get_current_tabpage(int64_t);

	void on_vim_set_current_tabpage(void);
	void err_vim_set_current_tabpage(const QString&, const QVariant&);

	void on_vim_subscribe(void);

	void on_vim_unsubscribe(void);

	void on_vim_name_to_color(int64_t);

};
} // namespace
#endif