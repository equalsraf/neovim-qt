#ifndef NEOVIM_QT_CONNECTOR
#define NEOVIM_QT_CONNECTOR

#include <QObject>
#include <QAbstractSocket>
#include <QHash>
#include <QProcess>
#include <QTextCodec>
#include <msgpack.h>
#include "util.h"
#include "function.h"
#include "auto/neovim.h"

namespace NeovimQt {

class NeovimRequest;
class NeovimConnector: public QObject
{
	Q_OBJECT
	Q_ENUMS(NeovimError)
public:
	enum NeovimError {
		NoError=0,
		DeviceNotOpen,
		InvalidDevice,
		NoMetadata,
		MetadataDescriptorError,
		UnexpectedMsg,
		APIMisMatch,
		NoSuchMethod,
		FailedToStart,
		Crashed,
		UnsupportedEncoding,
		SocketError,
	};

	NeovimConnector(QIODevice* s);
	~NeovimConnector();
	static NeovimConnector* spawn(const QStringList& params=QStringList());
	static NeovimConnector* connectToSocket(const QString&);
	static NeovimConnector* connectToNeovim();

	NeovimError errorCause();
	QString errorString();

	NeovimRequest* startRequestUnchecked(const QString& method, uint32_t argcount);

	// Methods to pack datatypes as message pack
	// This is what you can use for arguments after startRequest
	void send(int64_t);
	void send(const QVariant&);
	void send(const QByteArray&);
	void send(bool);
	void send(const QList<QByteArray>& list);

	Neovim* neovimObject();
	uint64_t channel();
	QString decode(const QByteArray&);
	QByteArray encode(const QString&);

signals:
	void ready();
	void error(NeovimError);
	void processExited(int exitCode);

protected:
	void setError(NeovimError err, const QString& msg);

	// Message handlers
	void dispatch(msgpack_object& obj);
	void dispatchRequest(msgpack_object& obj);
	void dispatchResponse(msgpack_object& obj);
	void dispatchNotification(msgpack_object& obj);
	void sendError(const msgpack_object& req, const QString& msg);

	// Function table
	Function::FunctionId addFunction(const msgpack_object& ftable);
	void addFunctions(const msgpack_object& ftable);
	void addClasses(const msgpack_object& ftable);
	QList<QByteArray> parseParameterTypes(const msgpack_object&);
	uint32_t msgId();

protected slots:
	void discoverMetadata();
	void dataAvailable();
	void handleMetadata(uint32_t, Function::FunctionId, const msgpack_object& result);
	void handleMetadataError(uint32_t msgid, Function::FunctionId,
		const QString& msg, const msgpack_object& errobj);
	void processError(QProcess::ProcessError);
	void socketError();
	void encodingChanged(const QVariant&);

private:
	static int msgpack_write_cb(void* data, const char* buf, unsigned long int len);

	uint32_t reqid;
	QIODevice *m_dev;
	msgpack_packer m_pk;
	msgpack_unpacker m_uk;

	QHash<uint32_t, NeovimRequest*> m_requests;
	QString m_errorString;
	NeovimError m_error;

	Neovim *m_neovimobj;
	uint64_t m_channel;
	QTextCodec *m_encoding;
};
} // namespace NeovimQt
Q_DECLARE_METATYPE(NeovimQt::NeovimConnector::NeovimError)

#endif
