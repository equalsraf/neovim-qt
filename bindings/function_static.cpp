// Auto generated {{date}}
const QList<Function> Function::knownFunctions = QList<Function>()
{% for f in functions %}
<< Function("{{f.return_type.neovim_type}}", "{{f.name}}",
	QList<QString>()
	{% for param in f.parameters %}
		<< QString("{{param.neovim_type}}")
	{% endfor %}
	, false)
{% endfor %}
	;
