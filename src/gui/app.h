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
	QCommandLineParser& commandLineParser() { return m_parser; }
	static void checkArgumentsMayTerminate(QCommandLineParser&) noexcept;
	static void processCommandlineOptions(QCommandLineParser&, QStringList) noexcept;
	static void openNewWindow(const QVariantList& args) noexcept;

private:
	static QString getRuntimePath() noexcept;
	static QStringList getNeovimArgs() noexcept;
	static void showVersionInfo(QCommandLineParser&) noexcept;

	QCommandLineParser m_parser;

signals:
	void openFilesTriggered(const QList<QUrl>);
};

} // Namespace NeovimQt

#endif
