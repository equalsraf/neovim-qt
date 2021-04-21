#include <QFont>

QFont::StyleStrategy fontStyleStrategy()
{
	return QFont::StyleStrategy(QFont::PreferDefault | QFont::ForceIntegerMetrics);
}
