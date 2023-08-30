#ifndef SUDOKU_INCLUDE
#define SUDOKU_INCLUDE

const int SUDOKU_DIMENSION = 9;

struct sudoku
{
	int sudokuArray[9][9];
	void importSudoku();
	void copyArray(int array[9][9]);
	void printSudoku();
};

#endif
