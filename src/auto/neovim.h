// Auto generated 2015-02-10 14:21:01.165297
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
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb1e5b00>>
	void window_get_buffer(int64_t window);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb1e5248>>
	void window_get_cursor(int64_t window);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb1e5758>>
	void window_set_cursor(int64_t window, QPoint pos);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb1e5d40>>
	void window_get_height(int64_t window);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb1e7f80>>
	void window_set_height(int64_t window, int64_t height);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb1debd8>>
	void window_get_width(int64_t window);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb180050>>
	void window_set_width(int64_t window, int64_t width);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb180200>>
	void window_get_var(int64_t window, QByteArray name);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb1803b0>>
	void window_set_var(int64_t window, QByteArray name, QVariant value);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb1805a8>>
	void window_get_option(int64_t window, QByteArray name);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb180758>>
	void window_set_option(int64_t window, QByteArray name, QVariant value);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb180998>>
	void window_get_position(int64_t window);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb180a28>>
	void window_get_tabpage(int64_t window);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb180b48>>
	void window_is_valid(int64_t window);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb180c68>>
	void tabpage_get_windows(int64_t tabpage);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb180d88>>
	void tabpage_get_var(int64_t tabpage, QByteArray name);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb180ef0>>
	void tabpage_set_var(int64_t tabpage, QByteArray name, QVariant value);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb1850e0>>
	void tabpage_get_window(int64_t tabpage);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb1851b8>>
	void tabpage_is_valid(int64_t tabpage);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb1852d8>>
	void vim_command(QByteArray str);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb1853b0>>
	void vim_feedkeys(QByteArray keys, QByteArray mode, bool escape_csi);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb185560>>
	void vim_input(QByteArray keys);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb185638>>
	void vim_replace_termcodes(QByteArray str, bool from_part, bool do_lt, bool special);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb185878>>
	void vim_command_output(QByteArray str);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb185950>>
	void vim_eval(QByteArray str);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb185a70>>
	void vim_strwidth(QByteArray str);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb185b90>>
	void vim_list_runtime_paths();
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb185c20>>
	void vim_change_directory(QByteArray dir);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb185d40>>
	void vim_get_current_line();
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb185dd0>>
	void vim_set_current_line(QByteArray line);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb185ef0>>
	void vim_del_current_line();
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb185fc8>>
	void vim_get_var(QByteArray name);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb18b128>>
	void vim_set_var(QByteArray name, QVariant value);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb18b290>>
	void vim_get_vvar(QByteArray name);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb18b368>>
	void vim_get_option(QByteArray name);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb18b488>>
	void vim_set_option(QByteArray name, QVariant value);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb1767a0>>
	void vim_out_write(QByteArray str);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb176878>>
	void vim_err_write(QByteArray str);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb176950>>
	void vim_report_error(QByteArray str);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb176a28>>
	void vim_get_buffers();
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb176ab8>>
	void vim_get_current_buffer();
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb176b48>>
	void vim_set_current_buffer(int64_t buffer);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb176c20>>
	void vim_get_windows();
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb176cb0>>
	void vim_get_current_window();
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb176d40>>
	void vim_set_current_window(int64_t window);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb176e18>>
	void vim_get_tabpages();
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb176ea8>>
	void vim_get_current_tabpage();
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb176f38>>
	void vim_set_current_tabpage(int64_t tabpage);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb17e050>>
	void vim_subscribe(QByteArray event);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb17e170>>
	void vim_unsubscribe(QByteArray event);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb17e290>>
	void vim_name_to_color(QByteArray name);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb17e368>>
	void buffer_line_count(int64_t buffer);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb17e440>>
	void buffer_get_line(int64_t buffer, int64_t index);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb17e560>>
	void buffer_set_line(int64_t buffer, int64_t index, QByteArray line);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb17e6c8>>
	void buffer_del_line(int64_t buffer, int64_t index);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb17e7e8>>
	void buffer_get_line_slice(int64_t buffer, int64_t start, int64_t end, bool include_start, bool include_end);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb17e9e0>>
	void buffer_set_line_slice(int64_t buffer, int64_t start, int64_t end, bool include_start, bool include_end, QList<QByteArray> replacement);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb17ec20>>
	void buffer_get_var(int64_t buffer, QByteArray name);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb17ed40>>
	void buffer_set_var(int64_t buffer, QByteArray name, QVariant value);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb17eea8>>
	void buffer_get_option(int64_t buffer, QByteArray name);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb17efc8>>
	void buffer_set_option(int64_t buffer, QByteArray name, QVariant value);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb18e170>>
	void buffer_get_number(int64_t buffer);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb18e248>>
	void buffer_get_name(int64_t buffer);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb18e320>>
	void buffer_set_name(int64_t buffer, QByteArray name);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb18e440>>
	void buffer_is_valid(int64_t buffer);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb18e518>>
	void buffer_insert(int64_t buffer, int64_t lnum, QList<QByteArray> lines);
	// <bound method Function.signature of <__main__.Function instance at 0x7f22fb18e680>>
	void buffer_get_mark(int64_t buffer, QByteArray name);

signals:
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

	void on_tabpage_get_windows(QList<int64_t>);
	void err_tabpage_get_windows(const QString&, const QVariant&);

	void on_tabpage_get_var(QVariant);
	void err_tabpage_get_var(const QString&, const QVariant&);

	void on_tabpage_set_var(QVariant);
	void err_tabpage_set_var(const QString&, const QVariant&);

	void on_tabpage_get_window(int64_t);
	void err_tabpage_get_window(const QString&, const QVariant&);

	void on_tabpage_is_valid(bool);

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

};
} // namespace
#endif