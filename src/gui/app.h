#ifndef NEOVIM_QT_APP
#define NEOVIM_QT_APP

#include <QApplication>
#include <QEvent>

namespace NeovimQt {

class App: public QApplication
{
	Q_OBJECT
public:
  App(int &argc, char ** argv);
  bool event(QEvent *event);
signals:
  void neovimOpenFileTriggered(const QString &file);
};

} // Namespace

#endif
