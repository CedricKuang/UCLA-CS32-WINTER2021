#include <iostream>
#include "History.h"
#include "globals.h"
using namespace std;

History::History(int nRows, int nCols)
{
	ncols = nCols;
	nrows = nRows;
	for (int i = 0; i < nrows; i++)
		for (int m = 0; m < ncols; m++)
			grid[i][m] = '.';
}

bool History::record(int r, int c)
{
	if (r > nrows || c > ncols || r <= 0 || c <= 0)
		return false;

	char gridchar = grid[r - 1][c - 1];
	switch (gridchar)
	{
	case '.': grid[r - 1][c - 1] = 'A'; break;
	case 'Z': break;
	default: grid[r - 1][c - 1]++; break;
	}

	return true;
}

void History::display() const
{
	clearScreen();
	for (int i = 0; i < nrows; i++)
	{
		for (int m = 0; m < ncols; m++)
			cout << grid[i][m];
		cout << endl;
	}
	cout << endl;
}