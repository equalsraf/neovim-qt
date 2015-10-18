#ifndef NEOVIM_QT_CONNECTOR
#define NEOVIM_QT_CONNECTOR

#include <QObject>
#include <QAbstractSocket>
#include <QProcess>
#include <QTextCodec>
#include "function.h"
#include "auto/neovim.h"

namespace NeovimQt {

class MsgpackIODevice;
class NeovimConnectorHelper;
class NeovimConnector: public QObject
{
	friend class Neovim;
	friend class NeovimConnectorHelper;
	Q_OBJECT
	/**
	 * True if the Neovim instance is ready
	 * @see neovimObject
	 */
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

	/** Underlying connection used to read Neovim */
        enum NeovimConnectionType {
		OtherConnection,
		SpawnedConnection,
		HostConnection,
		SocketConnection,
        };

	NeovimConnector(QIODevice* s);
	NeovimConnector(MsgpackIODevice* s);
	static NeovimConnector* spawn(const QStringList& params=QStringList());
	static NeovimConnector* connectToSocket(const QString&);
	static NeovimConnector* connectToHost(const QString& host, int port);
	static NeovimConnector* connectToNeovim(const QString& server=QString());
	static NeovimConnector* fromStdinOut();

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
	NeovimConnectionType connectionType();

signals:
	/** Emitted when Neovim is ready @see ready */
	void ready();
	void error(NeovimError);
	void processExited(int exitCode);

public slots:
	void fatalTimeout();

protected:
	void setError(NeovimError err, const QString& msg);
	void clearError();

protected slots:
	void discoverMetadata();
	void processError(QProcess::ProcessError);
	void socketError();
	void msgpackError();

private:
	MsgpackIODevice *m_dev;
	NeovimConnectorHelper *m_helper;
	QString m_errorString;
	NeovimError m_error;

	Neovim *m_neovimobj;
	quint64 m_channel;

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
