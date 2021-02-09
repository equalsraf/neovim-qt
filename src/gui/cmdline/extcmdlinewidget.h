#pragma once

#include <QFrame>
#include <QLineEdit>
#include <QTextEdit>
#include <QTextLayout>
#include <QVBoxLayout>
#include <shellwidget.h>

#include "blockdisplay.h"
#include "linemodel.h"
#include "neovimconnector.h"
#include "position.h"

namespace NeovimQt { namespace Cmdline {

class ExtCmdlineWidget : public QFrame
{
	Q_OBJECT

public:
	ExtCmdlineWidget(NeovimConnector* nvim, ShellWidget* parent) noexcept;

	void handleNeovimNotification(const QByteArray& name, const QVariantList& args) noexcept;
	void handleRedraw(const QByteArray& name, const QVariantList& opargs) noexcept;


// FIXME private:
	void neovimConnectorReady() noexcept;

	// FIXME Label redraw events
	void handleCmdlineShow(const QVariantList& args) noexcept;
	void handleCmdlinePos(const QVariantList& args) noexcept;
	void handleCmdlineSpecialChar(const QVariantList& args) noexcept;
	void handleCmdlineBlockShow(const QVariantList& args) noexcept;
	void handleCmdlineBlockAppend(const QVariantList& args) noexcept;
	void handleCmdlineHide() noexcept;
	void handleCmdlineBlockHide() noexcept;

	// FIXME Label GUI Command events
	void handleGuiCommandlinePosition(const QVariantList& args) noexcept;

	// FIXME label ??? functions below
	void updateGeometry() noexcept;

	void setCursorPosition(int pos) noexcept;

	QSize sizeHint() const noexcept override;

	void updatePalette() noexcept;

	void setFont(const QFont& font) noexcept;

	void setCursorStyle(const Cursor& cursor) noexcept
	{
		m_cmdTextBox->CopyCursorStyle(cursor);
	}

private:
	NeovimConnector *m_nvim{ nullptr };

	int getMaxPromptLength() const noexcept;

	QSize getShellSizeForText(const QString& text) const noexcept;

	QFrame* m_cmdTextBoxFrame;
	ShellWidget* m_cmdTextBox;
	BlockDisplay* m_cmdBlockText;

	QVBoxLayout* m_vLayout;
	QPalette m_palette;

	QList<LineModel> m_model;

	// FIXME const?
	double m_maxWidth{ .75 };
	double m_minWidth{ 300 };

	Position m_position{ Position::Top };
};

} } // namespace NeovimQt::Cmdline
