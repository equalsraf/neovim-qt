#ifndef NEOVIM_QT_REQUEST
#define NEOVIM_QT_REQUEST

#include <QObject>
#include "function.h"
namespace NeovimQt {

class MsgpackIODevice;
class MsgpackRequest: public QObject {
	Q_OBJECT
public:
	MsgpackRequest(quint32 id, MsgpackIODevice *dev, QObject *parent=0);
	void processResponse(const msgpack_object& res, bool failed=false);
	void setFunction(Function::FunctionId);
	Function::FunctionId function();
signals:
	void finished(quint32 msgid, Function::FunctionId fun, const msgpack_object&);
	void error(quint32 msgid, Function::FunctionId fun, const QString& msg, const msgpack_object&);
private:
	quint32 m_id;
	MsgpackIODevice *m_dev;
	Function::FunctionId m_function;
};
} // Namespace

#endif
