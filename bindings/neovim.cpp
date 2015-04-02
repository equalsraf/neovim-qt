// Auto generated {{date}}
#include "neovim.h"
#include "neovimconnector.h"
#include "neovimrequest.h"

namespace NeovimQt {

Neovim::Neovim(NeovimConnector *c)
:m_c(c)
{
}


void Neovim::ui_try_resize(int64_t width, int64_t height)
{
	NeovimRequest *r = m_c->startRequestUnchecked("ui_try_resize", 2);
	m_c->send(width);
	m_c->send(height);
	connect(r, &NeovimRequest::finished, this, &Neovim::on_ui_try_resize);
}


// Slots
{% for f in functions %}
void Neovim::{{f.name}}({{f.argstring}})
{
	NeovimRequest *r = m_c->startRequestUnchecked("{{f.name}}", {{f.argcount}});
	r->setFunction(Function::NEOVIM_FN_{{f.name.upper()}});
	connect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);
	connect(r, &NeovimRequest::error, this, &Neovim::handleResponseError);
{% for param in f.parameters %}
	m_c->{{param.sendmethod}}({{param.name}});
{% endfor %}
}
{% endfor %}

// Handlers

void Neovim::handleResponseError(uint32_t msgid, Function::FunctionId fun, const QString& msg, const msgpack_object& res)
{
	QVariant errObj;
	if (decodeMsgpack(res, errObj)) {
		m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking error object in function response");
		return;
	}
	switch(fun) {
{% for f in functions %}
{% if f.can_fail %}
	case Function::NEOVIM_FN_{{f.name.upper()}}:
		emit err_{{f.name}}(msg, errObj);
		break;
{% endif %}
{% endfor %}
	default:
		m_c->setError(NeovimConnector::RuntimeMsgpackError, QString("Received error for function that should not fail: %s").arg(fun));
	}
}

void Neovim::handleResponse(uint32_t msgid, Function::FunctionId fun, const msgpack_object& res)
{
	switch(fun) {
{% for f in functions %}
	case Function::NEOVIM_FN_{{f.name.upper()}}:
		{
{% if f.return_type.native_type != 'void' %}
			{{f.return_type.native_type}} data;
			if (decodeMsgpack(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for {{f.name}}");
				return;
			} else {
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
