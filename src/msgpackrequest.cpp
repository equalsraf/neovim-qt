#include "msgpackrequest.h"
#include "msgpackiodevice.h"

namespace NeovimQt {

/**
 * \class NeovimQt::MsgpackRequest
 *
 * \brief A MsgpackRequest represents an ongoing API call
 */

/**
 * \fn NeovimQt::MsgpackRequest::finished
 *
 * \brief The request has finished
 */

/**
 * Creates a new MsgpackRequest, identified by id
 *
 * \see NeovimQt::MsgpackIODevice::msgId
 */
MsgpackRequest::MsgpackRequest(quint32 id, MsgpackIODevice *dev, QObject *parent)
:QObject(parent), id(id), m_dev(dev), m_function(0)
{
	connect(&m_timer, &QTimer::timeout,
		this, &MsgpackRequest::requestTimeout);
}

/**
 * A function id used to match this request with a function
 */
quint64 MsgpackRequest::function()
{
	return m_function;
}

/**
 * Associate a function id with this request
 *
 * NeovimQt has auto-generated call handlers (e.g. in NeovimQt::NeovimConnector::api1)
 * that will be used to process the response
 */
void MsgpackRequest::setFunction(quint64 f)
{
	m_function = f;
}

void MsgpackRequest::setTimeout(int msec)
{
	m_timer.setInterval(msec);
	m_timer.setSingleShot(true);
	m_timer.start();
}

void MsgpackRequest::requestTimeout()
{
	emit timeout(this->id);
}

} // namespace NeovimQt
