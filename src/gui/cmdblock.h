#ifndef NEOVIM_QT_CMDBLOCK
#define NEOVIM_QT_CMDBLOCK

#include <QTextEdit>

#include "input.h"
#include "neovimconnector.h"
#include "shellwidget/shellwidget.h"

namespace NeovimQt {

// A Widget that complies to Neovim ui-cmdline API
class CmdBlock: public QTextEdit {
	Q_OBJECT
public:
	CmdBlock(ShellWidget *parent=nullptr);
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

	inline uint16_t lines() const { return line_count; }

public slots:
	void setDefaultFont();
        void fitSizeToDocument();
        void clear();

signals:
	void reconnectNeovim();

protected:
	virtual void keyPressEvent(QKeyEvent *ev) Q_DECL_OVERRIDE;
        virtual QSize viewportSizeHint() const Q_DECL_OVERRIDE;
        virtual QSize minimumSizeHint() const Q_DECL_OVERRIDE;
	QColor color(qint64 color, const QColor& fallback) const;
	void style_cursor_CharFormat(const QVariantMap& attrs);

private:
	uint16_t line_count{0};
        int64_t fitted_height{0};
        int64_t fitted_width{0};
        int32_t longest_line_pix{0};
};

} // Namespace

#endif
