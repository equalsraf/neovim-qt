// Auto generated {{date}} from nvim API level:{{api_level}}
#include "auto/neovimapi{{api_level}}.h"
#include "msgpackiodevice.h"
#include "msgpackrequest.h"
#include "neovimconnector.h"
#include "util.h"

namespace NeovimQt {
/* Unpack Neovim EXT types Window, Buffer Tabpage which are all
 * uint64_t see Neovim:msgpack_rpc_to_
 */
static QVariant unpackBufferApi{{api_level}}(MsgpackIODevice *dev, const char* in, uint32_t size) noexcept
{
	msgpack_unpacked result;
	msgpack_unpacked_init(&result);
	msgpack_unpack_return ret = msgpack_unpack_next(&result, in, size, NULL);

	QVariant variant;

	if (ret == MSGPACK_UNPACK_SUCCESS) {
		switch (result.data.type) {
			case MSGPACK_OBJECT_NEGATIVE_INTEGER:
				variant.setValue<int64_t>(result.data.via.i64);
				break;
			case MSGPACK_OBJECT_POSITIVE_INTEGER:
				variant.setValue<uint64_t>(result.data.via.u64);
				break;
			default:
				// TODO it would be nice if we could call back MsgpackIoDevice method or primitive types here
				qWarning() << "Unsupported type found for EXT type" << result.data.type << result.data;
		}
	}

	msgpack_unpacked_destroy(&result);
	return variant;
}

static QVariant unpackWindowApi{{api_level}}(MsgpackIODevice *dev, const char* in, uint32_t size) noexcept
{
	return unpackBufferApi{{api_level}}(dev, in, size);
}

static QVariant unpackTabpageApi{{api_level}}(MsgpackIODevice *dev, const char* in, uint32_t size) noexcept
{
	return unpackBufferApi{{api_level}}(dev, in, size);
}

NeovimApi{{api_level}}::NeovimApi{{api_level}}(NeovimConnector *c) noexcept
	: m_connector{ c }
{
	// EXT types
	{% for typename in exttypes %}
m_connector->m_dev->registerExtType({{exttypes[typename]}}, unpack{{typename}}Api{{api_level}});
	{% endfor %}
connect(m_connector->m_dev, &MsgpackIODevice::notification,
		this, &NeovimApi{{api_level}}::neovimNotification);
}

// Slots
{% for f in functions %}
MsgpackRequest* NeovimApi{{api_level}}::{{f.name}}({{f.argstring}})
{
	MsgpackRequest* r{ m_connector->m_dev->startRequestUnchecked("{{f.name}}", {{f.argcount}}) };
	r->setFunction(NeovimApi{{api_level}}::NEOVIM_FN_{{f.name.upper()}});
	connect(r, &MsgpackRequest::finished, this, &NeovimApi{{api_level}}::handleResponse);
	connect(r, &MsgpackRequest::error, this, &NeovimApi{{api_level}}::handleResponseError);
{% for param in f.parameters %}
	m_connector->m_dev->{{param.sendmethod}}({{param.name}});
{% endfor %}
	return r;
}

{% endfor %}

// Handlers

void NeovimApi{{api_level}}::handleResponseError(uint32_t msgid, uint64_t fun, const QVariant& res)
{

	// TODO: support Neovim error types Exception/Validation/etc
	QString errMsg;
	const QVariantList asList = res.toList();
	if (asList.size() >= 2) {
		if (asList.at(1).canConvert<QByteArray>()) {
			errMsg = m_connector->m_dev->decode(asList.at(1).toByteArray());
		} else {
			errMsg = tr("Received unsupported Neovim error type");
		}
	}

	switch(fun) {
{% for f in functions %}
	case NeovimApi{{api_level}}::NEOVIM_FN_{{f.name.upper()}}:
		emit err_{{f.name}}(errMsg, res);
		break;
{% endfor %}
	default:
		m_connector->setError(NeovimConnector::RuntimeMsgpackError, QString("Received error for function that should not fail: %s").arg(fun));
	}
}

void NeovimApi{{api_level}}::handleResponse(uint32_t msgid, uint64_t fun, const QVariant& res)
{
	switch(fun) {
{% for f in functions %}
		case NeovimApi{{api_level}}::NEOVIM_FN_{{f.name.upper()}}:
		{
{% if f.return_type.native_type != 'void' %}
			{{f.return_type.native_type}} data;
			if (decode(res, data)) {
				m_connector->setError(NeovimConnector::RuntimeMsgpackError, "Error unpacking return type for {{f.name}}");
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

/**
 * Check function table from api_metadata[1]
 *
 * This checks the API metadata build from the bindings against the metadata
 * passed as argument.
 *
 * Returns false if there is an API mismatch
 */
/*static*/ bool NeovimApi{{api_level}}::checkFunctions(const QVariantList& ftable) noexcept
{
	static const QList<Function> required{
	{% for f in functions %}
		Function{
				QStringLiteral("{{f.return_type.neovim_type}}"),
				QStringLiteral("{{f.name}}"),
				QStringList{
			{% for param in f.parameters %}
		QStringLiteral("{{param.neovim_type}}"),
			{% endfor %}
		}
				, false },
	{% endfor %}
	};

	QVector<Function> supported;
	supported.reserve(ftable.size());
	for(const auto& val : ftable) {
		auto f = Function::fromVariant(val);
		if (!f.isValid()) {
			qDebug() << "Invalid function in metadata" << f;
			continue;
		}
		supported.append(f);

		if (!required.contains(f)) {
			qDebug() << "Unknown function(api {{api_level}})" << f;
		}
	}

	bool ok = true;
	for(const auto& f : required) {
		if (!supported.contains(f)) {
			qDebug() << "- instance DOES NOT support API{{api_level}}:" << f;
			ok = false;
		}
	}
	return ok;
}

} // Namespace
