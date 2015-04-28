#include "neovimconnector.h"
#include <QtGlobal>
#include <QMetaMethod>
#include <QLocalSocket>
#include "neovimrequest.h"
#include "msgpackiodevice.h"
#include "util.h"

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
:QObject(), m_dev(0), m_error(NoError), m_neovimobj(NULL), 
	m_channel(0), m_ctype(OtherConnection), m_ready(false)
{
	m_dev = new MsgpackIODevice(dev, this);
	qRegisterMetaType<NeovimError>("NeovimError");

	connect(m_dev, &MsgpackIODevice::error,
			this, &NeovimConnector::msgpackError);

	if ( !m_dev->isOpen() ) {
		return;
	}
	discoverMetadata();
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

void NeovimConnector::attachUi(int64_t width, int64_t height)
{
	// FIXME: this should be in class Neovim
	m_dev->startRequestUnchecked("ui_attach", 3);
	m_dev->send(width);
	m_dev->send(height);
	m_dev->send(true);
}

void NeovimConnector::detachUi()
{
	// FIXME: this should be in class Neovim
	m_dev->startRequestUnchecked("ui_detach", 0);
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
	NeovimRequest *r = m_dev->startRequestUnchecked("vim_get_api_info", 0);
	connect(r, &NeovimRequest::finished,
			this, &NeovimConnector::handleMetadata);
	connect(r, &NeovimRequest::error,
			this, &NeovimConnector::handleMetadataError);
}

/**
 * Handle the *functions* attribute in the metadata
 */
void NeovimConnector::addFunctions(const msgpack_object& ftable)
{
	if ( ftable.type != MSGPACK_OBJECT_ARRAY ) {
		setError( UnexpectedMsg,
				tr("Found unexpected data type when unpacking function table"));
		return;
	}

	QList<Function::FunctionId> supported;
	for (uint32_t i=0; i<ftable.via.array.size; i++) {
		Function::FunctionId fid = Function::functionId(Function::fromMsgpack(ftable.via.array.ptr[i]));
		if (fid != Function::NEOVIM_FN_NULL) {
			supported.append(fid);
		}
	}

	if ( Function::knownFunctions.size() != supported.size() ) {
		setError( APIMisMatch,
				tr("API methods mismatch: Cannot connect to this instance of Neovim, its version is likely too old, or the API has changed"));
		return;
	}
}

void NeovimConnector::handleMetadataError(uint32_t msgid, Function::FunctionId,
		const QString& msg, const msgpack_object& errobj)
{
	setError( NoMetadata,
		tr("Unable to get Neovim api information"));
	// TODO: better error message (from result?)
	return;
}

/**
 * Process metadata object returned by Neovim
 *
 * - Set channel_id
 * - Check if all functions we need are available
 */
void NeovimConnector::handleMetadata(uint32_t msgid, Function::FunctionId, const msgpack_object& result)
{
	if ( result.type != MSGPACK_OBJECT_ARRAY || 
			result.via.array.size != 2 ||
			result.via.array.ptr[0].type != MSGPACK_OBJECT_POSITIVE_INTEGER ||
			result.via.array.ptr[1].type != MSGPACK_OBJECT_MAP ) {
		setError(UnexpectedMsg,
				tr("Unable to unpack metadata response description, unexpected data type"));
		return;
	}

	m_channel = result.via.array.ptr[0].via.u64;

	const msgpack_object metadata = result.via.array.ptr[1];
	if (metadata.type != MSGPACK_OBJECT_MAP) {
		setError(MetadataDescriptorError,
				tr("Found unexpected data type for metadata description"));
		return;
	}

	for (uint32_t i=0; i< metadata.via.map.size; i++) {
		QByteArray key;
		if (decodeMsgpack(metadata.via.map.ptr[i].key, key)) {
			setError(MetadataDescriptorError,
				tr("Found unexpected data type for metadata key"));
			continue;
		}
		if ( key == "functions" ) {
			addFunctions(metadata.via.map.ptr[i].val);
		} else if ( key == "classes" ) {
		}
	}

	if (errorCause() == NoError) {
		// Get &encoding before we signal readyness
		connect(neovimObject(), &Neovim::on_vim_get_option,
				this, &NeovimConnector::encodingChanged);
		neovimObject()->vim_get_option("encoding");
	}
}

/**
 * Called after metadata discovery, to get the &encoding
 */
void NeovimConnector::encodingChanged(const QVariant&  obj)
{
	disconnect(neovimObject(), &Neovim::on_vim_get_option,
			this, &NeovimConnector::encodingChanged);
	m_dev->setEncoding(obj.toByteArray());
	const QByteArray enc_name = obj.toByteArray();
	if (m_dev->setEncoding(enc_name)) {
		m_ready = true;
		emit ready();
	}
}

bool NeovimConnector::isReady()
{
	return m_ready;
}

QString NeovimConnector::decode(const QByteArray& in)
{
	return m_dev->decode(in);
}
QByteArray NeovimConnector::encode(const QString& in)
{
	return m_dev->encode(in);
}

/**
 * Get main NeovimObject
 *
 * \warning Do not call this before NeovimConnector::ready as been signaled
 * \see NeovimConnector::isReady
 */
Neovim* NeovimConnector::neovimObject()
{
	Q_ASSERT(m_ready);
	if ( !m_neovimobj ) {
		m_neovimobj = new Neovim(this);
	}
	return m_neovimobj;
}

/**
 * Launch an embedded Neovim process
 *
 * Use ::processExited to know when the process has exited
 */
NeovimConnector* NeovimConnector::spawn(const QStringList& params)
{
	QProcess *p = new QProcess();
	QStringList args = params;
	args << "--embed";
	args << "-T" << "abstract_ui";

	NeovimConnector *c = new NeovimConnector(p);
	c->m_ctype = SpawnedConnection;
	c->m_connParams = params;

	connect(p, SIGNAL(error(QProcess::ProcessError)),
			c, SLOT(processError(QProcess::ProcessError)));
	connect(p, SIGNAL(finished(int, QProcess::ExitStatus)),
			c, SIGNAL(processExited(int)));
	connect(p, &QProcess::started,
			c, &NeovimConnector::discoverMetadata);
	p->start("nvim", args);
	return c;
}

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

void NeovimConnector::socketError()
{
	setError(SocketError, m_dev->errorString());
}

void NeovimConnector::msgpackError()
{
	setError(MsgpackError, m_dev->errorString());
}

bool NeovimConnector::canReconnect()
{
	return m_ctype != OtherConnection;
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
		return NeovimConnector::spawn(m_connParams);
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

/**
 * \fn NeovimQt::NeovimConnector::ready()
 *
 * This signal is emitted when the connector has beem able to successfuly setup
 * a connection with Neovim. Some methods SHOULD NOT be called before the signal
 * is emitted, otherwise you get an invalid object or NULL pointer.
 */

/**
 * \fn NeovimQt::NeovimConnector::error(NeovimError)
 *
 * This signal is emitted when an error occurs. Use NeovimConnector::errorString
 * to get an error message.
 */

/**
 * \fn NeovimQt::Neovim::neovimNotification(const QByteArray &name, const QVariantList& args)
 *
 * Signal emitted when Neovim sends a notification with given name and args
 */

/**
 * \fn NeovimQt::NeovimConnector::processExited(int exitStatus)
 *
 * If the Neovim process was started using NeovimQt::NeovimConnector::spawn this signal
 * is emitted when the process exits.
 */

} // namespace NeovimQt

#include "moc_neovimconnector.cpp"
