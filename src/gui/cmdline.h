#ifndef NEOVIM_QT_CMDLINE
#define NEOVIM_QT_CMDLINE

#include <QTextEdit>

#include "input.h"
#include "neovimconnector.h"
#include "shellwidget/shellwidget.h"

namespace NeovimQt {

class CmdWidget;

// A Widget that holds one cmdline
class CmdLine: public QTextEdit {
	Q_OBJECT
public:
        CmdLine(CmdWidget *parent=nullptr);
        /**
         * @brief compute_document sets the underlying QTextDocument properly with highlights
         * @param content the list of content following :h ui-event-highlight_set specs
         */
        void compute_document(const QString& firstc, const QString& prompt, const QVariantList& content);

        /**
         * @brief add_special_char sets QTextDocument after cmdline_special_char is received
         * @param c is the char to show to mark the pending state
         * @param shift_c is true if c should be printed at the end of the line; if false, c overwrites last user written char
         */
        void add_special_char(const QString& c, bool shift_c);

        void setPos(int64_t pos);

public slots:

signals:
	void reconnectNeovim();

protected:
	virtual void keyPressEvent(QKeyEvent *ev) Q_DECL_OVERRIDE;
        QColor color(qint64 color, const QColor& fallback) const;
        void style_cursor_CharFormat(const QVariantMap& attrs);

private:
        CmdWidget* cmdwidget_parent;
};

} // Namespace NeovimQt

#endif
