#include "util.h"

namespace NeovimQt {

/**
 * Some Neovim functions take arguments of type **Object** which works an
 * abstract container for various data types - In this lib we use QVariant
 * for this purpose
 *
 * The problem being that QVariant can encapsulate some data types that cannot
 * be serialised at as msgpack data, or just would not make sense for Neovim to
 * handle.
 *
 * Ideally we would detect such errors early on and avoid sending those objects
 * entirely - this function checks if QVariant objects are admissible
 */
bool checkVariant(const QVariant& )
{
	// FIXME: implement my please :D
	return true;
}

/**
 * Convert msgpack object into QByteArray
 */
QByteArray toByteArray(const msgpack_object& obj)
{
	if ( obj.type != MSGPACK_OBJECT_RAW ) {
		return QByteArray();
	}
	return QByteArray(obj.via.raw.ptr, obj.via.raw.size);
}

bool decodeMsgpack(const msgpack_object& in, bool& out)
{
	if ( in.type != MSGPACK_OBJECT_BOOLEAN) {
		qWarning() << "Attempting to decode as bool when type is" << in.type << in;
		out = false;
		return true;
	}
	out = in.via.boolean;
	return false;
}

bool decodeMsgpack(const msgpack_object& in, QByteArray& out)
{
	if ( in.type != MSGPACK_OBJECT_RAW) {
		qWarning() << "Attempting to decode as QByteArray when type is" << in.type << in;
		out = QByteArray();
		return true;
	}
	out = QByteArray(in.via.raw.ptr, in.via.raw.size);
	return false;
}

bool decodeMsgpack(const msgpack_object& in, int64_t& out)
{
	if ( in.type != MSGPACK_OBJECT_POSITIVE_INTEGER) {
		qWarning() << "Attempting to decode as int64_t when type is" << in.type << in;
		out = -1;
		return true;
	}
	out = in.via.i64;
	return false;
}

bool decodeMsgpack(const msgpack_object& in, QList<int64_t>& out)
{
	out.clear();
	if ( in.type != MSGPACK_OBJECT_ARRAY) {
		qWarning() << "Attempting to decode as QList<int64_t> when type is" << in.type << in;
		return true;
	}

	for (uint64_t i=0; i<in.via.array.size; i++) {
		int64_t val;
		if (decodeMsgpack(in.via.array.ptr[i], val)) {
			out.clear();
			return true;
		}
		out.append(val);
	}
	return false;
}

bool decodeMsgpack(const msgpack_object& in, QList<QByteArray>& out)
{
	out.clear();
	if ( in.type != MSGPACK_OBJECT_ARRAY) {
		qWarning() << "Attempting to decode as QList<QByteArray> when type is" << in.type << in;
		return true;
	}

	for (uint64_t i=0; i<in.via.array.size; i++) {
		QByteArray val;
		if (decodeMsgpack(in.via.array.ptr[i], val)) {
			out.clear();
			return true;
		}
		out.append(val);
	}
	return false;
}

bool decodeMsgpack(const msgpack_object& in, QPoint& out)
{
	if ( in.type != MSGPACK_OBJECT_ARRAY || in.via.array.size != 2 ) {
		goto fail;
	}
	int64_t col, row;
	if (decodeMsgpack(in.via.array.ptr[0], row)) {
		goto fail;
	}
	if (decodeMsgpack(in.via.array.ptr[1], col)) {
		goto fail;
	}

	// QPoint is (x,y)  neovim Position is (row, col)
	out = QPoint(col, row);
	return false;

fail:
	qWarning() << "Attempting to decode as QPoint failed" << in.type << in;
	out =  QPoint();
	return true;
}

// FIXME: this should match send(QVariant) -> enforce it
/**
 * We use QVariants for RPC functions that use the *Object* type do not use
 * this in any other conditions
 */
bool decodeMsgpack(const msgpack_object& in, QVariant& out)
{
	switch (in.type) {
	case MSGPACK_OBJECT_NIL:
		out = QVariant(QMetaType::Void, NULL);
		break;
	case MSGPACK_OBJECT_BOOLEAN:
		out = in.via.boolean;
		break;
	case MSGPACK_OBJECT_NEGATIVE_INTEGER:
		out = QVariant((qint64)in.via.i64);
		break;
	case MSGPACK_OBJECT_POSITIVE_INTEGER:
		out = QVariant((quint64)in.via.u64);
		break;
	case MSGPACK_OBJECT_DOUBLE:
		out = in.via.dec;
		break;
	case MSGPACK_OBJECT_RAW:
		{
		QByteArray val;
		if (decodeMsgpack(in, val)) {
			qWarning() << "Error unpacking ByteArray as QVariant";
			out = QVariant();
			return true;
		}
		out = val;
		}
		break;
	case MSGPACK_OBJECT_ARRAY:
		// Either a QVariantList or a QStringList
		{
		QVariantList ls;
		for (uint64_t i=0; i<in.via.array.size; i++) {
			QVariant v;
			bool failed = decodeMsgpack(in.via.array.ptr[i], v);
			if (failed) {
				qWarning() << "Error unpacking Map as QVariantList";
				out = QVariant();
				return true;
			}
			ls.append(v);
		}
		out = ls;
		}
		break;
	case MSGPACK_OBJECT_MAP:
		{
		QVariantMap m;
		for (uint64_t i=0; i<in.via.map.size; i++) {
			QByteArray key;
			if (decodeMsgpack(in.via.map.ptr[i].key, key)) {
				qWarning() << "Error decoding Object(Map) key";
				out = QVariant();
				return true;
			}
			QVariant val;
			if (decodeMsgpack(in.via.map.ptr[i].val, val)) {
				qWarning() << "Error decoding Object(Map) value";
				out = QVariant();
				return true;
			}
			m.insert(key,val);
		}
		out = m;
		}
		break;
	
	default:
		out = QVariant();
		qWarning() << "Unsupported type found in Object" << in.type;
		return true;
	}
	return false;
}

} // namespace

/**
 * Overload to use msgpack_object with qDebug()
 *
 */
QDebug operator<<(QDebug dbg, const msgpack_object& obj)
{
	switch(obj.type) {
	case MSGPACK_OBJECT_NIL:
		dbg.space() << "NIL";
		break;
	case MSGPACK_OBJECT_BOOLEAN:
		dbg.space() <<  obj.via.boolean;
		break;
	case MSGPACK_OBJECT_POSITIVE_INTEGER:
		dbg.space() <<  obj.via.u64;
		break;
	case MSGPACK_OBJECT_NEGATIVE_INTEGER:
		dbg.space() <<  obj.via.i64;
		break;
	case MSGPACK_OBJECT_DOUBLE:
		dbg.space() <<  obj.via.dec;
		break;
	case MSGPACK_OBJECT_RAW:
		dbg.space() << QByteArray(obj.via.raw.ptr, obj.via.raw.size);
		break;
	case MSGPACK_OBJECT_ARRAY:
		dbg.nospace() << "[";
		for (uint32_t i=0; i<obj.via.array.size; i++) {
			dbg.nospace() << obj.via.array.ptr[i];
			dbg.space() << ",";
		}
		dbg.nospace() << "]";
		break;
	case MSGPACK_OBJECT_MAP:
		dbg.nospace() << "{";
		for (uint32_t i=0; i<obj.via.map.size; i++) {
			dbg.nospace() << obj.via.map.ptr[i].key;
			dbg.space() << ":";
			dbg.nospace() << obj.via.map.ptr[i].val;
			dbg.space() << ",";
		}
		dbg.nospace() << "}";
		break;
	default:
		dbg.space() <<  "[Uknown msgpack type]";
	}
	return dbg.maybeSpace();
}

typedef QPair<QString,QString> Param;
QDebug operator<<(QDebug dbg, const NeovimQt::Function& f)
{
	dbg.space() << f.return_type << f.name << "(";
	foreach(Param p, f.parameters) {
		dbg.space() << p.first << ",";
	}
	dbg.space() << ")" << "fails:" << f.can_fail;
	return dbg.maybeSpace();
}


