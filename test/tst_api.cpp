#include <msgpackrequest.h>
#include <neovimconnector.h>
#include <QLocalSocket>
#include <QMetaMethod>
#include <QtTest/QtTest>

#include "common.h"

class TestApi : public QObject
{
	Q_OBJECT

private slots:
	void initTestCase();
	void nvim_get_keymap();

private:
	NeovimQt::NeovimConnector* m_c;
};

void TestApi::initTestCase()
{
	m_c = NeovimQt::NeovimConnector::spawn({ "-u", "NORC" });
	QSignalSpy onReady(m_c, SIGNAL(ready()));
	QVERIFY(onReady.isValid());

	QVERIFY(SPYWAIT(onReady));
}


/**
 * This only exists to document api behavior - the LuaRef type can be seen in the callback
 * attribute, but a breaking change means previously this was NIL/Invalid but now it is
 * a string.
 *
 */
void TestApi::nvim_get_keymap()
{
	QVERIFY(m_c->neovimObject());
	auto* api = m_c->api3();

	auto r = api->nvim_execute_lua("vim.api.nvim_set_keymap('n', '<leader>b', '', {noremap = true, "
								   "desc = 'Test', callback = function() end})",
		QVariantList());

	QSignalSpy wait(r, &NeovimQt::MsgpackRequest::finished);
	QVERIFY(wait.isValid());
	QVERIFY(SPYWAIT(wait));
	qDebug() << "nvim_execute_lua" << wait.takeFirst();


	QSignalSpy result(api, SIGNAL(on_nvim_get_keymap(QList<QVariantMap>)));
	QVERIFY(result.isValid());

	api->nvim_get_keymap("n");
	QVERIFY(SPYWAIT(result));

	auto value = result.takeFirst();
	qDebug() << "nvim_get_keymap" << value;
}

QTEST_MAIN(TestApi)
#include "tst_api.moc"
