#include "cmdwidget.h"
#include <QVBoxLayout>

namespace NeovimQt {

CmdWidget::CmdWidget(ShellWidget *parent) : QFrame(parent) {
	shell_parent = parent;
	cmd_lines.reserve(5);
	setFrameShape(QFrame::Panel);
	setFrameShadow(QFrame::Raised);
	connect(shell_parent, &ShellWidget::shellFontChanged,
                this, &CmdWidget::setDefaultFont);
}

void CmdWidget::setDefaultFont() { setFont(shell_parent->font()); }

CmdLine *CmdWidget::getLevel(int64_t level) {
	if (level <= 0) {
		qWarning() << "Wrong level asked : " << level;
		return nullptr;
	}

	if (level <= levels) {
		return cmd_lines[level - 1];
	}

	for (int i = levels; i < level; ++i) {
		cmd_lines.push_back(new CmdLine(this));
		++levels;
	}
	return cmd_lines[level - 1];
}

void CmdWidget::compute_layout() {
	auto old_layout = layout();
	if (old_layout) {
                delete old_layout;
        }

	auto new_layout = new QVBoxLayout(this);
	new_layout->setDirection(QBoxLayout::BottomToTop);
	new_layout->setSpacing(0);
	new_layout->setContentsMargins(0, 0, 0, 0);

	for (auto line : cmd_lines) {
		new_layout->addWidget(line);
	}
	setLayout(new_layout);
}

void CmdWidget::setGeometry2() {
        auto cols_in_px = shell_parent->columns() * shell_parent->cellSize().width();
        // How wide the CmdWidget should be w.r.t. grid width
        auto ratio_width = 0.66;
	auto anchor_x = static_cast<int64_t>((1 - ratio_width) / 2 * cols_in_px);
	auto anchor_y = shell_parent->frameGeometry().bottom();
	auto cmdline_width = static_cast<int64_t>(ratio_width * cols_in_px);
	auto cmdline_height = shell_parent->cellSize().height() + 5;
        anchor_y -= levels * cmdline_height - frameWidth();
	setGeometry(anchor_x, anchor_y, cmdline_width, levels * cmdline_height);

	compute_layout();
}

void CmdWidget::compute_document(const QString &firstc, const QString &prompt,
		                 const QVariantList &content, int64_t level) {
	auto current_line = getLevel(level);
	current_line->compute_document(firstc, prompt, content);
	current_line->show();
}

void CmdWidget::add_special_char(const QString &c, bool shift_c,
		                 int64_t level) {
	auto current_line = getLevel(level);
	current_line->add_special_char(c, shift_c);
	current_line->show();
}

void CmdWidget::setPos(int64_t pos, int64_t level) {
	auto current_line = getLevel(level);
	current_line->setPos(pos);
}

void CmdWidget::handleCmdlineHide() {
	// Clear layout and cmd_lines
	hide();
	for (auto line : cmd_lines) {
                delete line;
	}
        cmd_lines.clear();
        levels = 0;
}

void CmdWidget::keyPressEvent(QKeyEvent *ev) {
        QWidget::keyPressEvent(ev);
}

} // namespace NeovimQt
