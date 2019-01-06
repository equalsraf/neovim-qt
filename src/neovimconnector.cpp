#include "neovimconnector.h"
#include <QtGlobal>
#include <QMetaMethod>
#include <QLocalSocket>
#include <QTcpSocket>
#include "msgpackrequest.h"
#include "neovimconnectorhelper.h"
#include "msgpackiodevice.h"

namespace NeovimQt {

/**
 * \class NeovimQt::NeovimConnector
 * 
 * \brief A Connection to a Neovim instance
 *
 */

/**
 * Create a new Neovim API connection from an open IO device
 */
NeovimConnector::NeovimConnector(QIODevice *dev)
:NeovimConnector(new MsgpackIODevice(dev))
{
}

NeovimConnector::NeovimConnector(MsgpackIODevice *dev)
:QObject(), m_dev(dev), m_helper(0), m_error(NoError), m_api0(NULL), m_api1(NULL), m_api2(NULL), m_api3(NULL),
	m_api4(NULL), m_channel(0), m_api_compat(0), m_api_supported(0), m_ctype(OtherConnection), m_ready(false),
	m_timeout(20000)
{
	m_helper = new NeovimConnectorHelper(this);
	qRegisterMetaType<NeovimError>("NeovimError");
	qRegisterMetaType<int64_t>("int64_t");

	connect(m_dev, &MsgpackIODevice::error,
			this, &NeovimConnector::msgpackError);

	if ( !m_dev->isOpen() ) {
		return;
	}
	discoverMetadata();
}

void NeovimConnector::setRequestTimeout(int ms)
{
	this->m_timeout = ms;
}

/**
 * Sets latest error code and message for this connector
 */
void NeovimConnector::setError(NeovimError err, const QString& msg)
{
	m_ready = false;
	if (m_error == NoError && err != NoError) {
		m_error = err;
		m_errorString = msg;
		qWarning() << "Neovim fatal error" << m_errorString;
		emit error(m_error);
	} else {
		// Only the first error is raised
		qDebug() << "(Ignored) Neovim fatal error" << msg;
	}
}

/** Reset error state */
void NeovimConnector::clearError()
{
	m_error = NoError;
	m_errorString = "";
}

/**
 * Called when an error takes place
 */ 
NeovimConnector::NeovimError NeovimConnector::errorCause()
{
	return m_error;
}

/**
 * An human readable error message for the last error
 */
QString NeovimConnector::errorString()
{
	return m_errorString;
}

/**
 * Returns the channel id used by Neovim to identify this connection
 */
uint64_t NeovimConnector::channel()
{
	return m_channel;
}

/**
 * Request API information from Neovim
 */
void NeovimConnector::discoverMetadata()
{
	MsgpackRequest *r = m_dev->startRequestUnchecked("vim_get_api_info", 0);
	connect(r, &MsgpackRequest::finished,
			m_helper, &NeovimConnectorHelper::handleMetadata);
	connect(r, &MsgpackRequest::error,
			m_helper, &NeovimConnectorHelper::handleMetadataError);
	connect(r, &MsgpackRequest::timeout,
			this, &NeovimConnector::fatalTimeout);
	r->setTimeout(m_timeout);
}

/**
 * True if the Neovim instance is ready
 * @see ready
 */
bool NeovimConnector::isReady()
{
	return m_ready;
}

/**
 * Decode a byte array as a string according to 'encoding'
 */
QString NeovimConnector::decode(const QByteArray& in)
{
	return m_dev->decode(in);
}
/**
 * Encode a string into the appropriate encoding for this Neovim instance
 *
 * see :h 'encoding'
 */
QByteArray NeovimConnector::encode(const QString& in)
{
	return m_dev->encode(in);
}

/**
 * @warning Do not call this before NeovimConnector::ready as been signaled
 * @see NeovimConnector::isReady
 */
NeovimApi0* NeovimConnector::api0()
{
	if ( !m_api0 ) {
		if (m_api_compat <= 0) {
			m_api0 = new NeovimApi0(this);
		} else {
			qDebug() << "This instance of neovim DOES NOT support api level 0";
		}
	}
	return m_api0;
}
/**
 * @warning Do not call this before NeovimConnector::ready as been signaled
 * @see NeovimConnector::isReady
 */
NeovimApi1* NeovimConnector::api1()
{
	if ( !m_api1 ) {
		if (m_api_compat <= 1 && 1 <= m_api_supported) {
			m_api1 = new NeovimApi1(this);
		} else {
			qDebug() << "This instance of neovim DOES NOT support api level 1";
		}
	}
	return m_api1;
}

/** For compatibility with older versions */
NeovimApi1* NeovimConnector::neovimObject()
{
	return api1();
}

/**
 * @warning Do not call this before NeovimConnector::ready as been signaled
 * @see NeovimConnector::isReady
 */
NeovimApi2* NeovimConnector::api2()
{
	if ( !m_api2 ) {
		if (m_api_compat <= 2 && 2 <= m_api_supported) {
			m_api2 = new NeovimApi2(this);
		} else {
			qWarning() << "This instance of neovim not support api level 2";
		}
	}
	return m_api2;
}

/**
 * @warning Do not call this before NeovimConnector::ready as been signaled
 * @see NeovimConnector::isReady
 */
NeovimApi3* NeovimConnector::api3()
{
	if ( !m_api3 ) {
		if (m_api_compat <= 3 && 3 <= m_api_supported) {
			m_api3 = new NeovimApi3(this);
		} else {
			qWarning() << "This instance of neovim not support api level 3";
		}
	}
	return m_api3;
}

/**
 * @warning Do not call this before NeovimConnector::ready as been signaled
 * @see NeovimConnector::isReady
 */
NeovimApi4* NeovimConnector::api4()
{
	if ( !m_api4 ) {
		if (m_api_compat <= 4 && 4 <= m_api_supported) {
			m_api4 = new NeovimApi4(this);
		} else {
			qWarning() << "This instance of neovim not support api level 4";
		}
	}
	return m_api4;
}

/**
 * Launch an embedded Neovim process
 * @see processExited
 */
NeovimConnector* NeovimConnector::spawn(const QStringList& params, const QString& exe)
{
	QProcess *p = new QProcess();
	QStringList args;
	// Neovim accepts a `--' argument after which only filenames are passed.
	// If the user has supplied it, our arguments must appear before.
	if (params.indexOf("--") == -1) {
		args << "--embed";
		args.append(params);
	} else {
		int idx = params.indexOf("--");
		args.append(params.mid(0, idx));
		args << "--embed";
		args.append(params.mid(idx));
	}

	NeovimConnector *c = new NeovimConnector(p);
	c->m_ctype = SpawnedConnection;
	c->m_spawnArgs = params;
	c->m_spawnExe = exe;

	connect(p, SIGNAL(error(QProcess::ProcessError)),
			c, SLOT(processError(QProcess::ProcessError)));
	connect(p, SIGNAL(finished(int, QProcess::ExitStatus)),
			c, SIGNAL(processExited(int)));
	connect(p, &QProcess::started,
			c, &NeovimConnector::discoverMetadata);
	p->start(exe, args);
	return c;
}

/**
 * Connect to Neovim using a local UNIX socket.
 *
 * This method also works in Windows, using named pipes.
 *
 * @see QLocalSocket
 */
NeovimConnector* NeovimConnector::connectToSocket(const QString& path)
{
	QLocalSocket *s = new QLocalSocket();
	NeovimConnector *c = new NeovimConnector(s);

	c->m_ctype = SocketConnection;
	c->m_connSocket = path;

	connect(s, SIGNAL(error(QLocalSocket::LocalSocketError)),
			c, SLOT(socketError()));
	connect(s, &QLocalSocket::connected,
			c, &NeovimConnector::discoverMetadata);
	s->connectToServer(path);
	return c;
}

/**
 * Connect to a Neovim through a TCP connection
 *
 * @param host is a valid hostname or IP address
 * @param port is the TCP port
 */
NeovimConnector* NeovimConnector::connectToHost(const QString& host, int port)
{
	QTcpSocket *s = new QTcpSocket();
	NeovimConnector *c = new NeovimConnector(s);

	c->m_ctype = HostConnection;
	c->m_connHost = host;
	c->m_connPort = port;

	connect(s, SIGNAL(error(QAbstractSocket::SocketError)),
			c, SLOT(socketError()));
	connect(s, &QAbstractSocket::connected,
			c, &NeovimConnector::discoverMetadata);
	s->connectToHost(host, port);
	return c;
}

/**
 * Connect to a running instance of Neovim (if available).
 *
 * This method gets the Neovim endpoint from the NVIM_LISTEN_ADDRESS environment
 * variable, if it is not available a new Neovim instance is spawned().
 *
 * @see spawn()
 */
NeovimConnector* NeovimConnector::connectToNeovim(const QString& server)
{
	QString addr = server;
	if (addr.isEmpty()) {
		 addr = QString::fromLocal8Bit(qgetenv("NVIM_LISTEN_ADDRESS"));
	}
	if (addr.isEmpty()) {
		return spawn();
	}

	int colon_pos = addr.lastIndexOf(':');
	if (colon_pos != -1 && colon_pos != 0 && addr[colon_pos-1] != ':') {
		bool ok;
		int port = addr.mid(colon_pos+1).toInt(&ok);
		if (ok) {
			QString host = addr.mid(0, colon_pos);
			return connectToHost(host, port);
		}
	}
	return connectToSocket(addr);
}

NeovimConnector* NeovimConnector::fromStdinOut()
{
	return new NeovimConnector(MsgpackIODevice::fromStdinOut());
}

/**
 * Called when running embedded Neovim to report an error
 * with the Neovim process
 */
void NeovimConnector::processError(QProcess::ProcessError err)
{
	switch(err) {
	case QProcess::FailedToStart:
		setError(FailedToStart, m_dev->errorString());
		break;
	case QProcess::Crashed:
		setError(Crashed, "The Neovim process has crashed");
		break;
	default:
		// In practice we should be able to catch other types of
		// errors from the QIODevice
		qDebug() << "Neovim process error " << m_dev->errorString();
	}
}

/** Handle errors from QLocalSocket or QTcpSocket */
void NeovimConnector::socketError()
{
	setError(SocketError, m_dev->errorString());
}

/** Handle errors in MsgpackIODevice */
void NeovimConnector::msgpackError()
{
	setError(MsgpackError, m_dev->errorString());
}

/**
 * Raise a fatal error for a Neovim timeout
 *
 * Sometimes Neovim takes too long to respond to some requests, or maybe
 * the channel is stuck. In such cases it is preferable to raise and error,
 * internally this is what discoverMetadata does if Neovim does not reply.
 */
void NeovimConnector::fatalTimeout()
{
	setError(RuntimeMsgpackError, "Neovim is taking too long to respond");
}

void NeovimConnector::setRequestHandler(MsgpackRequestHandler *h)
{
	m_dev->setRequestHandler(h);
}

/**
 * True if NeovimConnector::reconnect can be called to reconnect with Neovim. This
 * is true unless you built the NeovimConnector ctor directly instead
 * of using on of the static methods.
 */
bool NeovimConnector::canReconnect()
{
	return m_ctype != OtherConnection;
}

/** @see NeovimConnector::NeovimConnectionType */
NeovimConnector::NeovimConnectionType NeovimConnector::connectionType()
{
	return m_ctype;
}

/**
 * Create a new connection using the same parameters as the current one.
 *
 * This is the equivalent of creating a new object with spawn(), connectToHost(),
 * or connectToSocket()
 *
 * If canReconnect() returns false, this function will return NULL.
 */
NeovimConnector* NeovimConnector::reconnect()
{
	switch(m_ctype) {
	case SpawnedConnection:
		return NeovimConnector::spawn(m_spawnArgs, m_spawnExe);
	case HostConnection:
		return NeovimConnector::connectToHost(m_connHost, m_connPort);
	case SocketConnection:
		return NeovimConnector::connectToSocket(m_connSocket);
	default:
		return NULL;
	}
	// NOT-REACHED
	return NULL;
}

/** The minimum API level supported by this instance */
quint64 NeovimConnector::apiCompatibility()
{
	return m_api_compat;
}
/** The maximum API level supported by this instance */
quint64 NeovimConnector::apiLevel()
{
	return m_api_supported;
}

/**
 * \fn NeovimQt::NeovimConnector::error(NeovimError)
 *
 * This signal is emitted when an error occurs. Use NeovimConnector::errorString
 * to get an error message.
 */

/**
 * \fn NeovimQt::NeovimConnector::processExited(int exitStatus)
 *
 * If the Neovim process was started using NeovimQt::NeovimConnector::spawn this signal
 * is emitted when the process exits.
 */

} // namespace NeovimQt

#include "moc_neovimconnector.cpp"
