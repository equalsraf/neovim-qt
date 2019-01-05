#ifndef NEOVIM_QT_CMDWIDGET
#define NEOVIM_QT_CMDWIDGET

#include <QFrame>
#include <vector>

#include "cmdline.h"
#include "input.h"
#include "neovimconnector.h"
#include "shellwidget/shellwidget.h"

namespace NeovimQt {

// A Widget that complies to Neovim ui-cmdline API. It holds all single cmdlines
class CmdWidget: public QFrame {
	Q_OBJECT
public:
        /**
         * @brief Contstructor from a parent ShellWidget
         * @param parent is the parent ShellWidget,
         *               mostly useful for appearance consistency
         */
        CmdWidget(ShellWidget *parent=nullptr);

        /**
         * @brief compute_document sets the underlying QTextDocument
         * 			   properly with highlights
         * @param firstc is the first char of a computed line like ':' or '='
         * @param prompt is the read-only text for input()-style prompts
         * @param content the list of content following :h ui-event-highlight_set specs
         * @param level is the targeted cmdline level
         */
        void compute_document(const QString& firstc, const QString& prompt,
                              const QVariantList& content, int64_t level);

        /**
         * @brief add_special_char sets QTextDocument after cmdline_special_char
         *                         is received
         * @param c is the char to show to mark the pending state
         * @param shift_c if true, c should be printed at the end of the line;
         *                if false, c overwrites last user written char
         * @param level is the targeted cmdline level
         */
        void add_special_char(const QString& c, bool shift_c, int64_t level);

        /**
         * @brief setPos sets the cursor position in a cmdline
         * @param pos the wanted position
         * @param level is the targeted cmdline level
         */
        void setPos(int64_t pos, int64_t level);

        /**
         * @brief setGeometry2 sets the geometry of the widget.
         *                     It should probably be a slot or an override ?
         */
        void setGeometry2();

        inline uint16_t lines() const { return levels; }

        inline const ShellWidget* shell() { return shell_parent; }

        inline const QFont font() const { return shell_parent->font(); }

        /**
         * @brief handleCmdlineHide handles cmdline_hide events. It clears cmdlines
         */
        void handleCmdlineHide();

public slots:
        void setDefaultFont();

signals:
	void reconnectNeovim();

protected:
	virtual void keyPressEvent(QKeyEvent *ev) Q_DECL_OVERRIDE;
        CmdLine* getLevel(int64_t level);
        void compute_layout();

private:
        uint16_t levels{0};
        std::vector<CmdLine*> cmd_lines;
        ShellWidget* shell_parent{nullptr};
};

} // Namespace

#endif
