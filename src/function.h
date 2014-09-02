#ifndef NEOVIM_QT_FUNCTIONS
#define NEOVIM_QT_FUNCTIONS

#include <QtGlobal>
#include <QByteArray>
#include <QList>
#include <QPair>
#include <QDebug>
#include <QStringList>
#include <QPoint>
#include <msgpack.h>

namespace NeovimQt {

typedef int64_t Integer;
typedef QPoint Position;
typedef bool Boolean;
typedef QString String;
typedef QVariant Object;
typedef int64_t Buffer;
typedef int64_t Window;
typedef int64_t Tabpage;
typedef QList<int64_t> BufferArray;
typedef QList<int64_t> WindowArray;
typedef QList<int64_t> TabpageArray;
typedef QStringList StringArray;

/**
 * A function signature used to identify Neovim RPC functions 
 *
 * The static list **knowFunctions** holds a list of all the supported
 * signature.
 */
class Function {
	Q_ENUMS(FunctionId)
public:

#ifndef NEOVIMQT_NO_AUTO
// Bring in auto-generated enum
#include "auto/function_enum.h"
#endif

	Function();
	Function(const QString& ret, const QString& name, QList<QPair<QString,QString> > params, bool can_fail);
	Function(const QString& ret, const QString& name, QList<QString> paramTypes, bool can_fail);
	bool isValid();
	bool operator==(const Function& other);
	static Function fromMsgpack(const msgpack_object&);
	static QList<QPair<QString,QString> > parseParameters(const msgpack_object& obj);

	bool can_fail;
	QString return_type;
	QString name;
	QList<QPair<QString,QString> > parameters;
	const static QList<Function> knownFunctions;
private:
	bool m_valid;
};

}

#endif // NEOVIM_QT_FUNCTIONS
