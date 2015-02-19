// Auto generated 2015-02-19 10:53:09.027166
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
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bfd8560>>
	void buffer_line_count(int64_t buffer);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bfd8290>>
	void buffer_get_line(int64_t buffer, int64_t index);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bfd8950>>
	void buffer_set_line(int64_t buffer, int64_t index, QByteArray line);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bfda290>>
	void buffer_del_line(int64_t buffer, int64_t index);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bfd07e8>>
	void buffer_get_line_slice(int64_t buffer, int64_t start, int64_t end, bool include_start, bool include_end);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bfdb440>>
	void buffer_set_line_slice(int64_t buffer, int64_t start, int64_t end, bool include_start, bool include_end, QList<QByteArray> replacement);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bfdb680>>
	void buffer_get_var(int64_t buffer, QByteArray name);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bfdb7a0>>
	void buffer_set_var(int64_t buffer, QByteArray name, QVariant value);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bfdb908>>
	void buffer_get_option(int64_t buffer, QByteArray name);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bfdba28>>
	void buffer_set_option(int64_t buffer, QByteArray name, QVariant value);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bfdbb90>>
	void buffer_get_number(int64_t buffer);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bfdbc68>>
	void buffer_get_name(int64_t buffer);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bfdbd40>>
	void buffer_set_name(int64_t buffer, QByteArray name);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bfdbe60>>
	void buffer_is_valid(int64_t buffer);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bfdbf38>>
	void buffer_insert(int64_t buffer, int64_t lnum, QList<QByteArray> lines);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf750e0>>
	void buffer_get_mark(int64_t buffer, QByteArray name);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf75200>>
	void tabpage_get_windows(int64_t tabpage);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf752d8>>
	void tabpage_get_var(int64_t tabpage, QByteArray name);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf753f8>>
	void tabpage_set_var(int64_t tabpage, QByteArray name, QVariant value);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf75560>>
	void tabpage_get_window(int64_t tabpage);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf75638>>
	void tabpage_is_valid(int64_t tabpage);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf75710>>
	void window_get_buffer(int64_t window);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf757e8>>
	void window_get_cursor(int64_t window);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf758c0>>
	void window_set_cursor(int64_t window, QPoint pos);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf759e0>>
	void window_get_height(int64_t window);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf75b00>>
	void window_set_height(int64_t window, int64_t height);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf75c68>>
	void window_get_width(int64_t window);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf75d88>>
	void window_set_width(int64_t window, int64_t width);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf75f38>>
	void window_get_var(int64_t window, QByteArray name);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf790e0>>
	void window_set_var(int64_t window, QByteArray name, QVariant value);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf792d8>>
	void window_get_option(int64_t window, QByteArray name);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf79440>>
	void window_set_option(int64_t window, QByteArray name, QVariant value);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf79638>>
	void window_get_position(int64_t window);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf79758>>
	void window_get_tabpage(int64_t window);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf79878>>
	void window_is_valid(int64_t window);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf79998>>
	void vim_command(QByteArray str);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf79ab8>>
	void vim_feedkeys(QByteArray keys, QByteArray mode, bool escape_csi);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf79cb0>>
	void vim_input(QByteArray keys);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf79dd0>>
	void vim_replace_termcodes(QByteArray str, bool from_part, bool do_lt, bool special);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf7d098>>
	void vim_command_output(QByteArray str);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf7d1b8>>
	void vim_eval(QByteArray str);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf7d2d8>>
	void vim_strwidth(QByteArray str);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf7d3f8>>
	void vim_list_runtime_paths();
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf7d4d0>>
	void vim_change_directory(QByteArray dir);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf7d5f0>>
	void vim_get_current_line();
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf7d710>>
	void vim_set_current_line(QByteArray line);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf7d7a0>>
	void vim_del_current_line();
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf7d878>>
	void vim_get_var(QByteArray name);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf80a70>>
	void vim_set_var(QByteArray name, QVariant value);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf80c20>>
	void vim_get_vvar(QByteArray name);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf80cf8>>
	void vim_get_option(QByteArray name);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf80dd0>>
	void vim_set_option(QByteArray name, QVariant value);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf80ef0>>
	void vim_out_write(QByteArray str);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf80fc8>>
	void vim_err_write(QByteArray str);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf860e0>>
	void vim_report_error(QByteArray str);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf861b8>>
	void vim_get_buffers();
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf86248>>
	void vim_get_current_buffer();
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf862d8>>
	void vim_set_current_buffer(int64_t buffer);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf863b0>>
	void vim_get_windows();
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf86440>>
	void vim_get_current_window();
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf864d0>>
	void vim_set_current_window(int64_t window);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf865a8>>
	void vim_get_tabpages();
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf86638>>
	void vim_get_current_tabpage();
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf866c8>>
	void vim_set_current_tabpage(int64_t tabpage);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf867a0>>
	void vim_subscribe(QByteArray event);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf868c0>>
	void vim_unsubscribe(QByteArray event);
	// <bound method Function.signature of <__main__.Function instance at 0x7f1c0bf869e0>>
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