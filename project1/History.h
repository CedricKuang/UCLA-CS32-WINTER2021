#include "globals.h"

#ifndef HISTORY_H
#define HISTORY_H

class History
{
private:
	int nrows;
	int ncols;
	char grid[MAXROWS][MAXCOLS];
public:
	History(int nRows, int nCols);
	bool record(int r, int c);
	void display() const;
};


#endif
