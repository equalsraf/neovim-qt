// Auto generated {{date}}
#ifndef NEOVIM_QT_NEOVIMOBJ
#define NEOVIM_QT_NEOVIMOBJ
#include "function.h"
#include <msgpack.h>
namespace NeovimQt {
class NeovimConnector;
class Neovim: public QObject
{

	Q_OBJECT
public:
	Neovim(NeovimConnector *);
protected slots:
	void handleResponse(uint32_t id, Function::FunctionId fun, const msgpack_object&);
	void handleResponseError(uint32_t id, Function::FunctionId fun, const QString& msg, const msgpack_object&);
signals:
	void error(const QString& errmsg, const QVariant& errObj);
	void neovimEvent(const QByteArray &name, const QVariantList& args);
private:
	NeovimConnector *m_c;
public slots:
{% for f in functions %}
	void {{f.name}}({{f.argstring}});
{% endfor %}

signals:
{% for f in functions %}
	void on_{{f.name}}({{f.real_return_type}});
{% if f.can_fail %}
	void err_{{f.name}}(const QString&, const QVariant&);
{% endif%}

{% endfor %}
};
} // namespace
#endif
