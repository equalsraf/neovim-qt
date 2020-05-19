#pragma once

#include <QPaintEvent>
#include <QColor>
#include <QObject>
#include <QTimer>

#include "highlight.h"

class Cursor : public QObject
{
	Q_OBJECT

signals:
	void CursorChanged();

public:

	enum class Shape : uint8_t
	{
		Block,
		Horizontal,
		Vertical,
	};

	Cursor() noexcept;

	void ResetTimer() noexcept;

	void SetTimer(uint64_t blinkWaitTime, uint64_t blinkOnTime, uint64_t blinkOffTime) noexcept;

	void SetColor(const HighlightAttribute& highlight) noexcept
	{
		m_background = highlight.GetBackgroundColor();
		m_foreground = highlight.GetForegroundColor();
	}

	void SetStyle(Shape cursorShape, uint8_t cellPercentage) noexcept
	{
		m_shape = cursorShape;
		m_percentage = cellPercentage;
	}

	void SetIsStyleEnabled(bool isStyleEnabled) noexcept
	{
		m_styleEnabled = isStyleEnabled;
	}

	void SetIsBusy(bool isBusy) noexcept
	{
		m_isBusy = isBusy;
	}

	bool IsStyleEnabled() const noexcept
	{
		return m_styleEnabled && !m_isBusy;
	}

	bool IsVisible() const noexcept
	{
		return !m_isBusy && m_blinkState != BlinkState::Off;
	}

	QColor GetBackgroundColor() const noexcept
	{
		return m_background;
	}

	QColor GetForegroundColor() const noexcept
	{
		return m_foreground;
	}

	uint8_t GetPercentage() const noexcept
	{
		return m_percentage;
	}

	Shape GetShape() const noexcept
	{
		return m_shape;
	}

private:
	void TimerInterrupt() noexcept;

	void StartTimer() noexcept;

	void StopTimer() noexcept;

	enum class BlinkState : uint8_t
	{
		Disabled,
		On,
		Off,
		Wait,
	};

	QColor m_background;
	QColor m_foreground;

	Shape m_shape{ Shape::Block };
	QVariantList m_modeInfo;
	QTimer m_timer;
	BlinkState m_blinkState{ BlinkState::Disabled };

	bool m_styleEnabled{ false };
	bool m_isBusy{ false };

	uint8_t m_percentage{ 100 };

	uint64_t m_blinkWaitTime{ 0 };
	uint64_t m_blinkOnTime{ 0 };
	uint64_t m_blinkOffTime{ 0 };
};
