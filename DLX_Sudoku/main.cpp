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
{0, 2, 3, 0, 7, 8, 9, 4, 5},
{5, 8, 4, 2, 3, 9, 7, 6, 1},
{9, 6, 7, 1, 4, 5, 3, 2, 8},
{3, 7, 2, 4, 6, 1, 5, 8, 9},
{6, 9, 1, 0, 0, 3, 2, 0, 0},
{0, 5, 8, 7, 9, 2, 6, 1, 0},
{0, 0, 0, 9, 0, 0, 1, 5, 7},
{0, 1, 0, 8, 5, 7, 4, 3, 6},
{7, 0, 0, 3, 1, 6, 8, 9, 2}
};

int main()
{

	sudoku sudokuA; 
	sudoku sudokuB;
	sudokuA.generateRandomSudoku();
	sudokuA.printSolutions(); 
	sudokuB.copyArray(testArray);
	sudokuB.solveSudoku();
	sudokuB.printSolutions();

	return 0;
}