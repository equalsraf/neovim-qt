// Auto generated 2021-07-31 13:15:11.084919 from nvim API level:6
#pragma once

#include "function.h"
#include "msgpack.h"

#include <QObject>
#include <QPoint>
#include <QVariant>

namespace NeovimQt {

class NeovimConnector;
class MsgpackRequest;

class NeovimApi6 : public QObject
{
	Q_OBJECT

public:

	NeovimApi6(NeovimConnector* c) noexcept;

	// FIXME enum class?
	enum FunctionId {
		NEOVIM_FN_NULL=0,
				NEOVIM_FN_NVIM_BUF_LINE_COUNT,
				NEOVIM_FN_BUFFER_GET_LINE,
				NEOVIM_FN_NVIM_BUF_ATTACH,
				NEOVIM_FN_NVIM_BUF_DETACH,
				NEOVIM_FN_BUFFER_SET_LINE,
				NEOVIM_FN_BUFFER_DEL_LINE,
				NEOVIM_FN_BUFFER_GET_LINE_SLICE,
				NEOVIM_FN_NVIM_BUF_GET_LINES,
				NEOVIM_FN_BUFFER_SET_LINE_SLICE,
				NEOVIM_FN_NVIM_BUF_SET_LINES,
				NEOVIM_FN_NVIM_BUF_GET_OFFSET,
				NEOVIM_FN_NVIM_BUF_GET_VAR,
				NEOVIM_FN_NVIM_BUF_GET_CHANGEDTICK,
				NEOVIM_FN_NVIM_BUF_GET_KEYMAP,
				NEOVIM_FN_NVIM_BUF_SET_KEYMAP,
				NEOVIM_FN_NVIM_BUF_DEL_KEYMAP,
				NEOVIM_FN_NVIM_BUF_GET_COMMANDS,
				NEOVIM_FN_NVIM_BUF_SET_VAR,
				NEOVIM_FN_NVIM_BUF_DEL_VAR,
				NEOVIM_FN_BUFFER_SET_VAR,
				NEOVIM_FN_BUFFER_DEL_VAR,
				NEOVIM_FN_NVIM_BUF_GET_OPTION,
				NEOVIM_FN_NVIM_BUF_SET_OPTION,
				NEOVIM_FN_NVIM_BUF_GET_NUMBER,
				NEOVIM_FN_NVIM_BUF_GET_NAME,
				NEOVIM_FN_NVIM_BUF_SET_NAME,
				NEOVIM_FN_NVIM_BUF_IS_LOADED,
				NEOVIM_FN_NVIM_BUF_IS_VALID,
				NEOVIM_FN_BUFFER_INSERT,
				NEOVIM_FN_NVIM_BUF_GET_MARK,
				NEOVIM_FN_NVIM_BUF_ADD_HIGHLIGHT,
				NEOVIM_FN_NVIM_BUF_CLEAR_NAMESPACE,
				NEOVIM_FN_NVIM_BUF_CLEAR_HIGHLIGHT,
				NEOVIM_FN_NVIM_BUF_SET_VIRTUAL_TEXT,
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
				NEOVIM_FN_NVIM_UI_TRY_RESIZE_GRID,
				NEOVIM_FN_NVIM_UI_PUM_SET_HEIGHT,
				NEOVIM_FN_NVIM_COMMAND,
				NEOVIM_FN_NVIM_GET_HL_BY_NAME,
				NEOVIM_FN_NVIM_GET_HL_BY_ID,
				NEOVIM_FN_NVIM_FEEDKEYS,
				NEOVIM_FN_NVIM_INPUT,
				NEOVIM_FN_NVIM_INPUT_MOUSE,
				NEOVIM_FN_NVIM_REPLACE_TERMCODES,
				NEOVIM_FN_NVIM_COMMAND_OUTPUT,
				NEOVIM_FN_NVIM_EVAL,
				NEOVIM_FN_NVIM_EXECUTE_LUA,
				NEOVIM_FN_NVIM_CALL_FUNCTION,
				NEOVIM_FN_NVIM_CALL_DICT_FUNCTION,
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
				NEOVIM_FN_NVIM_SET_VVAR,
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
				NEOVIM_FN_NVIM_CREATE_BUF,
				NEOVIM_FN_NVIM_OPEN_WIN,
				NEOVIM_FN_NVIM_LIST_TABPAGES,
				NEOVIM_FN_NVIM_GET_CURRENT_TABPAGE,
				NEOVIM_FN_NVIM_SET_CURRENT_TABPAGE,
				NEOVIM_FN_NVIM_CREATE_NAMESPACE,
				NEOVIM_FN_NVIM_GET_NAMESPACES,
				NEOVIM_FN_NVIM_PASTE,
				NEOVIM_FN_NVIM_PUT,
				NEOVIM_FN_NVIM_SUBSCRIBE,
				NEOVIM_FN_NVIM_UNSUBSCRIBE,
				NEOVIM_FN_NVIM_GET_COLOR_BY_NAME,
				NEOVIM_FN_NVIM_GET_COLOR_MAP,
				NEOVIM_FN_NVIM_GET_CONTEXT,
				NEOVIM_FN_NVIM_LOAD_CONTEXT,
				NEOVIM_FN_NVIM_GET_MODE,
				NEOVIM_FN_NVIM_GET_KEYMAP,
				NEOVIM_FN_NVIM_SET_KEYMAP,
				NEOVIM_FN_NVIM_DEL_KEYMAP,
				NEOVIM_FN_NVIM_GET_COMMANDS,
				NEOVIM_FN_NVIM_GET_API_INFO,
				NEOVIM_FN_NVIM_SET_CLIENT_INFO,
				NEOVIM_FN_NVIM_GET_CHAN_INFO,
				NEOVIM_FN_NVIM_LIST_CHANS,
				NEOVIM_FN_NVIM_CALL_ATOMIC,
				NEOVIM_FN_NVIM_PARSE_EXPRESSION,
				NEOVIM_FN_NVIM_LIST_UIS,
				NEOVIM_FN_NVIM_GET_PROC_CHILDREN,
				NEOVIM_FN_NVIM_GET_PROC,
				NEOVIM_FN_NVIM_SELECT_POPUPMENU_ITEM,
				NEOVIM_FN_NVIM_WIN_GET_BUF,
				NEOVIM_FN_NVIM_WIN_SET_BUF,
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
				NEOVIM_FN_NVIM_WIN_SET_CONFIG,
				NEOVIM_FN_NVIM_WIN_GET_CONFIG,
				NEOVIM_FN_NVIM_WIN_CLOSE,
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
	Q_ENUM(FunctionId)

	static bool checkFunctions(const QVariantList& ftable) noexcept;
	static FunctionId functionId(const Function& f) noexcept;

public slots:
			// Integer nvim_buf_line_count(Buffer buffer, )
	NeovimQt::MsgpackRequest* nvim_buf_line_count(int64_t buffer) noexcept;
			// DEPRECATED
		// String buffer_get_line(Buffer buffer, Integer index, )
	NeovimQt::MsgpackRequest* buffer_get_line(int64_t buffer, int64_t index) noexcept;
			// Boolean nvim_buf_attach(Buffer buffer, Boolean send_buffer, Dictionary opts, )
	NeovimQt::MsgpackRequest* nvim_buf_attach(int64_t buffer, bool send_buffer, QVariantMap opts) noexcept;
			// Boolean nvim_buf_detach(Buffer buffer, )
	NeovimQt::MsgpackRequest* nvim_buf_detach(int64_t buffer) noexcept;
			// DEPRECATED
		// void buffer_set_line(Buffer buffer, Integer index, String line, )
	NeovimQt::MsgpackRequest* buffer_set_line(int64_t buffer, int64_t index, QByteArray line) noexcept;
			// DEPRECATED
		// void buffer_del_line(Buffer buffer, Integer index, )
	NeovimQt::MsgpackRequest* buffer_del_line(int64_t buffer, int64_t index) noexcept;
			// DEPRECATED
		// ArrayOf(String) buffer_get_line_slice(Buffer buffer, Integer start, Integer end, Boolean include_start, Boolean include_end, )
	NeovimQt::MsgpackRequest* buffer_get_line_slice(int64_t buffer, int64_t start, int64_t end, bool include_start, bool include_end) noexcept;
			// ArrayOf(String) nvim_buf_get_lines(Buffer buffer, Integer start, Integer end, Boolean strict_indexing, )
	NeovimQt::MsgpackRequest* nvim_buf_get_lines(int64_t buffer, int64_t start, int64_t end, bool strict_indexing) noexcept;
			// DEPRECATED
		// void buffer_set_line_slice(Buffer buffer, Integer start, Integer end, Boolean include_start, Boolean include_end, ArrayOf(String) replacement, )
	NeovimQt::MsgpackRequest* buffer_set_line_slice(int64_t buffer, int64_t start, int64_t end, bool include_start, bool include_end, QList<QByteArray> replacement) noexcept;
			// void nvim_buf_set_lines(Buffer buffer, Integer start, Integer end, Boolean strict_indexing, ArrayOf(String) replacement, )
	NeovimQt::MsgpackRequest* nvim_buf_set_lines(int64_t buffer, int64_t start, int64_t end, bool strict_indexing, QList<QByteArray> replacement) noexcept;
			// Integer nvim_buf_get_offset(Buffer buffer, Integer index, )
	NeovimQt::MsgpackRequest* nvim_buf_get_offset(int64_t buffer, int64_t index) noexcept;
			// Object nvim_buf_get_var(Buffer buffer, String name, )
	NeovimQt::MsgpackRequest* nvim_buf_get_var(int64_t buffer, QByteArray name) noexcept;
			// Integer nvim_buf_get_changedtick(Buffer buffer, )
	NeovimQt::MsgpackRequest* nvim_buf_get_changedtick(int64_t buffer) noexcept;
			// ArrayOf(Dictionary) nvim_buf_get_keymap(Buffer buffer, String mode, )
	NeovimQt::MsgpackRequest* nvim_buf_get_keymap(int64_t buffer, QByteArray mode) noexcept;
			// void nvim_buf_set_keymap(Buffer buffer, String mode, String lhs, String rhs, Dictionary opts, )
	NeovimQt::MsgpackRequest* nvim_buf_set_keymap(int64_t buffer, QByteArray mode, QByteArray lhs, QByteArray rhs, QVariantMap opts) noexcept;
			// void nvim_buf_del_keymap(Buffer buffer, String mode, String lhs, )
	NeovimQt::MsgpackRequest* nvim_buf_del_keymap(int64_t buffer, QByteArray mode, QByteArray lhs) noexcept;
			// Dictionary nvim_buf_get_commands(Buffer buffer, Dictionary opts, )
	NeovimQt::MsgpackRequest* nvim_buf_get_commands(int64_t buffer, QVariantMap opts) noexcept;
			// void nvim_buf_set_var(Buffer buffer, String name, Object value, )
	NeovimQt::MsgpackRequest* nvim_buf_set_var(int64_t buffer, QByteArray name, QVariant value) noexcept;
			// void nvim_buf_del_var(Buffer buffer, String name, )
	NeovimQt::MsgpackRequest* nvim_buf_del_var(int64_t buffer, QByteArray name) noexcept;
			// DEPRECATED
		// Object buffer_set_var(Buffer buffer, String name, Object value, )
	NeovimQt::MsgpackRequest* buffer_set_var(int64_t buffer, QByteArray name, QVariant value) noexcept;
			// DEPRECATED
		// Object buffer_del_var(Buffer buffer, String name, )
	NeovimQt::MsgpackRequest* buffer_del_var(int64_t buffer, QByteArray name) noexcept;
			// Object nvim_buf_get_option(Buffer buffer, String name, )
	NeovimQt::MsgpackRequest* nvim_buf_get_option(int64_t buffer, QByteArray name) noexcept;
			// void nvim_buf_set_option(Buffer buffer, String name, Object value, )
	NeovimQt::MsgpackRequest* nvim_buf_set_option(int64_t buffer, QByteArray name, QVariant value) noexcept;
			// DEPRECATED
		// Integer nvim_buf_get_number(Buffer buffer, )
	NeovimQt::MsgpackRequest* nvim_buf_get_number(int64_t buffer) noexcept;
			// String nvim_buf_get_name(Buffer buffer, )
	NeovimQt::MsgpackRequest* nvim_buf_get_name(int64_t buffer) noexcept;
			// void nvim_buf_set_name(Buffer buffer, String name, )
	NeovimQt::MsgpackRequest* nvim_buf_set_name(int64_t buffer, QByteArray name) noexcept;
			// Boolean nvim_buf_is_loaded(Buffer buffer, )
	NeovimQt::MsgpackRequest* nvim_buf_is_loaded(int64_t buffer) noexcept;
			// Boolean nvim_buf_is_valid(Buffer buffer, )
	NeovimQt::MsgpackRequest* nvim_buf_is_valid(int64_t buffer) noexcept;
			// DEPRECATED
		// void buffer_insert(Buffer buffer, Integer lnum, ArrayOf(String) lines, )
	NeovimQt::MsgpackRequest* buffer_insert(int64_t buffer, int64_t lnum, QList<QByteArray> lines) noexcept;
			// ArrayOf(Integer, 2) nvim_buf_get_mark(Buffer buffer, String name, )
	NeovimQt::MsgpackRequest* nvim_buf_get_mark(int64_t buffer, QByteArray name) noexcept;
			// Integer nvim_buf_add_highlight(Buffer buffer, Integer ns_id, String hl_group, Integer line, Integer col_start, Integer col_end, )
	NeovimQt::MsgpackRequest* nvim_buf_add_highlight(int64_t buffer, int64_t ns_id, QByteArray hl_group, int64_t line, int64_t col_start, int64_t col_end) noexcept;
			// void nvim_buf_clear_namespace(Buffer buffer, Integer ns_id, Integer line_start, Integer line_end, )
	NeovimQt::MsgpackRequest* nvim_buf_clear_namespace(int64_t buffer, int64_t ns_id, int64_t line_start, int64_t line_end) noexcept;
			// void nvim_buf_clear_highlight(Buffer buffer, Integer ns_id, Integer line_start, Integer line_end, )
	NeovimQt::MsgpackRequest* nvim_buf_clear_highlight(int64_t buffer, int64_t ns_id, int64_t line_start, int64_t line_end) noexcept;
			// Integer nvim_buf_set_virtual_text(Buffer buffer, Integer ns_id, Integer line, Array chunks, Dictionary opts, )
	NeovimQt::MsgpackRequest* nvim_buf_set_virtual_text(int64_t buffer, int64_t ns_id, int64_t line, QVariantList chunks, QVariantMap opts) noexcept;
			// ArrayOf(Window) nvim_tabpage_list_wins(Tabpage tabpage, )
	NeovimQt::MsgpackRequest* nvim_tabpage_list_wins(int64_t tabpage) noexcept;
			// Object nvim_tabpage_get_var(Tabpage tabpage, String name, )
	NeovimQt::MsgpackRequest* nvim_tabpage_get_var(int64_t tabpage, QByteArray name) noexcept;
			// void nvim_tabpage_set_var(Tabpage tabpage, String name, Object value, )
	NeovimQt::MsgpackRequest* nvim_tabpage_set_var(int64_t tabpage, QByteArray name, QVariant value) noexcept;
			// void nvim_tabpage_del_var(Tabpage tabpage, String name, )
	NeovimQt::MsgpackRequest* nvim_tabpage_del_var(int64_t tabpage, QByteArray name) noexcept;
			// DEPRECATED
		// Object tabpage_set_var(Tabpage tabpage, String name, Object value, )
	NeovimQt::MsgpackRequest* tabpage_set_var(int64_t tabpage, QByteArray name, QVariant value) noexcept;
			// DEPRECATED
		// Object tabpage_del_var(Tabpage tabpage, String name, )
	NeovimQt::MsgpackRequest* tabpage_del_var(int64_t tabpage, QByteArray name) noexcept;
			// Window nvim_tabpage_get_win(Tabpage tabpage, )
	NeovimQt::MsgpackRequest* nvim_tabpage_get_win(int64_t tabpage) noexcept;
			// Integer nvim_tabpage_get_number(Tabpage tabpage, )
	NeovimQt::MsgpackRequest* nvim_tabpage_get_number(int64_t tabpage) noexcept;
			// Boolean nvim_tabpage_is_valid(Tabpage tabpage, )
	NeovimQt::MsgpackRequest* nvim_tabpage_is_valid(int64_t tabpage) noexcept;
			// void nvim_ui_attach(Integer width, Integer height, Dictionary options, )
	NeovimQt::MsgpackRequest* nvim_ui_attach(int64_t width, int64_t height, QVariantMap options) noexcept;
			// DEPRECATED
		// void ui_attach(Integer width, Integer height, Boolean enable_rgb, )
	NeovimQt::MsgpackRequest* ui_attach(int64_t width, int64_t height, bool enable_rgb) noexcept;
			// void nvim_ui_detach()
	NeovimQt::MsgpackRequest* nvim_ui_detach() noexcept;
			// void nvim_ui_try_resize(Integer width, Integer height, )
	NeovimQt::MsgpackRequest* nvim_ui_try_resize(int64_t width, int64_t height) noexcept;
			// void nvim_ui_set_option(String name, Object value, )
	NeovimQt::MsgpackRequest* nvim_ui_set_option(QByteArray name, QVariant value) noexcept;
			// void nvim_ui_try_resize_grid(Integer grid, Integer width, Integer height, )
	NeovimQt::MsgpackRequest* nvim_ui_try_resize_grid(int64_t grid, int64_t width, int64_t height) noexcept;
			// void nvim_ui_pum_set_height(Integer height, )
	NeovimQt::MsgpackRequest* nvim_ui_pum_set_height(int64_t height) noexcept;
			// void nvim_command(String command, )
	NeovimQt::MsgpackRequest* nvim_command(QByteArray command) noexcept;
			// Dictionary nvim_get_hl_by_name(String name, Boolean rgb, )
	NeovimQt::MsgpackRequest* nvim_get_hl_by_name(QByteArray name, bool rgb) noexcept;
			// Dictionary nvim_get_hl_by_id(Integer hl_id, Boolean rgb, )
	NeovimQt::MsgpackRequest* nvim_get_hl_by_id(int64_t hl_id, bool rgb) noexcept;
			// void nvim_feedkeys(String keys, String mode, Boolean escape_csi, )
	NeovimQt::MsgpackRequest* nvim_feedkeys(QByteArray keys, QByteArray mode, bool escape_csi) noexcept;
			// Integer nvim_input(String keys, )
	NeovimQt::MsgpackRequest* nvim_input(QByteArray keys) noexcept;
			// void nvim_input_mouse(String button, String action, String modifier, Integer grid, Integer row, Integer col, )
	NeovimQt::MsgpackRequest* nvim_input_mouse(QByteArray button, QByteArray action, QByteArray modifier, int64_t grid, int64_t row, int64_t col) noexcept;
			// String nvim_replace_termcodes(String str, Boolean from_part, Boolean do_lt, Boolean special, )
	NeovimQt::MsgpackRequest* nvim_replace_termcodes(QByteArray str, bool from_part, bool do_lt, bool special) noexcept;
			// String nvim_command_output(String command, )
	NeovimQt::MsgpackRequest* nvim_command_output(QByteArray command) noexcept;
			// Object nvim_eval(String expr, )
	NeovimQt::MsgpackRequest* nvim_eval(QByteArray expr) noexcept;
			// Object nvim_execute_lua(String code, Array args, )
	NeovimQt::MsgpackRequest* nvim_execute_lua(QByteArray code, QVariantList args) noexcept;
			// Object nvim_call_function(String fn, Array args, )
	NeovimQt::MsgpackRequest* nvim_call_function(QByteArray fn, QVariantList args) noexcept;
			// Object nvim_call_dict_function(Object dict, String fn, Array args, )
	NeovimQt::MsgpackRequest* nvim_call_dict_function(QVariant dict, QByteArray fn, QVariantList args) noexcept;
			// Integer nvim_strwidth(String text, )
	NeovimQt::MsgpackRequest* nvim_strwidth(QByteArray text) noexcept;
			// ArrayOf(String) nvim_list_runtime_paths()
	NeovimQt::MsgpackRequest* nvim_list_runtime_paths() noexcept;
			// void nvim_set_current_dir(String dir, )
	NeovimQt::MsgpackRequest* nvim_set_current_dir(QByteArray dir) noexcept;
			// String nvim_get_current_line()
	NeovimQt::MsgpackRequest* nvim_get_current_line() noexcept;
			// void nvim_set_current_line(String line, )
	NeovimQt::MsgpackRequest* nvim_set_current_line(QByteArray line) noexcept;
			// void nvim_del_current_line()
	NeovimQt::MsgpackRequest* nvim_del_current_line() noexcept;
			// Object nvim_get_var(String name, )
	NeovimQt::MsgpackRequest* nvim_get_var(QByteArray name) noexcept;
			// void nvim_set_var(String name, Object value, )
	NeovimQt::MsgpackRequest* nvim_set_var(QByteArray name, QVariant value) noexcept;
			// void nvim_del_var(String name, )
	NeovimQt::MsgpackRequest* nvim_del_var(QByteArray name) noexcept;
			// DEPRECATED
		// Object vim_set_var(String name, Object value, )
	NeovimQt::MsgpackRequest* vim_set_var(QByteArray name, QVariant value) noexcept;
			// DEPRECATED
		// Object vim_del_var(String name, )
	NeovimQt::MsgpackRequest* vim_del_var(QByteArray name) noexcept;
			// Object nvim_get_vvar(String name, )
	NeovimQt::MsgpackRequest* nvim_get_vvar(QByteArray name) noexcept;
			// void nvim_set_vvar(String name, Object value, )
	NeovimQt::MsgpackRequest* nvim_set_vvar(QByteArray name, QVariant value) noexcept;
			// Object nvim_get_option(String name, )
	NeovimQt::MsgpackRequest* nvim_get_option(QByteArray name) noexcept;
			// void nvim_set_option(String name, Object value, )
	NeovimQt::MsgpackRequest* nvim_set_option(QByteArray name, QVariant value) noexcept;
			// void nvim_out_write(String str, )
	NeovimQt::MsgpackRequest* nvim_out_write(QByteArray str) noexcept;
			// void nvim_err_write(String str, )
	NeovimQt::MsgpackRequest* nvim_err_write(QByteArray str) noexcept;
			// void nvim_err_writeln(String str, )
	NeovimQt::MsgpackRequest* nvim_err_writeln(QByteArray str) noexcept;
			// ArrayOf(Buffer) nvim_list_bufs()
	NeovimQt::MsgpackRequest* nvim_list_bufs() noexcept;
			// Buffer nvim_get_current_buf()
	NeovimQt::MsgpackRequest* nvim_get_current_buf() noexcept;
			// void nvim_set_current_buf(Buffer buffer, )
	NeovimQt::MsgpackRequest* nvim_set_current_buf(int64_t buffer) noexcept;
			// ArrayOf(Window) nvim_list_wins()
	NeovimQt::MsgpackRequest* nvim_list_wins() noexcept;
			// Window nvim_get_current_win()
	NeovimQt::MsgpackRequest* nvim_get_current_win() noexcept;
			// void nvim_set_current_win(Window window, )
	NeovimQt::MsgpackRequest* nvim_set_current_win(int64_t window) noexcept;
			// Buffer nvim_create_buf(Boolean listed, Boolean scratch, )
	NeovimQt::MsgpackRequest* nvim_create_buf(bool listed, bool scratch) noexcept;
			// Window nvim_open_win(Buffer buffer, Boolean enter, Dictionary config, )
	NeovimQt::MsgpackRequest* nvim_open_win(int64_t buffer, bool enter, QVariantMap config) noexcept;
			// ArrayOf(Tabpage) nvim_list_tabpages()
	NeovimQt::MsgpackRequest* nvim_list_tabpages() noexcept;
			// Tabpage nvim_get_current_tabpage()
	NeovimQt::MsgpackRequest* nvim_get_current_tabpage() noexcept;
			// void nvim_set_current_tabpage(Tabpage tabpage, )
	NeovimQt::MsgpackRequest* nvim_set_current_tabpage(int64_t tabpage) noexcept;
			// Integer nvim_create_namespace(String name, )
	NeovimQt::MsgpackRequest* nvim_create_namespace(QByteArray name) noexcept;
			// Dictionary nvim_get_namespaces()
	NeovimQt::MsgpackRequest* nvim_get_namespaces() noexcept;
			// Boolean nvim_paste(String data, Boolean crlf, Integer phase, )
	NeovimQt::MsgpackRequest* nvim_paste(QByteArray data, bool crlf, int64_t phase) noexcept;
			// void nvim_put(ArrayOf(String) lines, String type, Boolean after, Boolean follow, )
	NeovimQt::MsgpackRequest* nvim_put(QList<QByteArray> lines, QByteArray type, bool after, bool follow) noexcept;
			// void nvim_subscribe(String event, )
	NeovimQt::MsgpackRequest* nvim_subscribe(QByteArray event) noexcept;
			// void nvim_unsubscribe(String event, )
	NeovimQt::MsgpackRequest* nvim_unsubscribe(QByteArray event) noexcept;
			// Integer nvim_get_color_by_name(String name, )
	NeovimQt::MsgpackRequest* nvim_get_color_by_name(QByteArray name) noexcept;
			// Dictionary nvim_get_color_map()
	NeovimQt::MsgpackRequest* nvim_get_color_map() noexcept;
			// Dictionary nvim_get_context(Dictionary opts, )
	NeovimQt::MsgpackRequest* nvim_get_context(QVariantMap opts) noexcept;
			// Object nvim_load_context(Dictionary dict, )
	NeovimQt::MsgpackRequest* nvim_load_context(QVariantMap dict) noexcept;
			// Dictionary nvim_get_mode()
	NeovimQt::MsgpackRequest* nvim_get_mode() noexcept;
			// ArrayOf(Dictionary) nvim_get_keymap(String mode, )
	NeovimQt::MsgpackRequest* nvim_get_keymap(QByteArray mode) noexcept;
			// void nvim_set_keymap(String mode, String lhs, String rhs, Dictionary opts, )
	NeovimQt::MsgpackRequest* nvim_set_keymap(QByteArray mode, QByteArray lhs, QByteArray rhs, QVariantMap opts) noexcept;
			// void nvim_del_keymap(String mode, String lhs, )
	NeovimQt::MsgpackRequest* nvim_del_keymap(QByteArray mode, QByteArray lhs) noexcept;
			// Dictionary nvim_get_commands(Dictionary opts, )
	NeovimQt::MsgpackRequest* nvim_get_commands(QVariantMap opts) noexcept;
			// Array nvim_get_api_info()
	NeovimQt::MsgpackRequest* nvim_get_api_info() noexcept;
			// void nvim_set_client_info(String name, Dictionary version, String type, Dictionary methods, Dictionary attributes, )
	NeovimQt::MsgpackRequest* nvim_set_client_info(QByteArray name, QVariantMap version, QByteArray type, QVariantMap methods, QVariantMap attributes) noexcept;
			// Dictionary nvim_get_chan_info(Integer chan, )
	NeovimQt::MsgpackRequest* nvim_get_chan_info(int64_t chan) noexcept;
			// Array nvim_list_chans()
	NeovimQt::MsgpackRequest* nvim_list_chans() noexcept;
			// Array nvim_call_atomic(Array calls, )
	NeovimQt::MsgpackRequest* nvim_call_atomic(QVariantList calls) noexcept;
			// Dictionary nvim_parse_expression(String expr, String flags, Boolean highlight, )
	NeovimQt::MsgpackRequest* nvim_parse_expression(QByteArray expr, QByteArray flags, bool highlight) noexcept;
			// Array nvim_list_uis()
	NeovimQt::MsgpackRequest* nvim_list_uis() noexcept;
			// Array nvim_get_proc_children(Integer pid, )
	NeovimQt::MsgpackRequest* nvim_get_proc_children(int64_t pid) noexcept;
			// Object nvim_get_proc(Integer pid, )
	NeovimQt::MsgpackRequest* nvim_get_proc(int64_t pid) noexcept;
			// void nvim_select_popupmenu_item(Integer item, Boolean insert, Boolean finish, Dictionary opts, )
	NeovimQt::MsgpackRequest* nvim_select_popupmenu_item(int64_t item, bool insert, bool finish, QVariantMap opts) noexcept;
			// Buffer nvim_win_get_buf(Window window, )
	NeovimQt::MsgpackRequest* nvim_win_get_buf(int64_t window) noexcept;
			// void nvim_win_set_buf(Window window, Buffer buffer, )
	NeovimQt::MsgpackRequest* nvim_win_set_buf(int64_t window, int64_t buffer) noexcept;
			// ArrayOf(Integer, 2) nvim_win_get_cursor(Window window, )
	NeovimQt::MsgpackRequest* nvim_win_get_cursor(int64_t window) noexcept;
			// void nvim_win_set_cursor(Window window, ArrayOf(Integer, 2) pos, )
	NeovimQt::MsgpackRequest* nvim_win_set_cursor(int64_t window, QPoint pos) noexcept;
			// Integer nvim_win_get_height(Window window, )
	NeovimQt::MsgpackRequest* nvim_win_get_height(int64_t window) noexcept;
			// void nvim_win_set_height(Window window, Integer height, )
	NeovimQt::MsgpackRequest* nvim_win_set_height(int64_t window, int64_t height) noexcept;
			// Integer nvim_win_get_width(Window window, )
	NeovimQt::MsgpackRequest* nvim_win_get_width(int64_t window) noexcept;
			// void nvim_win_set_width(Window window, Integer width, )
	NeovimQt::MsgpackRequest* nvim_win_set_width(int64_t window, int64_t width) noexcept;
			// Object nvim_win_get_var(Window window, String name, )
	NeovimQt::MsgpackRequest* nvim_win_get_var(int64_t window, QByteArray name) noexcept;
			// void nvim_win_set_var(Window window, String name, Object value, )
	NeovimQt::MsgpackRequest* nvim_win_set_var(int64_t window, QByteArray name, QVariant value) noexcept;
			// void nvim_win_del_var(Window window, String name, )
	NeovimQt::MsgpackRequest* nvim_win_del_var(int64_t window, QByteArray name) noexcept;
			// DEPRECATED
		// Object window_set_var(Window window, String name, Object value, )
	NeovimQt::MsgpackRequest* window_set_var(int64_t window, QByteArray name, QVariant value) noexcept;
			// DEPRECATED
		// Object window_del_var(Window window, String name, )
	NeovimQt::MsgpackRequest* window_del_var(int64_t window, QByteArray name) noexcept;
			// Object nvim_win_get_option(Window window, String name, )
	NeovimQt::MsgpackRequest* nvim_win_get_option(int64_t window, QByteArray name) noexcept;
			// void nvim_win_set_option(Window window, String name, Object value, )
	NeovimQt::MsgpackRequest* nvim_win_set_option(int64_t window, QByteArray name, QVariant value) noexcept;
			// ArrayOf(Integer, 2) nvim_win_get_position(Window window, )
	NeovimQt::MsgpackRequest* nvim_win_get_position(int64_t window) noexcept;
			// Tabpage nvim_win_get_tabpage(Window window, )
	NeovimQt::MsgpackRequest* nvim_win_get_tabpage(int64_t window) noexcept;
			// Integer nvim_win_get_number(Window window, )
	NeovimQt::MsgpackRequest* nvim_win_get_number(int64_t window) noexcept;
			// Boolean nvim_win_is_valid(Window window, )
	NeovimQt::MsgpackRequest* nvim_win_is_valid(int64_t window) noexcept;
			// void nvim_win_set_config(Window window, Dictionary config, )
	NeovimQt::MsgpackRequest* nvim_win_set_config(int64_t window, QVariantMap config) noexcept;
			// Dictionary nvim_win_get_config(Window window, )
	NeovimQt::MsgpackRequest* nvim_win_get_config(int64_t window) noexcept;
			// void nvim_win_close(Window window, Boolean force, )
	NeovimQt::MsgpackRequest* nvim_win_close(int64_t window, bool force) noexcept;
			// DEPRECATED
		// Integer buffer_line_count(Buffer buffer, )
	NeovimQt::MsgpackRequest* buffer_line_count(int64_t buffer) noexcept;
			// DEPRECATED
		// ArrayOf(String) buffer_get_lines(Buffer buffer, Integer start, Integer end, Boolean strict_indexing, )
	NeovimQt::MsgpackRequest* buffer_get_lines(int64_t buffer, int64_t start, int64_t end, bool strict_indexing) noexcept;
			// DEPRECATED
		// void buffer_set_lines(Buffer buffer, Integer start, Integer end, Boolean strict_indexing, ArrayOf(String) replacement, )
	NeovimQt::MsgpackRequest* buffer_set_lines(int64_t buffer, int64_t start, int64_t end, bool strict_indexing, QList<QByteArray> replacement) noexcept;
			// DEPRECATED
		// Object buffer_get_var(Buffer buffer, String name, )
	NeovimQt::MsgpackRequest* buffer_get_var(int64_t buffer, QByteArray name) noexcept;
			// DEPRECATED
		// Object buffer_get_option(Buffer buffer, String name, )
	NeovimQt::MsgpackRequest* buffer_get_option(int64_t buffer, QByteArray name) noexcept;
			// DEPRECATED
		// void buffer_set_option(Buffer buffer, String name, Object value, )
	NeovimQt::MsgpackRequest* buffer_set_option(int64_t buffer, QByteArray name, QVariant value) noexcept;
			// DEPRECATED
		// Integer buffer_get_number(Buffer buffer, )
	NeovimQt::MsgpackRequest* buffer_get_number(int64_t buffer) noexcept;
			// DEPRECATED
		// String buffer_get_name(Buffer buffer, )
	NeovimQt::MsgpackRequest* buffer_get_name(int64_t buffer) noexcept;
			// DEPRECATED
		// void buffer_set_name(Buffer buffer, String name, )
	NeovimQt::MsgpackRequest* buffer_set_name(int64_t buffer, QByteArray name) noexcept;
			// DEPRECATED
		// Boolean buffer_is_valid(Buffer buffer, )
	NeovimQt::MsgpackRequest* buffer_is_valid(int64_t buffer) noexcept;
			// DEPRECATED
		// ArrayOf(Integer, 2) buffer_get_mark(Buffer buffer, String name, )
	NeovimQt::MsgpackRequest* buffer_get_mark(int64_t buffer, QByteArray name) noexcept;
			// DEPRECATED
		// Integer buffer_add_highlight(Buffer buffer, Integer ns_id, String hl_group, Integer line, Integer col_start, Integer col_end, )
	NeovimQt::MsgpackRequest* buffer_add_highlight(int64_t buffer, int64_t ns_id, QByteArray hl_group, int64_t line, int64_t col_start, int64_t col_end) noexcept;
			// DEPRECATED
		// void buffer_clear_highlight(Buffer buffer, Integer ns_id, Integer line_start, Integer line_end, )
	NeovimQt::MsgpackRequest* buffer_clear_highlight(int64_t buffer, int64_t ns_id, int64_t line_start, int64_t line_end) noexcept;
			// DEPRECATED
		// ArrayOf(Window) tabpage_get_windows(Tabpage tabpage, )
	NeovimQt::MsgpackRequest* tabpage_get_windows(int64_t tabpage) noexcept;
			// DEPRECATED
		// Object tabpage_get_var(Tabpage tabpage, String name, )
	NeovimQt::MsgpackRequest* tabpage_get_var(int64_t tabpage, QByteArray name) noexcept;
			// DEPRECATED
		// Window tabpage_get_window(Tabpage tabpage, )
	NeovimQt::MsgpackRequest* tabpage_get_window(int64_t tabpage) noexcept;
			// DEPRECATED
		// Boolean tabpage_is_valid(Tabpage tabpage, )
	NeovimQt::MsgpackRequest* tabpage_is_valid(int64_t tabpage) noexcept;
			// DEPRECATED
		// void ui_detach()
	NeovimQt::MsgpackRequest* ui_detach() noexcept;
			// DEPRECATED
		// Object ui_try_resize(Integer width, Integer height, )
	NeovimQt::MsgpackRequest* ui_try_resize(int64_t width, int64_t height) noexcept;
			// DEPRECATED
		// void vim_command(String command, )
	NeovimQt::MsgpackRequest* vim_command(QByteArray command) noexcept;
			// DEPRECATED
		// void vim_feedkeys(String keys, String mode, Boolean escape_csi, )
	NeovimQt::MsgpackRequest* vim_feedkeys(QByteArray keys, QByteArray mode, bool escape_csi) noexcept;
			// DEPRECATED
		// Integer vim_input(String keys, )
	NeovimQt::MsgpackRequest* vim_input(QByteArray keys) noexcept;
			// DEPRECATED
		// String vim_replace_termcodes(String str, Boolean from_part, Boolean do_lt, Boolean special, )
	NeovimQt::MsgpackRequest* vim_replace_termcodes(QByteArray str, bool from_part, bool do_lt, bool special) noexcept;
			// DEPRECATED
		// String vim_command_output(String command, )
	NeovimQt::MsgpackRequest* vim_command_output(QByteArray command) noexcept;
			// DEPRECATED
		// Object vim_eval(String expr, )
	NeovimQt::MsgpackRequest* vim_eval(QByteArray expr) noexcept;
			// DEPRECATED
		// Object vim_call_function(String fn, Array args, )
	NeovimQt::MsgpackRequest* vim_call_function(QByteArray fn, QVariantList args) noexcept;
			// DEPRECATED
		// Integer vim_strwidth(String text, )
	NeovimQt::MsgpackRequest* vim_strwidth(QByteArray text) noexcept;
			// DEPRECATED
		// ArrayOf(String) vim_list_runtime_paths()
	NeovimQt::MsgpackRequest* vim_list_runtime_paths() noexcept;
			// DEPRECATED
		// void vim_change_directory(String dir, )
	NeovimQt::MsgpackRequest* vim_change_directory(QByteArray dir) noexcept;
			// DEPRECATED
		// String vim_get_current_line()
	NeovimQt::MsgpackRequest* vim_get_current_line() noexcept;
			// DEPRECATED
		// void vim_set_current_line(String line, )
	NeovimQt::MsgpackRequest* vim_set_current_line(QByteArray line) noexcept;
			// DEPRECATED
		// void vim_del_current_line()
	NeovimQt::MsgpackRequest* vim_del_current_line() noexcept;
			// DEPRECATED
		// Object vim_get_var(String name, )
	NeovimQt::MsgpackRequest* vim_get_var(QByteArray name) noexcept;
			// DEPRECATED
		// Object vim_get_vvar(String name, )
	NeovimQt::MsgpackRequest* vim_get_vvar(QByteArray name) noexcept;
			// DEPRECATED
		// Object vim_get_option(String name, )
	NeovimQt::MsgpackRequest* vim_get_option(QByteArray name) noexcept;
			// DEPRECATED
		// void vim_set_option(String name, Object value, )
	NeovimQt::MsgpackRequest* vim_set_option(QByteArray name, QVariant value) noexcept;
			// DEPRECATED
		// void vim_out_write(String str, )
	NeovimQt::MsgpackRequest* vim_out_write(QByteArray str) noexcept;
			// DEPRECATED
		// void vim_err_write(String str, )
	NeovimQt::MsgpackRequest* vim_err_write(QByteArray str) noexcept;
			// DEPRECATED
		// void vim_report_error(String str, )
	NeovimQt::MsgpackRequest* vim_report_error(QByteArray str) noexcept;
			// DEPRECATED
		// ArrayOf(Buffer) vim_get_buffers()
	NeovimQt::MsgpackRequest* vim_get_buffers() noexcept;
			// DEPRECATED
		// Buffer vim_get_current_buffer()
	NeovimQt::MsgpackRequest* vim_get_current_buffer() noexcept;
			// DEPRECATED
		// void vim_set_current_buffer(Buffer buffer, )
	NeovimQt::MsgpackRequest* vim_set_current_buffer(int64_t buffer) noexcept;
			// DEPRECATED
		// ArrayOf(Window) vim_get_windows()
	NeovimQt::MsgpackRequest* vim_get_windows() noexcept;
			// DEPRECATED
		// Window vim_get_current_window()
	NeovimQt::MsgpackRequest* vim_get_current_window() noexcept;
			// DEPRECATED
		// void vim_set_current_window(Window window, )
	NeovimQt::MsgpackRequest* vim_set_current_window(int64_t window) noexcept;
			// DEPRECATED
		// ArrayOf(Tabpage) vim_get_tabpages()
	NeovimQt::MsgpackRequest* vim_get_tabpages() noexcept;
			// DEPRECATED
		// Tabpage vim_get_current_tabpage()
	NeovimQt::MsgpackRequest* vim_get_current_tabpage() noexcept;
			// DEPRECATED
		// void vim_set_current_tabpage(Tabpage tabpage, )
	NeovimQt::MsgpackRequest* vim_set_current_tabpage(int64_t tabpage) noexcept;
			// DEPRECATED
		// void vim_subscribe(String event, )
	NeovimQt::MsgpackRequest* vim_subscribe(QByteArray event) noexcept;
			// DEPRECATED
		// void vim_unsubscribe(String event, )
	NeovimQt::MsgpackRequest* vim_unsubscribe(QByteArray event) noexcept;
			// DEPRECATED
		// Integer vim_name_to_color(String name, )
	NeovimQt::MsgpackRequest* vim_name_to_color(QByteArray name) noexcept;
			// DEPRECATED
		// Dictionary vim_get_color_map()
	NeovimQt::MsgpackRequest* vim_get_color_map() noexcept;
			// DEPRECATED
		// Buffer window_get_buffer(Window window, )
	NeovimQt::MsgpackRequest* window_get_buffer(int64_t window) noexcept;
			// DEPRECATED
		// ArrayOf(Integer, 2) window_get_cursor(Window window, )
	NeovimQt::MsgpackRequest* window_get_cursor(int64_t window) noexcept;
			// DEPRECATED
		// void window_set_cursor(Window window, ArrayOf(Integer, 2) pos, )
	NeovimQt::MsgpackRequest* window_set_cursor(int64_t window, QPoint pos) noexcept;
			// DEPRECATED
		// Integer window_get_height(Window window, )
	NeovimQt::MsgpackRequest* window_get_height(int64_t window) noexcept;
			// DEPRECATED
		// void window_set_height(Window window, Integer height, )
	NeovimQt::MsgpackRequest* window_set_height(int64_t window, int64_t height) noexcept;
			// DEPRECATED
		// Integer window_get_width(Window window, )
	NeovimQt::MsgpackRequest* window_get_width(int64_t window) noexcept;
			// DEPRECATED
		// void window_set_width(Window window, Integer width, )
	NeovimQt::MsgpackRequest* window_set_width(int64_t window, int64_t width) noexcept;
			// DEPRECATED
		// Object window_get_var(Window window, String name, )
	NeovimQt::MsgpackRequest* window_get_var(int64_t window, QByteArray name) noexcept;
			// DEPRECATED
		// Object window_get_option(Window window, String name, )
	NeovimQt::MsgpackRequest* window_get_option(int64_t window, QByteArray name) noexcept;
			// DEPRECATED
		// void window_set_option(Window window, String name, Object value, )
	NeovimQt::MsgpackRequest* window_set_option(int64_t window, QByteArray name, QVariant value) noexcept;
			// DEPRECATED
		// ArrayOf(Integer, 2) window_get_position(Window window, )
	NeovimQt::MsgpackRequest* window_get_position(int64_t window) noexcept;
			// DEPRECATED
		// Tabpage window_get_tabpage(Window window, )
	NeovimQt::MsgpackRequest* window_get_tabpage(int64_t window) noexcept;
			// DEPRECATED
		// Boolean window_is_valid(Window window, )
	NeovimQt::MsgpackRequest* window_is_valid(int64_t window) noexcept;
	
signals:
	void error(const QString& errmsg, const QVariant& errObj);
	void neovimNotification(const QByteArray &name, const QVariantList& args);

		void on_nvim_buf_line_count(int64_t);
	void err_nvim_buf_line_count(const QString&, const QVariant&);
		void on_buffer_get_line(QByteArray);
	void err_buffer_get_line(const QString&, const QVariant&);
		void on_nvim_buf_attach(bool);
	void err_nvim_buf_attach(const QString&, const QVariant&);
		void on_nvim_buf_detach(bool);
	void err_nvim_buf_detach(const QString&, const QVariant&);
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
		void on_nvim_buf_get_offset(int64_t);
	void err_nvim_buf_get_offset(const QString&, const QVariant&);
		void on_nvim_buf_get_var(QVariant);
	void err_nvim_buf_get_var(const QString&, const QVariant&);
		void on_nvim_buf_get_changedtick(int64_t);
	void err_nvim_buf_get_changedtick(const QString&, const QVariant&);
		void on_nvim_buf_get_keymap(QList<QVariantMap>);
	void err_nvim_buf_get_keymap(const QString&, const QVariant&);
		void on_nvim_buf_set_keymap(void);
	void err_nvim_buf_set_keymap(const QString&, const QVariant&);
		void on_nvim_buf_del_keymap(void);
	void err_nvim_buf_del_keymap(const QString&, const QVariant&);
		void on_nvim_buf_get_commands(QVariantMap);
	void err_nvim_buf_get_commands(const QString&, const QVariant&);
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
		void on_nvim_buf_is_loaded(bool);
	void err_nvim_buf_is_loaded(const QString&, const QVariant&);
		void on_nvim_buf_is_valid(bool);
	void err_nvim_buf_is_valid(const QString&, const QVariant&);
		void on_buffer_insert(void);
	void err_buffer_insert(const QString&, const QVariant&);
		void on_nvim_buf_get_mark(QPoint);
	void err_nvim_buf_get_mark(const QString&, const QVariant&);
		void on_nvim_buf_add_highlight(int64_t);
	void err_nvim_buf_add_highlight(const QString&, const QVariant&);
		void on_nvim_buf_clear_namespace(void);
	void err_nvim_buf_clear_namespace(const QString&, const QVariant&);
		void on_nvim_buf_clear_highlight(void);
	void err_nvim_buf_clear_highlight(const QString&, const QVariant&);
		void on_nvim_buf_set_virtual_text(int64_t);
	void err_nvim_buf_set_virtual_text(const QString&, const QVariant&);
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
		void on_nvim_ui_try_resize_grid(void);
	void err_nvim_ui_try_resize_grid(const QString&, const QVariant&);
		void on_nvim_ui_pum_set_height(void);
	void err_nvim_ui_pum_set_height(const QString&, const QVariant&);
		void on_nvim_command(void);
	void err_nvim_command(const QString&, const QVariant&);
		void on_nvim_get_hl_by_name(QVariantMap);
	void err_nvim_get_hl_by_name(const QString&, const QVariant&);
		void on_nvim_get_hl_by_id(QVariantMap);
	void err_nvim_get_hl_by_id(const QString&, const QVariant&);
		void on_nvim_feedkeys(void);
	void err_nvim_feedkeys(const QString&, const QVariant&);
		void on_nvim_input(int64_t);
	void err_nvim_input(const QString&, const QVariant&);
		void on_nvim_input_mouse(void);
	void err_nvim_input_mouse(const QString&, const QVariant&);
		void on_nvim_replace_termcodes(QByteArray);
	void err_nvim_replace_termcodes(const QString&, const QVariant&);
		void on_nvim_command_output(QByteArray);
	void err_nvim_command_output(const QString&, const QVariant&);
		void on_nvim_eval(QVariant);
	void err_nvim_eval(const QString&, const QVariant&);
		void on_nvim_execute_lua(QVariant);
	void err_nvim_execute_lua(const QString&, const QVariant&);
		void on_nvim_call_function(QVariant);
	void err_nvim_call_function(const QString&, const QVariant&);
		void on_nvim_call_dict_function(QVariant);
	void err_nvim_call_dict_function(const QString&, const QVariant&);
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
		void on_nvim_set_vvar(void);
	void err_nvim_set_vvar(const QString&, const QVariant&);
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
		void on_nvim_create_buf(int64_t);
	void err_nvim_create_buf(const QString&, const QVariant&);
		void on_nvim_open_win(int64_t);
	void err_nvim_open_win(const QString&, const QVariant&);
		void on_nvim_list_tabpages(QList<int64_t>);
	void err_nvim_list_tabpages(const QString&, const QVariant&);
		void on_nvim_get_current_tabpage(int64_t);
	void err_nvim_get_current_tabpage(const QString&, const QVariant&);
		void on_nvim_set_current_tabpage(void);
	void err_nvim_set_current_tabpage(const QString&, const QVariant&);
		void on_nvim_create_namespace(int64_t);
	void err_nvim_create_namespace(const QString&, const QVariant&);
		void on_nvim_get_namespaces(QVariantMap);
	void err_nvim_get_namespaces(const QString&, const QVariant&);
		void on_nvim_paste(bool);
	void err_nvim_paste(const QString&, const QVariant&);
		void on_nvim_put(void);
	void err_nvim_put(const QString&, const QVariant&);
		void on_nvim_subscribe(void);
	void err_nvim_subscribe(const QString&, const QVariant&);
		void on_nvim_unsubscribe(void);
	void err_nvim_unsubscribe(const QString&, const QVariant&);
		void on_nvim_get_color_by_name(int64_t);
	void err_nvim_get_color_by_name(const QString&, const QVariant&);
		void on_nvim_get_color_map(QVariantMap);
	void err_nvim_get_color_map(const QString&, const QVariant&);
		void on_nvim_get_context(QVariantMap);
	void err_nvim_get_context(const QString&, const QVariant&);
		void on_nvim_load_context(QVariant);
	void err_nvim_load_context(const QString&, const QVariant&);
		void on_nvim_get_mode(QVariantMap);
	void err_nvim_get_mode(const QString&, const QVariant&);
		void on_nvim_get_keymap(QList<QVariantMap>);
	void err_nvim_get_keymap(const QString&, const QVariant&);
		void on_nvim_set_keymap(void);
	void err_nvim_set_keymap(const QString&, const QVariant&);
		void on_nvim_del_keymap(void);
	void err_nvim_del_keymap(const QString&, const QVariant&);
		void on_nvim_get_commands(QVariantMap);
	void err_nvim_get_commands(const QString&, const QVariant&);
		void on_nvim_get_api_info(QVariantList);
	void err_nvim_get_api_info(const QString&, const QVariant&);
		void on_nvim_set_client_info(void);
	void err_nvim_set_client_info(const QString&, const QVariant&);
		void on_nvim_get_chan_info(QVariantMap);
	void err_nvim_get_chan_info(const QString&, const QVariant&);
		void on_nvim_list_chans(QVariantList);
	void err_nvim_list_chans(const QString&, const QVariant&);
		void on_nvim_call_atomic(QVariantList);
	void err_nvim_call_atomic(const QString&, const QVariant&);
		void on_nvim_parse_expression(QVariantMap);
	void err_nvim_parse_expression(const QString&, const QVariant&);
		void on_nvim_list_uis(QVariantList);
	void err_nvim_list_uis(const QString&, const QVariant&);
		void on_nvim_get_proc_children(QVariantList);
	void err_nvim_get_proc_children(const QString&, const QVariant&);
		void on_nvim_get_proc(QVariant);
	void err_nvim_get_proc(const QString&, const QVariant&);
		void on_nvim_select_popupmenu_item(void);
	void err_nvim_select_popupmenu_item(const QString&, const QVariant&);
		void on_nvim_win_get_buf(int64_t);
	void err_nvim_win_get_buf(const QString&, const QVariant&);
		void on_nvim_win_set_buf(void);
	void err_nvim_win_set_buf(const QString&, const QVariant&);
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
		void on_nvim_win_set_config(void);
	void err_nvim_win_set_config(const QString&, const QVariant&);
		void on_nvim_win_get_config(QVariantMap);
	void err_nvim_win_get_config(const QString&, const QVariant&);
		void on_nvim_win_close(void);
	void err_nvim_win_close(const QString&, const QVariant&);
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
	
protected slots:
	void handleResponse(quint32 id, quint64 fun, const QVariant&) noexcept;
	void handleResponseError(quint32 id, quint64 fun, const QVariant&) noexcept;

private:
	NeovimConnector *m_c;
};

} // namespace NeovimQt