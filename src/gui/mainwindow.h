#ifndef NEOVIM_QT_MAINWINDOW
#define NEOVIM_QT_MAINWINDOW

#include <QMainWindow>
#include <QPalette>
#include <QSplitter>
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

	MainWindow(NeovimConnector *, ShellOptions opts, QWidget *parent=0);
	bool neovimAttached() const;
	Shell* shell();
	void restoreWindowGeometry();
public slots:
	void delayedShow(DelayedShow type=DelayedShow::Normal);
signals:
	void neovimAttached(bool);
protected:
	virtual void closeEvent(QCloseEvent *ev) Q_DECL_OVERRIDE;
	virtual void changeEvent(QEvent *ev) Q_DECL_OVERRIDE;
private slots:
	void neovimSetTitle(const QString &title);
	void neovimWidgetResized();
	void neovimMaximized(bool);
	void neovimSuspend();
	void neovimFullScreen(bool);
	void neovimGuiCloseRequest();
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
	ShellOptions m_shell_options;

	bool m_neovim_requested_close{ false };
	QMenu* m_contextMenu{ nullptr };
	QAction* m_actCut{ nullptr };
	QAction* m_actCopy{ nullptr };
	QAction* m_actPaste{ nullptr };
	QAction* m_actSelectAll{ nullptr };
	ScrollBar* m_scrollbar{ nullptr };

	// GuiAdaptive Color/Font/Style
	bool m_isAdaptiveColorEnabled{ false };
	bool m_isAdaptiveFontEnabled{ false };
	QFont m_defaultFont;
	QPalette m_defaultPalette;

	void updateAdaptiveColor() noexcept;
	void updateAdaptiveFont() noexcept;
};

} // Namespace

#endif
