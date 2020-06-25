#include <QLocalSocket>
#include <QMetaMethod>
#include <QtTest/QtTest>
#include <neovimconnector.h>
#include "common.h"

/**
 * This is the closest I can get to writing a 5 minute unit test without
 * putting any effort into this - basically we call all API methods that
 * have no arguments.
 *
 * -- Don't take this too seriously, I just wanted to trigger some unpacking
 *    bugs :D
 */

class TestCallAllMethods: public QObject
{
	Q_OBJECT

private slots:
	void initTestCase();
	void vim_get_current_buffer();
	void vim_list_runtime_paths();
	void vim_call_function();
	void callAll();

	void buffer_line_count();
	void vim_var();
	void tabpage();
private:
	template <typename API>
	void buffer_line_count(API*);
	template <typename API>
	void vim_var(API*);
	template <typename API>
	void tabpage(API*);

	void call_metaobject_slots(QObject *obj);
	NeovimQt::NeovimConnector *m_c;
};

void TestCallAllMethods::initTestCase()
{
	m_c = NeovimQt::NeovimConnector::spawn({"-u", "NONE"});
	QSignalSpy onReady(m_c, SIGNAL(ready()));
	QVERIFY(onReady.isValid());

	QVERIFY(SPYWAIT(onReady));
}

void TestCallAllMethods::vim_get_current_buffer()
{
	QVERIFY(m_c->neovimObject());
	auto *obj = m_c->neovimObject();

	QSignalSpy result(obj, SIGNAL(on_vim_get_current_buffer(int64_t)));
	QVERIFY(result.isValid());

	obj->vim_get_current_buffer();
	QVERIFY(SPYWAIT(result));
}

void TestCallAllMethods::vim_list_runtime_paths()
{
	QVERIFY(m_c->neovimObject());
	auto *obj = m_c->neovimObject();

	QSignalSpy result(obj, SIGNAL(on_vim_list_runtime_paths(QList<QByteArray>)));
	QVERIFY(result.isValid());

	obj->vim_list_runtime_paths();
	QVERIFY(SPYWAIT(result));
}

void TestCallAllMethods::callAll()
{
	call_metaobject_slots(m_c->neovimObject());
	call_metaobject_slots(m_c->api0());
	call_metaobject_slots(m_c->api1());
	call_metaobject_slots(m_c->api2());
	call_metaobject_slots(m_c->api3());
	call_metaobject_slots(m_c->api4());
	call_metaobject_slots(m_c->api5());
	call_metaobject_slots(m_c->api6());
}

void TestCallAllMethods::call_metaobject_slots(QObject *obj)
{
	QVERIFY(obj);
	const QMetaObject *meta = obj->metaObject();
	QSignalSpy neovimErrors(m_c, SIGNAL(error(NeovimError)));
	QVERIFY(neovimErrors.isValid());
	for (int i=0; i<meta->methodCount(); i++) {
		QMetaMethod meth = meta->method(i);
		if ( meth.methodType() != QMetaMethod::Slot ||
				meth.access() != QMetaMethod::Public) {
			continue;
		}

		if ( meth.parameterNames().size() == 0 && meth.name() != "deleteLater") {
			qDebug() << "Calling" << meth.name();
			meth.invoke(obj);
		} else if ( meth.parameterNames().size() == 1 ) {
			if ( meth.parameterTypes().at(0) == "Window" ) {
				meth.invoke(obj, Q_ARG(int64_t, 1));
			}
		} else {
			qDebug() << "Skipping" << meth.methodSignature();
		}
	}

	// Test Performance: timeout expected, set value carefully.
	QVERIFY2(!SPYWAIT(neovimErrors, 2500 /*msec*/), "Fatal errors");
}

/// vim_call_functions() was the first API function
/// to use the Array type
void TestCallAllMethods::vim_call_function()
{
	QVERIFY(m_c->neovimObject());
	auto *obj = m_c->neovimObject();

	QSignalSpy result(obj, SIGNAL(on_vim_call_function(QVariant)));
	QVERIFY(result.isValid());

	QVariantList args;
	args << -2;
	obj->vim_call_function("abs", args);
	QVERIFY(SPYWAIT(result));
	QCOMPARE(result.at(0).at(0), QVariant(2));
}

#define call_test_all_api(NAME) \
	NAME(m_c->neovimObject());	\
	NAME(m_c->api0());			\
	NAME(m_c->api1());			\
	NAME(m_c->api2());			\
	NAME(m_c->api3());			\
	NAME(m_c->api4());			\
	NAME(m_c->api5());			\
	NAME(m_c->api6());			\

void TestCallAllMethods::buffer_line_count()
{
	call_test_all_api(buffer_line_count);
}
template <typename API>
void TestCallAllMethods::buffer_line_count(API* obj)
{
	QVERIFY(obj);

	QSignalSpy result(obj, SIGNAL(on_buffer_line_count(int64_t)));
	QVERIFY(result.isValid());

	obj->buffer_line_count(0);
	QVERIFY(SPYWAIT(result));
}

void TestCallAllMethods::vim_var()
{
	call_test_all_api(vim_var);
}
/// Set/get/del variable
template <typename API>
void TestCallAllMethods::vim_var(API* obj)
{
	auto varname = "vim_test_var";
	auto varvalue = "testvalue";

	QVERIFY(obj);

	obj->vim_set_var(varname, QString(varvalue));

	QSignalSpy result(obj, SIGNAL(on_vim_get_var(QVariant)));
	QVERIFY(result.isValid());
	obj->vim_get_var(varname);

	QVERIFY(SPYWAIT(result));

	QVERIFY(result.at(0).at(0) == QByteArray(varvalue));

	QSignalSpy result2(obj, SIGNAL(on_vim_del_var(QVariant)));
	QVERIFY(result2.isValid());
	obj->vim_del_var(varname);
	QVERIFY(SPYWAIT(result2));
}

void TestCallAllMethods::tabpage()
{
	call_test_all_api(tabpage);
}

template <typename API>
void TestCallAllMethods::tabpage(API* obj)
{
	auto varname = "tabpage-testvar";
	QVERIFY(obj);
	obj->tabpage_is_valid(0);
	obj->tabpage_get_window(0);
	obj->tabpage_get_windows(0);
	obj->tabpage_set_var(0, varname, "testval");
	obj->tabpage_get_var(0, varname);

	QSignalSpy result(obj, SIGNAL(on_tabpage_del_var(QVariant)));
	QVERIFY(result.isValid());
	obj->tabpage_del_var(0, varname);
	QVERIFY(SPYWAIT(result));
}

QTEST_MAIN(TestCallAllMethods)
#include "tst_callallmethods.moc"
