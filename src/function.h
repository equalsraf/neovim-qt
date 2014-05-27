#ifndef NEOVIM_QT_FUNCTIONS
#define NEOVIM_QT_FUNCTIONS

#include <QtGlobal>
#include <QByteArray>
#include <QList>
#include <QPair>
#include <QDebug>
#include <QStringList>
#include <QPoint>

namespace NeoVimQt {

typedef QList<int64_t> BufferArray;
typedef QList<int64_t> WindowArray;
typedef QList<int64_t> TabpageArray;

/**
 * A function signature used to identify NeoVim RPC functions 
 *
 * The static list **knowFunctions** holds a list of all the supported
 * signature.
 */
class Function {
	Q_ENUMS(FunctionId)
public:

// Bring in auto-generated enum
#include "auto/function_enum.h"

	Function();
	Function(const QByteArray& ret, const QByteArray& name, QList<QByteArray> params, bool can_fail);
	bool operator==(const Function& other);

	bool can_fail;
	QByteArray return_type;
	QByteArray name;
	QList<QByteArray> parameterTypes;
	const static QList<Function> knownFunctions;
};

};

#endif // NEOVIM_QT_FUNCTIONS
