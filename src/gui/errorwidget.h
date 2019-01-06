#ifndef NEOVIM_QT_ERRORWIDGET
#define NEOVIM_QT_ERRORWIDGET

#include <QWidget>
#include <QPushButton>
#include <QLabel>

namespace NeovimQt {

class ErrorWidget: public QWidget {
	Q_OBJECT
public:
	ErrorWidget(QWidget *parent=0);
public slots:
	void setText(const QString& text);
	void showReconnect(bool);
signals:
	void reconnectNeovim();

private:
	QLabel *m_errorLabel;
	QLabel *m_image;
	QPushButton *m_closeButton;
};

} // Namespace

#endif
