local shim_script, help_text = ...

local function prepare_docs()
	local plugin_dir = vim.fn.stdpath("data") .. "/nvim-qt/plugin"
	local doc_dir = plugin_dir .. "/doc"
	vim.fn.mkdir(doc_dir, "p")

	local help_path = doc_dir .. "/nvim_gui_shim.txt"
	local f = io.open(help_path, "w")
	if f then
		f:write(help_text)
		f:close()
		vim.opt.runtimepath:append(plugin_dir)
		pcall(vim.cmd, "helptags " .. vim.fn.fnameescape(doc_dir))
	end
end

local function load_shim()
	vim.cmd(shim_script)
end

prepare_docs()
load_shim()
