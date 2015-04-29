#include <QAbstractSocket>
#include <QTextCodec>
#include "msgpackiodevice.h"
#include "util.h"
#include "neovimrequest.h"

namespace NeovimQt {

/**
 * \class NeovimQt::MsgpackIODevice
 * 
 * \brief A msgpack-rpc channel
 *
 */


MsgpackIODevice::MsgpackIODevice(QIODevice *dev, QObject *parent)
:QObject(parent), m_reqid(0), m_dev(dev), m_encoding(0), m_error(NoError)
{
	qRegisterMetaType<MsgpackError>("MsgpackError");
	m_dev->setParent(this);

	// MSVC: wont build without the (mspack_packer_write) cast
	msgpack_packer_init(&m_pk, this, (msgpack_packer_write)MsgpackIODevice::msgpack_write_cb);
	msgpack_unpacker_init(&m_uk, MSGPACK_UNPACKER_INIT_BUFFER_SIZE);

	connect(m_dev, &QAbstractSocket::readyRead,
			this, &MsgpackIODevice::dataAvailable);

	if ( !m_dev->isSequential() ) {
		setError(InvalidDevice, tr("IO device needs to be sequential"));
		return;
	}
}

MsgpackIODevice::~MsgpackIODevice()
{
	//msgpack_packer_destroy(&m_pk);
	msgpack_unpacker_destroy(&m_uk);
}

QByteArray MsgpackIODevice::encoding() const
{
	if (m_encoding) {
		return m_encoding->name();
	}
	return QByteArray();
}

/**
 * Set encoding for strings in this RPC, if it not set we
 * fall back to utf8
 *
 * \see QTextCode
 */
bool MsgpackIODevice::setEncoding(const QByteArray& name)
{
	m_encoding = QTextCodec::codecForName(name);
	if (!m_encoding) {
		setError(UnsupportedEncoding, QString("Unsupported encoding (%1)").arg(QString::fromLatin1(name)));
		return false;
	}
	return true;
}

/**
 * Called when the msgpack packer has data to write
 */
int MsgpackIODevice::msgpack_write_cb(void* data, const char* buf, unsigned long int len)
{
	MsgpackIODevice *c = static_cast<MsgpackIODevice*>(data);
	return c->m_dev->write(buf, len);
}

/**
 * Called when new data is available to be parsed
 */
void MsgpackIODevice::dataAvailable()
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
 * Send error response for the given request message
 */
void MsgpackIODevice::sendError(const msgpack_object& req, const QString& msg)
{
	if ( req.via.array.ptr[0].via.u64 != 0 ) {
		qFatal("Errors can only be send as replies to Requests(type=0)");
	}
	sendError(req.via.array.ptr[1].via.u64, msg);
}

void MsgpackIODevice::sendError(uint64_t msgid, const QString& msg)
{
	// [type(1), msgid, error, result(nil)]
	msgpack_pack_array(&m_pk, 4);
	msgpack_pack_int(&m_pk, 1); // 1 = Response
	msgpack_pack_int(&m_pk, msgid);
	QByteArray utf8 = msg.toUtf8();
	msgpack_pack_bin(&m_pk, utf8.size());
	msgpack_pack_bin_body(&m_pk, utf8.constData(), utf8.size());
	msgpack_pack_nil(&m_pk);
}

/**
 * Do some sanity checks and forward message to the proper handler
 */
void MsgpackIODevice::dispatch(msgpack_object& req)
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
void MsgpackIODevice::dispatchRequest(msgpack_object& req)
{
	qDebug() << "Received request for unknown method" << req.via.array.ptr[2];
	uint64_t msgid = req.via.array.ptr[1].via.u64;

	// Send error reply [type(1), msgid, error, NIL]
	msgpack_pack_array(&m_pk, 4);
	msgpack_pack_int(&m_pk, 1);
	msgpack_pack_int(&m_pk, msgid);
	QByteArray err = "Unknown method";
	msgpack_pack_bin(&m_pk, err.size());
	msgpack_pack_bin_body(&m_pk, err.constData(), err.size());
	msgpack_pack_nil(&m_pk);
}

/**
 * Send back a response [type(1), msgid(uint), error(...), result(...)]
 */
bool MsgpackIODevice::sendResponse(uint64_t msgid, const QVariant& err, const QVariant& res)
{
	if (!checkVariant(err) || !checkVariant(res)) {
		sendError(msgid, tr("Internal server error, could not serialize response"));
		return false;
	}

	msgpack_pack_array(&m_pk, 4);
	msgpack_pack_int(&m_pk, 1);
	msgpack_pack_int(&m_pk, msgid);
	send(err);
	send(res);
	return true;
}

/**
 * Handle response messages
 *
 * [type(1), msgid(uint), error(str?), result(...)]
 */
void MsgpackIODevice::dispatchResponse(msgpack_object& resp)
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
void MsgpackIODevice::dispatchNotification(msgpack_object& nt)
{
	QByteArray methodName;
	if (decodeMsgpack(nt.via.array.ptr[1], methodName)) {
		qDebug() << "Received Invalid notification: event MUST be a String";
		return;
	}

	QVariant val; 
	if (decodeMsgpack(nt.via.array.ptr[2], val) ||
			(QMetaType::Type)val.type() != QMetaType::QVariantList  ) {
		qDebug() << "Unable to unpack notification parameters" << nt;
		return;
	}
	emit notification(methodName, val.toList());
}

/**
 * Sets latest error code and message for this connector
 */
void MsgpackIODevice::setError(MsgpackError err, const QString& msg)
{
	m_error = err;
	m_errorString = msg;
	qWarning() << "MsgpackIO fatal error" << m_errorString;
	emit error(m_error);
}

QString MsgpackIODevice::errorString() const
{
	if (m_error) {
		return m_errorString;
	} else {
		return m_dev->errorString();
	}
}

/**
 * Start an RPC request
 *
 * Use send() to pass on each of the call parameters
 *
 * Returns a NeovimRequest object. You can connect to
 * its finished() SIGNAL to handle the response
 */
NeovimRequest* MsgpackIODevice::startRequestUnchecked(const QString& method, uint32_t argcount)
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
 * Returns a new msgid that can be used for a msg
 */
uint32_t MsgpackIODevice::msgId()
{
	return this->m_reqid++;
}


/**
 * Serialise a value into the msgpack stream
 */
void MsgpackIODevice::send(int64_t i)
{
	msgpack_pack_int64(&m_pk, i);
}

/**
 * Serialise a value into the msgpack stream
 */
void MsgpackIODevice::send(const QByteArray& bin)
{
	msgpack_pack_bin(&m_pk, bin.size());
	msgpack_pack_bin_body(&m_pk, bin.constData(), bin.size());
}

/**
 * Serialise a valud into the msgpack stream
 */
void MsgpackIODevice::send(bool b)
{
	if ( b ) {
		msgpack_pack_true(&m_pk);
	} else {
		msgpack_pack_false(&m_pk);
	}
}

void MsgpackIODevice::send(const QList<QByteArray>& list)
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
void MsgpackIODevice::send(const QVariant& var)
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
	case QMetaType::QString:
		send(encode(var.toString()));
		break;
	default:
		msgpack_pack_nil(&m_pk);
		qWarning() << "There is a BUG in the QVariant serializer" << var.type();
	}
}

/**
 * Convert string to the proper encoding
 * \see MsgpackIODevice::setEncoding
 */
QByteArray MsgpackIODevice::encode(const QString& str)
{
	if (m_encoding) {
		return m_encoding->fromUnicode(str);
	} else {
		qWarning() << "Encoding String into MsgpackIODevice without an encoding (defaulting to utf8)";
		return str.toUtf8();
	}
}

/**
 * Decode byte array as string, from Neovim's encoding
 */
QString MsgpackIODevice::decode(const QByteArray& data)
{
	if (m_encoding) {
		return m_encoding->toUnicode(data);
	} else {
		qWarning() << "Decoding String from MsgpackIODevice without an encoding (defaulting to utf8)";
		return QString::fromUtf8(data);
	}
}


} // Namespace NeovimQt
