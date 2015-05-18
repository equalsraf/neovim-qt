/* $XFree86: xc/programs/xterm/wcwidth.h,v 1.5 2005/05/03 00:38:25 dickey Exp $ */

/* Markus Kuhn -- 2001-01-12 -- public domain */
/* Adaptions for KDE by Waldo Bastian <bastian@kde.org> */

#ifndef KONSOLE_WCWIDTH_H
#define KONSOLE_WCWIDTH_H

// Qt
#include <QtCore/QString>

int konsole_wcwidth(quint16 oucs);

int string_width(const QString& text);

#endif
