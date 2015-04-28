#ifndef NEOVIM_QT_CONNECTORHELPER
#define NEOVIM_QT_CONNECTORHELPER

#include "neovimconnector.h"

namespace NeovimQt {

class NeovimConnectorHelper: public QObject
{
	Q_OBJECT
public:
	NeovimConnectorHelper(NeovimConnector *);

public slots:
	void handleMetadata(uint32_t, Function::FunctionId, const msgpack_object& result);
	void handleMetadataError(uint32_t msgid, Function::FunctionId,
		const QString& msg, const msgpack_object& errobj);
	void encodingChanged(const QVariant&);
protected:
	void addFunctions(const msgpack_object& ftable);
private:
	NeovimConnector *m_c;

};

} // Namespace NeovimQt
#endif
