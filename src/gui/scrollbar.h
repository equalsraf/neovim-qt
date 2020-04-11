#pragma once

#include <QScrollBar>

#include "neovimconnector.h"

namespace NeovimQt {

class ScrollBar : public QScrollBar
{
	Q_OBJECT

public:
	ScrollBar(NeovimConnector* nvim, QWidget* parent = nullptr) noexcept;

	/// Checks the nvim client for support of win_viewport events.
	bool IsWinViewportSupported() const noexcept;

	void handleNeovimNotification(const QByteArray& name, const QVariantList& args) noexcept;
	void handleRedraw(const QByteArray& name, const QVariantList& opargs) noexcept;

public slots:
	void setIsVisible(bool isVisible);
	void setAbsolutePosition(uint64_t minLine, uint64_t bufferSize, uint64_t windowHeight);
	void setRelativePosition(int64_t rowCount);

private:
	void neovimConnectorReady() noexcept;

	// Gui Events
	void handleCursorMoved(const QVariantList& opargs) noexcept;
	void handleSetScrollBarVisible(const QVariantList& opargs) noexcept;

	// Redraw Events
	void handleGridScroll(const QVariantList& opargs) noexcept;
	void handleScroll(const QVariantList& opargs) noexcept;
	void handleWinViewport(const QVariantList& opargs) noexcept;

	/// Attached to valueChanged signal. Sends scroll commands to Neovim API.
	void handleValueChanged(int value);

	NeovimConnector *m_nvim{ nullptr };

	/// Locks out GUI triggered scrolling for a number of lines. Prevent double registration.
	int m_lineScrollLockout{ 0 };

	/// Last known cursor position. Cursor position is in absolute units, and scroll
	/// commands are sent with relative units. Necessary for conversion.
	int m_lastKnownPosition{ 0 };

	/// Workaround for win_viewport. Saves buffer size from last cursor movement.
	int m_lastKnownBufferSize{ 0 };

	/// Workaround for win_viewport. Saves window height from last cursor movement.
	int m_lastKnownWindowHeight{ 0 };
};

} // namespace NeovimQt
