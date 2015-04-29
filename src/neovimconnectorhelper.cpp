#include "neovimconnectorhelper.h"
#include "neovimconnector.h"
#include "msgpackiodevice.h"
#include "util.h"

namespace NeovimQt {

/**
 * \class NeovimQt::NeovimConnectorHelper
 * 
 * The helper deals with Neovim API internals, it handles
 * msgpack responses on behalf of the connector.
 *
 */

NeovimConnectorHelper::NeovimConnectorHelper(NeovimConnector *c)
:QObject(c), m_c(c)
{
}

void NeovimConnectorHelper::handleMetadataError(quint32 msgid, Function::FunctionId,
		const QString& msg, const msgpack_object& errobj)
{
	m_c->setError(NeovimConnector::NoMetadata,
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
void NeovimConnectorHelper::handleMetadata(quint32 msgid, Function::FunctionId, const msgpack_object& result)
{
	if ( result.type != MSGPACK_OBJECT_ARRAY || 
			result.via.array.size != 2 ||
			result.via.array.ptr[0].type != MSGPACK_OBJECT_POSITIVE_INTEGER ||
			result.via.array.ptr[1].type != MSGPACK_OBJECT_MAP ) {
		m_c->setError(NeovimConnector::UnexpectedMsg,
				tr("Unable to unpack metadata response description, unexpected data type"));
		return;
	}

	m_c->m_channel = result.via.array.ptr[0].via.u64;

	const msgpack_object metadata = result.via.array.ptr[1];
	if (metadata.type != MSGPACK_OBJECT_MAP) {
		m_c->setError(NeovimConnector::MetadataDescriptorError,
				tr("Found unexpected data type for metadata description"));
		return;
	}

	for (quint32 i=0; i< metadata.via.map.size; i++) {
		QByteArray key;
		if (decodeMsgpack(metadata.via.map.ptr[i].key, key)) {
			m_c->setError(NeovimConnector::MetadataDescriptorError,
				tr("Found unexpected data type for metadata key"));
			continue;
		}
		if ( key == "functions" ) {
			addFunctions(metadata.via.map.ptr[i].val);
		} else if ( key == "classes" ) {
		}
	}

	if (m_c->errorCause() == NeovimConnector::NoError) {
		// Get &encoding before we signal readyness
		connect(m_c->neovimObject(), &Neovim::on_vim_get_option,
				this, &NeovimConnectorHelper::encodingChanged);
		m_c->neovimObject()->vim_get_option("encoding");
	}
}

/**
 * Called after metadata discovery, to get the &encoding
 */
void NeovimConnectorHelper::encodingChanged(const QVariant&  obj)
{
	disconnect(m_c->neovimObject(), &Neovim::on_vim_get_option,
			this, &NeovimConnectorHelper::encodingChanged);
	m_c->m_dev->setEncoding(obj.toByteArray());
	const QByteArray enc_name = obj.toByteArray();
	if (m_c->m_dev->setEncoding(enc_name)) {
		m_c->m_ready = true;
		emit m_c->ready();
	}
}


/**
 * Handle the *functions* attribute in the metadata
 */
void NeovimConnectorHelper::addFunctions(const msgpack_object& ftable)
{
	if ( ftable.type != MSGPACK_OBJECT_ARRAY ) {
		m_c->setError(NeovimConnector::UnexpectedMsg,
				tr("Found unexpected data type when unpacking function table"));
		return;
	}

	QList<Function::FunctionId> supported;
	for (quint32 i=0; i<ftable.via.array.size; i++) {
		Function::FunctionId fid = Function::functionId(Function::fromMsgpack(ftable.via.array.ptr[i]));
		if (fid != Function::NEOVIM_FN_NULL) {
			supported.append(fid);
		}
	}

	if ( Function::knownFunctions.size() != supported.size() ) {
		m_c->setError(NeovimConnector::APIMisMatch,
				tr("API methods mismatch: Cannot connect to this instance of Neovim, its version is likely too old, or the API has changed"));
		return;
	}
}


} // Namespace NeovimQt
