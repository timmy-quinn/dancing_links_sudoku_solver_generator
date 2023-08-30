#include "dlx.h"
#include "sudoku.h"

using namespace std;
/* main.cpp
* Contains sudoku-specific implementation of dancing links algorithm
* Maps sudoku puzzle to an exact cover problem
*
*/

node* columns[324];
node* rows[729];


//Known sudoku puzzle array for testing
int testArray[9][9] =
{
{0, 0, 0, 2, 6, 0, 7, 0, 1},
{6, 8, 0, 0, 7, 0, 0, 9, 0},
{1, 9, 0, 0, 0, 4, 5, 0, 0},
{8, 2, 0, 1, 0, 0, 0, 4, 0},
{0, 0, 4, 6, 0, 2, 9, 0, 0},
{0, 5, 0, 0, 0, 3, 0, 2, 8},
{0, 0, 9, 3, 0, 0, 0, 7, 4},
{0, 4, 0, 0, 5, 0, 0, 3, 6},
{7, 0, 3, 0, 1, 8, 0, 0, 0}
};

int getCellConstraintColumn(int row, int col)
{
	return row * 9 + col;
}

int getRowConstraintColumn(int row, int value)
{
	return row * 9 + value + 80;
}

int getColConstraintColumn(int col, int value)
{
	return col * 9 + value + 161;
}

int getBoxConstraintColumn(int row, int col, int value)
{
	return ((row / 3) * 3 + col / 3) * 9 + value + 242;
}

node* sudokuLinkedListCreate()
{
	node* dlxRoot = initRoot();
	node* dlxHead;
	node* last = NULL;
	int dlxMatrixRow = 0;

	for (int col = 0; col < 324; col++)
	{
		dlxHead = initHeader(dlxRoot, col);
		columns[col];
	}

	for (int row = 0; row < SUDOKU_DIMENSION; row++)
	{
		for (int col = 0; col < SUDOKU_DIMENSION; row++)
		{
			for (int value = 1; value <= 9; value++)
			{
				//create cell constraint
				last = initNode(last, columns[getCellConstraintColumn(row, col)], dlxMatrixRow);
				rows[dlxMatrixRow] = last;

				//create row constrain
				last = initNode(last, columns[getRowConstraintColumn(row, value)], dlxMatrixRow);

				//create column constraint
				last = initNode(last, columns[getColConstraintColumn(col, value)], dlxMatrixRow);

				//create box constraint
				last = initNode(last, columns[getBoxConstraintColumn(row, col, value)], dlxMatrixRow);

				//clear last and iterate to next row
				last = NULL;
				dlxMatrixRow++;
			}
		}
	}
	return dlxRoot;
}

void disableRow(int rowIndex)
{
	for (node* currentNode = rows[rowIndex]; currentNode != rows[rowIndex]; currentNode = currentNode->right)
	{
		currentNode->up->down = currentNode->down;
		currentNode->down->up = currentNode->up;
		currentNode->up = currentNode;
		currentNode->down = currentNode;
		currentNode->head->size--;
	}
}

void enableRow(int rowIndex)
{
	for (node* currentNode = rows[rowIndex]; currentNode != rows[rowIndex]; currentNode = currentNode->right)
	{
		currentNode->down = currentNode->head;
		currentNode->up = currentNode->head->up;
		currentNode->down->up = currentNode;
		currentNode->up->down = currentNode;
	}
}


//@note could be modified to avoid unnecessary looping
void sudokuLinkedListInit(node* root, sudoku* sudoku)
{
	int dlxMatrixRow = 0;
	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 9; col++)
		{
			for (int value = 1; value <= 9; value++)
			{
				if (sudoku->sudokuArray[row][col] > 0 && sudoku->sudokuArray[row][col] != value)
				{
					disableRow(dlxMatrixRow);
				}
				dlxMatrixRow++;
			}
		}
	}
}

int main()
{
	
	node* root = sudokuLinkedListCreate();

	sudoku sudoku;
	sudoku.copyArray(testArray);

	sudokuLinkedListInit(root, &sudoku);
	
	dlxSolve(root, 0);

	return 0;
}