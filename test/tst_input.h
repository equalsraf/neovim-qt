#pragma once

#include <QKeyEvent>
#include <QString>

// A class to hold test data. An event type/key/modifiers
// as used in QKeyEvent and a matching Neovim input string.
struct InputTest final
{
	QKeyEvent event;
	QString expected_input;
};
