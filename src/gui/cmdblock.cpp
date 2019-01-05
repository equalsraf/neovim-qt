#include "cmdblock.h"

namespace NeovimQt {

CmdBlock::CmdBlock(ShellWidget *parent) : QTextEdit(parent) {
	setReadOnly(true);
	document()->setDefaultFont(parent->font());
        setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

	setTextBackgroundColor(parent->background());
	setTextColor(parent->foreground());
        setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

	connect(parent, &ShellWidget::shellFontChanged,
		    this, &CmdBlock::setDefaultFont);
        connect(this, &QTextEdit::textChanged,
                this, &CmdBlock::fitSizeToDocument);
}

void CmdBlock::setDefaultFont() {
	auto shell_parent = dynamic_cast<ShellWidget*>(parent());
	setFont(shell_parent->font());
}

void CmdBlock::compute_block(const QList<QVariantList> &lines) {
	foreach(QVariantList line, lines) {
		append_block(line);
	}
}

void CmdBlock::append_block(const QVariantList &content) {
	QString plaintext_piece, plaintext_line;
	foreach(QVariant piece, content){
		style_cursor_CharFormat(piece.toList().at(0).toMap());
		plaintext_piece = piece.toStringList().at(1);
                plaintext_line += plaintext_piece;
		textCursor().insertText(plaintext_piece);
        }

        auto fm = QFontMetrics(font());
        auto current_line_len = fm.width(plaintext_line);
        if (current_line_len > longest_line_pix) {
                longest_line_pix = current_line_len;
        }
	textCursor().insertText("\n");
	line_count++;
        adjustSize();
}

QColor CmdBlock::color(qint64 color, const QColor& fallback) const
{
	if (color == -1) {
		return fallback;
	}
	return QRgb(color);
}

void CmdBlock::style_cursor_CharFormat(const QVariantMap& attrs) {
	QTextCharFormat cur_format;

	auto shell_parent = dynamic_cast<ShellWidget*>(parent());
	QColor cur_foreground, cur_background, cur_special;
	if (attrs.contains("foreground")) {
		cur_foreground = color(attrs.value("foreground").toLongLong(),
                                       shell_parent->foreground());
	} else {
		cur_foreground = shell_parent->foreground();
	}

	if (attrs.contains("background")) {
		cur_background = color(attrs.value("background").toLongLong(),
                                       shell_parent->background());
	} else {
		cur_background = shell_parent->background();
	}

	if (attrs.contains(("special"))) {
		cur_special = color(attrs.value("special").toLongLong(),
                                    shell_parent->special());
	} else {
		cur_special = shell_parent->special();
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

void CmdBlock::keyPressEvent(QKeyEvent *ev)
{
	QWidget::keyPressEvent(ev);
}

QSize CmdBlock::viewportSizeHint() const {
        qDebug() << "CmdBlock Viewport sizehint asked : "
                 << fitted_width << "x" << fitted_height;
        return QSize(fitted_width, fitted_height);
}

QSize CmdBlock::minimumSizeHint() const {
        return sizeHint();
}

void CmdBlock::fitSizeToDocument() {
        auto font_metrics = QFontMetrics(font());
        document()->setTextWidth(viewport()->width());
        QSize document_size(document()->size().toSize());
        fitted_height = document_size.height();
        // Pad the longest line with one additionnal char
        fitted_width = longest_line_pix + font_metrics.averageCharWidth();
        updateGeometry();
        adjustSize();
}

void CmdBlock::clear() {
        longest_line_pix = 0;
        QTextEdit::clear();
}

}  // namespace NeovimQt
