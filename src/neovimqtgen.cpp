#include <QCoreApplication>
#include <QtGlobal>
#include <QFile>
#include <QDebug>
#include <QDir>
#include <QDateTime>
#include <msgpack.h>
#include <stdio.h>
#include <unistd.h>
#include "function.h"
#include "util.h"

#define NL "\n"
#define TAB "\t"
#define COMMA ", "
#define SEMICOLON ";"
#define QUOT "\""

bool generate_function_enum(const QList<NeovimQt::Function> &ftable, QDir& dst)
{
	QFile f( dst.filePath(QLatin1String("function_enum.h")) );
	if ( !f.open(QIODevice::WriteOnly) ) {
		printf("Unable to open function_enum.h for writing\n");
		return false;
	}
	QTextStream out(&f);

	out << "// Auto generated " << QDateTime::currentDateTime().toString() << NL;
	out << "enum FunctionId {\n";
	foreach(NeovimQt::Function f, ftable) {
		QString l = "\tNEOVIM_FN_%1,\n";
		out << l.arg( QString(f.name).toUpper() );
	}
	out <<	"\tNEOVIM_FN_NULL\n};";
	return true;
}

typedef QPair<QString,QString> Param;
bool generate_function_static(const QList<NeovimQt::Function> &ftable, QDir& dst)
{
	QFile f( dst.filePath(QLatin1String("function_static.cpp")) );
	if ( !f.open(QIODevice::WriteOnly) ) {
		printf("Unable to open function_static.cpp for writing\n");
		return false;
	}
	QTextStream out(&f);

	out << "// Auto generated " << QDateTime::currentDateTime().toString() << NL;
	out << "const QList<Function> Function::knownFunctions = QList<Function>()\n";
	foreach(NeovimQt::Function f, ftable) {
		out << QString("<< Function(\"%1\", \"%2\",\n").arg(f.return_type, f.name);
		out << "\tQList<QString>()\n";
		foreach(Param p, f.parameters) {
			out << QString("\t\t<< QString(\"%1\")\n").arg(p.first);
		}
		out << QString("\t, %1)\n").arg(f.can_fail);
	}
	out << "\t;";
	return true;
}

bool generate_neovim_h(const QList<NeovimQt::Function> &ftable, QDir& dst)
{
	QFile f( dst.filePath(QLatin1String("neovim.h")) );
	if ( !f.open(QIODevice::WriteOnly) ) {
		printf("Unable to open neovim.h for writing\n");
		return false;
	}
	QTextStream out(&f);

	out << "// Auto generated " << QDateTime::currentDateTime().toString() << NL;
	out << "#ifndef NEOVIM_QT_NEOVIMOBJ\n";
	out << "#define NEOVIM_QT_NEOVIMOBJ\n";
	out << "#include \"function.h\"\n";
	out << "#include <msgpack.h>\n";
	out << "namespace NeovimQt {\n";
	out << "class NeovimConnector;\n";
	out << "class Neovim: public QObject\n";
	out << "{\n\n";
	out << "\tQ_OBJECT\n";
	out << "public:\n";
	out << "\tNeovim(NeovimConnector *);\n";
	out << "protected slots:\n";
	out << "\tvoid handleResponse(uint32_t id, Function::FunctionId fun, bool error, const msgpack_object&);\n";
	out << "signals:\n";
	out << "\tvoid error(const QString& errmsg);\n";
	out << "private:\n";
	out << "\tNeovimConnector *m_c;\n";

	out << "public slots:\n";
	foreach(NeovimQt::Function f, ftable) {
		out << QString("\tvoid %1(").arg(f.name);
		for (int i=0; i<f.parameters.size(); i++) {
			if (i) {
				out << COMMA;
			}
			out << QString("%1 %2").arg(f.parameters.at(i).first, f.parameters.at(i).second);
		}
		out << ");\n";
	}
	out << "\nsignals:\n";
	foreach(NeovimQt::Function f, ftable) {
		out << QString("\tvoid %1(%2);\n").arg("on_" + f.name, f.return_type);
	}

	out << "};\n";
	out << "} // namespace\n";
	out << "#endif\n";

	return true;
}

bool generate_neovim_cpp(const QList<NeovimQt::Function> &ftable, QDir& dst)
{
	QFile f( dst.filePath(QLatin1String("neovim.cpp")) );
	if ( !f.open(QIODevice::WriteOnly) ) {
		printf("Unable to open neovim.cpp for writing\n");
		return false;
	}
	QTextStream out(&f);

	out << "// Auto generated " << QDateTime::currentDateTime().toString() << NL;
	out << "#include \"neovim.h\"\n";
	out << "#include \"neovimconnector.h\"\n";
	out << "namespace NeovimQt {\n";
	out << "Neovim::Neovim(NeovimConnector *c)\n";
	out << ":m_c(c)\n{\n}\n\n";

	foreach(NeovimQt::Function f, ftable) {
		out << QString("void Neovim::%1(").arg(f.name);
		for (int i=0; i<f.parameters.size(); i++) {
			if (i) {
				out << COMMA;
			}
			out << QString("%1 %2").arg(f.parameters.at(i).first, f.parameters.at(i).second);
		}
		out << ")\n{\n";
		out << QString("\tNeovimRequest *r = m_c->startRequestUnchecked(\"%1\", %2);\n").arg(f.name).arg(f.parameters.size());
		out << QString("\tr->setFunction(Function::NEOVIM_FN_%1);\n").arg(f.name.toUpper());
		out << "\tconnect(r, &NeovimRequest::finished, this, &Neovim::handleResponse);\n";
		foreach(Param p, f.parameters) {
			out << QString("\tm_c->send(%1);\n").arg(p.second);
		}
		out << "}\n\n";
	}

	// dispatcher
	out << "void Neovim::handleResponse(uint32_t msgid, Function::FunctionId fun, bool failed, const msgpack_object& res)\n{\n";
	out << "\tbool convfail=true;\n";
	out << "\tif (failed) {\n";
	out << "\t\temit error(m_c->to_String(res));\n";
	out << "\t\treturn;\n";
	out << "\t}\n\n";
	out << "\tswitch(fun) {\n";
	foreach(NeovimQt::Function f, ftable) {
		out << "\tcase Function::NEOVIM_FN_" << f.name.toUpper() << ":\n";
		out << "\t\t{\n"; // ctx
		if ( f.return_type != "void" ) {
			out << QString("\t\t\t%1 data = m_c->to_%1(res, &convfail);\n").arg(f.return_type);
			out << "\t\t\tif (convfail) {\n";
			out << QString("\t\t\t\tqWarning() << \"Error unpacking data for signal %1\";\n").arg(f.name);
			out << "\t\t\t} else {\n";
			out << QString("\t\t\t\tqDebug() << \"%1 ->\" << data;\n").arg(f.name);
			out << QString("\t\t\t\temit on_%1(data);\n").arg(f.name);
			out << "\t\t\t}\n";
		} else {
    			out << "\t\t\tqDebug() << \"on_" << f.name << "\";\n";
			out << "\t\t\temit on_" << f.name << "();\n";
		}
		out << "\t\t}\n";
		out << "\t\tbreak;\n";
	}
	out << "\tdefault:\n";
	out << "\t\tqWarning() << \"Received unexpected response\";\n";
	out << "\t}\n";

	out << "}\n";
	out << "} // namespace\n";
	return true;
}

void usage()
{
	printf("Usage: nvim --api-info | neovimqtgen <output dir>");
}

int generate_bindings(const QString& path, const QList<NeovimQt::Function> &ftable)
{
	if (!QDir().mkpath(path)) {
		qFatal("Error creating output folder");
		return -1;
	}
	QDir dst(path);
	if ( !generate_function_enum(ftable, dst) ) {
		return -1;
	}
	if ( !generate_function_static(ftable, dst) ) {
		return -1;
	}
	if ( !generate_neovim_h(ftable, dst) ) {
		return -1;
	}
	if ( !generate_neovim_cpp(ftable, dst) ) {
		return -1;
	}

	printf("Generated Qt bindings for %d functions\n", ftable.size());
	return 0;
}

int main(int argc, char **argv)
{
	QCoreApplication app(argc, argv);
	if ( app.arguments().size() > 2 ) {
		usage();
		return -1;
	}
	bool bindings = app.arguments().size() == 2;

	QFile in;;
	in.open(stdin, QIODevice::ReadOnly);
	if ( isatty(in.handle()) ) {
		usage();
		return -1;
	}

	QByteArray data = in.readAll();

	msgpack_unpacked msg;
	msgpack_unpacked_init(&msg);
	bool ok = msgpack_unpack_next(&msg, 
			data.constData(),
			data.size(), NULL);
	if ( !ok ) {
		printf("Unable to decode metadata");
		return -1;
	}

	if (msg.data.type != MSGPACK_OBJECT_MAP) {
		printf("Found unexpected data type for metadata description\n");
		return -1;
	}

	QList<NeovimQt::Function> ftable;
	for (uint32_t i=0; i< msg.data.via.map.size; i++) {
		const msgpack_object& key = msg.data.via.map.ptr[i].key;
		const msgpack_object& val = msg.data.via.map.ptr[i].val;
		if ( key.type != MSGPACK_OBJECT_RAW || val.type != MSGPACK_OBJECT_ARRAY ) {
			printf("Found unexpected data type for metadata description\n");
			return -1;
		}

		QByteArray key_b = QByteArray(key.via.raw.ptr, key.via.raw.size);
		if ( key_b == "functions" ) {
			for(uint32_t j=0; j<val.via.array.size; j++) {
				NeovimQt::Function f = NeovimQt::Function::fromMsgpack(val.via.array.ptr[j]);
				if ( !f.isValid() ) {
					printf("Unable to parse metadata function\n");
					return -1;
				}
				ftable.append(f);
			}
		}
	}

	if (bindings) {
		return generate_bindings(app.arguments().at(1), ftable);
	} else {
		printf("API info");
		foreach(const NeovimQt::Function f, ftable) {
			QTextStream(stdout) << "  " << f.signature() << "\n";
		}
	}

	return 0;
}

