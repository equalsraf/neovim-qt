#include <QFile>
#include <QDebug>
#include "shellcontents.h"
#include "konsole_wcwidth.h"

Cell ShellContents::invalidCell = Cell::invalid();;

/// Build shell contents from file, each line in the
/// file is a shell line.
bool ShellContents::fromFile(const QString& path)
{
	if (_data != NULL) {
		delete[] _data;
	}
	_rows = 1;
	_columns = 1;
	allocData();

	QFile f(path);
	if (!f.open(QIODevice::ReadOnly)) {
		return false;
	}

	int row = 0;
	while(!f.atEnd()) {
		QString line = f.readLine();
		resize(_rows+1, qMax(_columns, string_width(line)));
		put(line, row, 0);
		row += 1;
	}
	return true;
}

ShellContents::ShellContents(int rows, int columns)
:_data(0), _rows(rows), _columns(columns)
{
	allocData();
}

ShellContents::~ShellContents()
{
	if (_data != NULL) {
		delete[] _data;
	}
}

ShellContents::ShellContents(const ShellContents& other)
:_data(0), _rows(other._rows), _columns(other._columns)
{
	if (other._data != NULL) {
		allocData();
		memcpy(_data, other._data, _rows*_columns);
	}
}

/// Allocates new shell data storage.
/// This leaks memory, make sure to free _data if
/// needed.
void ShellContents::allocData()
{
	_data = new Cell[_rows*_columns];
}

void ShellContents::clearAll(QColor bg)
{
	for (int i=0; i<_rows; i++) {
		for (int j=0; j<_columns; j++) {
			_data[i*_columns+j] = Cell::bg(bg);
		}
	}
}

void ShellContents::clearRow(int r, int startCol)
{
	if (r < 0 || r >= _rows || startCol < 0 ||
			startCol > _rows) {
		return;
	}

	int start = r*_columns + startCol;
	int end = start + _columns;
	for (int i=start; i<end-startCol; i++) {
		_data[i] = Cell();
	}
}


/// Verify if the region is valid adjust out of bounds values, returns
/// false if the region is invalid;
bool ShellContents::verifyRegion(int& row0, int& row1, int& col0, int& col1)
{
	if (row0 >= _rows || col0 >= _columns || row1 < 0 || col1 < 0) {
		return false;
	}
	if (row0 < 0 ) {
		row0 = 0;
	}
	if (col0 < 0 ) {
		col0 = 0;
	}
	if (row1 >= _rows  ) {
		row1 = _rows;
	}
	if (col1 >= _columns ) {
		col1 = _columns;
	}
	return true;
}

/// Clear shell region starting at (row0, col0) up until (row1, col1)
/// e.g. clearRegion(1, 1, 3, 3) clears a region with size 2x2
void ShellContents::clearRegion(int row0, int col0, int row1, int col1,
		QColor bg)
{
	if (!verifyRegion(row0, row1, col0, col1)) {
		return;
	}

	for (int i=row0; i<row1; i++) {
		for (int j=col0; j<col1; j++) {
			_data[i*_columns + j] = Cell::bg(bg);
		}
	}
}

/// Scroll the region by count lines. (row1, col1) is the first position outside
/// the scrolled area.
void ShellContents::scrollRegion(int row0, int row1, int col0, int col1, int count)
{
	if (count == 0) {
		return;
	}
	if (!verifyRegion(row0, row1, col0, col1)) {
		qDebug() << "Scroll region is invalid (row0, row1, col0, col1)"
			<< row0 << row1 << col0 << col1;
		return;
	}

	int start, stop, inc;
	if (count > 0) {
		start = row0;
		stop = row1;
		inc = +1;
	} else {
		start = row1-1;
		stop = row0-1;
		inc = -1;
	}

	for (int i=start; i!=stop; i+=inc) {
		int dst = i-count;
		if (dst >= row0 && dst < row1) {
			// Copy line
			memcpy(&_data[dst*_columns + col0],
				&_data[i*_columns + col0],
				(col1-col0)*sizeof(Cell));
		}

		// Clear src line
		for (int j=col0; j<col1; j++) {
			new (&_data[i*_columns + j]) Cell();
		}
	}
}

void ShellContents::scroll(int count)
{
	scrollRegion(0, _rows, 0, _columns, count);
}

void ShellContents::resize(int newRows, int newColumns)
{
	if (newRows <= 0 || newColumns <= 0) {
		qWarning() << "Invalid shell size" << newRows << newColumns;
		return;
	}

	if (newRows == _rows && newColumns == _columns) {
		return;
	}

	Cell *old = _data;
	int oldRows = _rows;
	int oldColumns = _columns;

	_rows = newRows;
	_columns = newColumns;
	allocData();

	// Copy the original
	int copyRows = qMin(oldRows, _rows);
	int copyColumns = qMin(oldColumns, _columns);

	for (int i=0; i<copyRows; i++) {
		memcpy(&_data[i*_columns],
			&old[i*oldColumns],
			copyColumns*sizeof(Cell));
	}

	delete [] old;
}

const Cell* ShellContents::data()
{
	return _data;
}

Cell& ShellContents::value(int row, int column)
{
	if (row < 0 || row >= _rows || column < 0 || column >= _columns) {
		return invalidCell;
	}
	return _data[row*_columns + column];
}
const Cell& ShellContents::constValue(int row, int column) const
{
	if (row < 0 || row >= _rows || column < 0 || column >= _columns) {
		return invalidCell;
	}
	return _data[row*_columns + column];
}

/// Writes content to the shell, returns the number of columns written
int ShellContents::put(const QString& str, int row, int column,
		QColor fg, QColor bg, QColor sp, bool bold, bool italic,
		bool underline, bool undercurl)
{
	if (row < 0 || row >= _rows || column < 0 || column >= _columns) {
		return 0;
	}
	
	int pos = column;
	for (int i=0; i<str.size(); i++) {
		Cell& c = value(row, pos);
		QChar cell_chr = str.at(i);
		c = Cell(cell_chr, fg, bg, sp, bold, italic, underline, undercurl);

		if (cell_chr.isHighSurrogate()) {
			if (i<str.size()-1 && str.at(i+1).isLowSurrogate()) {
				c.setLowSurrogate(str.at(i+1));
			}
			i++;
		}

		if (c.doubleWidth) {
			value(row, pos+1) = Cell();
			pos += 2;
		} else {
			pos += 1;
		}
	}
	return pos - column;
}


