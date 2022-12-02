#include "scrollbar.h"

#include <QSettings>

#include "shell.h"

namespace NeovimQt {

ScrollBar::ScrollBar(NeovimConnector* nvim, QWidget* parent) noexcept
	: QScrollBar{ parent }
	, m_nvim{ nvim }
{
	if (!m_nvim) {
		qFatal("Fatal Error: ScrollBar must have a valid NeovimConnector!");
	}

	connect(m_nvim, &NeovimConnector::ready, this, &ScrollBar::neovimConnectorReady);
	connect(this, &QScrollBar::valueChanged, this, &ScrollBar::handleValueChanged);

	QSettings settings;
	setVisible(settings.value("Gui/ScrollBar", false).toBool());

	setMinimum(1);
}

void ScrollBar::neovimConnectorReady() noexcept
{
	connect(m_nvim->api0(), &NeovimApi0::neovimNotification,
		this, &ScrollBar::handleNeovimNotification);

	m_nvim->api0()->vim_subscribe("Gui");
}

bool ScrollBar::IsWinViewportSupported() const noexcept
{
	static const bool isSupported{ m_nvim->apiLevel() >= 7 };
	return isSupported;
}

void ScrollBar::handleNeovimNotification(const QByteArray& name, const QVariantList& args) noexcept
{
	if (args.size() <= 0) {
		return;
	}

	if (name == "Gui") {
		const QString guiEvName{ args.at(0).toByteArray() };

		if (guiEvName == "CursorMoved") {
			handleCursorMoved(args);
			return;
		}

		if (guiEvName == "SetScrollBarVisible") {
			handleSetScrollBarVisible(args);
			return;
		}
	}

	if (name == "redraw") {
		Shell::DispatchRedrawNotifications<ScrollBar>(this, args);
		return;
	}
}

void ScrollBar::handleRedraw(const QByteArray& name, const QVariantList& args) noexcept
{
	if (name == "grid_scroll") {
		handleGridScroll(args);
		return;
	}

	if (name == "scroll") {
		handleScroll(args);
		return;
	}

	if (name == "win_viewport") {
		handleWinViewport(args);
		return;
	}
}

void ScrollBar::setIsVisible(bool isVisible)
{
	setVisible(isVisible);
}

void ScrollBar::setAbsolutePosition(uint64_t minLine, uint64_t bufferSize, uint64_t windowHeight)
{
	m_lineScrollLockout -= m_lastKnownPosition - minLine;

	m_lastKnownPosition = minLine;
	m_lastKnownBufferSize = bufferSize;
	m_lastKnownWindowHeight = windowHeight;

	// Block valueChanged signals for scroll events triggered by Neovim.
	{
		QSignalBlocker blockValueChanged{ this };

		setMaximum(bufferSize);
		setPageStep(windowHeight);
		setSliderPosition(minLine);
	}
}

void ScrollBar::setRelativePosition(int64_t rowCount)
{
	m_lineScrollLockout -= rowCount;

	// Block valueChanged signals for scroll events triggered by Neovim.
	{
		QSignalBlocker blockValueChanged{ this };

		setSliderPosition(sliderPosition() + rowCount);
	}
}

void ScrollBar::handleScroll(const QVariantList& args) noexcept
{
	// With win_viewport supported, scroll events aren't needed.
	if (IsWinViewportSupported()) {
		return;
	}

	if (args.size() < 1 || !args.at(0).canConvert<int64_t>()) {
		return;
	}

	const int64_t count{ args.at(0).toLongLong() };

	setRelativePosition(count);
}

void ScrollBar::handleGridScroll(const QVariantList& args) noexcept
{
	// With win_viewport supported, scroll events aren't needed.
	if (IsWinViewportSupported()) {
		return;
	}

	if (args.size() < 6 || !args.at(5).canConvert<uint64_t>()) {
		return;
	}

	const int64_t rows{ args.at(5).toLongLong() };

	setRelativePosition(rows);
}

void ScrollBar::handleWinViewport(const QVariantList& args) noexcept
{
	if (args.size() < 6
		|| !args.at(0).canConvert<uint64_t>()
		|| !args.at(1).canConvert<uint64_t>()
		|| !args.at(2).canConvert<uint64_t>()
		|| !args.at(3).canConvert<uint64_t>()
		|| !args.at(4).canConvert<uint64_t>()
		|| !args.at(5).canConvert<uint64_t>()) {
		qWarning() << "Unexpected arguments for win_viewport:" << args;
		return;
	}

	//const uint64_t grid { args.at(0).toULongLong() };
	//const uint64_t win { args.at(1).toULongLong() };
	//const uint64_t botline { args.at(3).toULongLong() };
	//const uint64_t curline { args.at(4).toULongLong() };
	//const uint64_t curcol { args.at(5).toULongLong() };

	const uint64_t topline { args.at(2).toULongLong() };

	setAbsolutePosition(topline, m_lastKnownBufferSize, m_lastKnownWindowHeight);
}


void ScrollBar::handleCursorMoved(const QVariantList& args) noexcept
{
	if (args.size() < 4
		|| !args.at(1).canConvert<uint64_t>()
		|| !args.at(2).canConvert<uint64_t>()
		|| !args.at(3).canConvert<uint64_t>()) {
		qWarning() << "Unexpected arguments for CursorMoved:" << args;
		return;
	}

	const uint64_t minLineVisible{ args.at(1).toULongLong() };
	const uint64_t bufferSize{ args.at(2).toULongLong() };
	const uint64_t windowHeight{ args.at(3).toULongLong() };

	setAbsolutePosition(minLineVisible, bufferSize, windowHeight);
}

void ScrollBar::handleSetScrollBarVisible(const QVariantList& args) noexcept
{
	if (args.size() < 2
		|| !args.at(1).canConvert<bool>()) {
		qWarning() << "Unexpected arguments for SetScrollBarVisible:" << args;
		return;
	}

	const bool isVisible{ args.at(1).toBool() };
	setVisible(isVisible);

	QSettings settings;
	settings.setValue("Gui/ScrollBar", isVisible);
}

void ScrollBar::handleValueChanged(int value)
{
	const int delta{ m_lastKnownPosition - value};
	if (delta == 0) {
		return;
	}

	m_lineScrollLockout += delta;

	// Scroll Up: normal! {Control + Y}
	// MSVC cannot parse the un-escaped sequence below `^Y`.
	if (delta > 0) {
		m_nvim->api0()->vim_command(
			QStringLiteral("normal! %1\031").arg(delta).toLatin1());
	}

	// Scroll Down normal! {Control + E}
	// MSVC cannot parse the un-escaped sequence below `^E`.
	if (delta < 0) {
		m_nvim->api0()->vim_command(
			QStringLiteral("normal! %1\005").arg(delta).toLatin1());
	}
}

} // namespace NeovimQt
