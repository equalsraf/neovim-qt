#include <QApplication>
#include <QDebug>
#include <QTimer>
#include "shellwidget.h"
#include "helpers.h"

#if defined(Q_OS_WIN) && defined(USE_STATIC_QT)
#include <QtPlugin>
Q_IMPORT_PLUGIN (QWindowsIntegrationPlugin);
#endif

ShellWidget *s = NULL;

void fun()
{
	int w = s->contents().columns();
	int r = s->contents().rows();
	int q = w*r;

	char c = 'a' + (qrand() % 20);
	for (int i=0; i<q; i++) {
		int row = qrand() % r;
		int col = qrand() % w;

		QColor fg = QColor(qrand() % 255, qrand() % 255, qrand() & 255); 
		QColor bg = QColor(qrand() % 255, qrand() % 255, qrand() & 255); 
		s->put(QString(c), row, col, fg, bg);
	}
}

int main(int argc, char **argv)
{
	QApplication app(argc, argv);

	s = new ShellWidget();
	s->resizeShell(20, 80);
	s->setShellFont("DejaVu Sans Mono", 20);
	s->setBackground(Qt::white);
	s->setForeground(Qt::black);
	s->show();

	QTimer *t = new QTimer();
	QObject::connect(t, &QTimer::timeout, fun);
	t->start(10);

	return app.exec();
}
