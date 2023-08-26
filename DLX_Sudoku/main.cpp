#include <iostream>
#include <fstream>
#include <algorithm>
#include <array>
#include <random>



using namespace std;

/* Doubly linked list classes */

class node
{
	public: 
		node* up;
		node* down;
		node* left;
		node* right;
		header* head;
};

class header : public node
{
	public: 
		int size;
};

/* Linked list functions */

node* initRoot()
{
	node root;
	root.left = &root;
	root.right = &root;
	return &root;
}

node* initHeader(node* root)
{
	header head;
	head.right = root;
	head.left = root->left;
	head.right->left = &head;
	head.left->right = &head;
	head.size = 0;
	return &head;
}

node* initNode(node* last, header* head)
{
	node newnode;
	// *last doesn't work for some reason? not sure why
	if (last != NULL)
	{
		newnode.left = last;
		newnode.right = last->right;
		newnode.left->right = &newnode;
		newnode.right->left = &newnode;
	}
	else
	{
		newnode.left = &newnode;
		newnode.right = &newnode;
	}
	newnode.head = head;
	head->size++;
	newnode.down = head;
	newnode.up = head->up;
	newnode.up->down = &newnode;
	newnode.down->up = &newnode;
	return &newnode;
}

void insert(node* prevNode)
{
	
}

class Sudoku
{
	int matrix[9][9];
	array<int, 9> guess { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	array<int, 81> oneDSpaces;



public:

	void getSpaces();
	void shuffleGuesses();
	void createSudoku();
	bool checkValue(int row, int col, int value);

	void setMatrix(int grid[9][9]);

	void printSudoku();
	void initializeTestZero();
	void setvalue(int row, int col, int val);
	void importSudoku();
	bool theGreatSolver(int row, int col);
	void clearZeroes();
	void partClear(int blanks);
};

//Partially clears the values 
void Sudoku::partClear(int blanks)
{
	int col;
	int row;
	//Get a random list of which spaces to clear
	getSpaces();
	for (int i = 0; i < blanks; i++)
	{
		//Convert the one d array to 2 d coordinates
		col = oneDSpaces[i] % 9;
		row = oneDSpaces[i] / 9;

		//Set the value at those coordinates to zero
		setvalue(col, row, 0);
	}
}

//Fill the Sudoku matrix with the values from a given array
void Sudoku::setMatrix(int grid[9][9]) {
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			setvalue(i, j, grid[i][j]);

		}

	}
}


//Randomize the order in which numbers are guessed
void Sudoku::shuffleGuesses()
{
	unsigned num = time(NULL);
	shuffle(guess.begin(), guess.end(), default_random_engine(num));

}

//Get the spaces that will be cleared
void Sudoku::getSpaces()
{
	//Creat an array with 81 number in order
	for (int i = 0; i < 81; i++)
	{
		oneDSpaces[i] = i;

	}

	//Shuffle this array. This gives us a way to randomly select spaces to be cleared
	unsigned num = time(NULL);
	shuffle(oneDSpaces.begin(), oneDSpaces.end(), default_random_engine(num));

}

//Creates a randomized Sudokue puzzle
void Sudoku::createSudoku()
{


	//initialize an all zero Sudoku matrix
	initializeTestZero();

	int blanks;

	//Choose a random row and columnn in which to place a random initial value

	int row = rand() % 9;

	int col = rand() % 9;

	int value = 1 + rand() % 9;

	//Now you have a Sudoku puzzle with only one space filled. The next step is to solve it. 
	setvalue(row, col, value);


	//The user can choose how many blank spaces they have in their sudoku puzzle. This will affect the difficulty
	cout << "How many blank spaces would you like to have in your Sudoku puzzle?" << endl;
	cout << "(Please enter a number between 1 and 81)" << endl;
	cin >> blanks;

	//Shuffle the order in which numbers are guessed.
	//This helps to generate a more randomized Sudoku puzzle 
	shuffleGuesses();

	//Fill the empty spaces of the sudoku puzzle 
	theGreatSolver(0, 0);


	//Clear away as many spaces as the user desired. 
	partClear(blanks);
}


//Print the sudoku puzzle
void Sudoku::printSudoku() {
	cout << "Sudoku printing" << endl << endl;

	//Print top border
	cout << " -------+-------+-------" << endl;
	for (int x = 0; x < 9; x++) {
		cout << "| ";
		for (int y = 0; y < 9; y++) {
			if (matrix[x][y] > 0) {
				cout << matrix[x][y] << " ";
			}
			else {
				cout << "  ";
			}
			if (y == 2 || y == 5 || y == 8) {
				//print dividers between each box
				cout << "| ";
			}


		}
		//print dividers between each box
		cout << endl;
		if (x == 2 || x == 5 || x == 8) {
			cout << " -------+-------+-------" << endl;
		}
	}
}

//Initialize a blank sudoku puzzle. In other words, every space is filled with a zero
void Sudoku::initializeTestZero() {

	for (int x = 0; x < 9; x++) {
		for (int y = 0; y < 9; y++) {
			matrix[x][y] = 0;
		}
	}

}


//Sets the value of the Sudoku matrix at the given row and column
void Sudoku::setvalue(int row, int col, int val) {
	if (val >= 0) {
		matrix[row][col] = val;

	}
	else {
		cout << "Attempted to enter a value less than zero" << endl;
		cout << val << endl;
		exit(1);
	}
}

//Checks each row, column, and box to see if they already have an attempted value
//Returns true if it detects a preexisting number of the same value
bool Sudoku::checkValue(int row, int col, int value)
{
	//check column 
	for (int x = 0; x < 9; x++) {
		if (matrix[x][col] == value) {

			return false;
		}
	}
	//check row
	for (int x = 0; x < 9; x++) {
		if (matrix[row][x] == value) {

			return false;
		}
	}

	//Determine what the top left corner coordinates of the box are, from the given inputs
	int startRow;
	int startCol;
	startRow = row - row % 3;
	startCol = col - col % 3;

	//check the box against the attempted value
	for (row = startRow; row < startRow + 3; row++) {
		for (col = startCol; col < startCol + 3; col++) {
			if (matrix[row][col] == value) {

				return false;
			}

		}
	}

	return true;
}


//Imports a sudoku txt file
void Sudoku::importSudoku() {
	string fileName;
	ifstream inFile;
	cout << "Enter file name here" << endl;
	cin >> fileName;
	inFile.open(fileName);
	if (!inFile.is_open())
	{
		cout << "Error. File not imported" << endl;
		exit(1);
	}
	for (int row = 0; row < 9; row++) {
		for (int col = 0; col < 9; col++) {
			inFile >> matrix[row][col];
		}
	}

}


//Solves the sudoku
//(I do not know why I chose to call it theGreatSolver. It seemed fun at the time
bool Sudoku::theGreatSolver(int row, int col) {
	//check if it has passed the limits of the entire grid
	if (row == 8 && col == 9) {
		cout << endl << "Sudoku completed!" << endl;
		return true;
	}
	//check if it has passed the limits of the number of columns
	//if so, then move onto the next row and beginning column
	if (col == 9) {
		row++;
		col = 0;


	}

	//iterate through a number of guesses
	for (int guessNum = 0; guessNum <= 8; guessNum++) {
		//cout << "Row: " << row << " Col: " << col << " Guess: " << guess << endl;
		//check if the space is blank or not. If not blank, move on
		if (matrix[row][col] == 0) {

			//cout << "Guess is "<<guess[guessNum]<<endl;
			if (checkValue(row, col, guess[guessNum]))
			{

				setvalue(row, col, guess[guessNum]);
				if (theGreatSolver(row, col + 1)) {
					return true;
				}
			}


		}
		else
		{
			//if there is already a value higher than zero in the space 
			//move on to the next space
			return theGreatSolver(row, col + 1);
		}



		//if the guess did not work, make the space blank and continue iterating
		matrix[row][col] = 0;
	}
	//if solution does not work return false and go back

	return false;
}






int main()
{
	srand((unsigned)time(NULL));
	Sudoku a, b;


	//uncomment to 
	//initializes a grid that can be passed to the sudoku solver
	int grid[9][9] = { {3, 0, 6, 5, 0, 8, 4, 0, 0},
					   { 5, 2, 0, 0, 0, 0, 0, 0, 0 },
					   { 0, 8, 7, 0, 0, 0, 0, 3, 1 },
					   { 0, 0, 3, 0, 1, 0, 0, 8, 0 },
					   { 9, 0, 0, 8, 6, 3, 0, 0, 5 },
					   { 0, 5, 0, 0, 9, 0, 6, 0, 0 },
					   { 1, 3, 0, 0, 0, 0, 2, 5, 0 },
					   { 0, 0, 0, 0, 0, 0, 0, 7, 4 },
					   { 0, 0, 5, 2, 0, 6, 3, 0, 0 } };
	a.setMatrix(grid);


	// uncomment the code below to import sudoku from text file
	//a.importSudoku();

	a.printSudoku();


	a.theGreatSolver(0, 0);
	a.printSudoku();


	cout << endl << "A new sudoku puzzle is being created:" << endl;
	a.createSudoku();

	cout << endl << "Here is your custom sudoku puzzle: " << endl;
	a.printSudoku();

	cout << "Good luck solving it!" << endl;


	return 0;
}