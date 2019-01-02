#include "cmdblock.h"

namespace NeovimQt {

CmdBlock::CmdBlock(ShellWidget *parent) : QTextEdit(parent) {
    hide();
    setReadOnly(true);
    document()->setDefaultFont(parent->font());
}

void CmdBlock::compute_block(const QList<QVariantList> &lines) {
    foreach(QVariantList line, lines) {
        append_block(line);
    }
}

void CmdBlock::append_block(const QVariantList &content) {
    QString plaintext_content;
    foreach(QVariant piece, content){
        qDebug() << piece.toList();
        plaintext_content += piece.toStringList().at(1);
    }
    append(plaintext_content);
    line_count++;
}

void CmdBlock::keyPressEvent(QKeyEvent *ev)
{
    // TODO : should it just eat the QKeyEvent ?
    QWidget::keyPressEvent(ev);
}

}  // namespace NeovimQt

