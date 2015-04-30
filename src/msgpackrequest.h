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
	void setFunction(Function::FunctionId);
	Function::FunctionId function();
	const quint32 id;
signals:
	void finished(quint32 msgid, Function::FunctionId fun, const msgpack_object& resp);
	void error(quint32 msgid, Function::FunctionId fun, const msgpack_object& err);
private:
	MsgpackIODevice *m_dev;
	Function::FunctionId m_function;
};
} // Namespace

#endif
