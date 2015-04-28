#ifndef NEOVIM_QT_CONNECTOR
#define NEOVIM_QT_CONNECTOR

#include <QObject>
#include <QAbstractSocket>
#include <QHash>
#include <QProcess>
#include <QTextCodec>
#include <QTcpSocket>
#include "function.h"
#include "auto/neovim.h"

namespace NeovimQt {

class MsgpackIODevice;
class NeovimRequest;
class NeovimConnector: public QObject
{
	friend class Neovim;
	Q_OBJECT
	Q_PROPERTY(bool ready READ isReady NOTIFY ready)
	Q_ENUMS(NeovimError)
public:
	enum NeovimError {
		NoError=0,
		NoMetadata,
		MetadataDescriptorError,
		UnexpectedMsg,
		APIMisMatch,
		NoSuchMethod,
		FailedToStart,
		Crashed,
		SocketError,
		MsgpackError,
		RuntimeMsgpackError,
	};

        enum NeovimConnectionType {
		OtherConnection,
		SpawnedConnection,
		HostConnection,
		SocketConnection,
        };

	NeovimConnector(QIODevice* s);
	static NeovimConnector* spawn(const QStringList& params=QStringList());
	static NeovimConnector* connectToSocket(const QString&);
	static NeovimConnector* connectToHost(const QString& host, int port);
	static NeovimConnector* connectToNeovim(const QString& server=QString());

	bool canReconnect();
	NeovimConnector* reconnect();

	NeovimError errorCause();
	QString errorString();

	// FIXME: remove this
	void attachUi(int64_t width, int64_t height);
	void detachUi();

	bool isReady();
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
	void clearError();

	// Function table
	Function::FunctionId addFunction(const msgpack_object& ftable);
	void addFunctions(const msgpack_object& ftable);
	void addClasses(const msgpack_object& ftable);

protected slots:
	void discoverMetadata();
	void handleMetadata(uint32_t, Function::FunctionId, const msgpack_object& result);
	void handleMetadataError(uint32_t msgid, Function::FunctionId,
		const QString& msg, const msgpack_object& errobj);
	void processError(QProcess::ProcessError);
	void socketError();
	void msgpackError();
	void encodingChanged(const QVariant&);

private:
	MsgpackIODevice *m_dev;
	QString m_errorString;
	NeovimError m_error;

	Neovim *m_neovimobj;
	uint64_t m_channel;

	// Store connection arguments for reconnect()
	NeovimConnectionType m_ctype;
	QStringList m_connParams;
	QString m_connSocket, m_connHost;
	int m_connPort;
	bool m_ready;
};
} // namespace NeovimQt
Q_DECLARE_METATYPE(NeovimQt::NeovimConnector::NeovimError)

#endif
