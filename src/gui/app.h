#ifndef NEOVIM_QT_APP
#define NEOVIM_QT_APP

#include <QApplication>
#include <QEvent>
#include <QUrl>
#include <QList>
#include <QCommandLineParser>

namespace NeovimQt {

class NeovimConnector;
class App: public QApplication
{
	Q_OBJECT
public:
	App(int &argc, char ** argv);
	bool event(QEvent *event);
	void showUi(NeovimConnector *c, const QCommandLineParser&);
	static void processCliOptions(QCommandLineParser& p, const QStringList& arguments);
	static NeovimConnector* createConnector(const QCommandLineParser& p);

signals:
	void openFilesTriggered(const QList<QUrl>);
};

} // Namespace

#endif
