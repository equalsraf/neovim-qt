const QList<Function> Function::knownFunctions = QList<Function>()
<< Function( "Integer", "buffer_get_length",
	QList<QByteArray>()
		<< QByteArray("Buffer")
	, true)
<< Function( "String", "buffer_get_line",
	QList<QByteArray>()
		<< QByteArray("Buffer")
		<< QByteArray("Integer")
	, true)
<< Function( "void", "buffer_set_line",
	QList<QByteArray>()
		<< QByteArray("Buffer")
		<< QByteArray("Integer")
		<< QByteArray("String")
	, true)
<< Function( "void", "buffer_del_line",
	QList<QByteArray>()
		<< QByteArray("Buffer")
		<< QByteArray("Integer")
	, true)
<< Function( "StringArray", "buffer_get_slice",
	QList<QByteArray>()
		<< QByteArray("Buffer")
		<< QByteArray("Integer")
		<< QByteArray("Integer")
		<< QByteArray("Boolean")
		<< QByteArray("Boolean")
	, true)
<< Function( "void", "buffer_set_slice",
	QList<QByteArray>()
		<< QByteArray("Buffer")
		<< QByteArray("Integer")
		<< QByteArray("Integer")
		<< QByteArray("Boolean")
		<< QByteArray("Boolean")
		<< QByteArray("StringArray")
	, true)
<< Function( "Object", "buffer_get_var",
	QList<QByteArray>()
		<< QByteArray("Buffer")
		<< QByteArray("String")
	, true)
<< Function( "Object", "buffer_set_var",
	QList<QByteArray>()
		<< QByteArray("Buffer")
		<< QByteArray("String")
		<< QByteArray("Object")
	, true)
<< Function( "Object", "buffer_get_option",
	QList<QByteArray>()
		<< QByteArray("Buffer")
		<< QByteArray("String")
	, true)
<< Function( "void", "buffer_set_option",
	QList<QByteArray>()
		<< QByteArray("Buffer")
		<< QByteArray("String")
		<< QByteArray("Object")
	, true)
<< Function( "String", "buffer_get_name",
	QList<QByteArray>()
		<< QByteArray("Buffer")
	, true)
<< Function( "void", "buffer_set_name",
	QList<QByteArray>()
		<< QByteArray("Buffer")
		<< QByteArray("String")
	, true)
<< Function( "Boolean", "buffer_is_valid",
	QList<QByteArray>()
		<< QByteArray("Buffer")
	, false)
<< Function( "void", "buffer_insert",
	QList<QByteArray>()
		<< QByteArray("Buffer")
		<< QByteArray("Integer")
		<< QByteArray("StringArray")
	, true)
<< Function( "Position", "buffer_get_mark",
	QList<QByteArray>()
		<< QByteArray("Buffer")
		<< QByteArray("String")
	, true)
<< Function( "Integer", "tabpage_get_window_count",
	QList<QByteArray>()
		<< QByteArray("Tabpage")
	, true)
<< Function( "Object", "tabpage_get_var",
	QList<QByteArray>()
		<< QByteArray("Tabpage")
		<< QByteArray("String")
	, true)
<< Function( "Object", "tabpage_set_var",
	QList<QByteArray>()
		<< QByteArray("Tabpage")
		<< QByteArray("String")
		<< QByteArray("Object")
	, true)
<< Function( "Window", "tabpage_get_window",
	QList<QByteArray>()
		<< QByteArray("Tabpage")
	, true)
<< Function( "Boolean", "tabpage_is_valid",
	QList<QByteArray>()
		<< QByteArray("Tabpage")
	, false)
<< Function( "void", "vim_push_keys",
	QList<QByteArray>()
		<< QByteArray("String")
	, false)
<< Function( "void", "vim_command",
	QList<QByteArray>()
		<< QByteArray("String")
	, true)
<< Function( "Object", "vim_eval",
	QList<QByteArray>()
		<< QByteArray("String")
	, true)
<< Function( "Integer", "vim_strwidth",
	QList<QByteArray>()
		<< QByteArray("String")
	, true)
<< Function( "StringArray", "vim_list_runtime_paths",
	QList<QByteArray>()
	, false)
<< Function( "void", "vim_change_directory",
	QList<QByteArray>()
		<< QByteArray("String")
	, true)
<< Function( "String", "vim_get_current_line",
	QList<QByteArray>()
	, true)
<< Function( "void", "vim_del_current_line",
	QList<QByteArray>()
	, true)
<< Function( "void", "vim_set_current_line",
	QList<QByteArray>()
		<< QByteArray("String")
	, true)
<< Function( "Object", "vim_get_var",
	QList<QByteArray>()
		<< QByteArray("String")
	, true)
<< Function( "Object", "vim_set_var",
	QList<QByteArray>()
		<< QByteArray("String")
		<< QByteArray("Object")
	, true)
<< Function( "Object", "vim_get_vvar",
	QList<QByteArray>()
		<< QByteArray("String")
	, true)
<< Function( "Object", "vim_get_option",
	QList<QByteArray>()
		<< QByteArray("String")
	, true)
<< Function( "void", "vim_set_option",
	QList<QByteArray>()
		<< QByteArray("String")
		<< QByteArray("Object")
	, true)
<< Function( "void", "vim_out_write",
	QList<QByteArray>()
		<< QByteArray("String")
	, false)
<< Function( "void", "vim_err_write",
	QList<QByteArray>()
		<< QByteArray("String")
	, false)
<< Function( "Integer", "vim_get_buffer_count",
	QList<QByteArray>()
	, false)
<< Function( "Buffer", "vim_get_current_buffer",
	QList<QByteArray>()
	, false)
<< Function( "void", "vim_set_current_buffer",
	QList<QByteArray>()
		<< QByteArray("Buffer")
	, true)
<< Function( "Integer", "vim_get_window_count",
	QList<QByteArray>()
	, false)
<< Function( "Window", "vim_get_current_window",
	QList<QByteArray>()
	, false)
<< Function( "void", "vim_set_current_window",
	QList<QByteArray>()
		<< QByteArray("Window")
	, true)
<< Function( "Integer", "vim_get_tabpage_count",
	QList<QByteArray>()
	, false)
<< Function( "Tabpage", "vim_get_current_tabpage",
	QList<QByteArray>()
	, false)
<< Function( "void", "vim_set_current_tabpage",
	QList<QByteArray>()
		<< QByteArray("Tabpage")
	, true)
<< Function( "Buffer", "window_get_buffer",
	QList<QByteArray>()
		<< QByteArray("Window")
	, true)
<< Function( "Position", "window_get_cursor",
	QList<QByteArray>()
		<< QByteArray("Window")
	, true)
<< Function( "void", "window_set_cursor",
	QList<QByteArray>()
		<< QByteArray("Window")
		<< QByteArray("Position")
	, true)
<< Function( "Integer", "window_get_height",
	QList<QByteArray>()
		<< QByteArray("Window")
	, true)
<< Function( "void", "window_set_height",
	QList<QByteArray>()
		<< QByteArray("Window")
		<< QByteArray("Integer")
	, true)
<< Function( "Integer", "window_get_width",
	QList<QByteArray>()
		<< QByteArray("Window")
	, true)
<< Function( "void", "window_set_width",
	QList<QByteArray>()
		<< QByteArray("Window")
		<< QByteArray("Integer")
	, true)
<< Function( "Object", "window_get_var",
	QList<QByteArray>()
		<< QByteArray("Window")
		<< QByteArray("String")
	, true)
<< Function( "Object", "window_set_var",
	QList<QByteArray>()
		<< QByteArray("Window")
		<< QByteArray("String")
		<< QByteArray("Object")
	, true)
<< Function( "Object", "window_get_option",
	QList<QByteArray>()
		<< QByteArray("Window")
		<< QByteArray("String")
	, true)
<< Function( "void", "window_set_option",
	QList<QByteArray>()
		<< QByteArray("Window")
		<< QByteArray("String")
		<< QByteArray("Object")
	, true)
<< Function( "Position", "window_get_position",
	QList<QByteArray>()
		<< QByteArray("Window")
	, true)
<< Function( "Tabpage", "window_get_tabpage",
	QList<QByteArray>()
		<< QByteArray("Window")
	, true)
<< Function( "Boolean", "window_is_valid",
	QList<QByteArray>()
		<< QByteArray("Window")
	, false)
	;
