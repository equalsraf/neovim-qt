#ifndef NEOVIM_QT_APP
#define NEOVIM_QT_APP

#include <memory>
#include <QApplication>
#include <QCommandLineParser>
#include <QEvent>
#include <QList>
#include <QUrl>

#include "shell.h"

namespace NeovimQt {

class NeovimConnector;
class App: public QApplication
{
	Q_OBJECT
public:
	App(int &argc, char ** argv) noexcept;
	bool event(QEvent *event) noexcept;
	void showUi() noexcept;
	void connectToRemoteNeovim() noexcept;
	void checkArgumentsMayTerminate() noexcept;

private:
	QString getRuntimePath() noexcept;
	QStringList getNeovimArgs() noexcept;
	void processCommandlineOptions() noexcept;
	void setupRequestTimeout() noexcept;
	void showVersionInfo() noexcept;

	QCommandLineParser m_parser;
	std::shared_ptr<NeovimConnector> m_connector;
	int m_exitStatus = 0;

public slots:
	void mainWindowClosing(int);
	void exitWithStatus();

signals:
	void openFilesTriggered(const QList<QUrl>);
};

} // Namespace NeovimQt

#endif
