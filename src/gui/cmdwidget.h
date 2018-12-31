#ifndef NEOVIM_QT_CMDWIDGET
#define NEOVIM_QT_CMDWIDGET

#include <QTextEdit>

#include "input.h"
#include "neovimconnector.h"
#include "shellwidget/shellwidget.h"

namespace NeovimQt {

// A Widget that complies to Neovim ui-cmdline API
class CmdWidget: public QTextEdit {
	Q_OBJECT
public:
    CmdWidget(ShellWidget *parent=nullptr);
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

    /**
     * @brief compute_block sets the underlying QTextDocument as a cmdline_block
     * @param lines the list of lines with content-syntax following :h ui-event-highlight_set specs
     * This typically means that lines is QVariantList of QVariantLists
     */
    void compute_block(const QList<QVariantList>& lines);

    /**
     * @brief append_block appends the underlying QTextDocument with the line "content"
     * @param content the new line to append
     */
    void append_block(const QVariantList& content);

    void setPos(int64_t pos);

    inline uint16_t lines() const { return line_count; }

public slots:

signals:
	void reconnectNeovim();

protected:
	virtual void keyPressEvent(QKeyEvent *ev) Q_DECL_OVERRIDE;

private:
    QTextCursor cursor;
    uint16_t line_count{0};
};

} // Namespace

#endif
