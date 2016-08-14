#include "app.h"

#include <QFileOpenEvent>
#include <QMessageBox>

namespace NeovimQt {

App::App(int &argc, char ** argv)
:QApplication(argc, argv)
{
}

bool App::event(QEvent *event)
{
  if( event->type()  == QEvent::FileOpen) {
    //QFileOpenEvent is triggered for each file|URL seperately. Kepping
    //the QList<QUrl> here does not cause any file|URL to be ignored.
    QList<QUrl> urls;
    QFileOpenEvent * fileOpenEvent = static_cast<QFileOpenEvent *>(event);
    if(fileOpenEvent) {
      QString file = fileOpenEvent->file();
      urls.append(fileOpenEvent->url());
      emit openFilesTriggered(urls);
    }
  }
  return QApplication::event(event);
}

} // Namespace
