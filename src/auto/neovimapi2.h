// Auto generated 2017-10-31 14:35:34.288640 from nvim API level:2
#ifndef NEOVIM_QT_NEOVIMAPI2
#define NEOVIM_QT_NEOVIMAPI2
#include "msgpack.h"
#include <QObject>
#include <QVariant>
#include <QPoint>
#include "function.h"

namespace NeovimQt {
class NeovimConnector;
class MsgpackRequest;

class NeovimApi2: public QObject
{
	Q_OBJECT
	Q_ENUMS(FunctionId)

public:

	enum FunctionId {
		NEOVIM_FN_NULL=0,
				NEOVIM_FN_NVIM_BUF_LINE_COUNT,
				NEOVIM_FN_BUFFER_GET_LINE,
				NEOVIM_FN_BUFFER_SET_LINE,
				NEOVIM_FN_BUFFER_DEL_LINE,
				NEOVIM_FN_BUFFER_GET_LINE_SLICE,
				NEOVIM_FN_NVIM_BUF_GET_LINES,
				NEOVIM_FN_BUFFER_SET_LINE_SLICE,
				NEOVIM_FN_NVIM_BUF_SET_LINES,
				NEOVIM_FN_NVIM_BUF_GET_VAR,
				NEOVIM_FN_NVIM_BUF_GET_CHANGEDTICK,
				NEOVIM_FN_NVIM_BUF_SET_VAR,
				NEOVIM_FN_NVIM_BUF_DEL_VAR,
				NEOVIM_FN_BUFFER_SET_VAR,
				NEOVIM_FN_BUFFER_DEL_VAR,
				NEOVIM_FN_NVIM_BUF_GET_OPTION,
				NEOVIM_FN_NVIM_BUF_SET_OPTION,
				NEOVIM_FN_NVIM_BUF_GET_NUMBER,
				NEOVIM_FN_NVIM_BUF_GET_NAME,
				NEOVIM_FN_NVIM_BUF_SET_NAME,
				NEOVIM_FN_NVIM_BUF_IS_VALID,
				NEOVIM_FN_BUFFER_INSERT,
				NEOVIM_FN_NVIM_BUF_GET_MARK,
				NEOVIM_FN_NVIM_BUF_ADD_HIGHLIGHT,
				NEOVIM_FN_NVIM_BUF_CLEAR_HIGHLIGHT,
				NEOVIM_FN_NVIM_TABPAGE_LIST_WINS,
				NEOVIM_FN_NVIM_TABPAGE_GET_VAR,
				NEOVIM_FN_NVIM_TABPAGE_SET_VAR,
				NEOVIM_FN_NVIM_TABPAGE_DEL_VAR,
				NEOVIM_FN_TABPAGE_SET_VAR,
				NEOVIM_FN_TABPAGE_DEL_VAR,
				NEOVIM_FN_NVIM_TABPAGE_GET_WIN,
				NEOVIM_FN_NVIM_TABPAGE_GET_NUMBER,
				NEOVIM_FN_NVIM_TABPAGE_IS_VALID,
				NEOVIM_FN_NVIM_UI_ATTACH,
				NEOVIM_FN_UI_ATTACH,
				NEOVIM_FN_NVIM_UI_DETACH,
				NEOVIM_FN_NVIM_UI_TRY_RESIZE,
				NEOVIM_FN_NVIM_UI_SET_OPTION,
				NEOVIM_FN_NVIM_COMMAND,
				NEOVIM_FN_NVIM_FEEDKEYS,
				NEOVIM_FN_NVIM_INPUT,
				NEOVIM_FN_NVIM_REPLACE_TERMCODES,
				NEOVIM_FN_NVIM_COMMAND_OUTPUT,
				NEOVIM_FN_NVIM_EVAL,
				NEOVIM_FN_NVIM_CALL_FUNCTION,
				NEOVIM_FN_NVIM_STRWIDTH,
				NEOVIM_FN_NVIM_LIST_RUNTIME_PATHS,
				NEOVIM_FN_NVIM_SET_CURRENT_DIR,
				NEOVIM_FN_NVIM_GET_CURRENT_LINE,
				NEOVIM_FN_NVIM_SET_CURRENT_LINE,
				NEOVIM_FN_NVIM_DEL_CURRENT_LINE,
				NEOVIM_FN_NVIM_GET_VAR,
				NEOVIM_FN_NVIM_SET_VAR,
				NEOVIM_FN_NVIM_DEL_VAR,
				NEOVIM_FN_VIM_SET_VAR,
				NEOVIM_FN_VIM_DEL_VAR,
				NEOVIM_FN_NVIM_GET_VVAR,
				NEOVIM_FN_NVIM_GET_OPTION,
				NEOVIM_FN_NVIM_SET_OPTION,
				NEOVIM_FN_NVIM_OUT_WRITE,
				NEOVIM_FN_NVIM_ERR_WRITE,
				NEOVIM_FN_NVIM_ERR_WRITELN,
				NEOVIM_FN_NVIM_LIST_BUFS,
				NEOVIM_FN_NVIM_GET_CURRENT_BUF,
				NEOVIM_FN_NVIM_SET_CURRENT_BUF,
				NEOVIM_FN_NVIM_LIST_WINS,
				NEOVIM_FN_NVIM_GET_CURRENT_WIN,
				NEOVIM_FN_NVIM_SET_CURRENT_WIN,
				NEOVIM_FN_NVIM_LIST_TABPAGES,
				NEOVIM_FN_NVIM_GET_CURRENT_TABPAGE,
				NEOVIM_FN_NVIM_SET_CURRENT_TABPAGE,
				NEOVIM_FN_NVIM_SUBSCRIBE,
				NEOVIM_FN_NVIM_UNSUBSCRIBE,
				NEOVIM_FN_NVIM_GET_COLOR_BY_NAME,
				NEOVIM_FN_NVIM_GET_COLOR_MAP,
				NEOVIM_FN_NVIM_GET_MODE,
				NEOVIM_FN_NVIM_GET_API_INFO,
				NEOVIM_FN_NVIM_CALL_ATOMIC,
				NEOVIM_FN_NVIM_WIN_GET_BUF,
				NEOVIM_FN_NVIM_WIN_GET_CURSOR,
				NEOVIM_FN_NVIM_WIN_SET_CURSOR,
				NEOVIM_FN_NVIM_WIN_GET_HEIGHT,
				NEOVIM_FN_NVIM_WIN_SET_HEIGHT,
				NEOVIM_FN_NVIM_WIN_GET_WIDTH,
				NEOVIM_FN_NVIM_WIN_SET_WIDTH,
				NEOVIM_FN_NVIM_WIN_GET_VAR,
				NEOVIM_FN_NVIM_WIN_SET_VAR,
				NEOVIM_FN_NVIM_WIN_DEL_VAR,
				NEOVIM_FN_WINDOW_SET_VAR,
				NEOVIM_FN_WINDOW_DEL_VAR,
				NEOVIM_FN_NVIM_WIN_GET_OPTION,
				NEOVIM_FN_NVIM_WIN_SET_OPTION,
				NEOVIM_FN_NVIM_WIN_GET_POSITION,
				NEOVIM_FN_NVIM_WIN_GET_TABPAGE,
				NEOVIM_FN_NVIM_WIN_GET_NUMBER,
				NEOVIM_FN_NVIM_WIN_IS_VALID,
				NEOVIM_FN_BUFFER_LINE_COUNT,
				NEOVIM_FN_BUFFER_GET_LINES,
				NEOVIM_FN_BUFFER_SET_LINES,
				NEOVIM_FN_BUFFER_GET_VAR,
				NEOVIM_FN_BUFFER_GET_OPTION,
				NEOVIM_FN_BUFFER_SET_OPTION,
				NEOVIM_FN_BUFFER_GET_NUMBER,
				NEOVIM_FN_BUFFER_GET_NAME,
				NEOVIM_FN_BUFFER_SET_NAME,
				NEOVIM_FN_BUFFER_IS_VALID,
				NEOVIM_FN_BUFFER_GET_MARK,
				NEOVIM_FN_BUFFER_ADD_HIGHLIGHT,
				NEOVIM_FN_BUFFER_CLEAR_HIGHLIGHT,
				NEOVIM_FN_TABPAGE_GET_WINDOWS,
				NEOVIM_FN_TABPAGE_GET_VAR,
				NEOVIM_FN_TABPAGE_GET_WINDOW,
				NEOVIM_FN_TABPAGE_IS_VALID,
				NEOVIM_FN_UI_DETACH,
				NEOVIM_FN_UI_TRY_RESIZE,
				NEOVIM_FN_VIM_COMMAND,
				NEOVIM_FN_VIM_FEEDKEYS,
				NEOVIM_FN_VIM_INPUT,
				NEOVIM_FN_VIM_REPLACE_TERMCODES,
				NEOVIM_FN_VIM_COMMAND_OUTPUT,
				NEOVIM_FN_VIM_EVAL,
				NEOVIM_FN_VIM_CALL_FUNCTION,
				NEOVIM_FN_VIM_STRWIDTH,
				NEOVIM_FN_VIM_LIST_RUNTIME_PATHS,
				NEOVIM_FN_VIM_CHANGE_DIRECTORY,
				NEOVIM_FN_VIM_GET_CURRENT_LINE,
				NEOVIM_FN_VIM_SET_CURRENT_LINE,
				NEOVIM_FN_VIM_DEL_CURRENT_LINE,
				NEOVIM_FN_VIM_GET_VAR,
				NEOVIM_FN_VIM_GET_VVAR,
				NEOVIM_FN_VIM_GET_OPTION,
				NEOVIM_FN_VIM_SET_OPTION,
				NEOVIM_FN_VIM_OUT_WRITE,
				NEOVIM_FN_VIM_ERR_WRITE,
				NEOVIM_FN_VIM_REPORT_ERROR,
				NEOVIM_FN_VIM_GET_BUFFERS,
				NEOVIM_FN_VIM_GET_CURRENT_BUFFER,
				NEOVIM_FN_VIM_SET_CURRENT_BUFFER,
				NEOVIM_FN_VIM_GET_WINDOWS,
				NEOVIM_FN_VIM_GET_CURRENT_WINDOW,
				NEOVIM_FN_VIM_SET_CURRENT_WINDOW,
				NEOVIM_FN_VIM_GET_TABPAGES,
				NEOVIM_FN_VIM_GET_CURRENT_TABPAGE,
				NEOVIM_FN_VIM_SET_CURRENT_TABPAGE,
				NEOVIM_FN_VIM_SUBSCRIBE,
				NEOVIM_FN_VIM_UNSUBSCRIBE,
				NEOVIM_FN_VIM_NAME_TO_COLOR,
				NEOVIM_FN_VIM_GET_COLOR_MAP,
				NEOVIM_FN_WINDOW_GET_BUFFER,
				NEOVIM_FN_WINDOW_GET_CURSOR,
				NEOVIM_FN_WINDOW_SET_CURSOR,
				NEOVIM_FN_WINDOW_GET_HEIGHT,
				NEOVIM_FN_WINDOW_SET_HEIGHT,
				NEOVIM_FN_WINDOW_GET_WIDTH,
				NEOVIM_FN_WINDOW_SET_WIDTH,
				NEOVIM_FN_WINDOW_GET_VAR,
				NEOVIM_FN_WINDOW_GET_OPTION,
				NEOVIM_FN_WINDOW_SET_OPTION,
				NEOVIM_FN_WINDOW_GET_POSITION,
				NEOVIM_FN_WINDOW_GET_TABPAGE,
				NEOVIM_FN_WINDOW_IS_VALID,
			};

	static bool checkFunctions(const QVariantList& ftable);
	static FunctionId functionId(const Function& f);

	NeovimApi2(NeovimConnector *);
protected slots:
	void handleResponse(quint32 id, quint64 fun, const QVariant&);
	void handleResponseError(quint32 id, quint64 fun, const QVariant&);
signals:
	void error(const QString& errmsg, const QVariant& errObj);
	void neovimNotification(const QByteArray &name, const QVariantList& args);
private:
	NeovimConnector *m_c;
public slots:
	// Integer nvim_buf_line_count(Buffer buffer, ) 
	MsgpackRequest* nvim_buf_line_count(int64_t buffer);
	// DEPRECATED
	// String buffer_get_line(Buffer buffer, Integer index, ) 
	MsgpackRequest* buffer_get_line(int64_t buffer, int64_t index);
	// DEPRECATED
	// void buffer_set_line(Buffer buffer, Integer index, String line, ) 
	MsgpackRequest* buffer_set_line(int64_t buffer, int64_t index, QByteArray line);
	// DEPRECATED
	// void buffer_del_line(Buffer buffer, Integer index, ) 
	MsgpackRequest* buffer_del_line(int64_t buffer, int64_t index);
	// DEPRECATED
	// ArrayOf(String) buffer_get_line_slice(Buffer buffer, Integer start, Integer end, Boolean include_start, Boolean include_end, ) 
	MsgpackRequest* buffer_get_line_slice(int64_t buffer, int64_t start, int64_t end, bool include_start, bool include_end);
	// ArrayOf(String) nvim_buf_get_lines(Buffer buffer, Integer start, Integer end, Boolean strict_indexing, ) 
	MsgpackRequest* nvim_buf_get_lines(int64_t buffer, int64_t start, int64_t end, bool strict_indexing);
	// DEPRECATED
	// void buffer_set_line_slice(Buffer buffer, Integer start, Integer end, Boolean include_start, Boolean include_end, ArrayOf(String) replacement, ) 
	MsgpackRequest* buffer_set_line_slice(int64_t buffer, int64_t start, int64_t end, bool include_start, bool include_end, QList<QByteArray> replacement);
	// void nvim_buf_set_lines(Buffer buffer, Integer start, Integer end, Boolean strict_indexing, ArrayOf(String) replacement, ) 
	MsgpackRequest* nvim_buf_set_lines(int64_t buffer, int64_t start, int64_t end, bool strict_indexing, QList<QByteArray> replacement);
	// Object nvim_buf_get_var(Buffer buffer, String name, ) 
	MsgpackRequest* nvim_buf_get_var(int64_t buffer, QByteArray name);
	// Integer nvim_buf_get_changedtick(Buffer buffer, ) 
	MsgpackRequest* nvim_buf_get_changedtick(int64_t buffer);
	// void nvim_buf_set_var(Buffer buffer, String name, Object value, ) 
	MsgpackRequest* nvim_buf_set_var(int64_t buffer, QByteArray name, QVariant value);
	// void nvim_buf_del_var(Buffer buffer, String name, ) 
	MsgpackRequest* nvim_buf_del_var(int64_t buffer, QByteArray name);
	// DEPRECATED
	// Object buffer_set_var(Buffer buffer, String name, Object value, ) 
	MsgpackRequest* buffer_set_var(int64_t buffer, QByteArray name, QVariant value);
	// DEPRECATED
	// Object buffer_del_var(Buffer buffer, String name, ) 
	MsgpackRequest* buffer_del_var(int64_t buffer, QByteArray name);
	// Object nvim_buf_get_option(Buffer buffer, String name, ) 
	MsgpackRequest* nvim_buf_get_option(int64_t buffer, QByteArray name);
	// void nvim_buf_set_option(Buffer buffer, String name, Object value, ) 
	MsgpackRequest* nvim_buf_set_option(int64_t buffer, QByteArray name, QVariant value);
	// DEPRECATED
	// Integer nvim_buf_get_number(Buffer buffer, ) 
	MsgpackRequest* nvim_buf_get_number(int64_t buffer);
	// String nvim_buf_get_name(Buffer buffer, ) 
	MsgpackRequest* nvim_buf_get_name(int64_t buffer);
	// void nvim_buf_set_name(Buffer buffer, String name, ) 
	MsgpackRequest* nvim_buf_set_name(int64_t buffer, QByteArray name);
	// Boolean nvim_buf_is_valid(Buffer buffer, ) 
	MsgpackRequest* nvim_buf_is_valid(int64_t buffer);
	// DEPRECATED
	// void buffer_insert(Buffer buffer, Integer lnum, ArrayOf(String) lines, ) 
	MsgpackRequest* buffer_insert(int64_t buffer, int64_t lnum, QList<QByteArray> lines);
	// ArrayOf(Integer, 2) nvim_buf_get_mark(Buffer buffer, String name, ) 
	MsgpackRequest* nvim_buf_get_mark(int64_t buffer, QByteArray name);
	// Integer nvim_buf_add_highlight(Buffer buffer, Integer src_id, String hl_group, Integer line, Integer col_start, Integer col_end, ) 
	MsgpackRequest* nvim_buf_add_highlight(int64_t buffer, int64_t src_id, QByteArray hl_group, int64_t line, int64_t col_start, int64_t col_end);
	// void nvim_buf_clear_highlight(Buffer buffer, Integer src_id, Integer line_start, Integer line_end, ) 
	MsgpackRequest* nvim_buf_clear_highlight(int64_t buffer, int64_t src_id, int64_t line_start, int64_t line_end);
	// ArrayOf(Window) nvim_tabpage_list_wins(Tabpage tabpage, ) 
	MsgpackRequest* nvim_tabpage_list_wins(int64_t tabpage);
	// Object nvim_tabpage_get_var(Tabpage tabpage, String name, ) 
	MsgpackRequest* nvim_tabpage_get_var(int64_t tabpage, QByteArray name);
	// void nvim_tabpage_set_var(Tabpage tabpage, String name, Object value, ) 
	MsgpackRequest* nvim_tabpage_set_var(int64_t tabpage, QByteArray name, QVariant value);
	// void nvim_tabpage_del_var(Tabpage tabpage, String name, ) 
	MsgpackRequest* nvim_tabpage_del_var(int64_t tabpage, QByteArray name);
	// DEPRECATED
	// Object tabpage_set_var(Tabpage tabpage, String name, Object value, ) 
	MsgpackRequest* tabpage_set_var(int64_t tabpage, QByteArray name, QVariant value);
	// DEPRECATED
	// Object tabpage_del_var(Tabpage tabpage, String name, ) 
	MsgpackRequest* tabpage_del_var(int64_t tabpage, QByteArray name);
	// Window nvim_tabpage_get_win(Tabpage tabpage, ) 
	MsgpackRequest* nvim_tabpage_get_win(int64_t tabpage);
	// Integer nvim_tabpage_get_number(Tabpage tabpage, ) 
	MsgpackRequest* nvim_tabpage_get_number(int64_t tabpage);
	// Boolean nvim_tabpage_is_valid(Tabpage tabpage, ) 
	MsgpackRequest* nvim_tabpage_is_valid(int64_t tabpage);
	// void nvim_ui_attach(Integer width, Integer height, Dictionary options, ) 
	MsgpackRequest* nvim_ui_attach(int64_t width, int64_t height, QVariantMap options);
	// DEPRECATED
	// void ui_attach(Integer width, Integer height, Boolean enable_rgb, ) 
	MsgpackRequest* ui_attach(int64_t width, int64_t height, bool enable_rgb);
	// void nvim_ui_detach() 
	MsgpackRequest* nvim_ui_detach();
	// void nvim_ui_try_resize(Integer width, Integer height, ) 
	MsgpackRequest* nvim_ui_try_resize(int64_t width, int64_t height);
	// void nvim_ui_set_option(String name, Object value, ) 
	MsgpackRequest* nvim_ui_set_option(QByteArray name, QVariant value);
	// void nvim_command(String command, ) 
	MsgpackRequest* nvim_command(QByteArray command);
	// void nvim_feedkeys(String keys, String mode, Boolean escape_csi, ) 
	MsgpackRequest* nvim_feedkeys(QByteArray keys, QByteArray mode, bool escape_csi);
	// Integer nvim_input(String keys, ) 
	MsgpackRequest* nvim_input(QByteArray keys);
	// String nvim_replace_termcodes(String str, Boolean from_part, Boolean do_lt, Boolean special, ) 
	MsgpackRequest* nvim_replace_termcodes(QByteArray str, bool from_part, bool do_lt, bool special);
	// String nvim_command_output(String str, ) 
	MsgpackRequest* nvim_command_output(QByteArray str);
	// Object nvim_eval(String expr, ) 
	MsgpackRequest* nvim_eval(QByteArray expr);
	// Object nvim_call_function(String fname, Array args, ) 
	MsgpackRequest* nvim_call_function(QByteArray fname, QVariantList args);
	// Integer nvim_strwidth(String str, ) 
	MsgpackRequest* nvim_strwidth(QByteArray str);
	// ArrayOf(String) nvim_list_runtime_paths() 
	MsgpackRequest* nvim_list_runtime_paths();
	// void nvim_set_current_dir(String dir, ) 
	MsgpackRequest* nvim_set_current_dir(QByteArray dir);
	// String nvim_get_current_line() 
	MsgpackRequest* nvim_get_current_line();
	// void nvim_set_current_line(String line, ) 
	MsgpackRequest* nvim_set_current_line(QByteArray line);
	// void nvim_del_current_line() 
	MsgpackRequest* nvim_del_current_line();
	// Object nvim_get_var(String name, ) 
	MsgpackRequest* nvim_get_var(QByteArray name);
	// void nvim_set_var(String name, Object value, ) 
	MsgpackRequest* nvim_set_var(QByteArray name, QVariant value);
	// void nvim_del_var(String name, ) 
	MsgpackRequest* nvim_del_var(QByteArray name);
	// DEPRECATED
	// Object vim_set_var(String name, Object value, ) 
	MsgpackRequest* vim_set_var(QByteArray name, QVariant value);
	// DEPRECATED
	// Object vim_del_var(String name, ) 
	MsgpackRequest* vim_del_var(QByteArray name);
	// Object nvim_get_vvar(String name, ) 
	MsgpackRequest* nvim_get_vvar(QByteArray name);
	// Object nvim_get_option(String name, ) 
	MsgpackRequest* nvim_get_option(QByteArray name);
	// void nvim_set_option(String name, Object value, ) 
	MsgpackRequest* nvim_set_option(QByteArray name, QVariant value);
	// void nvim_out_write(String str, ) 
	MsgpackRequest* nvim_out_write(QByteArray str);
	// void nvim_err_write(String str, ) 
	MsgpackRequest* nvim_err_write(QByteArray str);
	// void nvim_err_writeln(String str, ) 
	MsgpackRequest* nvim_err_writeln(QByteArray str);
	// ArrayOf(Buffer) nvim_list_bufs() 
	MsgpackRequest* nvim_list_bufs();
	// Buffer nvim_get_current_buf() 
	MsgpackRequest* nvim_get_current_buf();
	// void nvim_set_current_buf(Buffer buffer, ) 
	MsgpackRequest* nvim_set_current_buf(int64_t buffer);
	// ArrayOf(Window) nvim_list_wins() 
	MsgpackRequest* nvim_list_wins();
	// Window nvim_get_current_win() 
	MsgpackRequest* nvim_get_current_win();
	// void nvim_set_current_win(Window window, ) 
	MsgpackRequest* nvim_set_current_win(int64_t window);
	// ArrayOf(Tabpage) nvim_list_tabpages() 
	MsgpackRequest* nvim_list_tabpages();
	// Tabpage nvim_get_current_tabpage() 
	MsgpackRequest* nvim_get_current_tabpage();
	// void nvim_set_current_tabpage(Tabpage tabpage, ) 
	MsgpackRequest* nvim_set_current_tabpage(int64_t tabpage);
	// void nvim_subscribe(String event, ) 
	MsgpackRequest* nvim_subscribe(QByteArray event);
	// void nvim_unsubscribe(String event, ) 
	MsgpackRequest* nvim_unsubscribe(QByteArray event);
	// Integer nvim_get_color_by_name(String name, ) 
	MsgpackRequest* nvim_get_color_by_name(QByteArray name);
	// Dictionary nvim_get_color_map() 
	MsgpackRequest* nvim_get_color_map();
	// Dictionary nvim_get_mode() 
	MsgpackRequest* nvim_get_mode();
	// Array nvim_get_api_info() 
	MsgpackRequest* nvim_get_api_info();
	// Array nvim_call_atomic(Array calls, ) 
	MsgpackRequest* nvim_call_atomic(QVariantList calls);
	// Buffer nvim_win_get_buf(Window window, ) 
	MsgpackRequest* nvim_win_get_buf(int64_t window);
	// ArrayOf(Integer, 2) nvim_win_get_cursor(Window window, ) 
	MsgpackRequest* nvim_win_get_cursor(int64_t window);
	// void nvim_win_set_cursor(Window window, ArrayOf(Integer, 2) pos, ) 
	MsgpackRequest* nvim_win_set_cursor(int64_t window, QPoint pos);
	// Integer nvim_win_get_height(Window window, ) 
	MsgpackRequest* nvim_win_get_height(int64_t window);
	// void nvim_win_set_height(Window window, Integer height, ) 
	MsgpackRequest* nvim_win_set_height(int64_t window, int64_t height);
	// Integer nvim_win_get_width(Window window, ) 
	MsgpackRequest* nvim_win_get_width(int64_t window);
	// void nvim_win_set_width(Window window, Integer width, ) 
	MsgpackRequest* nvim_win_set_width(int64_t window, int64_t width);
	// Object nvim_win_get_var(Window window, String name, ) 
	MsgpackRequest* nvim_win_get_var(int64_t window, QByteArray name);
	// void nvim_win_set_var(Window window, String name, Object value, ) 
	MsgpackRequest* nvim_win_set_var(int64_t window, QByteArray name, QVariant value);
	// void nvim_win_del_var(Window window, String name, ) 
	MsgpackRequest* nvim_win_del_var(int64_t window, QByteArray name);
	// DEPRECATED
	// Object window_set_var(Window window, String name, Object value, ) 
	MsgpackRequest* window_set_var(int64_t window, QByteArray name, QVariant value);
	// DEPRECATED
	// Object window_del_var(Window window, String name, ) 
	MsgpackRequest* window_del_var(int64_t window, QByteArray name);
	// Object nvim_win_get_option(Window window, String name, ) 
	MsgpackRequest* nvim_win_get_option(int64_t window, QByteArray name);
	// void nvim_win_set_option(Window window, String name, Object value, ) 
	MsgpackRequest* nvim_win_set_option(int64_t window, QByteArray name, QVariant value);
	// ArrayOf(Integer, 2) nvim_win_get_position(Window window, ) 
	MsgpackRequest* nvim_win_get_position(int64_t window);
	// Tabpage nvim_win_get_tabpage(Window window, ) 
	MsgpackRequest* nvim_win_get_tabpage(int64_t window);
	// Integer nvim_win_get_number(Window window, ) 
	MsgpackRequest* nvim_win_get_number(int64_t window);
	// Boolean nvim_win_is_valid(Window window, ) 
	MsgpackRequest* nvim_win_is_valid(int64_t window);
	// DEPRECATED
	// Integer buffer_line_count(Buffer buffer, ) 
	MsgpackRequest* buffer_line_count(int64_t buffer);
	// DEPRECATED
	// ArrayOf(String) buffer_get_lines(Buffer buffer, Integer start, Integer end, Boolean strict_indexing, ) 
	MsgpackRequest* buffer_get_lines(int64_t buffer, int64_t start, int64_t end, bool strict_indexing);
	// DEPRECATED
	// void buffer_set_lines(Buffer buffer, Integer start, Integer end, Boolean strict_indexing, ArrayOf(String) replacement, ) 
	MsgpackRequest* buffer_set_lines(int64_t buffer, int64_t start, int64_t end, bool strict_indexing, QList<QByteArray> replacement);
	// DEPRECATED
	// Object buffer_get_var(Buffer buffer, String name, ) 
	MsgpackRequest* buffer_get_var(int64_t buffer, QByteArray name);
	// DEPRECATED
	// Object buffer_get_option(Buffer buffer, String name, ) 
	MsgpackRequest* buffer_get_option(int64_t buffer, QByteArray name);
	// DEPRECATED
	// void buffer_set_option(Buffer buffer, String name, Object value, ) 
	MsgpackRequest* buffer_set_option(int64_t buffer, QByteArray name, QVariant value);
	// DEPRECATED
	// Integer buffer_get_number(Buffer buffer, ) 
	MsgpackRequest* buffer_get_number(int64_t buffer);
	// DEPRECATED
	// String buffer_get_name(Buffer buffer, ) 
	MsgpackRequest* buffer_get_name(int64_t buffer);
	// DEPRECATED
	// void buffer_set_name(Buffer buffer, String name, ) 
	MsgpackRequest* buffer_set_name(int64_t buffer, QByteArray name);
	// DEPRECATED
	// Boolean buffer_is_valid(Buffer buffer, ) 
	MsgpackRequest* buffer_is_valid(int64_t buffer);
	// DEPRECATED
	// ArrayOf(Integer, 2) buffer_get_mark(Buffer buffer, String name, ) 
	MsgpackRequest* buffer_get_mark(int64_t buffer, QByteArray name);
	// DEPRECATED
	// Integer buffer_add_highlight(Buffer buffer, Integer src_id, String hl_group, Integer line, Integer col_start, Integer col_end, ) 
	MsgpackRequest* buffer_add_highlight(int64_t buffer, int64_t src_id, QByteArray hl_group, int64_t line, int64_t col_start, int64_t col_end);
	// DEPRECATED
	// void buffer_clear_highlight(Buffer buffer, Integer src_id, Integer line_start, Integer line_end, ) 
	MsgpackRequest* buffer_clear_highlight(int64_t buffer, int64_t src_id, int64_t line_start, int64_t line_end);
	// DEPRECATED
	// ArrayOf(Window) tabpage_get_windows(Tabpage tabpage, ) 
	MsgpackRequest* tabpage_get_windows(int64_t tabpage);
	// DEPRECATED
	// Object tabpage_get_var(Tabpage tabpage, String name, ) 
	MsgpackRequest* tabpage_get_var(int64_t tabpage, QByteArray name);
	// DEPRECATED
	// Window tabpage_get_window(Tabpage tabpage, ) 
	MsgpackRequest* tabpage_get_window(int64_t tabpage);
	// DEPRECATED
	// Boolean tabpage_is_valid(Tabpage tabpage, ) 
	MsgpackRequest* tabpage_is_valid(int64_t tabpage);
	// DEPRECATED
	// void ui_detach() 
	MsgpackRequest* ui_detach();
	// DEPRECATED
	// Object ui_try_resize(Integer width, Integer height, ) 
	MsgpackRequest* ui_try_resize(int64_t width, int64_t height);
	// DEPRECATED
	// void vim_command(String command, ) 
	MsgpackRequest* vim_command(QByteArray command);
	// DEPRECATED
	// void vim_feedkeys(String keys, String mode, Boolean escape_csi, ) 
	MsgpackRequest* vim_feedkeys(QByteArray keys, QByteArray mode, bool escape_csi);
	// DEPRECATED
	// Integer vim_input(String keys, ) 
	MsgpackRequest* vim_input(QByteArray keys);
	// DEPRECATED
	// String vim_replace_termcodes(String str, Boolean from_part, Boolean do_lt, Boolean special, ) 
	MsgpackRequest* vim_replace_termcodes(QByteArray str, bool from_part, bool do_lt, bool special);
	// DEPRECATED
	// String vim_command_output(String str, ) 
	MsgpackRequest* vim_command_output(QByteArray str);
	// DEPRECATED
	// Object vim_eval(String expr, ) 
	MsgpackRequest* vim_eval(QByteArray expr);
	// DEPRECATED
	// Object vim_call_function(String fname, Array args, ) 
	MsgpackRequest* vim_call_function(QByteArray fname, QVariantList args);
	// DEPRECATED
	// Integer vim_strwidth(String str, ) 
	MsgpackRequest* vim_strwidth(QByteArray str);
	// DEPRECATED
	// ArrayOf(String) vim_list_runtime_paths() 
	MsgpackRequest* vim_list_runtime_paths();
	// DEPRECATED
	// void vim_change_directory(String dir, ) 
	MsgpackRequest* vim_change_directory(QByteArray dir);
	// DEPRECATED
	// String vim_get_current_line() 
	MsgpackRequest* vim_get_current_line();
	// DEPRECATED
	// void vim_set_current_line(String line, ) 
	MsgpackRequest* vim_set_current_line(QByteArray line);
	// DEPRECATED
	// void vim_del_current_line() 
	MsgpackRequest* vim_del_current_line();
	// DEPRECATED
	// Object vim_get_var(String name, ) 
	MsgpackRequest* vim_get_var(QByteArray name);
	// DEPRECATED
	// Object vim_get_vvar(String name, ) 
	MsgpackRequest* vim_get_vvar(QByteArray name);
	// DEPRECATED
	// Object vim_get_option(String name, ) 
	MsgpackRequest* vim_get_option(QByteArray name);
	// DEPRECATED
	// void vim_set_option(String name, Object value, ) 
	MsgpackRequest* vim_set_option(QByteArray name, QVariant value);
	// DEPRECATED
	// void vim_out_write(String str, ) 
	MsgpackRequest* vim_out_write(QByteArray str);
	// DEPRECATED
	// void vim_err_write(String str, ) 
	MsgpackRequest* vim_err_write(QByteArray str);
	// DEPRECATED
	// void vim_report_error(String str, ) 
	MsgpackRequest* vim_report_error(QByteArray str);
	// DEPRECATED
	// ArrayOf(Buffer) vim_get_buffers() 
	MsgpackRequest* vim_get_buffers();
	// DEPRECATED
	// Buffer vim_get_current_buffer() 
	MsgpackRequest* vim_get_current_buffer();
	// DEPRECATED
	// void vim_set_current_buffer(Buffer buffer, ) 
	MsgpackRequest* vim_set_current_buffer(int64_t buffer);
	// DEPRECATED
	// ArrayOf(Window) vim_get_windows() 
	MsgpackRequest* vim_get_windows();
	// DEPRECATED
	// Window vim_get_current_window() 
	MsgpackRequest* vim_get_current_window();
	// DEPRECATED
	// void vim_set_current_window(Window window, ) 
	MsgpackRequest* vim_set_current_window(int64_t window);
	// DEPRECATED
	// ArrayOf(Tabpage) vim_get_tabpages() 
	MsgpackRequest* vim_get_tabpages();
	// DEPRECATED
	// Tabpage vim_get_current_tabpage() 
	MsgpackRequest* vim_get_current_tabpage();
	// DEPRECATED
	// void vim_set_current_tabpage(Tabpage tabpage, ) 
	MsgpackRequest* vim_set_current_tabpage(int64_t tabpage);
	// DEPRECATED
	// void vim_subscribe(String event, ) 
	MsgpackRequest* vim_subscribe(QByteArray event);
	// DEPRECATED
	// void vim_unsubscribe(String event, ) 
	MsgpackRequest* vim_unsubscribe(QByteArray event);
	// DEPRECATED
	// Integer vim_name_to_color(String name, ) 
	MsgpackRequest* vim_name_to_color(QByteArray name);
	// DEPRECATED
	// Dictionary vim_get_color_map() 
	MsgpackRequest* vim_get_color_map();
	// DEPRECATED
	// Buffer window_get_buffer(Window window, ) 
	MsgpackRequest* window_get_buffer(int64_t window);
	// DEPRECATED
	// ArrayOf(Integer, 2) window_get_cursor(Window window, ) 
	MsgpackRequest* window_get_cursor(int64_t window);
	// DEPRECATED
	// void window_set_cursor(Window window, ArrayOf(Integer, 2) pos, ) 
	MsgpackRequest* window_set_cursor(int64_t window, QPoint pos);
	// DEPRECATED
	// Integer window_get_height(Window window, ) 
	MsgpackRequest* window_get_height(int64_t window);
	// DEPRECATED
	// void window_set_height(Window window, Integer height, ) 
	MsgpackRequest* window_set_height(int64_t window, int64_t height);
	// DEPRECATED
	// Integer window_get_width(Window window, ) 
	MsgpackRequest* window_get_width(int64_t window);
	// DEPRECATED
	// void window_set_width(Window window, Integer width, ) 
	MsgpackRequest* window_set_width(int64_t window, int64_t width);
	// DEPRECATED
	// Object window_get_var(Window window, String name, ) 
	MsgpackRequest* window_get_var(int64_t window, QByteArray name);
	// DEPRECATED
	// Object window_get_option(Window window, String name, ) 
	MsgpackRequest* window_get_option(int64_t window, QByteArray name);
	// DEPRECATED
	// void window_set_option(Window window, String name, Object value, ) 
	MsgpackRequest* window_set_option(int64_t window, QByteArray name, QVariant value);
	// DEPRECATED
	// ArrayOf(Integer, 2) window_get_position(Window window, ) 
	MsgpackRequest* window_get_position(int64_t window);
	// DEPRECATED
	// Tabpage window_get_tabpage(Window window, ) 
	MsgpackRequest* window_get_tabpage(int64_t window);
	// DEPRECATED
	// Boolean window_is_valid(Window window, ) 
	MsgpackRequest* window_is_valid(int64_t window);

signals:
	void on_nvim_buf_line_count(int64_t);
	void err_nvim_buf_line_count(const QString&, const QVariant&);

	void on_buffer_get_line(QByteArray);
	void err_buffer_get_line(const QString&, const QVariant&);

	void on_buffer_set_line(void);
	void err_buffer_set_line(const QString&, const QVariant&);

	void on_buffer_del_line(void);
	void err_buffer_del_line(const QString&, const QVariant&);

	void on_buffer_get_line_slice(QList<QByteArray>);
	void err_buffer_get_line_slice(const QString&, const QVariant&);

	void on_nvim_buf_get_lines(QList<QByteArray>);
	void err_nvim_buf_get_lines(const QString&, const QVariant&);

	void on_buffer_set_line_slice(void);
	void err_buffer_set_line_slice(const QString&, const QVariant&);

	void on_nvim_buf_set_lines(void);
	void err_nvim_buf_set_lines(const QString&, const QVariant&);

	void on_nvim_buf_get_var(QVariant);
	void err_nvim_buf_get_var(const QString&, const QVariant&);

	void on_nvim_buf_get_changedtick(int64_t);
	void err_nvim_buf_get_changedtick(const QString&, const QVariant&);

	void on_nvim_buf_set_var(void);
	void err_nvim_buf_set_var(const QString&, const QVariant&);

	void on_nvim_buf_del_var(void);
	void err_nvim_buf_del_var(const QString&, const QVariant&);

	void on_buffer_set_var(QVariant);
	void err_buffer_set_var(const QString&, const QVariant&);

	void on_buffer_del_var(QVariant);
	void err_buffer_del_var(const QString&, const QVariant&);

	void on_nvim_buf_get_option(QVariant);
	void err_nvim_buf_get_option(const QString&, const QVariant&);

	void on_nvim_buf_set_option(void);
	void err_nvim_buf_set_option(const QString&, const QVariant&);

	void on_nvim_buf_get_number(int64_t);
	void err_nvim_buf_get_number(const QString&, const QVariant&);

	void on_nvim_buf_get_name(QByteArray);
	void err_nvim_buf_get_name(const QString&, const QVariant&);

	void on_nvim_buf_set_name(void);
	void err_nvim_buf_set_name(const QString&, const QVariant&);

	void on_nvim_buf_is_valid(bool);
	void err_nvim_buf_is_valid(const QString&, const QVariant&);

	void on_buffer_insert(void);
	void err_buffer_insert(const QString&, const QVariant&);

	void on_nvim_buf_get_mark(QPoint);
	void err_nvim_buf_get_mark(const QString&, const QVariant&);

	void on_nvim_buf_add_highlight(int64_t);
	void err_nvim_buf_add_highlight(const QString&, const QVariant&);

	void on_nvim_buf_clear_highlight(void);
	void err_nvim_buf_clear_highlight(const QString&, const QVariant&);

	void on_nvim_tabpage_list_wins(QList<int64_t>);
	void err_nvim_tabpage_list_wins(const QString&, const QVariant&);

	void on_nvim_tabpage_get_var(QVariant);
	void err_nvim_tabpage_get_var(const QString&, const QVariant&);

	void on_nvim_tabpage_set_var(void);
	void err_nvim_tabpage_set_var(const QString&, const QVariant&);

	void on_nvim_tabpage_del_var(void);
	void err_nvim_tabpage_del_var(const QString&, const QVariant&);

	void on_tabpage_set_var(QVariant);
	void err_tabpage_set_var(const QString&, const QVariant&);

	void on_tabpage_del_var(QVariant);
	void err_tabpage_del_var(const QString&, const QVariant&);

	void on_nvim_tabpage_get_win(int64_t);
	void err_nvim_tabpage_get_win(const QString&, const QVariant&);

	void on_nvim_tabpage_get_number(int64_t);
	void err_nvim_tabpage_get_number(const QString&, const QVariant&);

	void on_nvim_tabpage_is_valid(bool);
	void err_nvim_tabpage_is_valid(const QString&, const QVariant&);

	void on_nvim_ui_attach(void);
	void err_nvim_ui_attach(const QString&, const QVariant&);

	void on_ui_attach(void);
	void err_ui_attach(const QString&, const QVariant&);

	void on_nvim_ui_detach(void);
	void err_nvim_ui_detach(const QString&, const QVariant&);

	void on_nvim_ui_try_resize(void);
	void err_nvim_ui_try_resize(const QString&, const QVariant&);

	void on_nvim_ui_set_option(void);
	void err_nvim_ui_set_option(const QString&, const QVariant&);

	void on_nvim_command(void);
	void err_nvim_command(const QString&, const QVariant&);

	void on_nvim_feedkeys(void);
	void err_nvim_feedkeys(const QString&, const QVariant&);

	void on_nvim_input(int64_t);
	void err_nvim_input(const QString&, const QVariant&);

	void on_nvim_replace_termcodes(QByteArray);
	void err_nvim_replace_termcodes(const QString&, const QVariant&);

	void on_nvim_command_output(QByteArray);
	void err_nvim_command_output(const QString&, const QVariant&);

	void on_nvim_eval(QVariant);
	void err_nvim_eval(const QString&, const QVariant&);

	void on_nvim_call_function(QVariant);
	void err_nvim_call_function(const QString&, const QVariant&);

	void on_nvim_strwidth(int64_t);
	void err_nvim_strwidth(const QString&, const QVariant&);

	void on_nvim_list_runtime_paths(QList<QByteArray>);
	void err_nvim_list_runtime_paths(const QString&, const QVariant&);

	void on_nvim_set_current_dir(void);
	void err_nvim_set_current_dir(const QString&, const QVariant&);

	void on_nvim_get_current_line(QByteArray);
	void err_nvim_get_current_line(const QString&, const QVariant&);

	void on_nvim_set_current_line(void);
	void err_nvim_set_current_line(const QString&, const QVariant&);

	void on_nvim_del_current_line(void);
	void err_nvim_del_current_line(const QString&, const QVariant&);

	void on_nvim_get_var(QVariant);
	void err_nvim_get_var(const QString&, const QVariant&);

	void on_nvim_set_var(void);
	void err_nvim_set_var(const QString&, const QVariant&);

	void on_nvim_del_var(void);
	void err_nvim_del_var(const QString&, const QVariant&);

	void on_vim_set_var(QVariant);
	void err_vim_set_var(const QString&, const QVariant&);

	void on_vim_del_var(QVariant);
	void err_vim_del_var(const QString&, const QVariant&);

	void on_nvim_get_vvar(QVariant);
	void err_nvim_get_vvar(const QString&, const QVariant&);

	void on_nvim_get_option(QVariant);
	void err_nvim_get_option(const QString&, const QVariant&);

	void on_nvim_set_option(void);
	void err_nvim_set_option(const QString&, const QVariant&);

	void on_nvim_out_write(void);
	void err_nvim_out_write(const QString&, const QVariant&);

	void on_nvim_err_write(void);
	void err_nvim_err_write(const QString&, const QVariant&);

	void on_nvim_err_writeln(void);
	void err_nvim_err_writeln(const QString&, const QVariant&);

	void on_nvim_list_bufs(QList<int64_t>);
	void err_nvim_list_bufs(const QString&, const QVariant&);

	void on_nvim_get_current_buf(int64_t);
	void err_nvim_get_current_buf(const QString&, const QVariant&);

	void on_nvim_set_current_buf(void);
	void err_nvim_set_current_buf(const QString&, const QVariant&);

	void on_nvim_list_wins(QList<int64_t>);
	void err_nvim_list_wins(const QString&, const QVariant&);

	void on_nvim_get_current_win(int64_t);
	void err_nvim_get_current_win(const QString&, const QVariant&);

	void on_nvim_set_current_win(void);
	void err_nvim_set_current_win(const QString&, const QVariant&);

	void on_nvim_list_tabpages(QList<int64_t>);
	void err_nvim_list_tabpages(const QString&, const QVariant&);

	void on_nvim_get_current_tabpage(int64_t);
	void err_nvim_get_current_tabpage(const QString&, const QVariant&);

	void on_nvim_set_current_tabpage(void);
	void err_nvim_set_current_tabpage(const QString&, const QVariant&);

	void on_nvim_subscribe(void);
	void err_nvim_subscribe(const QString&, const QVariant&);

	void on_nvim_unsubscribe(void);
	void err_nvim_unsubscribe(const QString&, const QVariant&);

	void on_nvim_get_color_by_name(int64_t);
	void err_nvim_get_color_by_name(const QString&, const QVariant&);

	void on_nvim_get_color_map(QVariantMap);
	void err_nvim_get_color_map(const QString&, const QVariant&);

	void on_nvim_get_mode(QVariantMap);
	void err_nvim_get_mode(const QString&, const QVariant&);

	void on_nvim_get_api_info(QVariantList);
	void err_nvim_get_api_info(const QString&, const QVariant&);

	void on_nvim_call_atomic(QVariantList);
	void err_nvim_call_atomic(const QString&, const QVariant&);

	void on_nvim_win_get_buf(int64_t);
	void err_nvim_win_get_buf(const QString&, const QVariant&);

	void on_nvim_win_get_cursor(QPoint);
	void err_nvim_win_get_cursor(const QString&, const QVariant&);

	void on_nvim_win_set_cursor(void);
	void err_nvim_win_set_cursor(const QString&, const QVariant&);

	void on_nvim_win_get_height(int64_t);
	void err_nvim_win_get_height(const QString&, const QVariant&);

	void on_nvim_win_set_height(void);
	void err_nvim_win_set_height(const QString&, const QVariant&);

	void on_nvim_win_get_width(int64_t);
	void err_nvim_win_get_width(const QString&, const QVariant&);

	void on_nvim_win_set_width(void);
	void err_nvim_win_set_width(const QString&, const QVariant&);

	void on_nvim_win_get_var(QVariant);
	void err_nvim_win_get_var(const QString&, const QVariant&);

	void on_nvim_win_set_var(void);
	void err_nvim_win_set_var(const QString&, const QVariant&);

	void on_nvim_win_del_var(void);
	void err_nvim_win_del_var(const QString&, const QVariant&);

	void on_window_set_var(QVariant);
	void err_window_set_var(const QString&, const QVariant&);

	void on_window_del_var(QVariant);
	void err_window_del_var(const QString&, const QVariant&);

	void on_nvim_win_get_option(QVariant);
	void err_nvim_win_get_option(const QString&, const QVariant&);

	void on_nvim_win_set_option(void);
	void err_nvim_win_set_option(const QString&, const QVariant&);

	void on_nvim_win_get_position(QPoint);
	void err_nvim_win_get_position(const QString&, const QVariant&);

	void on_nvim_win_get_tabpage(int64_t);
	void err_nvim_win_get_tabpage(const QString&, const QVariant&);

	void on_nvim_win_get_number(int64_t);
	void err_nvim_win_get_number(const QString&, const QVariant&);

	void on_nvim_win_is_valid(bool);
	void err_nvim_win_is_valid(const QString&, const QVariant&);

	void on_buffer_line_count(int64_t);
	void err_buffer_line_count(const QString&, const QVariant&);

	void on_buffer_get_lines(QList<QByteArray>);
	void err_buffer_get_lines(const QString&, const QVariant&);

	void on_buffer_set_lines(void);
	void err_buffer_set_lines(const QString&, const QVariant&);

	void on_buffer_get_var(QVariant);
	void err_buffer_get_var(const QString&, const QVariant&);

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
	void err_buffer_is_valid(const QString&, const QVariant&);

	void on_buffer_get_mark(QPoint);
	void err_buffer_get_mark(const QString&, const QVariant&);

	void on_buffer_add_highlight(int64_t);
	void err_buffer_add_highlight(const QString&, const QVariant&);

	void on_buffer_clear_highlight(void);
	void err_buffer_clear_highlight(const QString&, const QVariant&);

	void on_tabpage_get_windows(QList<int64_t>);
	void err_tabpage_get_windows(const QString&, const QVariant&);

	void on_tabpage_get_var(QVariant);
	void err_tabpage_get_var(const QString&, const QVariant&);

	void on_tabpage_get_window(int64_t);
	void err_tabpage_get_window(const QString&, const QVariant&);

	void on_tabpage_is_valid(bool);
	void err_tabpage_is_valid(const QString&, const QVariant&);

	void on_ui_detach(void);
	void err_ui_detach(const QString&, const QVariant&);

	void on_ui_try_resize(QVariant);
	void err_ui_try_resize(const QString&, const QVariant&);

	void on_vim_command(void);
	void err_vim_command(const QString&, const QVariant&);

	void on_vim_feedkeys(void);
	void err_vim_feedkeys(const QString&, const QVariant&);

	void on_vim_input(int64_t);
	void err_vim_input(const QString&, const QVariant&);

	void on_vim_replace_termcodes(QByteArray);
	void err_vim_replace_termcodes(const QString&, const QVariant&);

	void on_vim_command_output(QByteArray);
	void err_vim_command_output(const QString&, const QVariant&);

	void on_vim_eval(QVariant);
	void err_vim_eval(const QString&, const QVariant&);

	void on_vim_call_function(QVariant);
	void err_vim_call_function(const QString&, const QVariant&);

	void on_vim_strwidth(int64_t);
	void err_vim_strwidth(const QString&, const QVariant&);

	void on_vim_list_runtime_paths(QList<QByteArray>);
	void err_vim_list_runtime_paths(const QString&, const QVariant&);

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

	void on_vim_get_vvar(QVariant);
	void err_vim_get_vvar(const QString&, const QVariant&);

	void on_vim_get_option(QVariant);
	void err_vim_get_option(const QString&, const QVariant&);

	void on_vim_set_option(void);
	void err_vim_set_option(const QString&, const QVariant&);

	void on_vim_out_write(void);
	void err_vim_out_write(const QString&, const QVariant&);

	void on_vim_err_write(void);
	void err_vim_err_write(const QString&, const QVariant&);

	void on_vim_report_error(void);
	void err_vim_report_error(const QString&, const QVariant&);

	void on_vim_get_buffers(QList<int64_t>);
	void err_vim_get_buffers(const QString&, const QVariant&);

	void on_vim_get_current_buffer(int64_t);
	void err_vim_get_current_buffer(const QString&, const QVariant&);

	void on_vim_set_current_buffer(void);
	void err_vim_set_current_buffer(const QString&, const QVariant&);

	void on_vim_get_windows(QList<int64_t>);
	void err_vim_get_windows(const QString&, const QVariant&);

	void on_vim_get_current_window(int64_t);
	void err_vim_get_current_window(const QString&, const QVariant&);

	void on_vim_set_current_window(void);
	void err_vim_set_current_window(const QString&, const QVariant&);

	void on_vim_get_tabpages(QList<int64_t>);
	void err_vim_get_tabpages(const QString&, const QVariant&);

	void on_vim_get_current_tabpage(int64_t);
	void err_vim_get_current_tabpage(const QString&, const QVariant&);

	void on_vim_set_current_tabpage(void);
	void err_vim_set_current_tabpage(const QString&, const QVariant&);

	void on_vim_subscribe(void);
	void err_vim_subscribe(const QString&, const QVariant&);

	void on_vim_unsubscribe(void);
	void err_vim_unsubscribe(const QString&, const QVariant&);

	void on_vim_name_to_color(int64_t);
	void err_vim_name_to_color(const QString&, const QVariant&);

	void on_vim_get_color_map(QVariantMap);
	void err_vim_get_color_map(const QString&, const QVariant&);

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

	void on_window_get_option(QVariant);
	void err_window_get_option(const QString&, const QVariant&);

	void on_window_set_option(void);
	void err_window_set_option(const QString&, const QVariant&);

	void on_window_get_position(QPoint);
	void err_window_get_position(const QString&, const QVariant&);

	void on_window_get_tabpage(int64_t);
	void err_window_get_tabpage(const QString&, const QVariant&);

	void on_window_is_valid(bool);
	void err_window_is_valid(const QString&, const QVariant&);

};
} // namespace
#endif