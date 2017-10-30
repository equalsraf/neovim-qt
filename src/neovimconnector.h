#ifndef NEOVIM_QT_CONNECTOR
#define NEOVIM_QT_CONNECTOR

#include <QObject>
#include <QAbstractSocket>
#include <QProcess>
#include <QTextCodec>
#include "auto/neovimapi0.h"
#include "auto/neovimapi1.h"
#include "auto/neovimapi2.h"

namespace NeovimQt {

class MsgpackIODevice;
class NeovimConnectorHelper;
class NeovimConnector: public QObject
{
	friend class NeovimApi0;
	friend class NeovimApi1;
	friend class NeovimApi2;
	friend class NeovimConnectorHelper;
	Q_OBJECT
	/**
	 * True if the Neovim instance is ready
	 * @see neovimObject
	 */
	Q_PROPERTY(bool ready READ isReady NOTIFY ready)
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
#ifdef Q_ENUM
	Q_ENUM(NeovimError)
#else
	Q_ENUMS(NeovimError)
#endif

	/** Underlying connection used to read Neovim */
        enum NeovimConnectionType {
		OtherConnection,
		SpawnedConnection,
		HostConnection,
		SocketConnection,
        };

	NeovimConnector(QIODevice* s);
	NeovimConnector(MsgpackIODevice* s);
	static NeovimConnector* spawn(const QStringList& params=QStringList(),
									const QString& exe="nvim");
	static NeovimConnector* connectToSocket(const QString&);
	static NeovimConnector* connectToHost(const QString& host, int port);
	static NeovimConnector* connectToNeovim(const QString& server=QString());
	static NeovimConnector* fromStdinOut();

	bool canReconnect();
	NeovimConnector* reconnect();

	NeovimError errorCause();
	QString errorString();

	bool isReady();
	NeovimApi0 * api0();
	NeovimApi1 * neovimObject();
	NeovimApi1 * api1();
	NeovimApi2 * api2();
	uint64_t channel();
	QString decode(const QByteArray&);
	QByteArray encode(const QString&);
	NeovimConnectionType connectionType();
	/** Some requests for metadata and ui attachment enforce a timeout in ms */
	void setRequestTimeout(int);

	quint64 apiCompatibility();
	quint64 apiLevel();

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

	NeovimApi0 *m_api0;
	NeovimApi1 *m_api1;
	NeovimApi2 *m_api2;
	quint64 m_channel;
	quint64 m_api_compat;
	quint64 m_api_supported;

	// Store connection arguments for reconnect()
	NeovimConnectionType m_ctype;
	QStringList m_spawnArgs;
	QString m_spawnExe;
	QString m_connSocket, m_connHost;
	int m_connPort;
	bool m_ready;
	int m_timeout;
};
} // namespace NeovimQt
Q_DECLARE_METATYPE(NeovimQt::NeovimConnector::NeovimError)

#endif
