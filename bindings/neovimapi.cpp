// Auto generated {{date}} from nvim API level:{{api_level}}
#include "auto/neovimapi{{api_level}}.h"

#include "msgpackiodevice.h"
#include "msgpackrequest.h"
#include "neovimconnector.h"
#include "util.h"

namespace NeovimQt {

static QVariant unpackBufferApi{{api_level}}(MsgpackIODevice *dev, const char* in, quint32 size) noexcept
{
	msgpack_unpacked result;
	msgpack_unpacked_init(&result);
	msgpack_unpack_return ret{ msgpack_unpack_next(&result, in, size, nullptr) };

	QVariant variant;

	if (ret == MSGPACK_UNPACK_SUCCESS) {
		switch (result.data.type) {
			case MSGPACK_OBJECT_NEGATIVE_INTEGER:
				variant = static_cast<int64_t>(result.data.via.i64);
				break;

			case MSGPACK_OBJECT_POSITIVE_INTEGER:
				variant = static_cast<uint64_t>(result.data.via.u64);
				break;

			default:
				qWarning() << "Unsupported type found for EXT type" << result.data.type << result.data;
		}
	}

	msgpack_unpacked_destroy(&result);
	return variant;
}

static QVariant unpackWindowApi{{api_level}}(MsgpackIODevice *dev, const char* in, quint32 size) noexcept
{
	return unpackBufferApi{{api_level}}(dev, in, size);
}

static QVariant unpackTabpageApi{{api_level}}(MsgpackIODevice* dev, const char* in, quint32 size) noexcept
{
	return unpackBufferApi{{api_level}}(dev, in, size);
}

NeovimApi{{api_level}}::NeovimApi{{api_level}}(NeovimConnector* c) noexcept
	: m_c{ c }
{
	// EXT types
{% for typename in exttypes %}
	m_c->m_dev->registerExtType({{exttypes[typename]}}, unpack{{typename}}Api{{api_level}});
{% endfor %}

	connect(m_c->m_dev, &MsgpackIODevice::notification,
		this, &NeovimApi{{api_level}}::neovimNotification);
}

// Slots
{% for f in functions %}
MsgpackRequest* NeovimApi{{api_level}}::{{f.name}}({{f.argstring}}) noexcept
{
	MsgpackRequest* r{ m_c->m_dev->startRequestUnchecked("{{f.name}}", {{f.argcount}}) };
	r->setFunction(NeovimApi{{api_level}}::NEOVIM_FN_{{f.name.upper()}});
	connect(r, &MsgpackRequest::finished, this, &NeovimApi{{api_level}}::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi{{api_level}}::handleResponseError);
{% for param in f.parameters %}
	m_c->m_dev->{{param.sendmethod}}({{param.name}});
{% endfor %}
	return r;
}

{% endfor %}
// Handlers
void NeovimApi{{api_level}}::handleResponseError(int32_t msgid, int64_t fun, const QVariant& res) noexcept
{
	QString errMsg;
	const QVariantList asList = res.toList();
	if (asList.size() >= 2) {
		if (asList.at(1).canConvert<QByteArray>()) {
			errMsg = m_c->m_dev->decode(asList.at(1).toByteArray());
		} else {
			errMsg = tr("Received unsupported Neovim error type");
		}
	}

	switch(fun)
	{
{% for f in functions %}
		case NeovimApi{{api_level}}::NEOVIM_FN_{{f.name.upper()}}:
			emit err_{{f.name}}(errMsg, res);
			break;

{% endfor %}
	default:
		m_c->setError(NeovimConnector::RuntimeMsgpackError,
			QStringLiteral("Received error for function that should not fail: %s").arg(fun));
	}
}

void NeovimApi{{api_level}}::handleResponse(uint32_t msgid, uint64_t fun, const QVariant& res) noexcept
{
	switch(fun)
	{
{% for f in functions %}
		case NeovimApi{{api_level}}::NEOVIM_FN_{{f.name.upper()}}:
		{
{% if f.return_type.native_type != 'void' %}
			{{f.return_type.native_type}} data{};
			if (decode(res, data)) {
				m_c->setError(NeovimConnector::RuntimeMsgpackError,
					"Error unpacking return type for {{f.name}}");
				return;
			}

			emit on_{{f.name}}(data);
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
} // namespace NeovimQt
