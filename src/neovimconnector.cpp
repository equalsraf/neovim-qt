#include "neovimconnector.h"
#include <QtGlobal>
#include <QMetaMethod>
#include <QLocalSocket>
#include "neovimrequest.h"

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
:QObject(), reqid(0), m_dev(dev), m_error(NoError), m_neovimobj(NULL), 
	m_channel(0), m_encoding(0)
{
	qRegisterMetaType<NeovimError>("NeovimError");

	msgpack_packer_init(&m_pk, this, NeovimConnector::msgpack_write_cb);
	msgpack_unpacker_init(&m_uk, MSGPACK_UNPACKER_INIT_BUFFER_SIZE);

	connect(m_dev, &QAbstractSocket::readyRead,
			this, &NeovimConnector::dataAvailable);

	if ( !m_dev->isOpen() ) {
		setError(DeviceNotOpen, tr("IO device is not open"));
		return;
	}

	if ( !m_dev->isSequential() ) {
		setError(InvalidDevice, tr("IO device needs to be sequential"));
		return;
	}

	discoverMetadata();
}

NeovimConnector::~NeovimConnector()
{
	//msgpack_packer_destroy(&m_pk);
	msgpack_unpacker_destroy(&m_uk);
}

/**
 * Sets latest error code and message for this connector
 */
void NeovimConnector::setError(NeovimError err, const QString& msg)
{
	m_error = err;
	m_errorString = msg;
	qWarning() << m_errorString;
	emit error(m_error);
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
 * Called when the msgpack packer has data to write
 */
int NeovimConnector::msgpack_write_cb(void* data, const char* buf, unsigned long int len)
{
	NeovimConnector *c = static_cast<NeovimConnector*>(data);
	return c->m_dev->write(buf, len);
}

void NeovimConnector::attachUi(int64_t width, int64_t height)
{
	// FIXME: this should be in class Neovim
	startRequestUnchecked("ui_attach", 3);
	send(width);
	send(height);
	send(true);
}

void NeovimConnector::detachUi()
{
	// FIXME: this should be in class Neovim
	startRequestUnchecked("ui_detach", 0);
}

void NeovimConnector::tryResizeUi(int64_t width, int64_t height)
{
	// FIXME: this should be in class Neovim
	startRequestUnchecked("ui_try_resize", 2);
	send(width);
	send(height);
}

/**
 * Start an RPC request
 *
 * Use send() to pass on each of the call parameters
 *
 * Returns a NeovimRequest object. You can connect to
 * its finished() SIGNAL to handle the response
 */
NeovimRequest* NeovimConnector::startRequestUnchecked(const QString& method, uint32_t argcount)
{
	uint32_t msgid = msgId();
	// [type(0), msgid, method, args]
	msgpack_pack_array(&m_pk, 4);
	msgpack_pack_int(&m_pk, 0);
	msgpack_pack_int(&m_pk, msgid);
	const QByteArray& utf8 = method.toUtf8();
	msgpack_pack_bin(&m_pk, utf8.size());
	msgpack_pack_bin_body(&m_pk, utf8.constData(), utf8.size());
	msgpack_pack_array(&m_pk, argcount);

	NeovimRequest *r = new NeovimRequest( msgid, this);
	m_requests.insert(msgid, r);
	return r;
}

/**
 * Serialise a value into the msgpack stream
 */
void NeovimConnector::send(int64_t i)
{
	qDebug() << __func__ << i;
	msgpack_pack_int64(&m_pk, i);
}

/**
 * Serialise a value into the msgpack stream
 */
void NeovimConnector::send(const QByteArray& bin)
{
	qDebug() << __func__ << bin;
	msgpack_pack_bin(&m_pk, bin.size());
	msgpack_pack_bin_body(&m_pk, bin.constData(), bin.size());
}

/**
 * Serialise a valud into the msgpack stream
 */
void NeovimConnector::send(bool b)
{
	if ( b ) {
		msgpack_pack_true(&m_pk);
	} else {
		msgpack_pack_false(&m_pk);
	}
}

void NeovimConnector::send(const QList<QByteArray>& list)
{
	msgpack_pack_array(&m_pk, list.size());
	foreach(const QByteArray& elem, list) {
		send(elem);
	}
}

/**
 * Serialise a value into the msgpack stream
 *
 * We use QVariants for RPC functions that use the *Object* type. Do not use
 * this in any other conditions
 */
void NeovimConnector::send(const QVariant& var)
{
	if (!checkVariant(var)) {
		msgpack_pack_nil(&m_pk);
		qWarning() << "Trying to pack unsupported variant type" << var.type() << "packing Nil instead";
		return;
	}

	switch((QMetaType::Type)var.type()) {
	case QMetaType::Void:
		msgpack_pack_nil(&m_pk);
	case QMetaType::Bool:
		send(var.toBool());
		break;
	case QMetaType::Int:
		msgpack_pack_int(&m_pk, var.toInt());
		break;
	case QMetaType::UInt:
		msgpack_pack_unsigned_int(&m_pk, var.toUInt());
		break;
	case QMetaType::Float:
		msgpack_pack_float(&m_pk, var.toFloat());
		break;
	case QMetaType::QString:
		send(encode(var.toString()));
		break;
	case QMetaType::Double:
		msgpack_pack_double(&m_pk, var.toDouble());
		break;
	case QMetaType::QByteArray:
		send(var.toByteArray());
		break;
	case QMetaType::QVariantList:
		msgpack_pack_array(&m_pk, var.toList().size());
		foreach(const QVariant& elem, var.toList()) {
			send(elem);
		}
		break;
	case QMetaType::QVariantMap:
		{
		const QVariantMap& m = var.toMap();
		msgpack_pack_map(&m_pk, m.size());
		QMapIterator<QString,QVariant> it(m);
		while(it.hasNext()) {
			it.next();
			send(it.key());
			send(it.value());
		}
		}
		break;
	case QMetaType::QPoint:
		// As an array [row, col]
		msgpack_pack_array(&m_pk, 2);
		msgpack_pack_int64(&m_pk, var.toPoint().y());
		msgpack_pack_int64(&m_pk, var.toPoint().x());
		break;
	default:
		msgpack_pack_nil(&m_pk);
		qWarning() << "There is a BUG in the QVariant serializer" << var.type();
	}
}

/**
 * Returns a new msgid that can be used for a msg
 */
uint32_t NeovimConnector::msgId()
{
	return this->reqid++;
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
	NeovimRequest *r = startRequestUnchecked("vim_get_api_info", 0);
	connect(r, &NeovimRequest::finished,
			this, &NeovimConnector::handleMetadata);
	connect(r, &NeovimRequest::error,
			this, &NeovimConnector::handleMetadataError);
}

/**
 * Retrieve parameter types from a list of function parameters in the metadata
 * object. Basically retrieves the even numbered elements of the array (types)
 * i.e. [Type0 name0 Type1 name1 ... ] -> [Type0 Type1 ...]
 *
 */
QList<QByteArray> NeovimConnector::parseParameterTypes(const msgpack_object& obj)
{
	QList<QByteArray> fail;
	if ( obj.type != MSGPACK_OBJECT_ARRAY ) {
		setError( UnexpectedMsg,
				tr("Found unexpected data type when unpacking a parameter array"));
		return fail;
	}
	
	QList<QByteArray> res;
	for (uint32_t i=0; i<obj.via.array.size; i++) {
		msgpack_object& param = obj.via.array.ptr[i];
		if ( param.type != MSGPACK_OBJECT_ARRAY ) {
			setError( UnexpectedMsg,
				tr("Found unexpected data type when unpacking a parameter"));
			return fail;
		}

		if ( param.via.array.size % 2 != 0 ) {
			setError( UnexpectedMsg,
				tr("Found uneven array when unpacking parameter"));
			return fail;
		}

		for (uint32_t j=0; j<param.via.array.size; j+=2) {
			// For full correctness we could check the parameter names
			// type as well but we don't actually need it
			QByteArray p;
			bool convfail = decodeMsgpack(param.via.array.ptr[j], p);
			if (convfail) {
				setError( UnexpectedMsg,
						tr("Decoding error when unpacking parameter value"));
				return fail;
			}
			res.append(p);
		}
	}
	return res;
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
		Function::FunctionId fid = addFunction(ftable.via.array.ptr[i]);
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

/**
 * Add a function, return the FunctionId or NEOVIM_FN_NULL if the
 * funciton is uknown
 */
Function::FunctionId NeovimConnector::addFunction(const msgpack_object& fun)
{
	if ( fun.type != MSGPACK_OBJECT_MAP ) {
		setError( UnexpectedMsg,
			tr("Found unexpected data type when unpacking function"));
		return Function::NEOVIM_FN_NULL;
	}	

	Function f = Function::fromMsgpack(fun);
	if ( !f.isValid() ) {
		setError( UnexpectedMsg,
			tr("Error parsing function metadata"));
		return Function::NEOVIM_FN_NULL;
	}
	int index = Function::knownFunctions.indexOf(f);
	if ( index != -1 ) {
		return Function::FunctionId(index);
	}
	qDebug() << "Found unknown function in metadata" << f.signature();
	return Function::NEOVIM_FN_NULL;
}

/**
 * Handle the *classes* attribute in the metadata
 */
void NeovimConnector::addClasses(const msgpack_object& ctable)
{
	if ( ctable.type != MSGPACK_OBJECT_ARRAY ) {
		setError( UnexpectedMsg,
			tr("Found unexpected data type when unpacking class table"));
		return;
	}
	for (uint32_t i=0; i<ctable.via.array.size; i++) {
		if ( ctable.via.array.ptr[i].type != MSGPACK_OBJECT_BIN ) {
			setError( UnexpectedMsg,
					tr("Found unexpected data type for class name"));
			return;
		}
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
			addClasses(metadata.via.map.ptr[i].val);
		}
	}

	if (errorCause() == NoError) {
		// Get &encoding before we signal readyness
		neovimObject()->vim_get_option("encoding");
		connect(neovimObject(), &Neovim::on_vim_get_option,
				this, &NeovimConnector::encodingChanged);
	}
}

/**
 * Called after metadata discovery, to get the &encoding
 */
void NeovimConnector::encodingChanged(const QVariant&  obj)
{
	disconnect(neovimObject(), &Neovim::on_vim_get_option,
			this, &NeovimConnector::encodingChanged);
	const QByteArray enc_name = obj.toByteArray();
	m_encoding = QTextCodec::codecForName(enc_name);
	if (m_encoding) {
		emit ready();
	} else {
		setError(UnsupportedEncoding, QString("Unsupported &encoding (%1)").arg(QString::fromLatin1(enc_name)));
	}
}

/**
 * Decode byte array as string, from Neovim's encoding
 *
 * \warning This method MUST not be called before NeovimConnector::ready
 */
QString NeovimConnector::decode(const QByteArray& data)
{
	if (m_encoding) {
		return m_encoding->toUnicode(data);
	} else {
		qWarning() << "Calling ::decode on a closed connector";
		return QString::fromUtf8(data);
	}
}

/**
 * Convert string to encoding expected by Neovim
 * \warning This method MUST not be called before NeovimConnector::ready
 */
QByteArray NeovimConnector::encode(const QString& str)
{
	if (m_encoding) {
		return m_encoding->fromUnicode(str);
	} else {
		qWarning() << "Calling ::encode on a closed connector";
		return str.toUtf8();
	}
}

/**
 * Send error response for the given request message
 */
void NeovimConnector::sendError(const msgpack_object& req, const QString& msg)
{
	if ( req.via.array.ptr[0].via.u64 != 0 ) {
		qFatal("Errors can only be send as replies to Requests(type=0)");
	}

	// [type(1), msgid, error, result(nil)]
	msgpack_pack_array(&m_pk, 4);
	msgpack_pack_int(&m_pk, 1); // 1 = Response
	msgpack_pack_int(&m_pk, req.via.array.ptr[1].via.u64);
	QByteArray utf8 = msg.toUtf8();
	msgpack_pack_bin(&m_pk, utf8.size());
	msgpack_pack_bin_body(&m_pk, utf8.constData(), utf8.size());
	msgpack_pack_nil(&m_pk);
}

/**
 * Called when new data is available to be parsed
 */
void NeovimConnector::dataAvailable()
{
	qint64 read = 1;
	while (read > 0) {
		if ( msgpack_unpacker_buffer_capacity(&m_uk) == 0 ) {
			if ( !msgpack_unpacker_reserve_buffer(&m_uk, 8192 ) ) {
				// FIXME: error out
				qWarning() << "Could not allocate memory in unpack buffer";
				return;
			}
		}

		read = m_dev->read(msgpack_unpacker_buffer(&m_uk), msgpack_unpacker_buffer_capacity(&m_uk));
		if ( read > 0 ) {
			msgpack_unpacker_buffer_consumed(&m_uk, read);
			msgpack_unpacked result;
			msgpack_unpacked_init(&result);
			while(msgpack_unpacker_next(&m_uk, &result)) {
				dispatch(result.data);
			}
		}
	}
}

/**
 * Do some sanity checks and forward message to the proper handler
 */
void NeovimConnector::dispatch(msgpack_object& req)
{
	//
	// neovim msgpack rpc calls are
	// [type(int), msgid(int), method(int), args(array)]
	//

	if (req.type != MSGPACK_OBJECT_ARRAY) {
		setError(InvalidMsgpack, "Received Invalid msgpack: not an array");
		return;
	}

	if (req.via.array.size < 3 || req.via.array.size > 4) {
		qDebug() << "Received Invalid msgpack: message len MUST be 3 or 4";
		return;
	}

	if (req.via.array.ptr[0].type != MSGPACK_OBJECT_POSITIVE_INTEGER) {
		qDebug() << "Received Invalid msgpack: msg type MUST be an integer";
		return;
	}
	uint64_t type = req.via.array.ptr[0].via.u64;

	switch(type) {
	case 0:
		if (req.via.array.ptr[1].type != MSGPACK_OBJECT_POSITIVE_INTEGER) {
			qDebug() << "Received Invalid request: msg id MUST be a positive integer";
			sendError(req, tr("Msg Id must be a positive integer"));
			return;
		}
		if (req.via.array.ptr[2].type != MSGPACK_OBJECT_BIN) {
			qDebug() << "Received Invalid request: method MUST be a binary string" << req.via.array.ptr[2];
			sendError(req, tr("Method id must be a positive integer"));
			return;
		}
		if (req.via.array.ptr[3].type != MSGPACK_OBJECT_ARRAY) {
			qDebug() << "Invalid request: arguments MUST be an array";
			sendError(req, tr("Paremeters must be an array"));
			return;
		}
		dispatchRequest(req);
		break;
	case 1:
		if (req.via.array.ptr[1].type != MSGPACK_OBJECT_POSITIVE_INTEGER) {
			qDebug() << "Received Invalid response: msg id MUST be a positive integer";
			return;
		}
		dispatchResponse(req);
		break;
	case 2:
		dispatchNotification(req);
		break;
	default:
		qDebug() << "Unsupported msg type" << type;
	}
}

/**
 * Handle request message
 *
 * [type(0), msgid(uint), method(bin), args([...])]
 */
void NeovimConnector::dispatchRequest(msgpack_object& req)
{
	qDebug() << "Received request for unknown method" << req.via.array.ptr[2];
	uint64_t msgid = req.via.array.ptr[1].via.u64;

	// Send error reply [type(1), msgid, error, NIL]
	msgpack_pack_array(&m_pk, 4);
	msgpack_pack_int(&m_pk, 1);
	msgpack_pack_int(&m_pk, msgid);
	QByteArray err = encode(tr("Unknown method"));
	msgpack_pack_bin(&m_pk, err.size());
	msgpack_pack_bin_body(&m_pk, err.constData(), err.size());
	msgpack_pack_nil(&m_pk);
}

/**
 * Handle response messages
 *
 * [type(1), msgid(uint), error(str?), result(...)]
 */
void NeovimConnector::dispatchResponse(msgpack_object& resp)
{
	// [type(1), msgid, error, result]
	uint64_t msgid = resp.via.array.ptr[1].via.u64;

	if ( !m_requests.contains(msgid) ) {
		qWarning() << "Received response for unknown message" << msgid;
		return;
	}

	NeovimRequest *req = m_requests.take(msgid);
	if ( resp.via.array.ptr[2].type != MSGPACK_OBJECT_NIL ) {
		// Error response
		req->processResponse(resp.via.array.ptr[2], true);
	} else {
		req->processResponse(resp.via.array.ptr[3], false);
	}
	req->deleteLater();
}

/**
 * [type(2), method, params]
 *
 */
void NeovimConnector::dispatchNotification(msgpack_object& nt)
{
	QByteArray methodName;
	if (decodeMsgpack(nt.via.array.ptr[1], methodName)) {
		qDebug() << "Received Invalid notification: event MUST be a String";
		return;
	}

	QVariant val; 
	if (decodeMsgpack(nt.via.array.ptr[2], val) ||
			(QMetaType::Type)val.type() != QMetaType::QVariantList  ) {
		qDebug() << "Unable to unpack notification parameters";
		return;
	}
	if (m_neovimobj) {
		emit m_neovimobj->neovimNotification(methodName, val.toList());
	}
}

/**
 * Get main NeovimObject
 *
 * \warning Do not call this before NeovimConnector::ready as been signaled
 */
Neovim* NeovimConnector::neovimObject()
{
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
	p->start("nvim", args);

	NeovimConnector *c = new NeovimConnector(p);
	connect(p, SIGNAL(error(QProcess::ProcessError)),
			c, SLOT(processError(QProcess::ProcessError)));
	connect(p, SIGNAL(finished(int, QProcess::ExitStatus)),
			c, SIGNAL(processExited(int)));
	// The connector raised and error because the IO device is
	// closed - reset error state
	c->setError(NoError, "");
	return c;
}

NeovimConnector* NeovimConnector::connectToSocket(const QString& path)
{
	QLocalSocket *s = new QLocalSocket();
	s->connectToServer(path);

	NeovimConnector *c = new NeovimConnector(s);
	connect(s, SIGNAL(error(QLocalSocket::LocalSocketError)),
			c, SLOT(socketError()));
	connect(s, &QLocalSocket::connected,
			c, &NeovimConnector::discoverMetadata);
	// The connector raised and error because the IO device is
	// closed - reset error state
	c->setError(NoError, "");
	return c;
}

NeovimConnector* NeovimConnector::connectToNeovim()
{
	QByteArray env = qgetenv("NVIM_LISTEN_ADDRESS");
	if (env.isEmpty()) {
		return spawn();
	}
	return connectToSocket(env);
}

/**
 * Called when running embedded Neovim to report an error
 * with the Neovim process
 */
void NeovimConnector::processError(QProcess::ProcessError err)
{
	switch(err) {
	case QProcess::FailedToStart:
		setError(FailedToStart, "Unable to start the Neovim process");
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
