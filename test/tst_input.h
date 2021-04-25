#pragma once

// A class to hold test data. An event type/key/modifiers
// as used in QKeyEvent and a matching Neovim input string.
struct InputTest
{
	QEvent::Type event_type;
	int key;
	Qt::KeyboardModifiers modifiers;
	QString expected_input;
};
