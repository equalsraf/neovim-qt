#ifndef NEOVIM_QT_REQUEST
#define NEOVIM_QT_REQUEST

#include <QObject>
#include "function.h"
namespace NeovimQt {

class MsgpackIODevice;
class NeovimRequest: public QObject {
	Q_OBJECT
public:
	NeovimRequest(uint32_t id, MsgpackIODevice *dev, QObject *parent=0);
	void processResponse(const msgpack_object& res, bool failed=false);
	void setFunction(Function::FunctionId);
	Function::FunctionId function();
signals:
	void finished(uint32_t msgid, Function::FunctionId fun, const msgpack_object&);
	void error(uint32_t msgid, Function::FunctionId fun, const QString& msg, const msgpack_object&);
private:
	uint32_t m_id;
	MsgpackIODevice *m_dev;
	Function::FunctionId m_function;
};
} // Namespace

#endif
