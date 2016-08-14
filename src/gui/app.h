#ifndef NEOVIM_QT_APP
#define NEOVIM_QT_APP

#include <QApplication>
#include <QEvent>
#include <QUrl>
#include <QList>

namespace NeovimQt {

class App: public QApplication
{
	Q_OBJECT
public:
  App(int &argc, char ** argv);
  bool event(QEvent *event);
signals:
  void openFilesTriggered(const QList<QUrl>);
};

} // Namespace

#endif
