#include "cmdline.h"
#include "cmdwidget.h"

namespace NeovimQt {

CmdLine::CmdLine(QWidget *parent) : QTextEdit(parent) {
    hide();

    setFrameShape(QFrame::NoFrame);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    document()->setDefaultFont(parent->font());

    auto cmdwidget_parent = dynamic_cast<CmdWidget*>(parent);
    setTextBackgroundColor(cmdwidget_parent->shell()->background());
    setTextColor(cmdwidget_parent->shell()->foreground());
}

void CmdLine::compute_document(const QString& firstc, const QString& prompt, const QVariantList& content) {
    QString plaintext_content;
    QString html_content;
    clear();
    style_cursor_CharFormat(QVariantMap());
    textCursor().insertText(firstc + prompt);
    foreach(QVariant piece, content){
        qDebug() << piece.toList();
        style_cursor_CharFormat(piece.toList().at(0).toMap());
        plaintext_content = piece.toStringList().at(1);
        textCursor().insertText(plaintext_content, textCursor().charFormat());
    }
    setFocus();
}

QColor CmdLine::color(qint64 color, const QColor& fallback) const
{
    if (color == -1) {
        return fallback;
    }
    return QRgb(color);
}

void CmdLine::style_cursor_CharFormat(const QVariantMap& attrs) {
    QTextCharFormat cur_format;

    auto cmdwidget_parent = dynamic_cast<CmdWidget*>(parent());
    QColor cur_foreground, cur_background, cur_special;
    if (attrs.contains("foreground")) {
        // TODO: When does Neovim send -1
        cur_foreground = color(attrs.value("foreground").toLongLong(), cmdwidget_parent->shell()->foreground());
    } else {
        cur_foreground = cmdwidget_parent->shell()->foreground();
    }

    if (attrs.contains("background")) {
        cur_background = color(attrs.value("background").toLongLong(), cmdwidget_parent->shell()->background());
    } else {
        cur_background = cmdwidget_parent->shell()->background();
    }

    // FIXME : the supported HTML tags do not seem to support special ( = different color for underline/undercurl)
    if (attrs.contains(("special"))) {
        cur_special = color(attrs.value("special").toLongLong(), cmdwidget_parent->shell()->special());
    } else {
        cur_special = cmdwidget_parent->shell()->special();
    }

    if (attrs.contains("reverse")) {
        auto tmp = cur_background;
        cur_background = cur_foreground;
        cur_foreground = tmp;
    }

    if (attrs.value("bold").toBool()) {
        cur_format.setFontWeight(QFont::Bold);
    } else {
        cur_format.setFontWeight(QFont::Normal);
    }
    cur_format.setFontItalic(attrs.value("italic").toBool());

    if (attrs.value("underline").toBool()) {
        cur_format.setFontUnderline(true);
        cur_format.setUnderlineStyle(QTextCharFormat::SingleUnderline);
    } else if (attrs.value("undercurl").toBool()){
        cur_format.setFontUnderline(true);
        cur_format.setUnderlineStyle(QTextCharFormat::WaveUnderline);
    } else {
        cur_format.setFontUnderline(false);
    }

    cur_format.setForeground(cur_foreground);
    cur_format.setBackground(cur_background);
    cur_format.setUnderlineColor(cur_special);
    setCurrentCharFormat(cur_format);
}

void CmdLine::add_special_char(const QString &c, bool shift_c) {
    if (!shift_c) {
        setOverwriteMode(true);
    }
    textCursor().insertText(c);
    setOverwriteMode(false);
}

void CmdLine::setPos(int64_t pos) {
    moveCursor(QTextCursor::StartOfLine);
    for (int i = 0; i < pos; ++i) {
        moveCursor(QTextCursor::Right, QTextCursor::MoveAnchor);
    }
}

void CmdLine::keyPressEvent(QKeyEvent *ev)
{
    QWidget::keyPressEvent(ev);
}

}  // namespace NeovimQt
