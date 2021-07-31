// Auto generated {{date}} from nvim API level:{{api_level}}
#pragma once

#include "function.h"
#include "msgpack.h"

#include <QObject>
#include <QPoint>
#include <QVariant>

namespace NeovimQt {

class NeovimConnector;
class MsgpackRequest;

class NeovimApi{{api_level}} : public QObject
{
	Q_OBJECT

public:

	NeovimApi{{api_level}}(NeovimConnector* c) noexcept;

	// FIXME enum class?
	enum FunctionId {
		NEOVIM_FN_NULL=0,
		{% for f in functions %}
		NEOVIM_FN_{{ f.name.upper() }},
		{% endfor %}
	};
	Q_ENUM(FunctionId)

	static bool checkFunctions(const QVariantList& ftable) noexcept;
	static FunctionId functionId(const Function& f) noexcept;

public slots:
	{% for f in functions %}
	{% if f.deprecated() %}
	// DEPRECATED
	{% endif %}
	// {{f.signature()}}
	NeovimQt::MsgpackRequest* {{f.name}}({{f.argstring}}) noexcept;
	{% endfor %}

signals:
	void error(const QString& errmsg, const QVariant& errObj);
	void neovimNotification(const QByteArray &name, const QVariantList& args);

	{% for f in functions %}
	void on_{{f.name}}({{f.return_type.native_type}});
	void err_{{f.name}}(const QString&, const QVariant&);
	{% endfor %}

protected slots:
	void handleResponse(quint32 id, quint64 fun, const QVariant&) noexcept;
	void handleResponseError(quint32 id, quint64 fun, const QVariant&) noexcept;

private:
	NeovimConnector *m_c;
};

} // namespace NeovimQt
