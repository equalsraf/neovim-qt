#include "mainwidget.h"

#include <QCoreApplication>
#include <QDebug>
#include <QSettings>

#include "../shell.h" // FIXME Ugly

namespace NeovimQt { namespace Commandline {

MainWidget::MainWidget(NeovimConnector* nvim, ShellWidget* parent) noexcept
	: m_nvim{ nvim }
{
	if (!m_nvim) {
		qFatal("Fatal Error: ScrollBar must have a valid NeovimConnector!");
	}

	connect(m_nvim, &NeovimConnector::ready, this, &MainWidget::neovimConnectorReady);

	setParent(parent);
	setVisible(false);

	m_cmdTextBoxFrame = new QFrame();
	m_cmdTextBoxFrame->setFrameShape(QFrame::StyledPanel);
	m_cmdTextBoxFrame->setFrameShadow(QFrame::Raised);
	m_cmdTextBoxFrame->setAutoFillBackground(true);
	QVBoxLayout* frameLayout = new QVBoxLayout(m_cmdTextBoxFrame);

	m_cmdTextBox = new ShellWidget();
	// FIXME The ShellWidget must be given an initial size, prevents 1st init height issues
	m_cmdTextBox->resizeShell(1, 1);
	m_cmdTextBox->setSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
	frameLayout->addWidget(m_cmdTextBox);

	m_cmdBlockText = new BlockWidget();

	m_vLayout = new QVBoxLayout();
	m_vLayout->addWidget(m_cmdBlockText);
	m_vLayout->addWidget(m_cmdTextBoxFrame);
	setLayout(m_vLayout);

	QSettings settings("nvim-qt", "nvim-qt");

	QVariant maxWidth{ settings.value("Commandline/maximum_width", m_maxWidth) };
	QVariant minWidth{ settings.value("Commandline/minimum_width", m_minWidth) };
	QVariant position{ settings.value("Commandline/position", "center") };

	if (maxWidth.canConvert<double>())
	{
		m_maxWidth = maxWidth.toDouble();
	}

	if (minWidth.canConvert<double>())
	{
		m_minWidth = minWidth.toDouble();
	}

	if (position.canConvert<QByteArray>())
	{
		m_position = PositionFromString(position.toString());
	}
}

void MainWidget::neovimConnectorReady() noexcept
{
	connect(m_nvim->api0(), &NeovimApi0::neovimNotification,
		this, &MainWidget::handleNeovimNotification);

	m_nvim->api0()->vim_subscribe("Gui");
}

void MainWidget::handleNeovimNotification(const QByteArray& name, const QVariantList& args) noexcept
{
	if (args.size() <= 0) {
		return;
	}

	if (name == "Gui") {
		const QString guiEvName{ m_nvim->decode(args.at(0).toByteArray()) };

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
	const int pos = args.at(1).toInt();
	const QString firstc = args.at(2).toString();
	const QString prompt = args.at(3).toString();
	const int indent = args.at(4).toInt();
	const int level = args.at(5).toInt();

	// FIXME Copy Cursor Style from parent shell widget
	ShellWidget* parentShellWidget{ qobject_cast<ShellWidget*>(parentWidget()) };
	if (parentShellWidget)
	{
		m_cmdTextBox->CopyCursorStyle(parentShellWidget->getCursor());
		// FIXME Cursor hiding logic????
		//parentWidget->cur
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

	// FIXME Is there a way around this ugly hack?
	// sizeHint depends on rendering info on overflow to the next line.
	// We're doing: compute -> update -> compute again with know good values
	updateGeometry();
	update();
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

	const int pos = args.at(0).toULongLong();
	//const int level = args.at(1).toULongLong();

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

	// FIXME PositionFromString?
	const QString position{ m_nvim->decode(args.at(1).toByteArray()).toLower() };

	if (position == "top") {
		m_position = Position::Top;
		WriteCommandlinePositionSetting(position);
	}
	else if (position == "center") {
		m_position = Position::Center;
		WriteCommandlinePositionSetting(position);
	}
	else if (position == "bottom") {
		m_position = Position::Bottom;
		WriteCommandlinePositionSetting(position);
	}
}

static void WriteCommandlineModeSetting(const QString& mode) noexcept
{
	QSettings settings("nvim-qt", "nvim-qt");

	if (!settings.isWritable()) {
		return;
	}

	settings.setValue("Commandline/display_mode", mode);
}

void MainWidget::handleGuiCommandlineMode(const QVariantList& args) noexcept
{
	if (args.size() < 2
		|| !args.at(1).canConvert<QByteArray>()) {
		qWarning() << "Unexpected arguments for GuiCommandlineMode:" << args;
	}

	const Mode mode{ ModeFromString(m_nvim->decode(args.at(1).toByteArray())) };

	m_displayMode = mode;

	switch (mode)
	{
		case Mode::Dynamic:
		{
			WriteCommandlineModeSetting(QStringLiteral("dynamic"));
			return;
		}

		case Mode::Fixed:
		{
			WriteCommandlineModeSetting(QStringLiteral("fixed"));
			return;
		}
	}
}

void MainWidget::updateGeometry() noexcept
{
	ShellWidget* parentShellWidget{ qobject_cast<ShellWidget*>(parentWidget()) };
	if (!parentShellWidget) {
		qDebug() << "No parent ShellWidget, cannot update size/position!";
		return;
	}

	const int maxWidth = parentShellWidget->width() * m_maxWidth;
	const int maxHeight = parentShellWidget->height() * m_maxWidth;
	setMaximumWidth(maxWidth);
	setMaximumHeight(maxHeight);

	const QSize sizeHintThis = sizeHint();
	const int width = qMin(sizeHintThis.width(), maxWidth);
	const int height = qMin(sizeHintThis.height(), maxHeight);

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

int MainWidget::getMaxPromptLength() const noexcept
{
	int maxLineLength = 0;
	for (const auto& line : m_model) {
		maxLineLength = qMax(line.getPromptText().size(), maxLineLength);
	}

	return qMax(m_cmdBlockText->GetMaxLineLength(), maxLineLength);
}

QSize MainWidget::sizeHint() const noexcept
{
	// FIXME FIXME This causes issues on +1 row transition
	QSize sizeHint{ QFrame::sizeHint() };

	QFontMetrics fm{ m_cmdTextBox->fontMetrics() };

	const int widthHint = fm.averageCharWidth() * (getMaxPromptLength() + 2) +
		layout()->contentsMargins().left() + layout()->contentsMargins().right();

	// FIXME unsafe cast
	sizeHint.setWidth(qMax(widthHint, (int)m_minWidth));
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
