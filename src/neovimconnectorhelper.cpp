#include <QMap>

#include "neovimconnectorhelper.h"
#include "neovimconnector.h"
#include "msgpackiodevice.h"
#include "msgpackrequest.h"
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

/** Handle Msgpack-rpc errors when fetching the API metadata */
void NeovimConnectorHelper::handleMetadataError(quint32 msgid, Function::FunctionId,
		const QVariant& errobj)
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
void NeovimConnectorHelper::handleMetadata(quint32 msgid, Function::FunctionId, const QVariant& result)
{
	const QVariantList asList = result.toList();
	if (asList.size() != 2 || 
			!asList.at(0).canConvert<quint64>() ||
			!asList.at(1).canConvert<QVariantMap>()) {
		m_c->setError(NeovimConnector::UnexpectedMsg,
				tr("Unable to unpack metadata response description, unexpected data type"));
	}

	m_c->m_channel = asList.at(0).toUInt();
	const QVariantMap metadata = asList.at(1).toMap();
	QMapIterator<QString,QVariant> it(metadata);

	while (it.hasNext()) {
		it.next();
		if (it.key() == "functions") {
			if (!checkFunctions(it.value().toList())) {
				m_c->setError(NeovimConnector::APIMisMatch,
						tr("API methods mismatch: Cannot connect to this instance of Neovim, its version is likely too old, or the API has changed"));
				return;
			}
		}
	}

	if (m_c->errorCause() == NeovimConnector::NoError) {
		// Get &encoding before we signal readyness
		connect(m_c->neovimObject(), &Neovim::on_vim_get_option,
				this, &NeovimConnectorHelper::encodingChanged);
		MsgpackRequest *r = m_c->neovimObject()->vim_get_option("encoding");
		connect(r, &MsgpackRequest::timeout,
				m_c, &NeovimConnector::fatalTimeout);
		r->setTimeout(10000);
	} else {
		qWarning() << "Error retrieving metadata" << m_c->errorString();
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
	} else {
		qWarning() << "Unable to set encoding" << obj;
	}
}

/**
 * Check function table from api_metadata[1]
 * Returns false if there is an API mismatch
 */
bool NeovimConnectorHelper::checkFunctions(const QVariantList& ftable)
{
	QList<Function::FunctionId> supported;
	foreach(const QVariant& val, ftable) {
		Function::FunctionId fid = Function::functionId(Function::fromVariant(val));
		if (fid != Function::NEOVIM_FN_NULL) {
			supported.append(fid);
		}
	}
	// true if all the generated functions are supported
	return Function::knownFunctions.size() == supported.size();
}

} // Namespace NeovimQt
