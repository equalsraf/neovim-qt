// Auto generated {{date}}
const QList<Function> Function::knownFunctions = QList<Function>()
{% for f in functions %}
<< Function("{{f.return_type}}", "{{f.name}}",
	QList<QString>()
	{% for ptype in f.argtypes %}
		<< QString("{{ptype}}")
	{% endfor %}
	, {% if f.can_fail%}true{%else%}false{%endif%})
{% endfor %}
	;
