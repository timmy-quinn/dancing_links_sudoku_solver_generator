#ifndef SUDOKU_INCLUDE
#define SUDOKU_INCLUDE

#include "dlx.h"

const int SUDOKU_DIMENSION = 9;

struct sudoku
{
	node* root; 
	int sudokuArray[9][9];
	int solutionNumber;
	void importSudoku();
	void copyArray(int array[9][9]);
	void printSudoku();
	sudoku* solution;
};

#endif
