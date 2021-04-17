#include "mainwidget.h"

#include <QCoreApplication>
#include <QDebug>
#include <QSettings>
#include <shell.h>

namespace NeovimQt { namespace Commandline {

MainWidget::MainWidget(NeovimConnector& nvim, ShellWidget& parent) noexcept
	: m_nvim{ nvim }
{
	connect(&m_nvim, &NeovimConnector::ready, this, &MainWidget::neovimConnectorReady);

	setParent(&parent);
	setVisible(false);

	m_cmdTextBoxFrame = new QFrame();
	m_cmdTextBoxFrame->setFrameShape(QFrame::StyledPanel);
	m_cmdTextBoxFrame->setFrameShadow(QFrame::Raised);
	m_cmdTextBoxFrame->setAutoFillBackground(true);
	QVBoxLayout* frameLayout = new QVBoxLayout(m_cmdTextBoxFrame);

	m_cmdTextBox = new ShellWidget();
	m_cmdTextBox->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
	frameLayout->addWidget(m_cmdTextBox);

	m_cmdBlockText = new BlockWidget();

	m_vLayout = new QVBoxLayout();
	m_vLayout->addWidget(m_cmdBlockText);
	m_vLayout->addWidget(m_cmdTextBoxFrame);
	setLayout(m_vLayout);

	QSettings settings("nvim-qt", "nvim-qt");

	QVariant widthPercent{ settings.value("Commandline/width_percent", m_widthPercent) };
	QVariant position{ settings.value("Commandline/position", "center") };

	if (widthPercent.canConvert<int>())
	{
		m_widthPercent = widthPercent.toInt();
	}

	if (position.canConvert<QByteArray>())
	{
		m_position = PositionFromString(position.toString());
	}
}

void MainWidget::neovimConnectorReady() noexcept
{
	connect(m_nvim.api0(), &NeovimApi0::neovimNotification,
		this, &MainWidget::handleNeovimNotification);

	m_nvim.api0()->vim_subscribe("Gui");
}

void MainWidget::handleNeovimNotification(const QByteArray& name, const QVariantList& args) noexcept
{
	if (args.size() <= 0) {
		return;
	}

	if (name == "Gui") {
		const QString guiEvName{ m_nvim.decode(args.at(0).toByteArray()) };

		if (guiEvName == "CommandlinePosition") {
			handleGuiCommandlinePosition(args);
			return;
		}
	}

	if (name == "redraw") {
		Shell::DispatchRedrawNotifications<MainWidget>(this, args);
		return;
	}
}

void MainWidget::handleRedraw(const QByteArray& name, const QVariantList& args) noexcept
{
	if (name == "cmdline_show") {
		handleCmdlineShow(args);
	}
	else if (name == "cmdline_pos") {
		handleCmdlinePos(args);
		return;
	}
	else if (name == "cmdline_special_char") {
		handleCmdlineSpecialChar(args);
	}
	else if (name == "cmdline_hide") {
		handleCmdlineHide();
	}
	else if (name == "cmdline_block_show") {
		handleCmdlineBlockShow(args);
	}
	else if (name == "cmdline_block_append") {
		handleCmdlineBlockAppend(args);
	}
	else if (name == "cmdline_block_hide") {
		handleCmdlineBlockHide();
	}
}

QSize MainWidget::getShellSizeForText(const QString& text) const noexcept
{
	const int border_padding{ width() - m_cmdTextBox->width() };
	const int maxWidth{ maximumWidth() - border_padding };

	const int cellSize{ m_cmdTextBox->cellSize().width() };
	const int columns{  maxWidth / cellSize };
	int rows{ text.size() / columns };
	if (text.size() % columns != 0) {
		rows++;
	}

	return { columns, rows};
};

void MainWidget::handleCmdlineShow(const QVariantList& args) noexcept
{
	if (args.size() < 6
		|| static_cast<QMetaType::Type>(args.at(0).type()) != QMetaType::QVariantList
		|| !args.at(1).canConvert<int>()
		|| !args.at(2).canConvert<QString>()
		|| !args.at(3).canConvert<QString>()
		|| !args.at(4).canConvert<int>()
		|| !args.at(4).canConvert<int>()) {
		qWarning() << "Unexpected arguments for cmdline_show:" << args;
	}

	const QVariantList content = args.at(0).toList();
	const int pos{ args.at(1).toInt() };
	const QString firstc{ args.at(2).toString() };
	const QString prompt{ args.at(3).toString() };
	const int indent{ args.at(4).toInt() };
	const int level{ args.at(5).toInt() };

	// Cursor style should be copied from the parent ShellWidget.
	ShellWidget* parentShellWidget{ qobject_cast<ShellWidget*>(parentWidget()) };
	if (parentShellWidget)
	{
		m_cmdTextBox->CopyCursorStyle(parentShellWidget->getCursor());
	}

	LineModel lineModel{ content, pos, firstc, prompt, indent, level };
	if (level > m_model.size()) {
		m_model.append(std::move(lineModel));
	}
	else {
		m_model[level - 1] = std::move(lineModel);
	}

	m_cmdTextBox->clearShell();

	const QString text{ m_model.last().getPromptText() };

	const QSize cmdSize{ getShellSizeForText(text) };
	m_cmdTextBox->resizeShell(cmdSize);

	for (int i=0;i<cmdSize.height();i++) {
		int startPos = i * cmdSize.width();
		QString row{ text.mid(startPos, cmdSize.width()) };
		m_cmdTextBox->put(row, i, 0);
	}

	setCursorPosition(pos);

	updateGeometry();
	show();
}

void MainWidget::handleCmdlinePos(const QVariantList& args) noexcept
{
	if (args.size() < 2
		|| !args.at(0).canConvert<int>()
		|| !args.at(1).canConvert<int>()) {
		qWarning() << "Unexpected arguments for cmdline_pos:" << args;
	}

	const int pos{ args.at(0).toInt() };
	//const int level{ args.at(1).toULongLong() };

	setCursorPosition(pos);
}

void MainWidget::handleCmdlineSpecialChar(const QVariantList& args) noexcept
{
	if (args.size() < 3
		|| !args.at(0).canConvert<QString>()
		|| !args.at(1).canConvert<bool>()
		|| !args.at(2).canConvert<int>()) {
		qWarning() << "Unexpected arguments for cmdline_special_char:" << args;
	}

	const QString c = args.at(0).toString();
	const bool shift = args.at(1).toBool();
	const int level = args.at(2).toInt();

	if (level > m_model.size()) {
		qDebug() << "Invalid value for 'level'" << level;
	}

	LineModel& line = m_model[level - 1];

	// FIXME Inaccurate flow/comments
	// Special characters are inserted in two steps
	//if (shift) {
	//	// Step One: insert a placeholder character.
	//	line.m_content.insert(line.m_position, c);
	//	qDebug() << "ONE:" << c;
	//}
	//else {
	//	// Step Two: replace the placeholder with the desired character.
	//	line.m_content.replace(line.m_position, 1, c);
	//	qDebug() << "TWO:" << c;
	//}
	qDebug() << "c:" << c;
	qDebug() << "shift:" << shift;
	qDebug() << "level:" << level;

	m_cmdTextBox->put(line.getPromptText(),0,0); // FIXME ???
}

void MainWidget::handleCmdlineBlockShow(const QVariantList& args) noexcept
{
	if (args.size() < 1
		|| static_cast<QMetaType::Type>(args.at(0).type()) != QMetaType::QVariantList) {
		qWarning() << "Unexpected arguments for cmdline_block_show:" << args;
	}

	const QVariantList lines = args.at(0).toList();

	qDebug() << "MainWidget::handleCmdlineBlockShow()";
	QString blockText;
	for (const auto& varLine: lines) {
		QVariantList line = varLine.toList();
		for (const auto& varSegment : line) {
			QVariantList segment = varSegment.toList();
			const QString segmentText = segment.at(1).toString();

			blockText += segmentText;
		}
	}
	m_cmdBlockText->setText(blockText);
	m_cmdBlockText->updateGeometry();
	m_cmdBlockText->show();
}

void MainWidget::handleCmdlineBlockAppend(const QVariantList& args) noexcept
{
	if (args.size() < 1
		|| static_cast<QMetaType::Type>(args.at(0).type()) != QMetaType::QVariantList) {
		qWarning() << "Unexpected arguments for cmdline_block_show:" << args;
	}

	const QVariantList lines = args.at(0).toList();

	for (const auto& varSegment : lines) {
		QVariantList segment = varSegment.toList();
		const QString segmentText = segment.at(1).toString();

		m_cmdBlockText->append(segmentText);
	}

	m_cmdBlockText->updateGeometry();
}

void MainWidget::handleCmdlineHide() noexcept
{
	if (!m_model.empty()) {
		m_model.pop_back();
	}

	hide();
}

void MainWidget::handleCmdlineBlockHide() noexcept
{
	m_cmdBlockText->hide();
}

static void WriteCommandlinePositionSetting(const QString& position) noexcept
{
	QSettings settings("nvim-qt", "nvim-qt");

	if (!settings.isWritable()) {
		return;
	}

	settings.setValue("Commandline/position", position);
}

void MainWidget::handleGuiCommandlinePosition(const QVariantList& args) noexcept
{
	if (args.size() < 2
		|| !args.at(1).canConvert<QByteArray>()) {
		qWarning() << "Unexpected arguments for GuiCommandlinePosition:" << args;
	}

	const QString position{ m_nvim.decode(args.at(1).toByteArray()) };

	m_position = PositionFromString(position);
	WriteCommandlinePositionSetting(position);
}

void MainWidget::updateGeometry() noexcept
{
	ShellWidget* parentShellWidget{ qobject_cast<ShellWidget*>(parentWidget()) };
	if (!parentShellWidget) {
		qFatal("ShellWidget parentWidget() must be defined!");
	}

	const int width{ parentShellWidget->width() * m_widthPercent / 100};
	setMaximumWidth(width);

	constexpr int maxHeightPercent{ 75 };
	const int maxHeight{ parentShellWidget->height() * maxHeightPercent / 100 };
	setMaximumHeight(maxHeight);

	const int height = qMin(sizeHint().height(), maxHeight);

	int anchorX{ (parentShellWidget->width() - width) / 2 };
	int anchorY{ (parentShellWidget->height() - height) / 2 };

	switch (m_position)
	{
		case Position::Center:
		{
			break;
		}

		case Position::Top:
			anchorY = 0;
			break;

		case Position::Bottom:
			anchorY = parentShellWidget->height() - height;
			break;
	}

	qDebug() << width << height;
	setGeometry(anchorX, anchorY, width, height);
	m_cmdTextBox->updateGeometry();
	QWidget::updateGeometry();
}

void MainWidget::setCursorPosition(int pos) noexcept
{
	int posWithIndent{ pos + m_model.last().getIndent() };
	const int cols{ m_cmdTextBox->shellGridSize().width() };
	m_cmdTextBox->setNeovimCursor((posWithIndent + 1) / cols, (posWithIndent + 1) % cols);
}

QSize MainWidget::sizeHint() const noexcept
{
	// FIXME Duplicated code?
	ShellWidget* parentShellWidget{ qobject_cast<ShellWidget*>(parentWidget()) };
	if (!parentShellWidget) {
		qFatal("ShellWidget parentWidget() must be defined!");
	}

	QSize sizeHint{ QFrame::sizeHint() };
	sizeHint.setWidth(parentShellWidget->width() * m_widthPercent / 100);
	return sizeHint;
}

void MainWidget::updatePalette() noexcept
{
	if (!parentWidget()) {
		qDebug() << "No parentWidget, cannot set palette";
		return;
	}

	ShellWidget* parentShellWidget{ static_cast<ShellWidget*>(parentWidget()) };

	m_palette.setColor(QPalette::Base, parentShellWidget->background());
	m_palette.setColor(QPalette::Text, parentShellWidget->foreground());
	m_palette.setColor(QPalette::Background, parentShellWidget->foreground());

	//m_cmdBlockText->setPalette(m_palette);

	m_cmdTextBox->setBackground(parentShellWidget->background());
	m_cmdTextBox->setForeground(parentShellWidget->foreground());
}

void MainWidget::setFont(const QFont &font) noexcept
{
	m_cmdBlockText->setFont(font);
	m_cmdTextBox->setShellFont(font, true);
}

} } // namespace NeovimQt::Commandline
