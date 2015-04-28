#ifndef NEOVIM_QT_MSGPACKIODEVICE
#define NEOVIM_QT_MSGPACKIODEVICE

#include <QIODevice>
#include <msgpack.h>

namespace NeovimQt {

class NeovimRequest;
class MsgpackIODevice: public QObject
{
	Q_OBJECT
	Q_ENUMS(MsgpackError)
	Q_PROPERTY(QByteArray encoding READ encoding WRITE setEncoding)
public:
	enum MsgpackError {
		NoError=0,
		InvalidDevice,
		InvalidMsgpack,
		UnsupportedEncoding,
	};
	MsgpackIODevice(QIODevice *, QObject *parent=0);
	~MsgpackIODevice();

	bool isOpen() {return m_dev->isOpen();}
	QString errorString() const;

	QByteArray encoding() const;
	bool setEncoding(const QByteArray&);

	uint32_t msgId();
	NeovimRequest* startRequestUnchecked(const QString& method, uint32_t argcount);

	void send(int64_t);
	void send(const QVariant&);
	void send(const QByteArray&);
	void send(bool);
	void send(const QList<QByteArray>& list);
	template <class T>
	void sendArrayOf(const QList<T>& list) {
		msgpack_pack_array(&m_pk, list.size());
		foreach(const T& elem, list) {
			send(elem);
		}
	}

	QByteArray encode(const QString&);
	QString decode(const QByteArray&);

	bool sendResponse(uint64_t msgid, const QVariant& err, const QVariant& res);

signals:
	void error(MsgpackError);
	void notification(const QByteArray &name, const QVariantList& args);

protected:
	void sendError(const msgpack_object& req, const QString& msg);
	void sendError(uint64_t msgid, const QString& msg);
	void dispatch(msgpack_object& obj);
	void dispatchRequest(msgpack_object& obj);
	void dispatchResponse(msgpack_object& obj);
	void dispatchNotification(msgpack_object& obj);

protected slots:
	void setError(MsgpackError err, const QString& msg);
	void dataAvailable();

private:
	static int msgpack_write_cb(void* data, const char* buf, unsigned long int len);

	uint32_t m_reqid;
	QIODevice *m_dev;
	QTextCodec *m_encoding;
	msgpack_packer m_pk;
	msgpack_unpacker m_uk;
	QHash<uint32_t, NeovimRequest*> m_requests;

	QString m_errorString;
	MsgpackError m_error;
};

} // Namespace NeovimQt

#endif
