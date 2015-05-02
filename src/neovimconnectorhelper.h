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
	void handleMetadata(quint32, Function::FunctionId, const QVariant& result);
	void handleMetadataError(quint32 msgid, Function::FunctionId,
		const QVariant& errobj);
	void encodingChanged(const QVariant&);
protected:
	bool checkFunctions(const QVariantList& ftable);
private:
	NeovimConnector *m_c;

};

} // Namespace NeovimQt
#endif
