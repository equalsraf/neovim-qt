#include "cursor.h"
#include "msgpackrequest.h"

Cursor::Cursor() noexcept
{
	connect(&m_timer, &QTimer::timeout, this, &Cursor::TimerInterrupt);
}

void Cursor::TimerInterrupt() noexcept
{
	switch (m_blinkState) {
		case BlinkState::Wait:
		{
			m_blinkState = BlinkState::Off;
			m_timer.setInterval(m_blinkOffTime);
		}
		break;

		case BlinkState::Off:
		{
			m_blinkState = BlinkState::On;
			m_timer.setInterval(m_blinkOnTime);
		}
		break;

		case BlinkState::On:
		{
			m_blinkState = BlinkState::Off;
			m_timer.setInterval(m_blinkOffTime);
		}
		break;

		case BlinkState::Disabled:
		{
		}
		break;
	}

	emit CursorChanged();
}

void Cursor::StopTimer() noexcept
{
	m_blinkState = BlinkState::Disabled;
	m_timer.stop();
}

void Cursor::StartTimer() noexcept
{
	m_blinkState = BlinkState::Wait;

	if (m_blinkOnTime > 0 && m_blinkOffTime > 0)
	{
		m_timer.start();
	}
}

void Cursor::ResetTimer() noexcept
{
	m_timer.setInterval(m_blinkWaitTime);
	m_blinkState = BlinkState::Wait;
}

void Cursor::SetTimer(uint64_t blinkWaitTime, uint64_t blinkOnTime, uint64_t blinkOffTime) noexcept
{
	StopTimer();

	m_blinkWaitTime = blinkWaitTime;
	m_blinkOnTime = blinkOnTime;
	m_blinkOffTime = blinkOffTime;

	StartTimer();
}
