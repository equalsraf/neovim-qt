// Auto generated {{date}}
#ifndef NEOVIM_QT_NEOVIMOBJ
#define NEOVIM_QT_NEOVIMOBJ
#include "function.h"
#include "msgpack.h"

namespace NeovimQt {
class NeovimConnector;
class MsgpackRequest;
class Neovim: public QObject
{

	Q_OBJECT
public:
	Neovim(NeovimConnector *);
protected slots:
	void handleResponse(quint32 id, Function::FunctionId fun, const QVariant&);
	void handleResponseError(quint32 id, Function::FunctionId fun, const QVariant&);
signals:
	void error(const QString& errmsg, const QVariant& errObj);
	void neovimNotification(const QByteArray &name, const QVariantList& args);
private:
	NeovimConnector *m_c;
public slots:
	void ui_try_resize(int64_t width, int64_t height);

{% for f in functions %}
	// {{f.signature()}}
	MsgpackRequest* {{f.name}}({{f.argstring}});
{% endfor %}

signals:
	void on_ui_try_resize();

{% for f in functions %}
	void on_{{f.name}}({{f.return_type.native_type}});
{% if f.can_fail %}
	void err_{{f.name}}(const QString&, const QVariant&);
{% endif%}

{% endfor %}
};
} // namespace
#endif
