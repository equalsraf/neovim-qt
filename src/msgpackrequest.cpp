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
:QObject(parent), id(id), m_dev(dev), m_function(Function::NEOVIM_FN_NULL)
{
	connect(&m_timer, &QTimer::timeout,
		this, &MsgpackRequest::requestTimeout);
}

/**
 * The function id for the function signature associated with this call.
 * The value NEOVIM_FN_NULL indicates this call will not go through the
 * the generated function handlers.
 */
Function::FunctionId MsgpackRequest::function()
{
	return m_function;
}

/**
 * Associate a function id with this request
 *
 * NeovimQt has auto-generated call handlers (in NeovimQt::NeovimConnector::neovimObject)
 * that will be used to process the response
 */
void MsgpackRequest::setFunction(Function::FunctionId f)
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
