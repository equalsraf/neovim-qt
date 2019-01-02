#include "cmdline.h"

namespace NeovimQt {

CmdLine::CmdLine(QWidget *parent) : QTextEdit(parent) {
    hide();
    cursor = QTextCursor(document());
    setTextCursor(cursor);
    document()->setDefaultFont(parent->font());
}

void CmdLine::compute_document(const QString& firstc, const QString& prompt, const QVariantList& content) {
    QString plaintext_content;
    foreach(QVariant piece, content){
        qDebug() << piece.toList();
        plaintext_content += piece.toStringList().at(1);
    }
    qDebug() << prompt << firstc;
    setPlainText(firstc + prompt + plaintext_content);
    cursor = QTextCursor(document());
}

void CmdLine::add_special_char(const QString &c, bool shift_c) {
    if (!shift_c) {
        setOverwriteMode(true);
    }
    cursor.insertText(c);
    setOverwriteMode(false);
}

void CmdLine::setPos(int64_t pos) {
    cursor.movePosition(QTextCursor::StartOfLine);
    cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, pos);
}

void CmdLine::keyPressEvent(QKeyEvent *ev)
{
    QWidget::keyPressEvent(ev);
}

}  // namespace NeovimQt
