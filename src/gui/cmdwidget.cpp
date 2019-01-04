#include "cmdwidget.h"
#include <QVBoxLayout>

namespace NeovimQt {

CmdWidget::CmdWidget(ShellWidget *parent) : QFrame(parent) {
  shell_parent = parent;
  hide();
  cmd_lines.reserve(5);
  setFrameShape(QFrame::Panel);
  setFrameShadow(QFrame::Raised);
  connect(shell_parent, &ShellWidget::shellFontChanged,
          this, &CmdWidget::setDefaultFont);
}

void CmdWidget::setDefaultFont() {
    setFont(shell_parent->font());
}

CmdLine* CmdWidget::getLevel(int64_t level) {
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
  auto current_layout = dynamic_cast<QVBoxLayout*>(layout());
  if (!current_layout) {
    current_layout = new QVBoxLayout(this);
    current_layout->setDirection(QBoxLayout::BottomToTop);
    current_layout->setSpacing(0);
    current_layout->setContentsMargins(0, 0, 0, 0);
  }

  for (auto line : cmd_lines) {
    current_layout->addWidget(line);
  }
  setLayout(current_layout);
}

void CmdWidget::setGeometry2() {
  auto anchor_x = 0;
  // FIXME : this anchor_y definition will break with cmdline_block commands
  // FIXME : Magic number : 5 is a tiny amount of pixels so chars fit the line
  auto anchor_y = (shell_parent->rows() - levels) * shell_parent->cellSize().height()
                  - 5 * levels;
  auto cmdline_width = shell_parent->columns() * shell_parent->cellSize().width();
  auto cmdline_height = shell_parent->cellSize().height() + 5; // cmdline_show is always called for one line
  setGeometry(anchor_x, anchor_y, cmdline_width, levels*cmdline_height);

  compute_layout();
}

void CmdWidget::compute_document(const QString& firstc, const QString& prompt, const QVariantList& content, int64_t level) {
  auto current_line = getLevel(level);
  current_line->compute_document(firstc, prompt, content);
  current_line->show();
}

void CmdWidget::add_special_char(const QString &c, bool shift_c, int64_t level) {
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
  auto old_layout = layout();
  delete old_layout;
}

void CmdWidget::keyPressEvent(QKeyEvent *ev)
{
  QWidget::keyPressEvent(ev);
}

}  // namespace NeovimQt
