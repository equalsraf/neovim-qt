#include <QRect>
#include <QStringView>

QString midString(const QString& s, int start, int len)
{
	return QStringView{ s }.mid(start, len).toString();
}

QString rightString(const QString& s, int pos)
{
	return QStringView{ s }.right(pos).toString();
}
