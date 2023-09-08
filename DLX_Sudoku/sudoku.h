#ifndef SUDOKU_INCLUDE
#define SUDOKU_INCLUDE

#include "dlx.h"

const int SUDOKU_DIMENSION = 9;

struct sudoku
{
		node* root; 
		node* columns[324];
		node* rows[729];
		sudoku* solution;
		int sudokuArray[9][9];
		int solvedArray[9][9]; 
		int numberSolutions;
		vector<vector < node*> >  solutions;

		// *** Basic sudoku array functions ***
		void importSudoku();
		void copyArray(int array[9][9]);
		void printSudoku();
		void printSolvedSudoku();

		// *** Sudoku Exact Cover Matrix (ECM) Functions ***
		int getCellConstraintColumn(int row, int col); 
		int getRowConstraintColumn(int row, int value);
		int getColConstraintColumn(int col, int value);
		int getBoxConstraintColumn(int row, int col, int value);
		//get value 
		int getValue(int linkedListRow);
	
		void enableRow(int rowIndex);
		void resetCell(int rowIndex);
		void disableRow(int rowIndex);
		void setCell(int cellNumber, int value);
		void setBox(int boxNumber, vector<int> values);

		node* sudokuECMCreate();
		void sudokuECMInit();
		void solutionToSudoku(vector <node*>* solution);
		void randomECMInit();
		bool eraseCells(int index, int blankCells, int targetNum, vector<int>* cells);
	
	public:
		void solveSudoku();
		void generateRandomSudoku(int blankSquares);
		void printSolutions();
};

#endif
