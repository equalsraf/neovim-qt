// Auto generated {{date}}
#include "neovim.h"
#include "neovimconnector.h"
#include "msgpackrequest.h"
#include "msgpackiodevice.h"
#include "util.h"

namespace NeovimQt {

/* Unpack Neovim EXT types Window, Buffer Tabpage which are all
 * uint64_t see Neovim:msgpack_rpc_to_
 */
QVariant unpackBuffer(MsgpackIODevice *dev, const char* in, quint32 size)
{
	msgpack_unpacked result;
	msgpack_unpacked_init(&result);
	msgpack_unpack_return ret = msgpack_unpack_next(&result, in, size, NULL);
	msgpack_unpacked_destroy(&result);

	if (ret != MSGPACK_UNPACK_SUCCESS) {
		return QVariant();
	}
	return QVariant((quint64)result.data.via.u64);
}
#define unpackWindow unpackBuffer
#define unpackTabpage unpackBuffer

Neovim::Neovim(NeovimConnector *c)
:m_c(c)
{
	// EXT types
	{% for typename in exttypes %}
	m_c->m_dev->registerExtType({{exttypes[typename]}}, unpack{{typename}});
	{% endfor %}
	connect(m_c->m_dev, &MsgpackIODevice::notification,
			this, &Neovim::neovimNotification);
}


void Neovim::ui_try_resize(int64_t width, int64_t height)
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("ui_try_resize", 2);
	m_c->m_dev->send(width);
	m_c->m_dev->send(height);
	connect(r, &MsgpackRequest::finished, this, &Neovim::on_ui_try_resize);
}


// Slots
{% for f in functions %}
MsgpackRequest* Neovim::{{f.name}}({{f.argstring}})
{
	MsgpackRequest *r = m_c->m_dev->startRequestUnchecked("{{f.name}}", {{f.argcount}});
	r->setFunction(Function::NEOVIM_FN_{{f.name.upper()}});
	connect(r, &MsgpackRequest::finished, this, &Neovim::handleResponse);
	connect(r, &MsgpackRequest::error, this, &Neovim::handleResponseError);
{% for param in f.parameters %}
	m_c->m_dev->{{param.sendmethod}}({{param.name}});
{% endfor %}
	return r;
}
{% endfor %}

// Handlers

void Neovim::handleResponseError(quint32 msgid, Function::FunctionId fun, const QVariant& res)
{

	// TODO: support Neovim error types Exception/Validation/etc
	QString errMsg;
	const QVariantList asList = res.toList();
	if (asList.size() >= 2) {
		if (asList.at(1).canConvert<QByteArray>()) {
			errMsg = m_c->m_dev->decode(asList.at(1).toByteArray());
		} else {
			errMsg = tr("Received unsupported Neovim error type");
		}
	}

	switch(fun) {
{% for f in functions %}
{% if f.can_fail %}
	case Function::NEOVIM_FN_{{f.name.upper()}}:
		emit err_{{f.name}}(errMsg, res);
		break;
{% endif %}
{% endfor %}
	default:
		m_c->setError(NeovimConnector::RuntimeMsgpackError, QString("Received error for function that should not fail: %s").arg(fun));
	}
}

void Neovim::handleResponse(quint32 msgid, Function::FunctionId fun, const QVariant& res)
{
	switch(fun) {
{% for f in functions %}
	case Function::NEOVIM_FN_{{f.name.upper()}}:
		{
{% if f.return_type.native_type != 'void' %}
			{{f.return_type.native_type}} data;
			if (decode(res, data)) {
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
