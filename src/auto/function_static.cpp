// Auto generated 2015-07-06 14:02:18.175840
const QList<Function> Function::knownFunctions = QList<Function>()
<< Function("ArrayOf(Window)", "tabpage_get_windows",
	QList<QString>()
			<< QString("Tabpage")
		, true)
<< Function("Object", "tabpage_get_var",
	QList<QString>()
			<< QString("Tabpage")
			<< QString("String")
		, true)
<< Function("Object", "tabpage_set_var",
	QList<QString>()
			<< QString("Tabpage")
			<< QString("String")
			<< QString("Object")
		, true)
<< Function("Window", "tabpage_get_window",
	QList<QString>()
			<< QString("Tabpage")
		, true)
<< Function("Boolean", "tabpage_is_valid",
	QList<QString>()
			<< QString("Tabpage")
		, false)
<< Function("Integer", "buffer_line_count",
	QList<QString>()
			<< QString("Buffer")
		, true)
<< Function("String", "buffer_get_line",
	QList<QString>()
			<< QString("Buffer")
			<< QString("Integer")
		, true)
<< Function("void", "buffer_set_line",
	QList<QString>()
			<< QString("Buffer")
			<< QString("Integer")
			<< QString("String")
		, true)
<< Function("void", "buffer_del_line",
	QList<QString>()
			<< QString("Buffer")
			<< QString("Integer")
		, true)
<< Function("ArrayOf(String)", "buffer_get_line_slice",
	QList<QString>()
			<< QString("Buffer")
			<< QString("Integer")
			<< QString("Integer")
			<< QString("Boolean")
			<< QString("Boolean")
		, true)
<< Function("void", "buffer_set_line_slice",
	QList<QString>()
			<< QString("Buffer")
			<< QString("Integer")
			<< QString("Integer")
			<< QString("Boolean")
			<< QString("Boolean")
			<< QString("ArrayOf(String)")
		, true)
<< Function("Object", "buffer_get_var",
	QList<QString>()
			<< QString("Buffer")
			<< QString("String")
		, true)
<< Function("Object", "buffer_set_var",
	QList<QString>()
			<< QString("Buffer")
			<< QString("String")
			<< QString("Object")
		, true)
<< Function("Object", "buffer_get_option",
	QList<QString>()
			<< QString("Buffer")
			<< QString("String")
		, true)
<< Function("void", "buffer_set_option",
	QList<QString>()
			<< QString("Buffer")
			<< QString("String")
			<< QString("Object")
		, true)
<< Function("Integer", "buffer_get_number",
	QList<QString>()
			<< QString("Buffer")
		, true)
<< Function("String", "buffer_get_name",
	QList<QString>()
			<< QString("Buffer")
		, true)
<< Function("void", "buffer_set_name",
	QList<QString>()
			<< QString("Buffer")
			<< QString("String")
		, true)
<< Function("Boolean", "buffer_is_valid",
	QList<QString>()
			<< QString("Buffer")
		, false)
<< Function("void", "buffer_insert",
	QList<QString>()
			<< QString("Buffer")
			<< QString("Integer")
			<< QString("ArrayOf(String)")
		, true)
<< Function("ArrayOf(Integer, 2)", "buffer_get_mark",
	QList<QString>()
			<< QString("Buffer")
			<< QString("String")
		, true)
<< Function("Buffer", "window_get_buffer",
	QList<QString>()
			<< QString("Window")
		, true)
<< Function("ArrayOf(Integer, 2)", "window_get_cursor",
	QList<QString>()
			<< QString("Window")
		, true)
<< Function("void", "window_set_cursor",
	QList<QString>()
			<< QString("Window")
			<< QString("ArrayOf(Integer, 2)")
		, true)
<< Function("Integer", "window_get_height",
	QList<QString>()
			<< QString("Window")
		, true)
<< Function("void", "window_set_height",
	QList<QString>()
			<< QString("Window")
			<< QString("Integer")
		, true)
<< Function("Integer", "window_get_width",
	QList<QString>()
			<< QString("Window")
		, true)
<< Function("void", "window_set_width",
	QList<QString>()
			<< QString("Window")
			<< QString("Integer")
		, true)
<< Function("Object", "window_get_var",
	QList<QString>()
			<< QString("Window")
			<< QString("String")
		, true)
<< Function("Object", "window_set_var",
	QList<QString>()
			<< QString("Window")
			<< QString("String")
			<< QString("Object")
		, true)
<< Function("Object", "window_get_option",
	QList<QString>()
			<< QString("Window")
			<< QString("String")
		, true)
<< Function("void", "window_set_option",
	QList<QString>()
			<< QString("Window")
			<< QString("String")
			<< QString("Object")
		, true)
<< Function("ArrayOf(Integer, 2)", "window_get_position",
	QList<QString>()
			<< QString("Window")
		, true)
<< Function("Tabpage", "window_get_tabpage",
	QList<QString>()
			<< QString("Window")
		, true)
<< Function("Boolean", "window_is_valid",
	QList<QString>()
			<< QString("Window")
		, false)
<< Function("void", "vim_command",
	QList<QString>()
			<< QString("String")
		, true)
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
		, true)
<< Function("Object", "vim_eval",
	QList<QString>()
			<< QString("String")
		, true)
<< Function("Integer", "vim_strwidth",
	QList<QString>()
			<< QString("String")
		, true)
<< Function("ArrayOf(String)", "vim_list_runtime_paths",
	QList<QString>()
		, false)
<< Function("void", "vim_change_directory",
	QList<QString>()
			<< QString("String")
		, true)
<< Function("String", "vim_get_current_line",
	QList<QString>()
		, true)
<< Function("void", "vim_set_current_line",
	QList<QString>()
			<< QString("String")
		, true)
<< Function("void", "vim_del_current_line",
	QList<QString>()
		, true)
<< Function("Object", "vim_get_var",
	QList<QString>()
			<< QString("String")
		, true)
<< Function("Object", "vim_set_var",
	QList<QString>()
			<< QString("String")
			<< QString("Object")
		, true)
<< Function("Object", "vim_get_vvar",
	QList<QString>()
			<< QString("String")
		, true)
<< Function("Object", "vim_get_option",
	QList<QString>()
			<< QString("String")
		, true)
<< Function("void", "vim_set_option",
	QList<QString>()
			<< QString("String")
			<< QString("Object")
		, true)
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
		, true)
<< Function("ArrayOf(Window)", "vim_get_windows",
	QList<QString>()
		, false)
<< Function("Window", "vim_get_current_window",
	QList<QString>()
		, false)
<< Function("void", "vim_set_current_window",
	QList<QString>()
			<< QString("Window")
		, true)
<< Function("ArrayOf(Tabpage)", "vim_get_tabpages",
	QList<QString>()
		, false)
<< Function("Tabpage", "vim_get_current_tabpage",
	QList<QString>()
		, false)
<< Function("void", "vim_set_current_tabpage",
	QList<QString>()
			<< QString("Tabpage")
		, true)
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
	;