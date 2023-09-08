#include "sudoku.h"
#include <string>
#include <fstream>
#include <iostream>

using namespace std;


// *** Basic sudoku array functions ***
void sudoku::importSudoku(string fileName)
{
	ifstream inFile;
	inFile.open(fileName);
	int temp; 
	if (!inFile.is_open())
	{
		cout << "Error:  File not imported" << endl;
		return;
	}
	for (int row = 0; row < 9; row++) 
	{
		for (int col = 0; col < 9; col++) {
			inFile >> temp; 
			if (temp <= 9 && temp > 0) sudokuArray[row][col] = temp;
			else sudokuArray[row][col] = 0;  
		}
	}

}

void sudoku::copyArray(int array[9][9])
{
	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 9; col++)
		{
			sudokuArray[row][col] = array[row][col];
		}

	}
}

void sudoku::printSudoku()
{
	cout << "\n";
	cout << " -------+-------+-------" << endl;
	for (int row = 0; row < 9; row++)
	{
		cout << "| ";
		for (int col = 0; col < 9; col++)
		{
			if (sudokuArray[row][col] > 0 && sudokuArray[row][col] <= 9) cout << sudokuArray[row][col] << " ";
			else cout << "  "; 
			if (col % 3 == 2) cout << "| ";
		}
		cout << "\n";
		if (row % 3 == 2) cout << " -------+-------+-------" << "\n";

	}

}

void sudoku::printSolvedSudoku()
{
	cout << "\n";
	cout << " -------+-------+-------" << endl;
	for (int row = 0; row < 9; row++)
	{
		cout << "| ";
		for (int col = 0; col < 9; col++)
		{
			if (solvedArray[row][col] > 0 && solvedArray[row][col] <= 9) cout << solvedArray[row][col] << " ";
			else cout << "  ";
			if (col % 3 == 2) cout << "| ";
		}
		cout << "\n";
		if (row % 3 == 2) cout << " -------+-------+-------" << "\n";

	}
}


// *** Sudoku exact cover matrix (ECM) functions ***
// Returns the exact cover matrix column which represents a sudoku puzzle cell 
int sudoku::getCellConstraintColumn(int row, int col)
{
	return row * 9 + col;
}

int sudoku::getRowConstraintColumn(int row, int value)
{
	return row * 9 + value + 80;
}

int sudoku::getColConstraintColumn(int col, int value)
{
	return col * 9 + value + 161;
}

int sudoku::getBoxConstraintColumn(int row, int col, int value)
{
	return ((row / 3) * 3 + col / 3) * 9 + value + 242;
}

node* sudoku::sudokuECMCreate()
{
	node* dlxRoot = initRoot();
	node* dlxHead;
	node* last = NULL;
	int dlxMatrixRow = 0;

	for (int col = 0; col < 324; col++)
	{
		dlxHead = initHeader(dlxRoot, col);
		columns[col] = dlxHead;
	}

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


// Remove a row from the sudoku ECM 
void sudoku::disableRow(int rowIndex)
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

// Reuturn a row to the sudoku ECM
void sudoku::enableRow(int rowIndex)
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

//reenables row in ECM. It takes as an argument 
void sudoku::resetCell(int rowIndex)
{
	node* startNode = rows[rowIndex]; 
	for (node* currentNode = startNode; currentNode->rowNumber % 9 != 8; currentNode = currentNode->down)
	{
		do
		{
			currentNode->down = rows[currentNode->rowNumber];
			currentNode->up = currentNode->head->up;
			currentNode->down->up = currentNode;
			currentNode->up->down = currentNode;
			currentNode = currentNode->right;

		} while (currentNode != rows[rowIndex]);
	}
}

//TODO use set cell in linked list init
/*Parameters
*	cellNumber: 0-80;
*	value: 0-8;
*/
void sudoku::setCell(int cellNumber, int value)
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
void sudoku::sudokuECMInit()
{
	int dlxMatrixRow = 0;
	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 9; col++)
		{
			for (int value = 1; value <= 9; value++)
			{
				if (sudokuArray[row][col] > 0 && sudokuArray[row][col] != value)
				{
					disableRow(dlxMatrixRow);
				}
				dlxMatrixRow++;
			}
		}
	}
}


int sudoku::getValue(int linkedListRow)
{
	int value = (linkedListRow + 1) % 9;
	if (value == 0) value = 9;
	return value;
}

//Convert the exact cover matrix to a sudoku array
void sudoku::solutionToSudoku(vector <node*>* solution)
{
	int col;
	int row;
	for (int i = 0; i < solution->size(); i++)
	{
		col = ((*solution)[i]->rowNumber / 9) % 9;
		row = ((*solution)[i]->rowNumber / 9) / 9;
		solvedArray[row][col] = getValue((*solution)[i]->rowNumber);
	}

}


// *** random sudoku generation functions ***
void sudoku::setBox(int boxNumber, vector<int> values)
{
	int array[9] = { 0 };
	int arrayIndex = 0;
	// box number initial: 0, 3, 6, 27, 30, 33 
	int cellNumber = (boxNumber / 3) * 27 + (boxNumber % 3) * 3;
	int index = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			setCell(cellNumber, values[index] - 1);
			index++;
			cellNumber++;
		}
		cellNumber += 6;
	}

}

void sudoku::randomECMInit()
{
	int dlxMatrixRow = 0;
	vector<int> values = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	//Shuffle this array. This gives us a way to randomly select spaces to be cleared
	random_device rd;
	int boxeNumbers[2][3] = { {0, 4, 8}, {2, 4, 6} };

	// Seed the engine
	mt19937_64 generator(rd());

	int choice = getRandomInt(0, 1);

	for (int i = 0; i < 3; i++)
	{
		shuffle(values.begin(), values.end(), generator);
		setBox(boxeNumbers[choice][i], values);
	}
}

void sudoku::printSolutions()
{
	cout << "Printing sudoku solutions \n\n";
	if (solutions.size() == 1) printSolvedSudoku();
	else
	{
		for (int i = 0; i < solutions.size(); i++)
		{
			cout << "\n Sudoku Solution: " << i << "\n";
			solutionToSudoku(&solutions[i]);
			printSolvedSudoku();
		}
	}
}

bool sudoku::eraseCells(int index, int blankCells, int targetNum, vector<int>* cells)
{
	cout << "blank cells: " << blankCells << "\n"; 
	if (blankCells == targetNum)
	{
		cout << "Blank cells = target: " << targetNum << "\n";
		return true;
	}
	if (index == 81) return false; 

	int row = (*cells)[index] / 9; 
	int col = (*cells)[index] % 9;
	int value = sudokuArray[row][col];  
	sudokuArray[row][col] = 0; 
	
	blankCells++;
	index++; 
	while (index != 81)
	{
		sudokuECMInit();
		clearAllSolutions();
		root = sudokuECMCreate();
		sudokuECMInit();
		dlxSolve(root, 0);
		solutions = getAllSolutions();
		numberSolutions = solutions.size();
		if (numberSolutions > 1) return false; 
		if(eraseCells(index, blankCells, targetNum, cells)) return true;
		index++; 
	}
	sudokuArray[row][col] = value;
	return false; 
}

void sudoku::solveSudoku()
{
	//clear all solutions so no extraneous solutiosn get pushed to the vector
	clearAllSolutions();
	root = sudokuECMCreate();
	sudokuECMInit();
	dlxSolve(root, 0);
	solutions = getAllSolutions();
	numberSolutions = solutions.size();
	solutionToSudoku(&solutions[0]);
}

void sudoku::generateRandomSudoku(int targetNum)
{
	// generate solved sudoku; 
	cout << "Generating random solution\n";
	solutions.clear();
	clearAllSolutions();
	numberSolutions = NULL;
	root = sudokuECMCreate();
	randomECMInit();
	dlxGetRandomSolution(root, 0);
	solutions = getAllSolutions();
	solutionToSudoku(&solutions[0]);
	numberSolutions = solutions.size();

	vector<int> cells; 
	for (int i = 0; i < 81; i++)
	{
		cells.push_back(i);
	}

	//Shuffle this array. This gives us a way to randomly select spaces to be cleared
	random_device rd;

	// Seed the engine
	mt19937_64 generator(rd());

	shuffle(cells.begin(), cells.end(), generator);
	
	// generate empty sudoku
	copyArray(solvedArray); 
	sudokuECMInit(); 
	eraseCells(0, 0, targetNum, &cells); 
}