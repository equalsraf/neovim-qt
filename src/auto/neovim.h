// Auto generated 2015-02-09 15:59:32.229744
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
	void buffer_line_count(int64_t buffer);
	void buffer_get_line(int64_t buffer, int64_t index);
	void buffer_set_line(int64_t buffer, int64_t index, QByteArray line);
	void buffer_del_line(int64_t buffer, int64_t index);
	void buffer_get_line_slice(int64_t buffer, int64_t start, int64_t end, bool include_start, bool include_end);
	void buffer_set_line_slice(int64_t buffer, int64_t start, int64_t end, bool include_start, bool include_end, QList<QByteArray> replacement);
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
	void tabpage_get_windows(int64_t tabpage);
	void tabpage_get_var(int64_t tabpage, QByteArray name);
	void tabpage_set_var(int64_t tabpage, QByteArray name, QVariant value);
	void tabpage_get_window(int64_t tabpage);
	void tabpage_is_valid(int64_t tabpage);
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
	void vim_command(QByteArray str);
	void vim_feedkeys(QByteArray keys, QByteArray mode, bool escape_csi);
	void vim_input(QByteArray keys);
	void vim_replace_termcodes(QByteArray str, bool from_part, bool do_lt, bool special);
	void vim_command_output(QByteArray str);
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
	void vim_name_to_color(QByteArray name);

signals:
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

	void on_tabpage_get_windows(QList<int64_t>);
	void err_tabpage_get_windows(const QString&, const QVariant&);

	void on_tabpage_get_var(QVariant);
	void err_tabpage_get_var(const QString&, const QVariant&);

	void on_tabpage_set_var(QVariant);
	void err_tabpage_set_var(const QString&, const QVariant&);

	void on_tabpage_get_window(int64_t);
	void err_tabpage_get_window(const QString&, const QVariant&);

	void on_tabpage_is_valid(bool);

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