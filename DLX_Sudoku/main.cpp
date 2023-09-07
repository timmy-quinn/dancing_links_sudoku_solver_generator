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

node* columns[324];
node* rows[729];

vector <sudoku> solvedPuzzles;

int blankArray[9][9];

//****Display solutions functions ********
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
	for (node* columnHeader = dlxRoot->right; columnHeader != dlxRoot; columnHeader = columnHeader->right)
	{
		for (node* r = columnHeader->down; r != columnHeader; r = r->down)
		{
			array[r->rowNumber][r->head->columnNumber] = 1;
		}
	}
	printFullDLXArray(array);
}

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

// Returns the exact cover matrix column which represents a sudoku puzzle cell 
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

//Create a toroidally linked list which represents a sudoku puzzle as an exact cover problem
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


//TODO use set cell in linked list init
/*Parameters
*	cellNumber: 0-80;
*	value: 0-8;
*/
void setCell(int cellNumber, int value)
{
	for (int i = 0; i < 9; i++)
	{
		if (i != value)
		{
			disableRow(cellNumber * 9 + i);
		}
	}
}

//Initializes a sudoku linked list with a specific puzzle
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

//Convert the exact cover matrix to a sudoku array
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

// *** Sudoku generation functions

int getRandomInt(int min, int max)
{
	// Initialize the random_device
	random_device rd;

	// Seed the engine
	mt19937_64 generator(rd());

	// Specify the range of numbers to generate, in this case [min, max]
	uniform_int_distribution<int> dist{min, max};

	return dist(generator);
}

bool compareRows(int rowIndex, vector<int>* compareRows)
{
	node* row = rows[rowIndex];
	for (int i = 0; i < compareRows->size(); i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (row->head->columnNumber == rows[(*compareRows)[i]]->head->columnNumber)
			{
				return false;
			}
		}
	}
	return true;
}

void setBox(int boxNumber, vector<int> values)
{
	int array[9] = { 0 };
	int arrayIndex = 0;
	// box number initial: 0, 3, 6, 27, 30, 33
	int row = (boxNumber / 3) *3; 
	int col = ( boxNumber % 3 ) * 3; 
	int cellNumber = row * 9 + col; 

	cout << "CellNumber: " << cellNumber << "\n";
	int index = 0; 
	for ( int i = 0; i < 3; i++ ) 
	{
		for (int j = 0; j < 3; j++)
		{
			// setCell(cellNumber, values[index]);
			cout << "cell set: " << cellNumber << " to " << values[index] << " \n";
			// array[index] = values[index];
			blankArray[row + j][col + i] = values[index];
			index++; 
			cellNumber++;
		}
		cellNumber += 6; 
	}
	
	for (int k = 0; k < 9; k++)
	{
		if (k % 3 == 0) cout << "\n"; 
		cout << " " << array[k] << " ";
	}
}

//TODO cleanup randomSudokuInit
void randomSudokuInit()
{
	int dlxMatrixRow = 0;
	vector<int> values = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	//Shuffle this array. This gives us a way to randomly select spaces to be cleared
	random_device rd;
	int boxeNumbers[2][3] = { {0, 4, 8}, {2, 4, 6} };
						

	// Seed the engine
	mt19937_64 generator(rd());

	
	int choice = getRandomInt(0, 1);

	for (int i = 0; i < 3; i++ )
	{
		shuffle(values.begin(), values.end(), generator);
		setBox(boxeNumbers[choice][i], values); 
	}
}




int main()
{
	int array[7][7] = {
		{1, 0, 0, 1, 0, 1, 0},
		{1, 0, 0, 0, 1, 0, 1},
		{0, 1, 0, 1, 0, 1, 0},
		{0, 1, 0, 0, 1, 0, 1},
		{0, 0, 1, 0, 0, 0, 0},
		{0, 0, 1, 0, 0, 0, 0},
		{1, 0, 1, 0, 0, 0, 0},
	};


	/*node* root = arrayToDLXLinkedList(array, 7, 7);
	dlxGetRandomSolution(root, 0);
	convertDLX2Array(root);*/
	cout << "creating linked list\n";
	node* root = sudokuLinkedListCreate();
	cout << "linked list created\n";

	 sudoku solvedPuzzle;
	 sudoku initialPuzzle;
	
	cout << "copying array\n";
	
	randomSudokuInit(); 
	initialPuzzle.copyArray(blankArray);
	initialPuzzle.printSudoku();
	sudokuLinkedListInit(root, &initialPuzzle);



	//cout << "random sudoku initialized\n";
	//cout << "free columns size: " << freeColumnsSize();

	// dlxGetRandomSolution(root, 0);
	// dlxSolve(root, 0);
	dlxGetOneSolution(root, 0);

	vector<vector < node*> >  solutions = getAllSolutions();
	cout << "Solutions found: " << solutions.size();
	
	for (int i = 0; i < solutions.size(); i++)
	{	
		cout << "\n Sudoku Solution: " << i << "\n";
		solutionToSudoku(&solutions[i], &solvedPuzzle);
		solvedPuzzle.printSudoku();
	}

	cout << "finding random solution \n"; 
	dlxGetRandomSolution(root, 0); 

	cout << "Random solution found " << "\n"; 

	solutions = getAllSolutions();
	cout << "Solutions found: " << solutions.size();

	for (int i = 0; i < solutions.size(); i++)
	{
		cout << "\n Sudoku Solution: " << i << "\n";
		solutionToSudoku(&solutions[i], &solvedPuzzle);
		solvedPuzzle.printSudoku();
	}


	return 0;
}