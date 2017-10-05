#ifndef NEOVIM_QT_FUNCTIONS
#define NEOVIM_QT_FUNCTIONS

#include <QtGlobal>
#include <QByteArray>
#include <QList>
#include <QPair>
#include <QDebug>
#include <QStringList>
#include <QPoint>

namespace NeovimQt {

class Function {
	Q_ENUMS(FunctionId)
public:

	Function();
	Function(const QString& ret, const QString& name, QList<QPair<QString,QString> > params, bool can_fail);
	Function(const QString& ret, const QString& name, QList<QString> paramTypes, bool can_fail);
	bool isValid() const;
	bool operator==(const Function& other);
	static Function fromVariant(const QVariant&);
	static QList<QPair<QString,QString> > parseParameters(const QVariantList& obj);

	/** Whether this function call fail without returning*/
	bool can_fail;
	/** Function return type */
	QString return_type;
	/** Function name */
	QString name;
	/** Function parameter types and name */
	QList<QPair<QString,QString> > parameters;

	QString signature() const;
	/**
	 * The static list **knowFunctions** holds a list of all the supported
	 * signature. The list is populated at compile time from a code generator.
	 */
	const static QList<Function> knownFunctions;
private:
	bool m_valid;
};

}

#endif // NEOVIM_QT_FUNCTIONS
