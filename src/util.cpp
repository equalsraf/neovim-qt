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


QDebug operator<<(QDebug dbg, const NeovimQt::Function& f)
{
	dbg.space() << f.return_type << f.name << "(";
	foreach(QByteArray p, f.parameterTypes) {
		dbg.space() << p << ",";
	}
	dbg.space() << ")" << "fails:" << f.can_fail;
	return dbg.maybeSpace();
}


