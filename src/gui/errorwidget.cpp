#include "errorwidget.h"

#include <QHBoxLayout>

namespace NeovimQt {

ErrorWidget::ErrorWidget(QWidget *parent)
:QWidget(parent), m_errorLabel(0), m_closeButton(0)
{
	m_errorLabel = new QLabel();
	m_closeButton = new QPushButton(tr("Retry"));

	connect(m_closeButton, &QPushButton::clicked,
			this, &ErrorWidget::reconnectNeovim);

	QHBoxLayout *layout = new QHBoxLayout();
	layout->addWidget(m_errorLabel);
	layout->addStretch();
	layout->addWidget(m_closeButton);
	setLayout(layout);
}

void ErrorWidget::setText(const QString& text)
{
	m_errorLabel->setText(text);
}

void ErrorWidget::showReconnect(bool on)
{
	m_closeButton->setVisible(on);
}

} //Namespace
