#include "dlx.h"
#include "sudoku.h"
#include <time.h>
#include <random>
#include <unordered_set>

using namespace std;
/* main.cpp
* Contains sudoku-specific implementation of dancing links algorithm
* Maps sudoku puzzle to an exact cover problem
*/

// Known sudoku puzzle array for testing. Temporary
int testArraySolution[9][9] =
{
{1, 2, 3, 6, 7, 8, 9, 4, 5},
{5, 8, 4, 2, 3, 9, 7, 6, 1},
{9, 6, 7, 1, 4, 5, 3, 2, 8},
{3, 7, 2, 4, 6, 1, 5, 8, 9},
{6, 9, 1, 5, 8, 3, 2, 7, 4},
{4, 5, 8, 7, 9, 2, 6, 1, 3},
{8, 3, 6, 9, 2, 4, 1, 5, 7},
{2, 1, 9, 8, 5, 7, 4, 3, 6},
{7, 4, 5, 3, 1, 6, 8, 9, 2}
};

int testArray[9][9] =
{
{0, 1, 8, 0, 0, 2, 3, 0, 4},
{0, 0, 3, 5, 0, 0, 0, 0, 0},
{5, 2, 4, 8, 9, 0, 0, 0, 0},
{1, 0, 5, 0, 7, 0, 4, 0, 6},
{0, 0, 7, 0, 0, 0, 9, 0, 0},
{2, 0, 9, 0, 4, 0, 5, 0, 8},
{0, 0, 0, 0, 8, 9, 6, 4, 3},
{0, 0, 0, 0, 0, 7, 2, 0, 0},
{3, 0, 1, 6, 0, 0, 7, 8, 0}
};

int main()
{

	sudoku sudokuA; 
	sudoku sudokuB;
	sudoku sudokuC; 
	

	//Generate a random sudoku puzzle with a solution
	cout << "Sudoku A\n"; 
	sudokuA.generateRandomSudoku(60);
	sudokuA.printSudoku();

	sudokuA.solveSudoku(); 
	cout << " test\n"; 
	sudokuA.printSolvedSudoku();
	sudokuA.solveSudoku(); 
	sudokuA.printSolutions(); 

	////Solve a puzzle from a given array
	//cout << "Sudoku B\n"; 
	//sudokuB.copyArray(testArray);
	//sudokuB.solveSudoku();
	//sudokuB.printSolutions();

	////Solve a sudoku puzzle from an imported txt file
	//cout << "Sudoku C\n"; 
	//sudokuC.importSudoku("sudoku1.txt");
	//sudokuC.printSudoku();
	//sudokuC.solveSudoku(); 
	//sudokuC.printSolutions();


	return 0;
}