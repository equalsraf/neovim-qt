#include "cmdwidget.h"

namespace NeovimQt {

CmdWidget::CmdWidget(ShellWidget *parent) {
    setParent(parent);
    hide();
    cursor = QTextCursor(document());
    setTextCursor(cursor);
    document()->setDefaultFont(parent->font());
}

void CmdWidget::compute_document(const QString& firstc, const QString& prompt, const QVariantList& content) {
    QString plaintext_content;
    foreach(QVariant piece, content){
        qDebug() << piece.toList();
        plaintext_content += piece.toStringList().at(1);
    }
    qDebug() << prompt << firstc;
    setPlainText(firstc + prompt + plaintext_content);
    cursor = QTextCursor(document());
}

void CmdWidget::add_special_char(const QString &c, bool shift_c) {
    if (!shift_c) {
        setOverwriteMode(true);
    }
    cursor.insertText(c);
    setOverwriteMode(false);
}

void CmdWidget::compute_block(const QList<QVariantList> &lines) {
    foreach(QVariantList line, lines) {
        append_block(line);
    }
    cursor = QTextCursor(document());
    cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::StartOfBlock, QTextCursor::MoveAnchor);
}

void CmdWidget::append_block(const QVariantList &content) {
    QString plaintext_content;
    foreach(QVariant piece, content){
        qDebug() << piece.toList();
        plaintext_content += piece.toStringList().at(1);
    }
    append(plaintext_content);
    line_count++;
}

void CmdWidget::setPos(int64_t pos) {
    cursor.movePosition(QTextCursor::StartOfLine);
    cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, pos);
}

void CmdWidget::keyPressEvent(QKeyEvent *ev)
{
    QWidget::keyPressEvent(ev);
}

}  // namespace NeovimQt
