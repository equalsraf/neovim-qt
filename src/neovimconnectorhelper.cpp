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
void NeovimConnectorHelper::handleMetadataError(quint32 msgid, quint64, const QVariant& errobj)
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
 */
void NeovimConnectorHelper::handleMetadata(quint32 msgid, quint64, const QVariant& result)
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

	int api_compat = metadata.value("version").toMap().value("api_compatible").toUInt();
	int api_level = metadata.value("version").toMap().value("api_level").toUInt();
	qDebug() << "Neovim API version compatible with" << api_compat << "supported" << api_level;
	m_c->m_api_compat = api_compat;
	m_c->m_api_supported = api_level;

#if 0
	QMapIterator<QString,QVariant> it(metadata);
	while (it.hasNext()) {
		it.next();
		if (it.key() == "functions") {
			auto api1 = m_c->api1();
			if (api1 ) {
				NeovimApi1::checkFunctions(it.value().toList());
			}
			auto api2 = m_c->api1();
			if (api2 ) {
				NeovimApi2::checkFunctions(it.value().toList());
			}
		}
	}
#endif

	if (m_c->errorCause() == NeovimConnector::NoError) {
		// Neovim is always utf8, but this was not the case in the early days of nvim
		// these days it should always be utf8
		if (m_c->m_dev->setEncoding("utf8")) {
			m_c->m_ready = true;
			emit m_c->ready();
		} else {
			qWarning() << "Unable to set encoding to utf8";
		}
	} else {
		qWarning() << "Error retrieving metadata" << m_c->errorString();
	}
}

} // Namespace NeovimQt
