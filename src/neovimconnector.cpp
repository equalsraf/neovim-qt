#include "neovimconnector.h"
#include <QtGlobal>

namespace NeoVimQt {

NeoVimConnector::NeoVimConnector(QIODevice *s)
:QObject(), m_socket(s), m_error(NoError), reqid(0), m_neovimobj(NULL)
{
	msgpack_packer_init(&m_pk, this, NeoVimConnector::msgpack_write_cb);
	msgpack_unpacker_init(&m_uk, MSGPACK_UNPACKER_INIT_BUFFER_SIZE);

	connect(m_socket, &QAbstractSocket::readyRead,
			this, &NeoVimConnector::dataAvailable);
	// FIXME: IO errors
	discoverMetadata();
}

NeoVimConnector::~NeoVimConnector()
{
	//msgpack_packer_destroy(&m_pk);
	msgpack_unpacker_destroy(&m_uk);
}

void NeoVimConnector::setError(NeoVimError err, const QString& msg)
{
	m_error = err;
	m_errorString = msg;
	qWarning() << m_error;
	emit error(m_error);
}

/**
 * Called when the msgpack packer has data to write
 */
int NeoVimConnector::msgpack_write_cb(void* data, const char* buf, unsigned int len)
{
	NeoVimConnector *c = static_cast<NeoVimConnector*>(data);
	return c->m_socket->write(buf, len);
}

/**
 * Send a request msg
 *
 * It is up to the caller to call msgpack_pack* to
 * pack each individual argument
 *
 * Returns a NeoVimRequest object. You can connect to
 * its finished() SIGNAL to handle the response
 */
NeoVimRequest* NeoVimConnector::startRequestUnchecked(uint32_t method, uint32_t argcount)
{
	qDebug() << __func__ << method << argcount;
	uint32_t msgid = this->reqid++;
	// [type(0), msgid, method, args]
	msgpack_pack_array(&m_pk, 4);
	msgpack_pack_int(&m_pk, 0);
	msgpack_pack_int(&m_pk, msgid);
	msgpack_pack_int(&m_pk, method);
	msgpack_pack_array(&m_pk, argcount);

	NeoVimRequest *r = new NeoVimRequest( msgid, this);
	m_requests.insert(msgid, r);
	return r;
}

NeoVimRequest* NeoVimConnector::startRequest(Function::FunctionId method, uint32_t argcount)
{
	if ( !m_functionToId.contains(method) ) {
		QString err = tr("Attempting to call method that is not supported by the remote server! Did you wait for the ready signal?");
		qWarning() << err;
		setError(NoSuchMethod, err);
		return NULL;
	}
	NeoVimRequest *r = startRequestUnchecked(m_functionToId.value(method), argcount);
	r->setFunction(method);
	return r;
}

void NeoVimConnector::send(int64_t i)
{
	qDebug() << __func__ << i;
	msgpack_pack_int64(&m_pk, i);
}

void NeoVimConnector::send(const QString& str)
{
	qDebug() << __func__ << str;
	QByteArray utf8 = str.toUtf8();
	msgpack_pack_raw(&m_pk, utf8.size());
	msgpack_pack_raw_body(&m_pk, utf8.constData(), utf8.size());
}

void NeoVimConnector::send(const QStringList& strlist)
{
	qDebug() << __func__ << strlist;
	msgpack_pack_array(&m_pk, strlist.size());
	foreach(const QString& str, strlist) {
		send(str);
	}
}

void NeoVimConnector::send(const QByteArray& raw)
{
	qDebug() << __func__ << raw;
	msgpack_pack_raw(&m_pk, raw.size());
	msgpack_pack_raw_body(&m_pk, raw.constData(), raw.size());
}

void NeoVimConnector::send(bool b)
{
	qDebug() << __func__ << b;
	if ( b ) {
		msgpack_pack_true(&m_pk);
	} else {
		msgpack_pack_false(&m_pk);
	}
}

/**
 * We use QVariants for RPC functions that use the *Object* type do not use
 * this in any other conditions
 */
void NeoVimConnector::send(const QVariant& var)
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
	case QMetaType::QString:
		send(var.toString());
		break;
	case QMetaType::Double:
		msgpack_pack_double(&m_pk, var.toDouble());
		break;
//	case QMetaType::QByteArray:
//		send(var.toByteArray());
//		break;
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
		// FIXME: This should not be possible - write checkVariant() was used
		msgpack_pack_nil(&m_pk);
		qWarning() << "Trying to pack unsupported variant type" << var.type() << "packing Nil instead";
	}
}

// FIXME: this should match send(QVariant) -> enforce it
/**
 * We use QVariants for RPC functions use the *Object* type do not use this in any other conditions
 *
 */
QVariant NeoVimConnector::to_QVariant(const msgpack_object& obj, bool *failed)
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
		res = to_QString(obj, failed);
		break;
	case MSGPACK_OBJECT_ARRAY:
		// Either a QVariantList or a QStringList
		{
		QVariantList ls;
		for (uint64_t i=0; i<obj.via.array.size; i++) {
			QVariant v = to_QVariant(obj.via.array.ptr[i], failed);
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
				to_QString(obj.via.map.ptr[i].key), 
				to_QVariant(obj.via.map.ptr[i].val, failed));
		}
		res = m;
		}
		break;
	
	}
	return res;
}

void NeoVimConnector::discoverMetadata()
{
	NeoVimRequest *r = startRequestUnchecked(0, 0);
	connect(r, &NeoVimRequest::finished,
			this, &NeoVimConnector::handleMetadata);
}

QString NeoVimConnector::to_QString(const msgpack_object& obj, bool *failed)
{
	if ( failed ) {
		*failed = true;
	}
	if ( obj.type != MSGPACK_OBJECT_RAW ) {
		setError(UnexpectedMsg,
				tr("Found unexpected data type when unpacking a string"));
		return QString();
	} else if ( failed ) {
		*failed = false;
	}
	return QString::fromUtf8(obj.via.raw.ptr, obj.via.raw.size);
}

QByteArray NeoVimConnector::to_QByteArray(const msgpack_object& obj, bool *failed)
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

QPoint NeoVimConnector::to_QPoint(const msgpack_object& obj, bool *failed)
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
	return QPoint(to_int64_t(obj.via.array.ptr[1]), to_int64_t(obj.via.array.ptr[0]));
}

bool NeoVimConnector::to_bool(const msgpack_object& obj, bool *failed)
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

QStringList NeoVimConnector::to_QStringList(const msgpack_object& obj, bool *failed)
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
		ret.append(to_QString(obj));
	}

	if ( failed ) {
		*failed = false;
	}
	return ret;
}

int64_t NeoVimConnector::to_int64_t(const msgpack_object& obj, bool *failed)
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

QList<QByteArray> NeoVimConnector::parseParameters(const msgpack_object& obj)
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
 * Processes msgpack with API function declarations
 *
 * The ftable MUST be an array
 */
void NeoVimConnector::addFunctions(const msgpack_object& ftable)
{
	if ( ftable.type != MSGPACK_OBJECT_ARRAY ) {
		setError( UnexpectedMsg,
				tr("Found unexpected data type when unpacking function table"));
		return;
	}
	for (uint32_t i=0; i<ftable.via.array.size; i++) {
		addFunction(ftable.via.array.ptr[i]);
	}

	Q_ASSERT(m_functionToId.size() == m_idToFunction.size());
	if ( Function::knownFunctions.size() != m_functionToId.size() ) {
		qDebug() << "API mismatch";
		setError( APIMisMatch,
				tr("Cannot connect to this instance of NeoVim, its version is likely too old"));
		return;
	}
	emit ready();
}

/**
 * Add a function
 *
 */
void NeoVimConnector::addFunction(const msgpack_object& fun)
{
	if ( fun.type != MSGPACK_OBJECT_MAP ) {
		setError( UnexpectedMsg,
			tr("Found unexpected data type when unpacking function"));
		return;
	}	

	Function f;
	uint64_t msgpack_id;
	for (uint32_t i=0; i<fun.via.map.size; i++) {
		if ( fun.via.map.ptr[i].key.type != MSGPACK_OBJECT_RAW ) {
			setError( UnexpectedMsg,
					tr("Found unexpected data type when unpacking function item"));
			return;
		}

		QByteArray key = to_QByteArray(fun.via.map.ptr[i].key);
		msgpack_object& val = fun.via.map.ptr[i].val;
		if ( key == "id" ) {
			if ( val.type != MSGPACK_OBJECT_POSITIVE_INTEGER ) {
				setError( UnexpectedMsg,
					tr("Found unexpected data type for function id"));
				return;
			}
			msgpack_id = val.via.u64;
		} else if ( key == "return_type" ) {
			if ( val.type != MSGPACK_OBJECT_RAW ) {
				setError( UnexpectedMsg,
					tr("Found unexpected data type for function return type"));
				return;
			}
			f.return_type = to_QByteArray(val);
		} else if ( key == "name" ) {
			if ( val.type != MSGPACK_OBJECT_RAW ) {
				setError( UnexpectedMsg,
					tr("Found unexpected data type for function name"));
				return;
			}
			f.name = to_QByteArray(val);
		} else if ( key == "can_fail" ) {
			if ( val.type != MSGPACK_OBJECT_BOOLEAN ) {
				setError( UnexpectedMsg,
					tr("Found unexpected data type for function can_fail"));
				return;
			}
			f.can_fail = val.via.boolean;
		} else if ( key == "parameters" ) {
			if ( val.type != MSGPACK_OBJECT_ARRAY ) {
				setError( UnexpectedMsg,
					tr("Found unexpected data type for function parameters"));
				return;
			}
			f.parameterTypes = parseParameters(val);
		} else {
			qWarning() << "Unsupported function attribute"<< key << val.type;
		}
	}
	int index = Function::knownFunctions.indexOf(f);
	if ( index != -1 ) {
		m_functionToId.insert(Function::FunctionId(index), msgpack_id);
		m_idToFunction.insert(msgpack_id, Function::FunctionId(index));
	}
}

void NeoVimConnector::addClasses(const msgpack_object& ctable)
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

void NeoVimConnector::handleMetadata(uint32_t msgid, Function::FunctionId, bool error, const msgpack_object& result)
{
	if ( error ) {
		setError( NoMetadata,
			tr("Unable to get NeoVim information"));
		return;
	}

	// The metadata bytearray is actually a serialized msgpack 
	msgpack_unpacked msg;
	msgpack_unpacked_init(&msg);
	bool ok = msgpack_unpack_next(&msg, result.via.raw.ptr, result.via.raw.size, NULL);
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
}

/**
 * Send error response
 */
void NeoVimConnector::error(const msgpack_object& req, const QString& msg)
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
 *
 */
void NeoVimConnector::dataAvailable()
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
void NeoVimConnector::dispatch(msgpack_object& req)
{
	//
	// neovim msgpack rpc calls are
	// [type(int), msgid(int), method(int), args(array)]
	//

	if (req.type != MSGPACK_OBJECT_ARRAY) {
		qDebug() << "Invalid msgpack: not an array";
		return;
	}

	if (req.via.array.size != 4) {
		qDebug() << "Received Invalid msgpack: message len MUST be 4";
		return;
	}

	if (req.via.array.ptr[0].type != MSGPACK_OBJECT_POSITIVE_INTEGER) {
		qDebug() << "Received Invalid msgpack: msg type MUST be an integer";
		return;
	}

	uint64_t type = req.via.array.ptr[0].via.u64;
	if (req.via.array.ptr[1].type != MSGPACK_OBJECT_POSITIVE_INTEGER) {
		qDebug() << "Received Invalid msgpack: msg id MUST be a positive integer";
		if ( type == 0 ) {
			error(req, tr("Msg Id must be a positive integer"));
		}
		return;
	}

	if (type == 0 && req.via.array.ptr[2].type != MSGPACK_OBJECT_POSITIVE_INTEGER) {
		qDebug() << "Received Invalid msgpack: method MUST be a positive integer" << req.via.array.ptr[2].type;
		if ( type == 0 ) {
			error(req, tr("Method id must be a positive integer"));
		}
		return;
	}

	if (type == 0 && req.via.array.ptr[3].type != MSGPACK_OBJECT_ARRAY) {
		qDebug() << "Invalid msgpack: arguments MUST be an array";
		if ( type == 0 ) {
			error(req, tr("Paremeters must be an array"));
		}
		return;
	}

	qDebug() << req;
	switch(req.via.array.ptr[0].via.u64) {
	case 0:
		dispatchRequest(req);
		break;
	case 1:
		dispatchResponse(req);
		break;
	case 2:
		dispatchNotification(req);
		break;
	default:
		error(req, tr("Message type is unknown"));
	}
}

/*
 * [type(0), msgid(uint), method(int), args([...])]
 */
void NeoVimConnector::dispatchRequest(msgpack_object& req)
{
	qWarning() << "We do not support requests (yet)";
}

/**
 * Handle response messages
 *
 * [type(1), msgid(uint), error(str?), result(...)]
 */
void NeoVimConnector::dispatchResponse(msgpack_object& resp)
{
	// [type(1), msgid, error, result]
	uint64_t msgid = resp.via.array.ptr[1].via.u64;

	if ( !m_requests.contains(resp.via.array.ptr[1].via.u64) ) {
		qWarning() << "Received response for unknown message";
		return;
	}

	NeoVimRequest *req = m_requests.take(msgid);
	if ( resp.via.array.ptr[2].type != MSGPACK_OBJECT_NIL ) {
		// Error response
		req->processResponse(resp.via.array.ptr[2], true);
	} else {
		req->processResponse(resp.via.array.ptr[3], false);
	}
	req->deleteLater();
}

void NeoVimConnector::dispatchNotification(msgpack_object& req)
{
	qWarning() << "We do not support notifications (yet)";
}

NeoVim* NeoVimConnector::neovimObject()
{
	// FIXME: return NULL if we have not check the metadata
	if ( !m_neovimobj ) {
		m_neovimobj = new NeoVim(this);
	}
	return m_neovimobj;
}

// Request obj
//

NeoVimRequest::NeoVimRequest(uint32_t id, QObject *parent)
:QObject(parent), m_id(id), m_function(Function::NEOVIM_FN_NULL)
{
}

void NeoVimRequest::processResponse(const msgpack_object& res, bool error)
{
	emit finished(this->m_id, m_function, error, res);
}

Function::FunctionId NeoVimRequest::function()
{
	return m_function;
}

void NeoVimRequest::setFunction(Function::FunctionId f)
{
	m_function = f;
}

}; // namespace NeoVimQt


#include "moc_neovimconnector.cpp"
