#include <QApplication>
#include <QDebug>
#include <QTimer>
#include <QFile>
#include "shellwidget.h"
#include "helpers.h"

#if defined(Q_OS_WIN) && defined(USE_STATIC_QT)
#include <QtPlugin>
Q_IMPORT_PLUGIN (QWindowsIntegrationPlugin);
#endif

ShellWidget *s = NULL;
QFile *f = NULL;

void fun()
{
	static bool start = true;
	int r = s->contents().rows();
	QColor fg = QColor(qrand() % 255, qrand() % 255, qrand() & 255); 

	if (start) {
		start = false;

		for (int i=0; i<r; i++) {
			if (f->atEnd()) {
				start = true;
				if (!f->seek(0)) {
					break;
				}
			}
			QString line = f->readLine();
			s->clearRow(i);
			s->put(line, i, 0, fg);
		}
	} else {
		s->scrollShell(1);
		for (int i=r-1; i<r; i++) {
			if (f->atEnd()) {
				if (!f->seek(0)) {
					break;
				}
			}
			QString line = f->readLine();
			s->clearRow(i);
			s->put(line, i, 0, fg);
		}
	}
}

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	if (app.arguments().size() < 2) {
		qWarning() << "Usage: example3 <file>";
		return -1;
	}

	f = new QFile(app.arguments().at(1));
	if (!f->open(QIODevice::ReadOnly)) {
		qWarning() << "Unable to open file";
		return -1;
	}

	s = new ShellWidget();
	s->setShellFont("DejaVu Sans Mono", 13);
	s->setBackground(Qt::white);
	s->setForeground(Qt::black);
	s->resize(300, 400);
	s->show();

	QTimer *t = new QTimer();
	QObject::connect(t, &QTimer::timeout, fun);
	t->start(10);

	return app.exec();
}
