#ifndef NEOVIM_QT_INPUT
#define NEOVIM_QT_INPUT

#include <QHash>
#include <QString>
#include <QEvent>
#include <QPoint>

namespace NeovimQt {

class InputConv {
public:
	InputConv();

	QString convertKey(const QString& text, int key, Qt::KeyboardModifiers mod);
	QString modPrefix(Qt::KeyboardModifiers mod);

	QHash<int, QString> specialKeys;
	QHash<QString, QString> replaceKeys;
	QString convertMouse(Qt::MouseButton bt, QEvent::Type type, Qt::KeyboardModifiers mod, QPoint pos, short clicksCount);
};

extern InputConv Input;

} //Namespace

#endif
