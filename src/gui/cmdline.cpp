#include "cmdline.h"
#include "cmdwidget.h"

namespace NeovimQt {

CmdLine::CmdLine(QWidget *parent) : QTextEdit(parent) {
    hide();

    cursor = QTextCursor(document());
    setTextCursor(cursor);
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
    foreach(QVariant piece, content){
        qDebug() << piece.toList();
        auto style = html_style_attribute(piece.toList().at(0).toMap());
        plaintext_content += piece.toStringList().at(1);
        html_content += "<span " + style + ">" + piece.toStringList().at(1) + "</span>";
    }
    QString header = "<span " + html_style_attribute(QVariantMap()) + ">" + firstc + prompt + "</span>";
    setText(header + html_content);
    cursor = QTextCursor(document());
}

QColor CmdLine::color(qint64 color, const QColor& fallback) const
{
    if (color == -1) {
        return fallback;
    }
    return QRgb(color);
}

QString CmdLine::html_style_attribute(const QVariantMap& attrs) {
    QString style = "style=\"white-space: pre; ";

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
        style += "font-weight: bold; ";
    } else {
        style += "font-weight: normal; ";
    }
    if (attrs.value("italic").toBool()) {
        style += "font-style: italic; ";
    } else {
        style += "font-style: normal; ";
    }

    if (attrs.value("underline").toBool()) {
        style += "text-decoration: underline; ";
    } else if (attrs.value("undercurl").toBool()){
        style += "text-decoration: underline; ";
    } else {
        style += "text-decoration: normal; ";
    }

    style += "color: " + cur_foreground.toRgb().name() + "; ";
    style += "background-color: " + cur_background.toRgb().name() + "; ";
    style += "\"";
    return style;
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
