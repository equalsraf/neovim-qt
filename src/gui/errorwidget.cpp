#include "errorwidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>

namespace NeovimQt {

ErrorWidget::ErrorWidget(QWidget *parent)
:QWidget(parent), m_errorLabel(0), m_closeButton(0)
{
	m_errorLabel = new QLabel();
	m_closeButton = new QPushButton(tr("Retry"));

	m_image = new QSvgWidget(":/neovim.svg");
	m_image->setFixedSize(64, 64);

	connect(m_closeButton, &QPushButton::clicked,
			this, &ErrorWidget::reconnectNeovim);

	QHBoxLayout *inner_layout = new QHBoxLayout();
	inner_layout->addStretch();
	inner_layout->addWidget(m_image);
	inner_layout->addWidget(m_errorLabel);
	inner_layout->addWidget(m_closeButton);
	inner_layout->addStretch();
	QVBoxLayout *outer_layout = new QVBoxLayout();
	outer_layout->addStretch();
	outer_layout->addLayout(inner_layout);
	outer_layout->addStretch();
	setLayout(outer_layout);
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
