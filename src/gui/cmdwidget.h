#ifndef NEOVIM_QT_CMDWIDGET
#define NEOVIM_QT_CMDWIDGET

#include <QFrame>
#include <vector>

#include "cmdline.h"
#include "input.h"
#include "neovimconnector.h"
#include "shellwidget/shellwidget.h"

namespace NeovimQt {

// A Widget that complies to Neovim ui-cmdline API
class CmdWidget: public QFrame {
	Q_OBJECT
public:
    CmdWidget(ShellWidget *parent=nullptr);
    /**
     * @brief compute_document sets the underlying QTextDocument properly with highlights
     * @param content the list of content following :h ui-event-highlight_set specs
     */
    void compute_document(const QString& firstc, const QString& prompt, const QVariantList& content, int64_t level);

    /**
     * @brief add_special_char sets QTextDocument after cmdline_special_char is received
     * @param c is the char to show to mark the pending state
     * @param shift_c is true if c should be printed at the end of the line; if false, c overwrites last user written char
     */
    void add_special_char(const QString& c, bool shift_c, int64_t level);

    void setPos(int64_t pos, int64_t level);

    void setGeometry2();

    inline uint16_t lines() const { return levels; }

    inline const ShellWidget* shell() {return shell_parent;}

    inline const QFont font() const {return shell_parent->font(); }

    void handleCmdlineHide();

public slots:

signals:
	void reconnectNeovim();

protected:
	virtual void keyPressEvent(QKeyEvent *ev) Q_DECL_OVERRIDE;
        CmdLine* getLevel(int64_t level);

private:
    uint16_t levels{0};
    std::vector<CmdLine*> cmd_lines;
    ShellWidget* shell_parent{nullptr};
    QFont current_font;
};

} // Namespace

#endif
