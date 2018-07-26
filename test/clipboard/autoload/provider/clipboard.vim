function! provider#clipboard#Call(method, args) abort

	let uis = nvim_list_uis()
	if len(uis) == 0
		echoerr "No UIs are attached"
		return
	endif
	let ui_chan = uis[-1].chan

	if a:method == 'get'
		" a:args is a list with a register name
		let reqargs = [ui_chan, 'Gui', 'GetClipboard'] + a:args
		let result =  call(function("rpcrequest"), reqargs)
		return result
	elseif a:method == 'set'
		" a:args is [list of lines, type, register name]
		let notargs = [ui_chan, 'Gui', 'SetClipboard'] + a:args
		let _ = call(function("rpcnotify"), notargs)
	endif
endfunction
