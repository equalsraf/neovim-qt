#ifndef NEOVIM_QT_SHELL
#define NEOVIM_QT_SHELL

#include <QWidget>
#include <QVariantList>
#include <QFont>
#include <QBackingStore>
#include <QLabel>
#include <QTimer>
#include <QUrl>
#include <QList>
#include <QMap>
#include <QMenu>

#include "neovimconnector.h"
#include "shellwidget/highlight.h"
#include "shellwidget/shellwidget.h"
#include "shellwidget/cursor.h"
#include "popupmenu.h"
#include "popupmenumodel.h"

namespace NeovimQt {

class Tab {
public:
	Tab(int64_t id, QString name) {
		this->tab = id;
		this->name = name;
	}
	/// The tab handle, a unique tab identifier
	int64_t tab;
	QString name;
};

class ShellOptions {
public:
	bool enable_ext_tabline{ true };
	bool enable_ext_popupmenu{ true };
	bool enable_ext_linegrid{ true };
	int nvim_show_tabline{ 1 };
};

class Shell: public ShellWidget
{
	Q_OBJECT
	Q_PROPERTY(bool neovimBusy READ neovimBusy() NOTIFY neovimBusy())
	Q_PROPERTY(bool neovimAttached READ neovimAttached() NOTIFY neovimAttached())
public:
	Shell(NeovimConnector *nvim, ShellOptions opts, QWidget *parent=0);
	~Shell();
	QSize sizeIncrement() const;
	virtual QVariant inputMethodQuery(Qt::InputMethodQuery) const Q_DECL_OVERRIDE;
	bool neovimBusy() const;
	bool neovimAttached() const;

	PopupMenu& getPopupMenu() noexcept
	{
		return m_pum;
	}

	/// Lookup highlight by name from hl_group_set
	HighlightAttribute GetHighlightGroup(const QString& name) const noexcept
	{
		const uint64_t hl_id{ m_highlightGroupNameMap.value(name) };
		return m_highlightMap.value(hl_id);
	}

	/// Check if highlight exists in hl_group_set
	bool IsHighlightGroup(const QString& name) const noexcept
	{
		const uint64_t hl_id{ m_highlightGroupNameMap.value(name) };
		return m_highlightMap.contains(hl_id);
	}

	/// Dispatches Neovim redraw notifications to T::handleRedraw
	template <class T>
	static void DispatchRedrawNotifications(
		T* pThis, const QVariantList& args) noexcept;

signals:
	void neovimTitleChanged(const QString &title);
	void neovimBusy(bool);
	void neovimResized(int rows, int cols);
	void neovimAttached(bool);
	void neovimMaximized(bool);
	void neovimSuspend();
	void neovimFullScreen(bool);
	void neovimGuiCloseRequest();
	/// This signal is emmited if the running neovim version is unsupported by the GUI
	void neovimIsUnsupported();
	void neovimExtTablineSet(bool);
	void neovimExtPopupmenuSet(bool);
	/// The tabline needs updating. curtab is the handle of the current tab (not its index)
	/// as seen in Tab::tab.
	void neovimTablineUpdate(int64_t curtab, QList<Tab> tabs);
	void neovimShowtablineSet(int);
	void neovimShowContextMenu();
	void fontChanged();
	void colorsChanged();

	// GuiAdaptive Color/Font Signals
	void setGuiAdaptiveColorEnabled(bool isEnabled);
	void setGuiAdaptiveFontEnabled(bool isEnabled);
	void setGuiAdaptiveStyle(const QString& styleName);
	void showGuiAdaptiveStyleList();

public slots:
	void handleNeovimNotification(const QByteArray &name, const QVariantList& args);
	void resizeNeovim(const QSize&);
	void resizeNeovim(int n_cols, int n_rows);
	bool setGuiFont(const QString& fdesc, bool force, bool updateOption);
	bool setGuiFontWide(const QString& fdesc) noexcept;
	void updateGuiWindowState(Qt::WindowStates state);
	void openFiles(const QList<QUrl> url);

protected slots:
	void neovimError(NeovimConnector::NeovimError);
	void neovimExited(int);
	void neovimResizeFinished();
	void mouseClickReset();
	void mouseClickIncrement(Qt::MouseButton bt);
	void init();
	void fontError(const QString& msg);
	void updateWindowId();
	void updateClientInfo();
	void handleGinitError(quint32 msgid, quint64 fun, const QVariant& err);
	void handleShimError(quint32 msgid, quint64 fun, const QVariant& err);
	void handleShimLoad(quint32 msgid, quint64 fun, const QVariant& resp);
	void handleGetBackgroundOption(quint32 msgid, quint64 fun, const QVariant& val);

protected:
	void tooltip(const QString& text);
	virtual void inputMethodEvent(QInputMethodEvent *event) Q_DECL_OVERRIDE;
	virtual void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
	virtual bool event(QEvent *event) Q_DECL_OVERRIDE;

	virtual void resizeEvent(QResizeEvent *ev) Q_DECL_OVERRIDE;
	virtual void keyPressEvent(QKeyEvent *ev) Q_DECL_OVERRIDE;
        void paintLogo(QPainter&);
	virtual void paintEvent(QPaintEvent *ev) Q_DECL_OVERRIDE;
	virtual void changeEvent(QEvent *ev) Q_DECL_OVERRIDE;
	virtual void closeEvent(QCloseEvent *ev) Q_DECL_OVERRIDE;
	virtual void focusInEvent(QFocusEvent *ev) Q_DECL_OVERRIDE;
	virtual void focusOutEvent(QFocusEvent *ev) Q_DECL_OVERRIDE;
	virtual void dragEnterEvent(QDragEnterEvent *) Q_DECL_OVERRIDE;
	virtual void dropEvent(QDropEvent *) Q_DECL_OVERRIDE;

	virtual void handleResize(uint64_t cols, uint64_t rows);
	virtual void handlePut(const QVariantList& args);
	virtual void handleHighlightSet(const QVariantMap& args);
	virtual void handleRedraw(const QByteArray& name, const QVariantList& args);
	virtual void handleScroll(const QVariantList& args);
	virtual void handleModeChange(const QVariantList& opargs);
	virtual void handleModeInfoSet(const QVariantList& opargs);
	virtual void handleSetTitle(const QVariantList& opargs);
	virtual void handleSetScrollRegion(const QVariantList& opargs);
	virtual void handleBusy(bool);
	virtual void handleSetOption(const QString& name, const QVariant& value);
	void handleExtGuiOption(const QString& name, const QVariant& value);
	virtual void handlePopupMenuShow(const QVariantList& opargs);
	virtual void handlePopupMenuSelect(const QVariantList& opargs);
	virtual void handleMouse(bool);
	virtual void handleGuiFontFunction(const QVariantList& args);
	virtual void handleGuiFontWide(const QVariant& value) noexcept;

	// Modern 'ext_linegrid' Grid UI Events
	virtual void handleGridResize(const QVariantList& opargs);
	virtual void handleDefaultColorsSet(const QVariantList& opargs);
	virtual void handleHighlightAttributeDefine(const QVariantList& opargs);
	virtual void handleHighlightGroupSet(const QVariantList& opargs) noexcept;
	virtual void handleGridLine(const QVariantList& opargs);
	virtual void handleGridCursorGoto(const QVariantList& opargs);
	virtual void handleGridScroll(const QVariantList& opargs);

	// GuiAdaptive Color/Font
	virtual void handleGuiAdaptiveColor(const QVariantList& opargs) noexcept;
	virtual void handleGuiAdaptiveFont(const QVariantList& opargs) noexcept;
	virtual void handleGuiAdaptiveStyle(const QVariantList& opargs) noexcept;
	virtual void handleGuiAdaptiveStyleList() noexcept;

	void neovimMouseEvent(QMouseEvent *ev);
	virtual void mousePressEvent(QMouseEvent *ev) Q_DECL_OVERRIDE;
	virtual void mouseReleaseEvent(QMouseEvent *ev) Q_DECL_OVERRIDE;
	virtual void mouseMoveEvent(QMouseEvent *ev) Q_DECL_OVERRIDE;
	void bailoutIfinputBlocking();
	void setCursorFromBusyState() noexcept;

	QString neovimErrorToString(const QVariant& err);

private slots:
        void setAttached(bool attached=true);

private:
	bool m_attached{ false };
	NeovimConnector* m_nvim{ nullptr };

	QList<QUrl> m_deferredOpen;

	QRect m_scroll_region;
	bool m_font_bold{ false };
	bool m_font_italic{ false };
	bool m_font_underline{ false };
	bool m_font_undercurl{ false };
	bool m_mouseHide{ true };

	// highlight fg/bg - from redraw:highlightset - by default we
	// use the values from above
	QColor m_hg_foreground{ Qt::black };
	QColor m_hg_background{ Qt:: white };
	QColor m_hg_special;

	/// Modern 'ext_linegrid' highlight definition map
	QMap<uint64_t, HighlightAttribute> m_highlightMap;

	/// Storage for hl_group_set, maps to hl_id in m_highlightMap
	QMap<QString, uint64_t> m_highlightGroupNameMap;

	/// Neovim mode descriptions from "mode_change", used by guicursor
	QVariantList m_modeInfo;

	bool m_resizing{ false };
	QSize m_resize_neovim_pending;
	QLabel* m_tooltip{ nullptr };
	QPoint m_mouse_pos;
	// 2/3/4 mouse click tracking
	QTimer m_mouseclick_timer;
	uint8_t m_mouseclick_count{ 0 };
	Qt::MouseButton m_mouseclick_pending;
	// Accumulates remainder of steppy scroll
	QPointF m_scroll_remainder;
	QPoint m_scroll_last_direction;

	// Properties
	bool m_neovimBusy{ false };
	ShellOptions m_options;
	PopupMenu m_pum{ this };
	bool m_mouseEnabled{ true };
};

class ShellRequestHandler: public QObject, public MsgpackRequestHandler
{
	Q_OBJECT
public:
	ShellRequestHandler(Shell *parent);
	virtual void handleRequest(MsgpackIODevice* dev, quint32 msgid, const QByteArray& method, const QVariantList& args);
};

template <class T>
/*static*/ void Shell::DispatchRedrawNotifications(T* pThis, const QVariantList& args) noexcept
{
	for(const auto& update_item : args) {
		if (!update_item.canConvert<QVariantList>()) {
			qWarning() << "Received unexpected redraw operation" << update_item;
			continue;
		}

		const QVariantList& redrawupdate{ update_item.toList() };
		if (redrawupdate.size() < 2) {
			qWarning() << "Received unexpected redraw operation" << update_item;
			continue;
		}

		const QByteArray& name{ redrawupdate.at(0).toByteArray() };
		const QVariantList& update_args{ redrawupdate.mid(1) };

		for(const auto& opargs_var : update_args) {
			if (!opargs_var.canConvert<QVariantList>()) {
				qWarning() << "Received unexpected redraw arguments, expecting list" << opargs_var;
				continue;
			}

			pThis->handleRedraw(name, opargs_var.toList());
		}
	}
}

} // Namespace
#endif
