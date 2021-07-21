// Auto generated {{date}} from nvim API level:{{api_level}}
#pragma once

#include <QObject>
#include <QPoint>
#include <QVariant>

#include "function.h"
#include "msgpack.h"

namespace NeovimQt {

class NeovimConnector;
class MsgpackRequest;

class NeovimApi{{api_level}}: public QObject
{
	Q_OBJECT

public:

	enum FunctionId {
		NEOVIM_FN_NULL=0,
		{% for f in functions %}
		NEOVIM_FN_{{ f.name.upper() }},
		{% endfor %}
	};
	Q_ENUM(FunctionId)

	static bool checkFunctions(const QVariantList& ftable);
	static FunctionId functionId(const Function& f);

	NeovimApi{{api_level}}(NeovimConnector *);
protected slots:
	void handleResponse(quint32 id, quint64 fun, const QVariant&);
	void handleResponseError(quint32 id, quint64 fun, const QVariant&);
signals:
	void error(const QString& errmsg, const QVariant& errObj);
	void neovimNotification(const QByteArray &name, const QVariantList& args);
private:
	NeovimConnector *m_c;
public slots:
{% for f in functions %}
{% if f.deprecated() %}
	// DEPRECATED
{% endif %}
	// {{f.signature()}}
	NeovimQt::MsgpackRequest* {{f.name}}({{f.argstring}});
{% endfor %}

signals:
{% for f in functions %}
	void on_{{f.name}}({{f.return_type.native_type}});
	void err_{{f.name}}(const QString&, const QVariant&);

{% endfor %}
};

} // namespace NeovimQt
