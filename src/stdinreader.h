#ifndef NEOVIM_QT_STDINNOTIFIER
#define NEOVIM_QT_STDINNOTIFIER

#include <QThread>
#include <QFile>

namespace NeovimQt {
class StdinReader: public QThread
{
        Q_OBJECT
public:
        StdinReader(qint64 maxSize, QObject *parent=0);
        virtual void run();
signals:
        void dataAvailable(const QByteArray& data);

private:
        QFile m_in;
        qint64 m_maxSize;
};

} // Namespace

#endif
