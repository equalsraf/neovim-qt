#pragma once

#include <QMainWindow>
#include <QPalette>
#include <QSplitter>
#include <QStackedWidget>
#include <QTabBar>

#include "contextmenu.h"
#include "errorwidget.h"
#include "neovimconnector.h"
#include "scrollbar.h"
#include "shell.h"
#include "tabline.h"
#include "treeview.h"

namespace NeovimQt {

class MainWindow: public QMainWindow
{
	Q_OBJECT
public:
	MainWindow(NeovimConnector* c, QWidget* parent = nullptr) noexcept;

	bool isNeovimAttached() const noexcept { return m_shell && m_shell->isNeovimAttached(); }

	Shell* shell();

	void restoreWindowGeometry();

	bool active() const noexcept { return m_isActive; }

signals:
	void neovimAttachmentChanged(bool);
	void closing(int);
	void activeChanged(NeovimQt::MainWindow& window);

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
	void neovimFrameless(bool);
	void neovimGuiCloseRequest(int);
	void neovimExited(int status);
	void neovimError(NeovimConnector::NeovimError);
	void reconnectNeovim();
	void handleNeovimAttachment(bool);
	void neovimIsUnsupported();
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
	QStackedWidget m_stack;

	bool m_neovim_requested_close{ false };
	ContextMenu* m_contextMenu{ nullptr };
	ScrollBar* m_scrollbar{ nullptr };
	Tabline m_tabline;
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

} // namespace NeovimQt
