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
	void handleMetadata(quint32, quint64, const QVariant& result);
	void handleMetadataError(quint32 msgid, quint64,
		const QVariant& errobj);
private:
	NeovimConnector *m_c;

};

} // Namespace NeovimQt
#endif
