#include <QString>

QString midString(const QString& s, int pos, int len)
{
	return s.midRef(pos, len).toString();
}

QString rightString(const QString& s, int pos)
{
	return s.rightRef(pos).toString();
}
