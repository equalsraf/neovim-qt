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

static void SendNeovimCommand(NeovimConnector& connector, const QString& command) noexcept
{
	QSignalSpy spyCommand{ connector.api0()->vim_command_output(command.toUtf8()),
		&MsgpackRequest::finished };

	QVERIFY(spyCommand.isValid());
	QVERIFY(SPYWAIT(spyCommand));
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
	auto csWithLineGrid{ CreateShellWidget() };
	Shell* sWithLineGrid{ csWithLineGrid.second };
	ShellOptions shellOptionsWithLineGrid{ sWithLineGrid->GetShellOptions() };

	QCOMPARE(shellOptionsWithLineGrid.IsLineGridEnabled(), true);

	settings.setValue("ext_linegrid", false);
	auto csLegacy{ CreateShellWidget() };
	Shell* sLegacy{ csLegacy.second };
	ShellOptions shellOptionsLegacy{ sLegacy->GetShellOptions() };

	QCOMPARE(shellOptionsLegacy.IsLineGridEnabled(), false);
}

void TestQSettings::OptionPopupMenu() noexcept
{
	auto cw{ CreateMainWindowWithRuntime() };
	NeovimConnector& connector{ *cw.first };

	QSettings settings;

	SendNeovimCommand(connector, "GuiPopupmenu 1");
	QCOMPARE(settings.value("ext_popupmenu").toBool(), true);

	SendNeovimCommand(connector, "GuiPopupmenu 0");
	QCOMPARE(settings.value("ext_popupmenu").toBool(), false);
}

void TestQSettings::OptionTabline() noexcept
{
	auto cw{ CreateMainWindowWithRuntime() };
	NeovimConnector& connector{ *cw.first };

	QSettings settings;

	SendNeovimCommand(connector, "GuiTabline 1");
	QCOMPARE(settings.value("ext_tabline").toBool(), true);

	SendNeovimCommand(connector, "GuiTabline 0");
	QCOMPARE(settings.value("ext_tabline").toBool(), false);
}

void TestQSettings::GuiFont() noexcept
{
	auto cw{ CreateMainWindowWithRuntime() };
	NeovimConnector& connector{ *cw.first };
	MainWindow& window{ *cw.second };

	QSettings settings;

	const QString fontDesc{ QStringLiteral("%1:h20").arg(GetPlatformTestFont()) };
	const QString fontCommand{ QStringLiteral("GuiFont! %1").arg(fontDesc) };

	SendNeovimCommand(connector, fontCommand);
	QCOMPARE(window.shell()->fontDesc(), fontDesc);
	QCOMPARE(settings.value("Gui/Font").toString(), fontDesc);
}

void TestQSettings::GuiScrollBar() noexcept
{
	auto cw{ CreateMainWindowWithRuntime() };
	NeovimConnector& connector{ *cw.first };

	QSettings settings;

	SendNeovimCommand(connector, "GuiScrollBar 1");
	QCOMPARE(settings.value("Gui/ScrollBar").toBool(), true);

	SendNeovimCommand(connector, "GuiScrollBar 0");
	QCOMPARE(settings.value("Gui/ScrollBar").toBool(), false);
}
void TestQSettings::GuiTreeView() noexcept
{
	auto cw{ CreateMainWindowWithRuntime() };
	NeovimConnector& connector{ *cw.first };

	QSettings settings;

	SendNeovimCommand(connector, "GuiTreeviewShow");
	QCOMPARE(settings.value("Gui/TreeView").toBool(), true);

	SendNeovimCommand(connector, "GuiTreeviewHide");
	QCOMPARE(settings.value("Gui/TreeView").toBool(), false);
}

} // Namespace NeovimQt

QTEST_MAIN(NeovimQt::TestQSettings)
#include "tst_qsettings.moc"
