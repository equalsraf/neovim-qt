// Auto generated Wed Oct 1 13:47:33 2014
const QList<Function> Function::knownFunctions = QList<Function>()
<< Function("ArrayOf(Window)", "tabpage_get_windows",
	QList<QString>()
		<< QString("Tabpage")
	, 1)
<< Function("Object", "tabpage_get_var",
	QList<QString>()
		<< QString("Tabpage")
		<< QString("String")
	, 1)
<< Function("Object", "tabpage_set_var",
	QList<QString>()
		<< QString("Tabpage")
		<< QString("String")
		<< QString("Object")
	, 1)
<< Function("Window", "tabpage_get_window",
	QList<QString>()
		<< QString("Tabpage")
	, 1)
<< Function("Boolean", "tabpage_is_valid",
	QList<QString>()
		<< QString("Tabpage")
	, 0)
<< Function("void", "vim_push_keys",
	QList<QString>()
		<< QString("String")
	, 0)
<< Function("void", "vim_command",
	QList<QString>()
		<< QString("String")
	, 1)
<< Function("void", "vim_feedkeys",
	QList<QString>()
		<< QString("String")
		<< QString("String")
	, 0)
<< Function("String", "vim_replace_termcodes",
	QList<QString>()
		<< QString("String")
		<< QString("Boolean")
		<< QString("Boolean")
		<< QString("Boolean")
	, 0)
<< Function("Object", "vim_eval",
	QList<QString>()
		<< QString("String")
	, 1)
<< Function("Integer", "vim_strwidth",
	QList<QString>()
		<< QString("String")
	, 1)
<< Function("ArrayOf(String)", "vim_list_runtime_paths",
	QList<QString>()
	, 0)
<< Function("void", "vim_change_directory",
	QList<QString>()
		<< QString("String")
	, 1)
<< Function("String", "vim_get_current_line",
	QList<QString>()
	, 1)
<< Function("void", "vim_set_current_line",
	QList<QString>()
		<< QString("String")
	, 1)
<< Function("void", "vim_del_current_line",
	QList<QString>()
	, 1)
<< Function("Object", "vim_get_var",
	QList<QString>()
		<< QString("String")
	, 1)
<< Function("Object", "vim_set_var",
	QList<QString>()
		<< QString("String")
		<< QString("Object")
	, 1)
<< Function("Object", "vim_get_vvar",
	QList<QString>()
		<< QString("String")
	, 1)
<< Function("Object", "vim_get_option",
	QList<QString>()
		<< QString("String")
	, 1)
<< Function("void", "vim_set_option",
	QList<QString>()
		<< QString("String")
		<< QString("Object")
	, 1)
<< Function("void", "vim_out_write",
	QList<QString>()
		<< QString("String")
	, 0)
<< Function("void", "vim_err_write",
	QList<QString>()
		<< QString("String")
	, 0)
<< Function("void", "vim_report_error",
	QList<QString>()
		<< QString("String")
	, 0)
<< Function("ArrayOf(Buffer)", "vim_get_buffers",
	QList<QString>()
	, 0)
<< Function("Buffer", "vim_get_current_buffer",
	QList<QString>()
	, 0)
<< Function("void", "vim_set_current_buffer",
	QList<QString>()
		<< QString("Buffer")
	, 1)
<< Function("ArrayOf(Window)", "vim_get_windows",
	QList<QString>()
	, 0)
<< Function("Window", "vim_get_current_window",
	QList<QString>()
	, 0)
<< Function("void", "vim_set_current_window",
	QList<QString>()
		<< QString("Window")
	, 1)
<< Function("ArrayOf(Tabpage)", "vim_get_tabpages",
	QList<QString>()
	, 0)
<< Function("Tabpage", "vim_get_current_tabpage",
	QList<QString>()
	, 0)
<< Function("void", "vim_set_current_tabpage",
	QList<QString>()
		<< QString("Tabpage")
	, 1)
<< Function("void", "vim_subscribe",
	QList<QString>()
		<< QString("String")
	, 0)
<< Function("void", "vim_unsubscribe",
	QList<QString>()
		<< QString("String")
	, 0)
<< Function("void", "vim_register_provider",
	QList<QString>()
		<< QString("String")
	, 1)
<< Function("Integer", "buffer_get_length",
	QList<QString>()
		<< QString("Buffer")
	, 1)
<< Function("String", "buffer_get_line",
	QList<QString>()
		<< QString("Buffer")
		<< QString("Integer")
	, 1)
<< Function("void", "buffer_set_line",
	QList<QString>()
		<< QString("Buffer")
		<< QString("Integer")
		<< QString("String")
	, 1)
<< Function("void", "buffer_del_line",
	QList<QString>()
		<< QString("Buffer")
		<< QString("Integer")
	, 1)
<< Function("ArrayOf(String)", "buffer_get_slice",
	QList<QString>()
		<< QString("Buffer")
		<< QString("Integer")
		<< QString("Integer")
		<< QString("Boolean")
		<< QString("Boolean")
	, 1)
<< Function("void", "buffer_set_slice",
	QList<QString>()
		<< QString("Buffer")
		<< QString("Integer")
		<< QString("Integer")
		<< QString("Boolean")
		<< QString("Boolean")
		<< QString("ArrayOf(String)")
	, 1)
<< Function("Object", "buffer_get_var",
	QList<QString>()
		<< QString("Buffer")
		<< QString("String")
	, 1)
<< Function("Object", "buffer_set_var",
	QList<QString>()
		<< QString("Buffer")
		<< QString("String")
		<< QString("Object")
	, 1)
<< Function("Object", "buffer_get_option",
	QList<QString>()
		<< QString("Buffer")
		<< QString("String")
	, 1)
<< Function("void", "buffer_set_option",
	QList<QString>()
		<< QString("Buffer")
		<< QString("String")
		<< QString("Object")
	, 1)
<< Function("Integer", "buffer_get_number",
	QList<QString>()
		<< QString("Buffer")
	, 1)
<< Function("String", "buffer_get_name",
	QList<QString>()
		<< QString("Buffer")
	, 1)
<< Function("void", "buffer_set_name",
	QList<QString>()
		<< QString("Buffer")
		<< QString("String")
	, 1)
<< Function("Boolean", "buffer_is_valid",
	QList<QString>()
		<< QString("Buffer")
	, 0)
<< Function("void", "buffer_insert",
	QList<QString>()
		<< QString("Buffer")
		<< QString("Integer")
		<< QString("ArrayOf(String)")
	, 1)
<< Function("ArrayOf(Integer, 2)", "buffer_get_mark",
	QList<QString>()
		<< QString("Buffer")
		<< QString("String")
	, 1)
<< Function("Buffer", "window_get_buffer",
	QList<QString>()
		<< QString("Window")
	, 1)
<< Function("ArrayOf(Integer, 2)", "window_get_cursor",
	QList<QString>()
		<< QString("Window")
	, 1)
<< Function("void", "window_set_cursor",
	QList<QString>()
		<< QString("Window")
		<< QString("ArrayOf(Integer, 2)")
	, 1)
<< Function("Integer", "window_get_height",
	QList<QString>()
		<< QString("Window")
	, 1)
<< Function("void", "window_set_height",
	QList<QString>()
		<< QString("Window")
		<< QString("Integer")
	, 1)
<< Function("Integer", "window_get_width",
	QList<QString>()
		<< QString("Window")
	, 1)
<< Function("void", "window_set_width",
	QList<QString>()
		<< QString("Window")
		<< QString("Integer")
	, 1)
<< Function("Object", "window_get_var",
	QList<QString>()
		<< QString("Window")
		<< QString("String")
	, 1)
<< Function("Object", "window_set_var",
	QList<QString>()
		<< QString("Window")
		<< QString("String")
		<< QString("Object")
	, 1)
<< Function("Object", "window_get_option",
	QList<QString>()
		<< QString("Window")
		<< QString("String")
	, 1)
<< Function("void", "window_set_option",
	QList<QString>()
		<< QString("Window")
		<< QString("String")
		<< QString("Object")
	, 1)
<< Function("ArrayOf(Integer, 2)", "window_get_position",
	QList<QString>()
		<< QString("Window")
	, 1)
<< Function("Tabpage", "window_get_tabpage",
	QList<QString>()
		<< QString("Window")
	, 1)
<< Function("Boolean", "window_is_valid",
	QList<QString>()
		<< QString("Window")
	, 0)
	;