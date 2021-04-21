#ifndef NEOVIM_QT_ERRORWIDGET
#define NEOVIM_QT_ERRORWIDGET

#include <QLabel>
#include <QPushButton>
#include <QSvgWidget>
#include <QWidget>

namespace NeovimQt {

class ErrorWidget: public QWidget {
	Q_OBJECT
public:
	ErrorWidget(QWidget *parent = nullptr);
public slots:
	void setText(const QString& text);
	void showReconnect(bool);
signals:
	void reconnectNeovim();

private:
	QLabel *m_errorLabel;
	QSvgWidget *m_image;
	QPushButton *m_closeButton;
};

} // Namespace

#endif
