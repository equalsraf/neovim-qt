#include "neovimrequest.h"
#include "neovimconnector.h"
#include "msgpackiodevice.h"
#include "util.h"

namespace NeovimQt {

/**
 * \class NeovimQt::NeovimRequest
 *
 * \brief A NeovimRequest represents an ongoing API call
 */

/**
 * \fn NeovimQt::NeovimRequest::finished
 *
 * \brief The request has finished
 */

/**
 * Creates a new NeovimRequest, identified by id
 *
 * \see NeovimQt::MsgpackIODevice::msgId
 */
NeovimRequest::NeovimRequest(uint32_t id, MsgpackIODevice *dev, QObject *parent)
:QObject(parent), m_id(id), m_dev(dev), m_function(Function::NEOVIM_FN_NULL)
{
}

/**
 * Process the response message for this call
 *
 * \see NeovimQt::NeovimRequest::finished
 */
void NeovimRequest::processResponse(const msgpack_object& res, bool failed)
{
	if (!failed) {
		emit finished(this->m_id, m_function, res);
		return;
	}

	// TODO: support Neovim error types Exception/Validation/etc
	// This will require chaing the error() signature to hold a type
	// parameter - and update the generator to get error types from the api
	// info
	if (res.type == MSGPACK_OBJECT_ARRAY &&
			res.via.array.size >= 2 ) {
		QByteArray val;
		if (!decodeMsgpack(res.via.array.ptr[1], val)) {
			emit error(this->m_id, m_function, m_dev->decode(val), res);
			return;
		}
	}
	emit error(this->m_id, m_function, tr("Received unsupported error type"), res);
}

/**
 * The function id for the function signature associated with this call.
 * The value NEOVIM_FN_NULL indicates this call will not go through the
 * the generated function handlers.
 */
Function::FunctionId NeovimRequest::function()
{
	return m_function;
}

/**
 * Associate a function id with this request
 *
 * NeovimQt has auto-generated call handlers (in NeovimQt::NeovimConnector::neovimObject)
 * that will be used to process the response
 */
void NeovimRequest::setFunction(Function::FunctionId f)
{
	m_function = f;
}

} // namespace NeovimQt
