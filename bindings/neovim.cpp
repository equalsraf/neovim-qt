// Auto generated {{date}}
#include "neovim.h"
#include "neovimconnector.h"
#include "neovimrequest.h"

namespace NeovimQt {

Neovim::Neovim(NeovimConnector *c)
:m_c(c)
{
}

// Slots
{% for f in functions %}
void Neovim::{{f.name}}({{f.argstring}})
{
	NeovimRequest *r = m_c->startRequestUnchecked("{{f.name}}", {{f.argcount}});
	r->setFunction(Function::NEOVIM_FN_{{f.name.upper()}});
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
{% for arg in f.argnames %}
	m_c->send({{arg}});
{% endfor %}
}
{% endfor %}

// Handlers

void Neovim::handleResponseError(uint32_t msgid, Function::FunctionId fun, const QString& msg, const msgpack_object& res)
{
	QVariant errObj;
	if (decodeMsgpack(res, errObj)) {
		qWarning() << "Error while decoding error object as" << fun << res;
	}
	emit error(msg, errObj);
	switch(fun) {
{% for f in functions %}
{% if f.can_fail %}
	case Function::NEOVIM_FN_{{f.name.upper()}}:
		emit err_{{f.name}}(msg, errObj);
		break;
{% endif %}
{% endfor %}
	default:
		qWarning() << "Received error for function call that should not fail" << fun << msg;
	}
}

void Neovim::handleResponse(uint32_t msgid, Function::FunctionId fun, const msgpack_object& res)
{
	switch(fun) {
{% for f in functions %}
	case Function::NEOVIM_FN_{{f.name.upper()}}:
		{
{% if f.real_return_type != 'void' %}
			{{f.real_return_type}} data;
			if (decodeMsgpack(res, data)) {
				qWarning() << "Error unpacking data for signal {{f.name}}";
			} else {
				qDebug() << "{{f.name}} ->" << data;
				emit on_{{f.name}}(data);
			}
{% else %}
			emit on_{{f.name}}();
{% endif %}

		}
		break;
{% endfor %}
	default:
		qWarning() << "Received unexpected response";
	}
}

} // Namespace
