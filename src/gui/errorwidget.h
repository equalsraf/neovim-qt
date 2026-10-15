#pragma once

#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QWidget>

namespace NeovimQt {

class ErrorWidget: public QWidget {
	Q_OBJECT
public:
	ErrorWidget(QWidget* parent = nullptr) noexcept;

public slots:
	void setText(const QString& text) noexcept;
	void showReconnect(bool isVisible) noexcept;

signals:
	void reconnectNeovim();

private:
	void reconnectTimeout() noexcept;
	QString getRetryMessageText() noexcept;

	QLabel* m_errorMessage{ nullptr };
	QLabel* m_retryMessage{ nullptr };
	QPushButton* m_retryButton{ nullptr };
	QTimer* m_reconnectTimer{ nullptr };

	uint32_t m_retryIndex{ 0 };
	uint32_t m_timerCount{ 0 };
};

} // Namespace NeovimQt
