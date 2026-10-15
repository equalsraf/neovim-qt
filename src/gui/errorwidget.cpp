#include "errorwidget.h"

#include <QHBoxLayout>
#include <QSvgWidget>
#include <QVBoxLayout>
#include <QDebug>

static constexpr std::array<uint32_t, 5> cs_retryTimeoutSeconds{ 5, 10, 30 };

namespace NeovimQt {

static QHBoxLayout* getHorizCenterLayout(QWidget* widget) noexcept
{
	QHBoxLayout* inner_layout = new QHBoxLayout();
	inner_layout->addStretch();
	inner_layout->addWidget(widget);
	inner_layout->addStretch();
	return inner_layout;
}

ErrorWidget::ErrorWidget(QWidget* parent) noexcept
	: QWidget(parent)
{
	m_reconnectTimer = new QTimer(this);
	connect(m_reconnectTimer, &QTimer::timeout, this, &ErrorWidget::reconnectTimeout);
	m_reconnectTimer->start(1000);

	m_errorMessage = new QLabel();

	m_retryMessage = new QLabel();
	m_retryMessage->setText(getRetryMessageText());

	m_retryButton = new QPushButton(tr("Retry Now"));
	connect(m_retryButton, &QPushButton::clicked, this, &ErrorWidget::reconnectNeovim);

	QSvgWidget* neovimIcon = new QSvgWidget(":/neovim.svg");
	neovimIcon->setFixedSize(64, 64);

	QVBoxLayout* outer_layout = new QVBoxLayout();
	outer_layout->addStretch();
	outer_layout->addLayout(getHorizCenterLayout(neovimIcon));
	outer_layout->addLayout(getHorizCenterLayout(m_errorMessage));
	outer_layout->addLayout(getHorizCenterLayout(m_retryMessage));
	outer_layout->addLayout(getHorizCenterLayout(m_retryButton));
	outer_layout->addStretch();
	setLayout(outer_layout);
}

void ErrorWidget::setText(const QString& text) noexcept
{
	m_errorMessage->setText(text);
	qDebug() << "Error:" << text;
}

void ErrorWidget::showReconnect(bool isVisible) noexcept
{
	m_retryButton->setVisible(isVisible);
}

QString ErrorWidget::getRetryMessageText() noexcept
{
	const uint32_t secondsLeft{ cs_retryTimeoutSeconds[m_retryIndex] - m_timerCount };
	return  tr("Reconnecting in ") + QString::number(secondsLeft) + tr(" seconds...");
}

void ErrorWidget::reconnectTimeout() noexcept
{
	++m_timerCount;
	m_retryMessage->setText(getRetryMessageText());

	if (m_timerCount >= cs_retryTimeoutSeconds[m_retryIndex])
	{

		if (m_retryIndex < cs_retryTimeoutSeconds.size()) {
			++m_retryIndex;
		}

		reconnectNeovim();

		m_timerCount = 0;
	}
}

} // Namespace NeovimQt
