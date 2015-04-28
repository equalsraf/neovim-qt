#ifndef NEOVIM_QT_INPUT
#define NEOVIM_QT_INPUT

#include <QHash>
#include <QString>

namespace NeovimQt {

class InputConv {
public:
	InputConv();

	QString convertKey(const QString& text, int key, Qt::KeyboardModifiers mod);
	QString modPrefix(Qt::KeyboardModifiers mod);

	QHash<int, QString> specialKeys;
	QHash<QString, QString> replaceKeys;
};

extern InputConv Input;

} //Namespace

#endif
