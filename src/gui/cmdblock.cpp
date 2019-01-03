#include "cmdblock.h"

namespace NeovimQt {

CmdBlock::CmdBlock(ShellWidget *parent) : QTextEdit(parent) {
    hide();
    setReadOnly(true);
    document()->setDefaultFont(parent->font());

    setTextBackgroundColor(parent->background());
    setTextColor(parent->foreground());
}

void CmdBlock::compute_block(const QList<QVariantList> &lines) {
    foreach(QVariantList line, lines) {
        append_block(line);
    }
}

void CmdBlock::append_block(const QVariantList &content) {
    QString plaintext_content;
    QString html_content;
    foreach(QVariant piece, content){
        qDebug() << piece.toList();
        auto style = html_style_attribute(piece.toList().at(0).toMap());
        plaintext_content += piece.toStringList().at(1);
        html_content += "<span " + style + ">" + piece.toStringList().at(1) + "</span>";
    }
    append(html_content);
    line_count++;
}

QColor CmdBlock::color(qint64 color, const QColor& fallback) const
{
    if (color == -1) {
        return fallback;
    }
    return QRgb(color);
}

QString CmdBlock::html_style_attribute(const QVariantMap& attrs) {
    QString style = "style=\"white-space: pre; ";

    auto shell_parent = dynamic_cast<ShellWidget*>(parent());
    QColor cur_foreground, cur_background, cur_special;
    if (attrs.contains("foreground")) {
        // TODO: When does Neovim send -1
        cur_foreground = color(attrs.value("foreground").toLongLong(), shell_parent->foreground());
    } else {
        cur_foreground = shell_parent->foreground();
    }

    if (attrs.contains("background")) {
        cur_background = color(attrs.value("background").toLongLong(), shell_parent->background());
    } else {
        cur_background = shell_parent->background();
    }

    // FIXME : the supported HTML tags do not seem to support special ( = different color for underline/undercurl)
    if (attrs.contains(("special"))) {
        cur_special = color(attrs.value("special").toLongLong(), shell_parent->special());
    } else {
        cur_special = shell_parent->special();
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

void CmdBlock::keyPressEvent(QKeyEvent *ev)
{
    // TODO : should it just eat the QKeyEvent ?
    QWidget::keyPressEvent(ev);
}

}  // namespace NeovimQt

