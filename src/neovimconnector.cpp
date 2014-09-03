#include "neovimconnector.h"
#include <QtGlobal>
#include <QMetaMethod>

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
:QObject(), reqid(0), m_socket(dev), m_error(NoError), m_neovimobj(NULL), 
	m_channel(0), m_encoding(0)
{
	qRegisterMetaType<NeovimError>("NeovimError");

	msgpack_packer_init(&m_pk, this, NeovimConnector::msgpack_write_cb);
	msgpack_unpacker_init(&m_uk, MSGPACK_UNPACKER_INIT_BUFFER_SIZE);

	connect(m_socket, &QAbstractSocket::readyRead,
			this, &NeovimConnector::dataAvailable);

	if ( !s->isOpen() ) {
		setError(DeviceNotOpen, tr("IO device is not open"));
		return;
	}

	if ( !s->isSequential() ) {
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
NeovimConnector::NeovimError NeovimConnector::error()
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
	return c->m_socket->write(buf, len);
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
	msgpack_pack_raw(&m_pk, utf8.size());
	msgpack_pack_raw_body(&m_pk, utf8.constData(), utf8.size());
	msgpack_pack_array(&m_pk, argcount);

	NeovimRequest *r = new NeovimRequest( msgid, this);
	m_requests.insert(msgid, r);
	return r;
}

/**
 * Serialise a valud into the msgpack stream
 */
void NeovimConnector::send(int64_t i)
{
	qDebug() << __func__ << i;
	msgpack_pack_int64(&m_pk, i);
}

/**
 * Serialise a valud into the msgpack stream
 */
void NeovimConnector::send(const QByteArray& raw)
{
	qDebug() << __func__ << raw;
	msgpack_pack_raw(&m_pk, raw.size());
	msgpack_pack_raw_body(&m_pk, raw.constData(), raw.size());
}

/**
 * Serialise a valud into the msgpack stream
 */
void NeovimConnector::send(bool b)
{
	qDebug() << __func__ << b;
	if ( b ) {
		msgpack_pack_true(&m_pk);
	} else {
		msgpack_pack_false(&m_pk);
	}
}

/**
 * Serialise a valud into the msgpack stream
 *
 * We use QVariants for RPC functions that use the *Object* type. Do not use
 * this in any other conditions
 */
void NeovimConnector::send(const QVariant& var)
{
	qDebug() << __func__ << var;
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
//	case QMetaType::QString:
//		send(var.toString());
//		break;
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
		QMapIterator<QString,QVariant> it(var.toMap());
		while(it.hasNext()) {
			it.next();
			send(it.key());
			send(it.value());
		}
		}
		break;
	case QMetaType::QVariant:
		// As an array [row, col]
		msgpack_pack_array(&m_pk, 2);
		msgpack_pack_int64(&m_pk, var.toPoint().y());
		msgpack_pack_int64(&m_pk, var.toPoint().x());
	default:
		msgpack_pack_nil(&m_pk);
		qWarning() << "Trying to pack unsupported variant type" << var.type() << "packing Nil instead";
	}
}

// FIXME: this should match send(QVariant) -> enforce it
/**
 * We use QVariants for RPC functions use the *Object* type do not use this in any other conditions
 *
 */
Object NeovimConnector::to_Object(const msgpack_object& obj, bool *failed)
{
	if ( failed ) {
		*failed = false;
	}

	QVariant res;
	switch (obj.type) {
	case MSGPACK_OBJECT_NIL:
		res =  QVariant(QMetaType::Void, NULL);
		break;
	case MSGPACK_OBJECT_BOOLEAN:
		res = obj.via.boolean;
		break;
	case MSGPACK_OBJECT_NEGATIVE_INTEGER:
		res = QVariant((qint64)obj.via.i64);
		break;
	case MSGPACK_OBJECT_POSITIVE_INTEGER:
		res = QVariant((quint64)obj.via.u64);
		break;
	case MSGPACK_OBJECT_DOUBLE:
		res = obj.via.dec;
		break;
	case MSGPACK_OBJECT_RAW:
		res = to_String(obj, failed);
		break;
	case MSGPACK_OBJECT_ARRAY:
		// Either a QVariantList or a QStringList
		{
		QVariantList ls;
		for (uint64_t i=0; i<obj.via.array.size; i++) {
			QVariant v = to_Object(obj.via.array.ptr[i], failed);
			if ( v.isNull() ) {
				setError(UnexpectedMsg,
					tr("Found unexpected data type when unpacking Map as QVariantList"));
				res = QVariant();
				break;
			}
			ls.append(v);
		}
		res = ls;
		}
		break;
	case MSGPACK_OBJECT_MAP:
		{
		QVariantMap m;
		for (uint64_t i=0; i<obj.via.map.size; i++) {
			if (obj.via.map.ptr[i].key.type != MSGPACK_OBJECT_RAW) {
				setError(UnexpectedMsg,
					tr("Found unexpected data type when unpacking Map as QVariant"));
				return QVariant();
			}
			m.insert(
				to_String(obj.via.map.ptr[i].key), 
				to_Object(obj.via.map.ptr[i].val, failed));
		}
		res = m;
		}
		break;
	
	}
	return res;
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
 * Call function 0 to request metadata from Neovim
 */
void NeovimConnector::discoverMetadata()
{
	// With the use of msgpack-rpc, this now the only method
	// that DOES NOT comply with the spec, because it uses
	// int(0) as method id

	uint32_t msgid = msgId();
	// [type(0), msgid, method, args]
	msgpack_pack_array(&m_pk, 4);
	msgpack_pack_int(&m_pk, 0);
	msgpack_pack_int(&m_pk, msgid);
	msgpack_pack_int(&m_pk, 0);
	msgpack_pack_array(&m_pk, 0);

	NeovimRequest *r = new NeovimRequest( msgid, this);
	m_requests.insert(msgid, r);

	connect(r, &NeovimRequest::finished,
			this, &NeovimConnector::handleMetadata);
}

QByteArray NeovimConnector::to_String(const msgpack_object& obj, bool *failed)
{
	if ( failed ) {
		*failed = true;
	}
	if ( obj.type != MSGPACK_OBJECT_RAW ) {
		setError(UnexpectedMsg,
				tr("Found unexpected data type when unpacking a String"));
		return QByteArray();
	} else if ( failed ) {
		*failed = false;
	}
	return QByteArray(obj.via.raw.ptr, obj.via.raw.size);
}

QByteArray NeovimConnector::to_QByteArray(const msgpack_object& obj, bool *failed)
{
	if ( obj.type != MSGPACK_OBJECT_RAW ) {
		setError( UnexpectedMsg,
				tr("Found unexpected data type when unpacking a byte array"));
		return QByteArray();
	} if ( failed ) {
		*failed = false;
	}
	return QByteArray(obj.via.raw.ptr, obj.via.raw.size);
}

Position NeovimConnector::to_Position(const msgpack_object& obj, bool *failed)
{
	if ( failed ) {
		*failed = true;
	}
	if ( obj.type != MSGPACK_OBJECT_ARRAY || obj.via.array.size != 2 ) {
		setError(UnexpectedMsg,
				tr("Found unexpected data type when unpacking a Point"));
		return QPoint();
	} else if ( failed ) {
		*failed = false;
	}
	// QPoint is (x,y)  neovim Position is (row, col)
	return QPoint(to_Integer(obj.via.array.ptr[1]), to_Integer(obj.via.array.ptr[0]));
}

Boolean NeovimConnector::to_Boolean(const msgpack_object& obj, bool *failed)
{
	if ( failed ) {
		*failed = true;
	}
	if ( obj.type != MSGPACK_OBJECT_BOOLEAN) {
		setError(UnexpectedMsg,
				tr("Found unexpected data type when unpacking a bool"));
		return false;
	} if ( failed ) {
		*failed = false;
	}
	return obj.via.boolean;
}

StringArray NeovimConnector::to_StringArray(const msgpack_object& obj, bool *failed)
{
	if ( failed ) {
		*failed = true;
	}
	if ( obj.type != MSGPACK_OBJECT_ARRAY) {
		setError(UnexpectedMsg,
				tr("Found unexpected data type when unpacking a QStringList"));
		return QStringList();
	} 

	QStringList ret;
	for (uint32_t i=0; i<obj.via.array.size; i++) {
		if ( obj.via.array.ptr[i].type != MSGPACK_OBJECT_RAW ) {
			setError(UnexpectedMsg,
				tr("Found non-raw element type when unpacking a QStringList"));
			return QStringList();
		}
		ret.append(to_String(obj.via.array.ptr[i]));
	}

	if ( failed ) {
		*failed = false;
	}
	return ret;
}

Integer NeovimConnector::to_Integer(const msgpack_object& obj, bool *failed)
{
	if ( failed ) {
		*failed = true;
	}
	if ( obj.type != MSGPACK_OBJECT_POSITIVE_INTEGER) {
		setError(UnexpectedMsg,
				tr("Found unexpected data type when unpacking int64_t"));
		return false;
	}
	if ( failed ) {
		*failed = false;
	}
	return obj.via.i64;
}

Buffer NeovimConnector::to_Buffer(const msgpack_object& obj, bool *failed)
{
	return to_Integer(obj, failed);
}
Window NeovimConnector::to_Window(const msgpack_object& obj, bool *failed)
{
	return to_Integer(obj, failed);
}
Tabpage NeovimConnector::to_Tabpage(const msgpack_object& obj, bool *failed)
{
	return to_Integer(obj, failed);
}

QList<int64_t> NeovimConnector::to_IntegerArray(const msgpack_object& obj, bool *failed)
{
	if ( failed ) {
		*failed = true;
	}
	if ( obj.type != MSGPACK_OBJECT_ARRAY) {
		setError(UnexpectedMsg,
				tr("Found unexpected data type when unpacking a QList<int64_t>"));
		return QList<int64_t>();
	}

	QList<int64_t> ret;
	for (uint32_t i=0; i<obj.via.array.size; i++) {
		if ( obj.via.array.ptr[i].type != MSGPACK_OBJECT_POSITIVE_INTEGER ) {
			setError(UnexpectedMsg,
				tr("Found non-raw element type when unpacking an IntegerArray"));
			return QList<int64_t>();
		}
		ret.append(to_Integer(obj.via.array.ptr[i]));
	}

	if ( failed ) {
		*failed = false;
	}
	return ret;
}

QList<int64_t> NeovimConnector::to_WindowArray(const msgpack_object& obj, bool *failed)
{
	return to_IntegerArray(obj, failed);
}
QList<int64_t> NeovimConnector::to_BufferArray(const msgpack_object& obj, bool *failed)
{
	return to_IntegerArray(obj, failed);
}
QList<int64_t> NeovimConnector::to_TabpageArray(const msgpack_object& obj, bool *failed)
{
	return to_IntegerArray(obj, failed);
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
			if ( param.via.array.ptr[j].type != MSGPACK_OBJECT_RAW ||
					param.via.array.ptr[j+1].type != MSGPACK_OBJECT_RAW ) {
				setError( UnexpectedMsg,
						tr("Found unexpected data when unpacking parameter value"));
				return fail;
			}
			res.append(to_QByteArray(param.via.array.ptr[j]));
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

	QList<Function::FunctionId> m_supported;
	for (uint32_t i=0; i<ftable.via.array.size; i++) {
		Function::FunctionId fid = addFunction(ftable.via.array.ptr[i]);
		if (fid != Function::NEOVIM_FN_NULL) {
			m_supported.append(fid);
		}
	}

	if ( Function::knownFunctions.size() != m_supported.size() ) {
		setError( APIMisMatch,
				tr("Cannot connect to this instance of Neovim, its version is likely too old, or the API has changed"));
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
		if ( ctable.via.array.ptr[i].type != MSGPACK_OBJECT_RAW ) {
			setError( UnexpectedMsg,
					tr("Found unexpected data type for class name"));
			return;
		}
		qDebug() << to_QByteArray(ctable.via.array.ptr[i]);
	}
}

/**
 * Process metadata object returned by Neovim
 *
 * - Set channel_id
 * - Check if all functions we need are available
 */
void NeovimConnector::handleMetadata(uint32_t msgid, Function::FunctionId, bool failed, const msgpack_object& result)
{
	if ( failed ) {
		setError( NoMetadata,
			tr("Unable to get Neovim information"));
		return;
	}

	if ( result.type != MSGPACK_OBJECT_ARRAY || 
			result.via.array.size != 2 ||
			result.via.array.ptr[0].type != MSGPACK_OBJECT_POSITIVE_INTEGER ||
			result.via.array.ptr[1].type != MSGPACK_OBJECT_RAW ) {
		setError(UnexpectedMsg,
				tr("Unable to unpack metadata response description, unexpected data type"));
		return;
	}

	m_channel = result.via.array.ptr[0].via.u64;

	const msgpack_object& metadataraw = result.via.array.ptr[1];
	// The metadata bytearray is actually a serialized msgpack 
	msgpack_unpacked msg;
	msgpack_unpacked_init(&msg);
	bool ok = msgpack_unpack_next(&msg, 
			metadataraw.via.raw.ptr,
			metadataraw.via.raw.size, NULL);
	if ( !ok ) {
		setError(UnexpectedMsg,
				tr("Unable to unpack metadata description"));
		return;
	}

	if (msg.data.type != MSGPACK_OBJECT_MAP) {
		setError(MetadataDescriptorError,
				tr("Found unexpected data type for metadata description"));
		return;
	}

	for (uint32_t i=0; i< msg.data.via.map.size; i++) {
		QByteArray key = to_QByteArray(msg.data.via.map.ptr[i].key);
		if ( key == "functions" ) {
			addFunctions(msg.data.via.map.ptr[i].val);
		} else if ( key == "classes" ) {
			addClasses(msg.data.via.map.ptr[i].val);
		}
	}

	if (error() == NoError) {
		// Get &encoding before we signal readyness
		neovimObject()->vim_get_option("encoding");
		connect(neovimObject(), &Neovim::on_vim_get_option,
				this, &NeovimConnector::encodingChanged);
	}
}

/**
 * Called after metadata discovery, to get the &encoding
 */
void NeovimConnector::encodingChanged(Object obj)
{
	disconnect(neovimObject(), &Neovim::on_vim_get_option,
			this, &NeovimConnector::encodingChanged);
	const QByteArray enc_name = obj.toByteArray();
	m_encoding = QTextCodec::codecForName(enc_name);
	if (m_encoding) {
		qDebug() << "Neovim &encoding is " << m_encoding->name();
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
	Q_ASSERT(m_encoding);
	return m_encoding->toUnicode(data);
}

/**
 * Convert string to encoding expected by Neovim
 * \warning This method MUST not be called before NeovimConnector::ready
 */
QByteArray NeovimConnector::encode(const QString& str)
{
	Q_ASSERT(m_encoding);
	return m_encoding->fromUnicode(str);
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
	msgpack_pack_raw(&m_pk, utf8.size());
	msgpack_pack_raw_body(&m_pk, utf8.constData(), utf8.size());
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

		read = m_socket->read(msgpack_unpacker_buffer(&m_uk), msgpack_unpacker_buffer_capacity(&m_uk));
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
		qDebug() << "Invalid msgpack: not an array";
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
		if (req.via.array.ptr[2].type != MSGPACK_OBJECT_POSITIVE_INTEGER) {
			qDebug() << "Received Invalid request: method MUST be a positive integer" << req.via.array.ptr[2].type;
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
 * \todo Respond with error
 *
 * [type(0), msgid(uint), method(int), args([...])]
 */
void NeovimConnector::dispatchRequest(msgpack_object& req)
{
	qWarning() << "We do not support requests (yet)";
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
	if (nt.via.array.ptr[1].type != MSGPACK_OBJECT_RAW) {
		qDebug() << "Received Invalid notification: event MUST be a String";
		return;
	}
	QByteArray methodName = to_QByteArray(nt.via.array.ptr[1]);

	bool convfail;
	QVariant val = to_Object(nt.via.array.ptr[2], &convfail);
	if ( convfail || 
			(QMetaType::Type)val.type() != QMetaType::QVariantList  ) {
		qDebug() << "Unable to unpack notification parameters";
		return;
	}
	emit neovimEvent(methodName, val.toList());
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
 */
NeovimConnector* NeovimConnector::launch()
{
	QProcess *p = new QProcess();
	QStringList args;
	args << "--embedded-mode";
	p->start("nvim", args);

	NeovimConnector *c = new NeovimConnector(p);
	connect(p, SIGNAL(error(QProcess::ProcessError)),
			c, SLOT(processError(QProcess::ProcessError)));
	connect(p, &QProcess::started,
			c, &NeovimConnector::discoverMetadata);
	// The connector raised and error because the IO device is
	// closed - reset error state
	c->setError(NoError, "");
	return c;
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
		qDebug() << "Neovim process error " << m_socket->errorString();
	}
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
 * \fn NeovimQt::NeovimConnector::neovimEvent(const QByteArray &name, const QVariantList& args)
 *
 * Signal emitted when Neovim sends a notification withen given name and args
 */

/**
 * \class NeovimQt::NeovimRequest
 *
 * \brief A NeovimRequest represents an ongoing API call
 */

/**
 * \fn NeovimQt::NeovimRequest::finished
 *
 * \brief The request has finished
 */

/**
 * Creates a new NeovimRequest, identified by id
 *
 * \see NeovimQt::NeovimConnector::msgId
 */
NeovimRequest::NeovimRequest(uint32_t id, QObject *parent)
:QObject(parent), m_id(id), m_function(Function::NEOVIM_FN_NULL)
{
}

/**
 * Process the response message for this call
 *
 * \see NeovimQt::NeovimRequest::finished
 */
void NeovimRequest::processResponse(const msgpack_object& res, bool error)
{
	emit finished(this->m_id, m_function, error, res);
}

/**
 * The function id for the function signature associated with this call.
 * The value NEOVIM_FN_NULL indicates this call will not go through the
 * the generated function handlers.
 */
Function::FunctionId NeovimRequest::function()
{
	return m_function;
}

/**
 * Associate a function id with this request
 *
 * NeovimQt has auto-generated call handlers (in NeovimQt::NeovimConnector::neovimObject)
 * that will be used to process the response
 */
void NeovimRequest::setFunction(Function::FunctionId f)
{
	m_function = f;
}


} // namespace NeovimQt

#include "moc_neovimconnector.cpp"
