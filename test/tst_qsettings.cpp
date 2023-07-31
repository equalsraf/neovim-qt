#include <gui/mainwindow.h>
#include <msgpackrequest.h>
#include <QtTest/QtTest>

#include "common.h"
#include "common_gui.h"
#include "mock_qsettings.h"
#include "tst_shell.h"

namespace NeovimQt {

class TestQSettings : public QObject
{
	Q_OBJECT

private slots:
	void initTestCase() noexcept;
	void cleanup() noexcept;

	void OptionLineGrid() noexcept;
	void OptionPopupMenu() noexcept;
	void OptionTabline() noexcept;
	void GuiFont() noexcept;
	void GuiScrollBar() noexcept;
	void GuiTreeView() noexcept;
};

static void SendNeovimCommand(NeovimConnector* connector, const QString& command) noexcept
{
	QSignalSpy spyCommand{ connector->api0()->vim_command_output(connector->encode(command)),
		&MsgpackRequest::finished };

	QVERIFY(spyCommand.isValid());
	QVERIFY(SPYWAIT(spyCommand));


	// Hypothesis: sometimes we do not wait long enough for the effects of
	// a command to manifest because it requires
	// 1. msg from gui to nvim
	// 2. msg from nvim to gui
	// The later are usually asynchronous notifications
	//
	// Attempt to ensure the previous command had the inteded effect
	QTest::qSleep(1000);
}

void TestQSettings::initTestCase() noexcept
{
	NeovimQt::MockQSettings::EnableByDefault();
}

void TestQSettings::cleanup() noexcept
{
	NeovimQt::MockQSettings::ClearAllContents();
}

void TestQSettings::OptionLineGrid() noexcept
{
	QSettings settings;

	settings.setValue("ext_linegrid", true);
	auto sWithLineGrid = CreateShellWidget();
	ShellOptions shellOptionsWithLineGrid{ sWithLineGrid->GetShellOptions() };

	QCOMPARE(shellOptionsWithLineGrid.IsLineGridEnabled(), true);

	settings.setValue("ext_linegrid", false);
	auto sLegacy = CreateShellWidget();
	ShellOptions shellOptionsLegacy{ sLegacy->GetShellOptions() };

	QCOMPARE(shellOptionsLegacy.IsLineGridEnabled(), false);
}

void TestQSettings::OptionPopupMenu() noexcept
{
	auto w = CreateMainWindowWithRuntime();
	NeovimConnector* connector = w->shell()->nvim();

	QSettings settings;
	QSignalSpy spy_fontchange(w->shell(), &ShellWidget::shellFontChanged);

	SendNeovimCommand(connector, "GuiPopupmenu 1");
	SPYWAIT(spy_fontchange, 2500 /*msec*/);
	QCOMPARE(settings.value("ext_popupmenu").toBool(), true);

	SendNeovimCommand(connector, "GuiPopupmenu 0");
	SPYWAIT(spy_fontchange, 2500 /*msec*/);
	QCOMPARE(settings.value("ext_popupmenu").toBool(), false);
}

void TestQSettings::OptionTabline() noexcept
{
	auto w = CreateMainWindowWithRuntime();
	NeovimConnector* connector = w->shell()->nvim();

	QSettings settings;

	SendNeovimCommand(connector, "GuiTabline 1");
	QCOMPARE(settings.value("ext_tabline").toBool(), true);

	SendNeovimCommand(connector, "GuiTabline 0");
	QCOMPARE(settings.value("ext_tabline").toBool(), false);
}

void TestQSettings::GuiFont() noexcept
{
	auto w = CreateMainWindowWithRuntime();
	NeovimConnector* connector = w->shell()->nvim();

	QSettings settings;

	const QString fontDesc{ QStringLiteral("%1:h20").arg(GetPlatformTestFont()) };
	const QString fontCommand{ QStringLiteral("GuiFont! %1").arg(fontDesc) };

	SendNeovimCommand(connector, fontCommand);
	QCOMPARE(w->shell()->fontDesc(), fontDesc);
	QCOMPARE(settings.value("Gui/Font").toString(), fontDesc);
}

void TestQSettings::GuiScrollBar() noexcept
{
	auto w = CreateMainWindowWithRuntime();
	NeovimConnector* connector = w->shell()->nvim();

	QSettings settings;

	SendNeovimCommand(connector, "GuiScrollBar 1");
	QCOMPARE(settings.value("Gui/ScrollBar").toBool(), true);

	SendNeovimCommand(connector, "GuiScrollBar 0");
	QCOMPARE(settings.value("Gui/ScrollBar").toBool(), false);
}
void TestQSettings::GuiTreeView() noexcept
{
	auto w = CreateMainWindowWithRuntime();
	NeovimConnector* connector = w->shell()->nvim();

	QSettings settings;

	SendNeovimCommand(connector, "GuiTreeviewShow");
	QCOMPARE(settings.value("Gui/TreeView").toBool(), true);

	SendNeovimCommand(connector, "GuiTreeviewHide");
	QCOMPARE(settings.value("Gui/TreeView").toBool(), false);
}

} // Namespace NeovimQt

QTEST_MAIN(NeovimQt::TestQSettings)
#include "tst_qsettings.moc"
