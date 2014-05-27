#ifndef NEOVIM_QT_CONNECTOR
#define NEOVIM_QT_CONNECTOR

#include <QObject>
#include <QAbstractSocket>
#include <QHash>
#include <msgpack.h>
#include "util.h"
#include "function.h"
#include "auto/neovim.h"

namespace NeoVimQt {

class NeoVimRequest: public QObject {
	Q_OBJECT
public:
	NeoVimRequest(uint32_t id, QObject *parent=0);
	void processResponse(const msgpack_object& res, bool error=false);
	void setFunction(Function::FunctionId);
	Function::FunctionId function();
signals:
	void finished(uint32_t msgid, Function::FunctionId fun, bool error, const msgpack_object&);
private:
	uint32_t m_id;
	Function::FunctionId m_function;
};

class NeoVimConnector: public QObject
{
	Q_OBJECT

public:
	enum NeoVimError {
		DeviceNotOpen,
		InvalidDevice,
		NoError,
		NoMetadata,
		MetadataDescriptorError,
		UnexpectedMsg,
		APIMisMatch,
		NoSuchMethod,
	};

	NeoVimConnector(QIODevice* s);
	~NeoVimConnector();

	QList<QByteArray> parseParameters(const msgpack_object&);

	NeoVimError error();
	QString errorString();

	NeoVimRequest* startRequestUnchecked(uint32_t method, uint32_t argcount);
	// FIXME: add argcount verification, e.g.
	// - remove the argcount from this method
	// - count calls to send() and match agains argcount - fail hard on mismatch
	NeoVimRequest* startRequest(Function::FunctionId method, uint32_t argcount);

	// Methods to pack datatypes as message pack
	// This is what you can use for arguments after startRequest
	void send(Integer);
	void send(const QString&);
	void send(const QLatin1String& s) { send(QString(s));}
	void send(const QStringList&);
	void send(const QVariant&);
	void send(const QByteArray&);
	void send(bool);

	// 
	// Methods to unpack msgpack responses
	//
	QByteArray to_QByteArray(const msgpack_object&, bool *failed=NULL);
	String to_String(const msgpack_object&, bool *failed=NULL);
	Boolean to_Boolean(const msgpack_object&, bool *failed=NULL);
	StringArray to_StringArray(const msgpack_object&, bool *failed=NULL);
	Position to_Position(const msgpack_object&, bool *failed=NULL);
	Object to_Object(const msgpack_object& msg, bool *failed=NULL);

	// These are all to_Integer
	Integer to_Integer(const msgpack_object&, bool *failed=NULL);
	Buffer to_Buffer(const msgpack_object&, bool *failed=NULL);
	Window to_Window(const msgpack_object&, bool *failed=NULL);
	Tabpage to_Tabpage(const msgpack_object&, bool *failed=NULL);

	// These are all basically the same as to_IntegerArray
	QList<int64_t> to_IntegerArray(const msgpack_object& msg, bool *failed=NULL);
	WindowArray to_WindowArray(const msgpack_object& msg, bool *failed=NULL);
	BufferArray to_BufferArray(const msgpack_object& msg, bool *failed=NULL);
	TabpageArray to_TabpageArray(const msgpack_object& msg, bool *failed=NULL);

	//static QVariant toVariant(const msgpack_object&);

	NeoVim* neovimObject();

signals:
	void ready();
	void error(NeoVimError);

protected:
	void discoverMetadata();

	void dispatch(msgpack_object& obj);
	void dispatchRequest(msgpack_object& obj);
	void dispatchResponse(msgpack_object& obj);
	void dispatchNotification(msgpack_object& obj);
	void error(const msgpack_object& req, const QString& msg);

	void addFunction(const msgpack_object& ftable);
	void addFunctions(const msgpack_object& ftable);
	void addClasses(const msgpack_object& ftable);

	void setError(NeoVimError err, const QString& msg);

protected slots:
	void dataAvailable();
	void handleMetadata(uint32_t, Function::FunctionId, bool error, const msgpack_object& result);

private:
	static int msgpack_write_cb(void* data, const char* buf, unsigned int len);

	uint32_t reqid;
	QIODevice *m_socket;
	msgpack_packer m_pk;
	msgpack_unpacker m_uk;

	QHash<uint32_t, NeoVimRequest*> m_requests;
	QString m_errorString;
	NeoVimError m_error;

	QHash<Function::FunctionId, uint64_t> m_functionToId;
	QHash<uint64_t, Function::FunctionId> m_idToFunction;
	NeoVim *m_neovimobj;
	uint64_t m_channel;
};

} // namespace NeoVimQt

#endif
