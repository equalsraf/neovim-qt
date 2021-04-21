#include <QApplication>
#include <QDesktopWidget>

QRect screenAvailableGeometry(const QWidget* w)
{
	return QApplication::desktop()->availableGeometry(w);
}
