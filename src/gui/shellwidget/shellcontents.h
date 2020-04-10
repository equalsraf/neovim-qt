#pragma once

#include "cell.h"

/// A class to hold the contents of the shell / i.e. a grid of characters. This
/// class is meant to hold state about shell contents, but no more - e.g. cursor
/// information should be stored somewhere else.
class ShellContents
{
public:
	ShellContents(int rows, int columns);
	~ShellContents();
	ShellContents(const ShellContents& other);

	inline int columns() const {
		return _columns;
	}
	inline int rows() const {
		return _rows;
	}

	bool fromFile(const QString& path);

	const Cell* data();
	Cell& value(int row, int column);
	const Cell& constValue(int row, int column) const;

	/// Insert string `str` into cell grid.
	int put(
		const QString& str,
		int row,
		int column,
		const HighlightAttribute& hl_attr = {}) noexcept;

	void clearAll(QColor bg=QColor());
	void clearRow(int r, int startCol=0);
	void clearRegion(int row0, int col0, int row1, int col1,
			QColor bg=QColor());
	void resize(int rows, int columns);
	void scrollRegion(int row0, int row1, int col0, int col1, int count);
	void scroll(int rows);

private:
	void allocData();
	bool verifyRegion(int& row0, int& row1, int& col0, int& col1);

	// row*columns
	Cell *_data;
	static Cell invalidCell;
	int _rows, _columns;

	ShellContents& operator=(const ShellContents& other);
};
