" A Neovim plugin that implements GUI helper commands
if !has("nvim") || exists('g:GuiLoaded')
	finish
endif
let g:GuiLoaded = 1

" A replacement for foreground()
function! GuiForeground()
	call rpcnotify(0, 'Gui', 'Foreground')
endfunction

" Set maximized state for GUI window (1 is enabled, 0 disabled)
function! GuiWindowMaximized(enabled)
	call rpcnotify(0, 'Gui', 'WindowMaximized', a:enabled)
endfunction

" Set fullscreen state for GUI window (1 is enabled, 0 disabled)
function! GuiWindowFullScreen(enabled)
	call rpcnotify(0, 'Gui', 'WindowFullScreen', a:enabled)
endfunction

" Set GUI font
function! GuiFont(fname)
	call rpcnotify(0, 'Gui', 'Font', a:fname)
endfunction

" The GuiFont command. For compatibility there is also Guifont
function s:GuiFontCommand(fname)
	if a:fname == ""
		if exists('g:GuiFont')
			echo g:GuiFont
		else
			echo 'No GuiFont is set'
		endif
	else
		call GuiFont(a:fname)
	endif
endfunction
command! -nargs=? Guifont call s:GuiFontCommand("<args>")
command! -nargs=? GuiFont call s:GuiFontCommand("<args>")
