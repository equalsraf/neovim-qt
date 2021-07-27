#ifndef NEOVIM_QT_MAINWINDOW
#define NEOVIM_QT_MAINWINDOW

#include <QMainWindow>
#include <QPalette>
#include <QSplitter>
#include <QCloseEvent>
#include <QStackedWidget>
#include <QTabBar>

#include "errorwidget.h"
#include "neovimconnector.h"
#include "scrollbar.h"
#include "shell.h"
#include "treeview.h"

namespace NeovimQt {

class MainWindow: public QMainWindow
{
	Q_OBJECT
public:
	enum DelayedShow {
		Disabled,
		Normal,
		Maximized,
		FullScreen,
	};

	MainWindow(NeovimConnector *, QWidget *parent=0);
	bool neovimAttached() const;
	Shell* shell();
	void restoreWindowGeometry();

	bool active() const noexcept { return m_isActive; }

public slots:
	void delayedShow(DelayedShow type=DelayedShow::Normal);
signals:
	void neovimAttached(bool);
	void closing(int);
	void activeChanged(MainWindow& window);

protected:
	virtual void closeEvent(QCloseEvent *ev) Q_DECL_OVERRIDE;
	virtual void changeEvent(QEvent *ev) Q_DECL_OVERRIDE;
private slots:
	void neovimSetTitle(const QString &title);
	void neovimWidgetResized();
	void neovimMaximized(bool);
	void neovimForeground();
	void neovimSuspend();
	void neovimFullScreen(bool);
	void neovimGuiCloseRequest(int);
	void neovimExited(int status);
	void neovimError(NeovimConnector::NeovimError);
	void reconnectNeovim();
	void showIfDelayed();
	void neovimAttachmentChanged(bool);
	void neovimIsUnsupported();
	void neovimShowtablineSet(int);
	void neovimTablineUpdate(int64_t curtab, QList<Tab> tabs);
	void neovimShowContextMenu();
	void neovimSendCut();
	void neovimSendCopy();
	void neovimSendPaste();
	void neovimSendSelectAll();
	void extTablineSet(bool);
	void changeTab(int index);
	void saveWindowGeometry();

	// GuiAdaptive Color/Font/Style Slots
	void setGuiAdaptiveColorEnabled(bool isEnabled);
	void setGuiAdaptiveFontEnabled(bool isEnabled);
	void setGuiAdaptiveStyle(const QString& style);
	void showGuiAdaptiveStyleList();

private:
	void init(NeovimConnector *);

	NeovimConnector* m_nvim{ nullptr };
	ErrorWidget* m_errorWidget{ nullptr };
	QSplitter* m_window{ nullptr };
	TreeView* m_tree{ nullptr };
	Shell* m_shell{ nullptr };
	DelayedShow m_delayedShow{ DelayedShow::Disabled };
	QStackedWidget m_stack;
	QTabBar* m_tabline{ nullptr };
	QToolBar* m_tabline_bar{ nullptr };

	bool m_neovim_requested_close{ false };
	QMenu* m_contextMenu{ nullptr };
	QAction* m_actCut{ nullptr };
	QAction* m_actCopy{ nullptr };
	QAction* m_actPaste{ nullptr };
	QAction* m_actSelectAll{ nullptr };
	ScrollBar* m_scrollbar{ nullptr };
	int m_exitStatus{ 0 };

	// GuiAdaptive Color/Font/Style
	bool m_isAdaptiveColorEnabled{ false };
	bool m_isAdaptiveFontEnabled{ false };
	QFont m_defaultFont;
	QPalette m_defaultPalette;

	bool m_isActive{ false };

	void updateAdaptiveColor() noexcept;
	void updateAdaptiveFont() noexcept;
};

} // Namespace

#endif
