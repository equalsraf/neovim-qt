#ifndef QSHELLWIDGET2_UTIL
#define QSHELLWIDGET2_UTIL

#include "shellcontents.h"

bool saveShellContents(const ShellContents& s, const QString& filename);
bool isBadMonospace(const QFont& f);

#endif
