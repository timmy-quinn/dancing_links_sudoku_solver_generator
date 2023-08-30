#include "sudoku.h"
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

void sudoku::importSudoku()
{
		string fileName;
		ifstream inFile;
		cout << "Enter file name here" << endl;
		cin >> fileName;
		inFile.open(fileName);
		if (!inFile.is_open())
		{
			cout << "Error. File not imported" << endl;
			return;
		}
		for (int row = 0; row < 9; row++) 
		{
			for (int col = 0; col < 9; col++) {
				inFile >> sudokuArray[row][col];
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
	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 9; col++)
		{
			cout << sudokuArray[row][col];
		}
		cout << "\n";
	}

}