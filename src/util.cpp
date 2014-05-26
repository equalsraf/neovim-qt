#include "util.h"

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


QDebug operator<<(QDebug dbg, const NeoVimQt::Function& f)
{
	dbg.space() << f.return_type << f.name << "(";
	foreach(QByteArray p, f.parameterTypes) {
		dbg.space() << p << ",";
	}
	dbg.space() << ")" << "fails:" << f.can_fail;
	return dbg.maybeSpace();
}


