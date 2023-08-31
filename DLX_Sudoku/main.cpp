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

// TODO replace vectors with stacks
vector <sudoku> solvedPuzzles;

//Known sudoku puzzle array for testing
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
{0, 2, 3, 6, 7, 8, 9, 4, 5},
{5, 0, 4, 2, 3, 9, 7, 6, 1},
{9, 6, 7, 1, 4, 5, 3, 2, 8},
{3, 7, 2, 4, 6, 1, 5, 8, 9},
{6, 9, 1, 5, 8, 3, 2, 7, 4},
{4, 5, 8, 7, 9, 2, 6, 1, 3},
{8, 3, 6, 9, 2, 4, 1, 5, 7},
{2, 1, 9, 8, 5, 7, 4, 3, 6},
{7, 4, 5, 3, 1, 6, 8, 9, 2}
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

	cout << "creating headers\n";
	for (int col = 0; col < 324; col++)
	{
		dlxHead = initHeader(dlxRoot, col);
		columns[col] = dlxHead;
	}
	cout << "headers creater\n";

	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 9; col++)
		{
			for (int value = 1; value <= 9; value++)
			{
				cout << "[Sudoku LL create] current row number: " << dlxMatrixRow << "\n";
				cout << "value: " << value << "\n";
				cout << " row: " << row << "\n";
				cout << "col: " << col << "\n";
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
	cout << "Disabling row: " << rowIndex << "\n";
	node* currentNode = rows[rowIndex];
	do
	{
		currentNode->up->down = currentNode->down;
		currentNode->down->up = currentNode->up;
		currentNode->up = currentNode;
		currentNode->down = currentNode;
		currentNode->head->size--;
		currentNode = currentNode->right;
	} while (currentNode != rows[rowIndex]);
}

void enableRow(int rowIndex)
{
	node* currentNode = rows[rowIndex];
	do
	{
		currentNode->down = currentNode->head;
		currentNode->up = currentNode->head->up;
		currentNode->down->up = currentNode;
		currentNode->up->down = currentNode;
		currentNode = currentNode->right;

	} while (currentNode != rows[rowIndex]);
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

int getValue(int linkedListRow)
{
	int value = (linkedListRow + 1) % 9;
	if (value == 0) value = 9;
	return value;
}

int getCellNumber(int linkedListRow)
{
	return linkedListRow / 9;
}

void solutionToSudoku(vector <node *> * solution, sudoku* sudoku)
{
	int col;
	int row;
	for (int i = 0; i < solution->size(); i++)
	{
		col = getCellNumber((*solution)[i]->rowNumber) % 9;
		row = getCellNumber((*solution)[i]->rowNumber) / 9;
		sudoku->sudokuArray[row][col] = getValue((*solution)[i]->rowNumber);
	}

}

void printDLXRow(int rowNumber, int array[729][324])
{
	for (int y = 0; y < 324; y++)
	{
		cout << array[rowNumber][y];
	}
	cout << "\n";

}


void printFullDLXArray(int array[729][324])
{
	cout << "Array printing" << endl << endl;
	cout << " -----------------" << endl;
	for (int x = 0; x < 729; x++)
	{
		cout << "Row: " << x << "\n";
		for (int y = 0; y < 324; y++)
		{
			cout << array[x][y];
		}
		cout << "\n";
	}
}

void sudokuDLX2Array(node* dlxRoot)
{
	int array[729][324] = { 0 };
	int filledRows;
	for (node* columnHeader = dlxRoot->right; columnHeader != dlxRoot; columnHeader = columnHeader->right)
	{
		for (node* r = columnHeader->down; r != columnHeader; r = r->down)
		{
			array[r->rowNumber][r->head->columnNumber] = 1;
		}
	}
	printFullDLXArray(array);
}


int main()
{
	cout << "creating linked list\n";
	node* root = sudokuLinkedListCreate();
	cout << "link list created\n";
	for (int i = 0; i < 729; i++)
	{
		cout << "row pointer: " << rows[i] << "\n";
	}

	sudoku solvedPuzzle;
	sudoku sudokuPuzzle;
	
	cout << "copying array\n";
	sudokuPuzzle.copyArray(testArray);
	cout << "done\n";

	cout << "initialing\n";
	sudokuLinkedListInit(root, &sudokuPuzzle);
	sudokuDLX2Array(root);

	/*
	dlxSolve(root, 0);

	vector<vector < node*> >  solutions = getAllSolutions();

	solutionToSudoku(&solutions[0], &solvedPuzzle);

	solvedPuzzle.printSudoku();*/

	return 0;
}