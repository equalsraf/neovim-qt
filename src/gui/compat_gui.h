#pragma once
// Compatibility functions between different Qt versions

#include <QRect>
#include <QWidget>

QRect screenAvailableGeometry(const QWidget*);
