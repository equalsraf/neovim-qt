#include <QRect>
#include <QScreen>
#include <QWidget>

QRect screenAvailableGeometry(const QWidget* w)
{
	return w->screen()->availableGeometry();
}
