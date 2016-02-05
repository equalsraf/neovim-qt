#include <QApplication>
#include <QDebug>
#include "shellwidget.h"
#include "helpers.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);

	QStringList args = app.arguments();
	if (args.size() != 2) {
		qWarning("Usage: example <file>");
		return -1;
	}

	ShellWidget *s = ShellWidget::fromFile(args.at(1));
	s->put("汉语", 0, 0);
	s->show();

	const ShellContents& data = s->contents();
	saveShellContents(data, "example.jpg");

	return app.exec();
}

