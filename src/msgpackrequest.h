#ifndef NEOVIM_QT_REQUEST
#define NEOVIM_QT_REQUEST

#include <QObject>
#include <QTimer>

namespace NeovimQt {

class MsgpackIODevice;
class MsgpackRequest: public QObject {
	Q_OBJECT
public:
	MsgpackRequest(quint32 id, MsgpackIODevice *dev, QObject *parent=0);
	void setFunction(quint64);
	quint64 function();
	void setTimeout(int msec);
	/** The identifier for this Msgpack request */
	const quint32 id;
signals:
	void finished(quint32 msgid, quint64 fun, const QVariant& resp);
	void error(quint32 msgid, quint64 fun, const QVariant& err);
	void timeout(quint32 id);
protected slots:
	void requestTimeout();

private:
	MsgpackIODevice *m_dev;
	quint64 m_function;
	QTimer m_timer;
};
} // Namespace

#endif
