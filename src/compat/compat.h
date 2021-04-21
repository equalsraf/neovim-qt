#pragma once
#include <QString>

// Slice a QString
QString midString(const QString& s, int pos, int len = -1);
QString rightString(const QString& s, int pos);
