// Auto generated {{date}}
enum FunctionId {
{% for f in functions %}
	NEOVIM_FN_{{ f.name.upper() }},
{% endfor %}
	NEOVIM_FN_NULL
};

