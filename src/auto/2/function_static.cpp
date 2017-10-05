// Auto generated 2017-10-05 15:14:51.243827
const QList<Function> Function::knownFunctions = QList<Function>()
<< Function("Integer", "nvim_buf_line_count",
	QList<QString>()
			<< QString("Buffer")
		, false)
<< Function("String", "buffer_get_line",
	QList<QString>()
			<< QString("Buffer")
			<< QString("Integer")
		, false)
<< Function("void", "buffer_set_line",
	QList<QString>()
			<< QString("Buffer")
			<< QString("Integer")
			<< QString("String")
		, false)
<< Function("void", "buffer_del_line",
	QList<QString>()
			<< QString("Buffer")
			<< QString("Integer")
		, false)
<< Function("ArrayOf(String)", "buffer_get_line_slice",
	QList<QString>()
			<< QString("Buffer")
			<< QString("Integer")
			<< QString("Integer")
			<< QString("Boolean")
			<< QString("Boolean")
		, false)
<< Function("ArrayOf(String)", "nvim_buf_get_lines",
	QList<QString>()
			<< QString("Buffer")
			<< QString("Integer")
			<< QString("Integer")
			<< QString("Boolean")
		, false)
<< Function("void", "buffer_set_line_slice",
	QList<QString>()
			<< QString("Buffer")
			<< QString("Integer")
			<< QString("Integer")
			<< QString("Boolean")
			<< QString("Boolean")
			<< QString("ArrayOf(String)")
		, false)
<< Function("void", "nvim_buf_set_lines",
	QList<QString>()
			<< QString("Buffer")
			<< QString("Integer")
			<< QString("Integer")
			<< QString("Boolean")
			<< QString("ArrayOf(String)")
		, false)
<< Function("Object", "nvim_buf_get_var",
	QList<QString>()
			<< QString("Buffer")
			<< QString("String")
		, false)
<< Function("Integer", "nvim_buf_get_changedtick",
	QList<QString>()
			<< QString("Buffer")
		, false)
<< Function("void", "nvim_buf_set_var",
	QList<QString>()
			<< QString("Buffer")
			<< QString("String")
			<< QString("Object")
		, false)
<< Function("void", "nvim_buf_del_var",
	QList<QString>()
			<< QString("Buffer")
			<< QString("String")
		, false)
<< Function("Object", "buffer_set_var",
	QList<QString>()
			<< QString("Buffer")
			<< QString("String")
			<< QString("Object")
		, false)
<< Function("Object", "buffer_del_var",
	QList<QString>()
			<< QString("Buffer")
			<< QString("String")
		, false)
<< Function("Object", "nvim_buf_get_option",
	QList<QString>()
			<< QString("Buffer")
			<< QString("String")
		, false)
<< Function("void", "nvim_buf_set_option",
	QList<QString>()
			<< QString("Buffer")
			<< QString("String")
			<< QString("Object")
		, false)
<< Function("Integer", "nvim_buf_get_number",
	QList<QString>()
			<< QString("Buffer")
		, false)
<< Function("String", "nvim_buf_get_name",
	QList<QString>()
			<< QString("Buffer")
		, false)
<< Function("void", "nvim_buf_set_name",
	QList<QString>()
			<< QString("Buffer")
			<< QString("String")
		, false)
<< Function("Boolean", "nvim_buf_is_valid",
	QList<QString>()
			<< QString("Buffer")
		, false)
<< Function("void", "buffer_insert",
	QList<QString>()
			<< QString("Buffer")
			<< QString("Integer")
			<< QString("ArrayOf(String)")
		, false)
<< Function("ArrayOf(Integer, 2)", "nvim_buf_get_mark",
	QList<QString>()
			<< QString("Buffer")
			<< QString("String")
		, false)
<< Function("Integer", "nvim_buf_add_highlight",
	QList<QString>()
			<< QString("Buffer")
			<< QString("Integer")
			<< QString("String")
			<< QString("Integer")
			<< QString("Integer")
			<< QString("Integer")
		, false)
<< Function("void", "nvim_buf_clear_highlight",
	QList<QString>()
			<< QString("Buffer")
			<< QString("Integer")
			<< QString("Integer")
			<< QString("Integer")
		, false)
<< Function("ArrayOf(Window)", "nvim_tabpage_list_wins",
	QList<QString>()
			<< QString("Tabpage")
		, false)
<< Function("Object", "nvim_tabpage_get_var",
	QList<QString>()
			<< QString("Tabpage")
			<< QString("String")
		, false)
<< Function("void", "nvim_tabpage_set_var",
	QList<QString>()
			<< QString("Tabpage")
			<< QString("String")
			<< QString("Object")
		, false)
<< Function("void", "nvim_tabpage_del_var",
	QList<QString>()
			<< QString("Tabpage")
			<< QString("String")
		, false)
<< Function("Object", "tabpage_set_var",
	QList<QString>()
			<< QString("Tabpage")
			<< QString("String")
			<< QString("Object")
		, false)
<< Function("Object", "tabpage_del_var",
	QList<QString>()
			<< QString("Tabpage")
			<< QString("String")
		, false)
<< Function("Window", "nvim_tabpage_get_win",
	QList<QString>()
			<< QString("Tabpage")
		, false)
<< Function("Integer", "nvim_tabpage_get_number",
	QList<QString>()
			<< QString("Tabpage")
		, false)
<< Function("Boolean", "nvim_tabpage_is_valid",
	QList<QString>()
			<< QString("Tabpage")
		, false)
<< Function("void", "nvim_ui_attach",
	QList<QString>()
			<< QString("Integer")
			<< QString("Integer")
			<< QString("Dictionary")
		, false)
<< Function("void", "ui_attach",
	QList<QString>()
			<< QString("Integer")
			<< QString("Integer")
			<< QString("Boolean")
		, false)
<< Function("void", "nvim_ui_detach",
	QList<QString>()
		, false)
<< Function("void", "nvim_ui_try_resize",
	QList<QString>()
			<< QString("Integer")
			<< QString("Integer")
		, false)
<< Function("void", "nvim_ui_set_option",
	QList<QString>()
			<< QString("String")
			<< QString("Object")
		, false)
<< Function("void", "nvim_command",
	QList<QString>()
			<< QString("String")
		, false)
<< Function("void", "nvim_feedkeys",
	QList<QString>()
			<< QString("String")
			<< QString("String")
			<< QString("Boolean")
		, false)
<< Function("Integer", "nvim_input",
	QList<QString>()
			<< QString("String")
		, false)
<< Function("String", "nvim_replace_termcodes",
	QList<QString>()
			<< QString("String")
			<< QString("Boolean")
			<< QString("Boolean")
			<< QString("Boolean")
		, false)
<< Function("String", "nvim_command_output",
	QList<QString>()
			<< QString("String")
		, false)
<< Function("Object", "nvim_eval",
	QList<QString>()
			<< QString("String")
		, false)
<< Function("Object", "nvim_call_function",
	QList<QString>()
			<< QString("String")
			<< QString("Array")
		, false)
<< Function("Integer", "nvim_strwidth",
	QList<QString>()
			<< QString("String")
		, false)
<< Function("ArrayOf(String)", "nvim_list_runtime_paths",
	QList<QString>()
		, false)
<< Function("void", "nvim_set_current_dir",
	QList<QString>()
			<< QString("String")
		, false)
<< Function("String", "nvim_get_current_line",
	QList<QString>()
		, false)
<< Function("void", "nvim_set_current_line",
	QList<QString>()
			<< QString("String")
		, false)
<< Function("void", "nvim_del_current_line",
	QList<QString>()
		, false)
<< Function("Object", "nvim_get_var",
	QList<QString>()
			<< QString("String")
		, false)
<< Function("void", "nvim_set_var",
	QList<QString>()
			<< QString("String")
			<< QString("Object")
		, false)
<< Function("void", "nvim_del_var",
	QList<QString>()
			<< QString("String")
		, false)
<< Function("Object", "vim_set_var",
	QList<QString>()
			<< QString("String")
			<< QString("Object")
		, false)
<< Function("Object", "vim_del_var",
	QList<QString>()
			<< QString("String")
		, false)
<< Function("Object", "nvim_get_vvar",
	QList<QString>()
			<< QString("String")
		, false)
<< Function("Object", "nvim_get_option",
	QList<QString>()
			<< QString("String")
		, false)
<< Function("void", "nvim_set_option",
	QList<QString>()
			<< QString("String")
			<< QString("Object")
		, false)
<< Function("void", "nvim_out_write",
	QList<QString>()
			<< QString("String")
		, false)
<< Function("void", "nvim_err_write",
	QList<QString>()
			<< QString("String")
		, false)
<< Function("void", "nvim_err_writeln",
	QList<QString>()
			<< QString("String")
		, false)
<< Function("ArrayOf(Buffer)", "nvim_list_bufs",
	QList<QString>()
		, false)
<< Function("Buffer", "nvim_get_current_buf",
	QList<QString>()
		, false)
<< Function("void", "nvim_set_current_buf",
	QList<QString>()
			<< QString("Buffer")
		, false)
<< Function("ArrayOf(Window)", "nvim_list_wins",
	QList<QString>()
		, false)
<< Function("Window", "nvim_get_current_win",
	QList<QString>()
		, false)
<< Function("void", "nvim_set_current_win",
	QList<QString>()
			<< QString("Window")
		, false)
<< Function("ArrayOf(Tabpage)", "nvim_list_tabpages",
	QList<QString>()
		, false)
<< Function("Tabpage", "nvim_get_current_tabpage",
	QList<QString>()
		, false)
<< Function("void", "nvim_set_current_tabpage",
	QList<QString>()
			<< QString("Tabpage")
		, false)
<< Function("void", "nvim_subscribe",
	QList<QString>()
			<< QString("String")
		, false)
<< Function("void", "nvim_unsubscribe",
	QList<QString>()
			<< QString("String")
		, false)
<< Function("Integer", "nvim_get_color_by_name",
	QList<QString>()
			<< QString("String")
		, false)
<< Function("Dictionary", "nvim_get_color_map",
	QList<QString>()
		, false)
<< Function("Dictionary", "nvim_get_mode",
	QList<QString>()
		, false)
<< Function("Array", "nvim_get_api_info",
	QList<QString>()
		, false)
<< Function("Array", "nvim_call_atomic",
	QList<QString>()
			<< QString("Array")
		, false)
<< Function("Buffer", "nvim_win_get_buf",
	QList<QString>()
			<< QString("Window")
		, false)
<< Function("ArrayOf(Integer, 2)", "nvim_win_get_cursor",
	QList<QString>()
			<< QString("Window")
		, false)
<< Function("void", "nvim_win_set_cursor",
	QList<QString>()
			<< QString("Window")
			<< QString("ArrayOf(Integer, 2)")
		, false)
<< Function("Integer", "nvim_win_get_height",
	QList<QString>()
			<< QString("Window")
		, false)
<< Function("void", "nvim_win_set_height",
	QList<QString>()
			<< QString("Window")
			<< QString("Integer")
		, false)
<< Function("Integer", "nvim_win_get_width",
	QList<QString>()
			<< QString("Window")
		, false)
<< Function("void", "nvim_win_set_width",
	QList<QString>()
			<< QString("Window")
			<< QString("Integer")
		, false)
<< Function("Object", "nvim_win_get_var",
	QList<QString>()
			<< QString("Window")
			<< QString("String")
		, false)
<< Function("void", "nvim_win_set_var",
	QList<QString>()
			<< QString("Window")
			<< QString("String")
			<< QString("Object")
		, false)
<< Function("void", "nvim_win_del_var",
	QList<QString>()
			<< QString("Window")
			<< QString("String")
		, false)
<< Function("Object", "window_set_var",
	QList<QString>()
			<< QString("Window")
			<< QString("String")
			<< QString("Object")
		, false)
<< Function("Object", "window_del_var",
	QList<QString>()
			<< QString("Window")
			<< QString("String")
		, false)
<< Function("Object", "nvim_win_get_option",
	QList<QString>()
			<< QString("Window")
			<< QString("String")
		, false)
<< Function("void", "nvim_win_set_option",
	QList<QString>()
			<< QString("Window")
			<< QString("String")
			<< QString("Object")
		, false)
<< Function("ArrayOf(Integer, 2)", "nvim_win_get_position",
	QList<QString>()
			<< QString("Window")
		, false)
<< Function("Tabpage", "nvim_win_get_tabpage",
	QList<QString>()
			<< QString("Window")
		, false)
<< Function("Integer", "nvim_win_get_number",
	QList<QString>()
			<< QString("Window")
		, false)
<< Function("Boolean", "nvim_win_is_valid",
	QList<QString>()
			<< QString("Window")
		, false)
<< Function("Integer", "buffer_line_count",
	QList<QString>()
			<< QString("Buffer")
		, false)
<< Function("ArrayOf(String)", "buffer_get_lines",
	QList<QString>()
			<< QString("Buffer")
			<< QString("Integer")
			<< QString("Integer")
			<< QString("Boolean")
		, false)
<< Function("void", "buffer_set_lines",
	QList<QString>()
			<< QString("Buffer")
			<< QString("Integer")
			<< QString("Integer")
			<< QString("Boolean")
			<< QString("ArrayOf(String)")
		, false)
<< Function("Object", "buffer_get_var",
	QList<QString>()
			<< QString("Buffer")
			<< QString("String")
		, false)
<< Function("Object", "buffer_get_option",
	QList<QString>()
			<< QString("Buffer")
			<< QString("String")
		, false)
<< Function("void", "buffer_set_option",
	QList<QString>()
			<< QString("Buffer")
			<< QString("String")
			<< QString("Object")
		, false)
<< Function("Integer", "buffer_get_number",
	QList<QString>()
			<< QString("Buffer")
		, false)
<< Function("String", "buffer_get_name",
	QList<QString>()
			<< QString("Buffer")
		, false)
<< Function("void", "buffer_set_name",
	QList<QString>()
			<< QString("Buffer")
			<< QString("String")
		, false)
<< Function("Boolean", "buffer_is_valid",
	QList<QString>()
			<< QString("Buffer")
		, false)
<< Function("ArrayOf(Integer, 2)", "buffer_get_mark",
	QList<QString>()
			<< QString("Buffer")
			<< QString("String")
		, false)
<< Function("Integer", "buffer_add_highlight",
	QList<QString>()
			<< QString("Buffer")
			<< QString("Integer")
			<< QString("String")
			<< QString("Integer")
			<< QString("Integer")
			<< QString("Integer")
		, false)
<< Function("void", "buffer_clear_highlight",
	QList<QString>()
			<< QString("Buffer")
			<< QString("Integer")
			<< QString("Integer")
			<< QString("Integer")
		, false)
<< Function("ArrayOf(Window)", "tabpage_get_windows",
	QList<QString>()
			<< QString("Tabpage")
		, false)
<< Function("Object", "tabpage_get_var",
	QList<QString>()
			<< QString("Tabpage")
			<< QString("String")
		, false)
<< Function("Window", "tabpage_get_window",
	QList<QString>()
			<< QString("Tabpage")
		, false)
<< Function("Boolean", "tabpage_is_valid",
	QList<QString>()
			<< QString("Tabpage")
		, false)
<< Function("void", "ui_detach",
	QList<QString>()
		, false)
<< Function("Object", "ui_try_resize",
	QList<QString>()
			<< QString("Integer")
			<< QString("Integer")
		, false)
<< Function("void", "vim_command",
	QList<QString>()
			<< QString("String")
		, false)
<< Function("void", "vim_feedkeys",
	QList<QString>()
			<< QString("String")
			<< QString("String")
			<< QString("Boolean")
		, false)
<< Function("Integer", "vim_input",
	QList<QString>()
			<< QString("String")
		, false)
<< Function("String", "vim_replace_termcodes",
	QList<QString>()
			<< QString("String")
			<< QString("Boolean")
			<< QString("Boolean")
			<< QString("Boolean")
		, false)
<< Function("String", "vim_command_output",
	QList<QString>()
			<< QString("String")
		, false)
<< Function("Object", "vim_eval",
	QList<QString>()
			<< QString("String")
		, false)
<< Function("Object", "vim_call_function",
	QList<QString>()
			<< QString("String")
			<< QString("Array")
		, false)
<< Function("Integer", "vim_strwidth",
	QList<QString>()
			<< QString("String")
		, false)
<< Function("ArrayOf(String)", "vim_list_runtime_paths",
	QList<QString>()
		, false)
<< Function("void", "vim_change_directory",
	QList<QString>()
			<< QString("String")
		, false)
<< Function("String", "vim_get_current_line",
	QList<QString>()
		, false)
<< Function("void", "vim_set_current_line",
	QList<QString>()
			<< QString("String")
		, false)
<< Function("void", "vim_del_current_line",
	QList<QString>()
		, false)
<< Function("Object", "vim_get_var",
	QList<QString>()
			<< QString("String")
		, false)
<< Function("Object", "vim_get_vvar",
	QList<QString>()
			<< QString("String")
		, false)
<< Function("Object", "vim_get_option",
	QList<QString>()
			<< QString("String")
		, false)
<< Function("void", "vim_set_option",
	QList<QString>()
			<< QString("String")
			<< QString("Object")
		, false)
<< Function("void", "vim_out_write",
	QList<QString>()
			<< QString("String")
		, false)
<< Function("void", "vim_err_write",
	QList<QString>()
			<< QString("String")
		, false)
<< Function("void", "vim_report_error",
	QList<QString>()
			<< QString("String")
		, false)
<< Function("ArrayOf(Buffer)", "vim_get_buffers",
	QList<QString>()
		, false)
<< Function("Buffer", "vim_get_current_buffer",
	QList<QString>()
		, false)
<< Function("void", "vim_set_current_buffer",
	QList<QString>()
			<< QString("Buffer")
		, false)
<< Function("ArrayOf(Window)", "vim_get_windows",
	QList<QString>()
		, false)
<< Function("Window", "vim_get_current_window",
	QList<QString>()
		, false)
<< Function("void", "vim_set_current_window",
	QList<QString>()
			<< QString("Window")
		, false)
<< Function("ArrayOf(Tabpage)", "vim_get_tabpages",
	QList<QString>()
		, false)
<< Function("Tabpage", "vim_get_current_tabpage",
	QList<QString>()
		, false)
<< Function("void", "vim_set_current_tabpage",
	QList<QString>()
			<< QString("Tabpage")
		, false)
<< Function("void", "vim_subscribe",
	QList<QString>()
			<< QString("String")
		, false)
<< Function("void", "vim_unsubscribe",
	QList<QString>()
			<< QString("String")
		, false)
<< Function("Integer", "vim_name_to_color",
	QList<QString>()
			<< QString("String")
		, false)
<< Function("Dictionary", "vim_get_color_map",
	QList<QString>()
		, false)
<< Function("Buffer", "window_get_buffer",
	QList<QString>()
			<< QString("Window")
		, false)
<< Function("ArrayOf(Integer, 2)", "window_get_cursor",
	QList<QString>()
			<< QString("Window")
		, false)
<< Function("void", "window_set_cursor",
	QList<QString>()
			<< QString("Window")
			<< QString("ArrayOf(Integer, 2)")
		, false)
<< Function("Integer", "window_get_height",
	QList<QString>()
			<< QString("Window")
		, false)
<< Function("void", "window_set_height",
	QList<QString>()
			<< QString("Window")
			<< QString("Integer")
		, false)
<< Function("Integer", "window_get_width",
	QList<QString>()
			<< QString("Window")
		, false)
<< Function("void", "window_set_width",
	QList<QString>()
			<< QString("Window")
			<< QString("Integer")
		, false)
<< Function("Object", "window_get_var",
	QList<QString>()
			<< QString("Window")
			<< QString("String")
		, false)
<< Function("Object", "window_get_option",
	QList<QString>()
			<< QString("Window")
			<< QString("String")
		, false)
<< Function("void", "window_set_option",
	QList<QString>()
			<< QString("Window")
			<< QString("String")
			<< QString("Object")
		, false)
<< Function("ArrayOf(Integer, 2)", "window_get_position",
	QList<QString>()
			<< QString("Window")
		, false)
<< Function("Tabpage", "window_get_tabpage",
	QList<QString>()
			<< QString("Window")
		, false)
<< Function("Boolean", "window_is_valid",
	QList<QString>()
			<< QString("Window")
		, false)
	;