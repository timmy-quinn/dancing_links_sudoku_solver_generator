#include <iostream>
#include <fstream>
#include <algorithm>
#include <array>
#include <random>
#include <vector>

using namespace std;

/* Doubly linked list classes */

class node
{
	public: 
		node* up;
		node* down;
		node* left;
		node* right;
		node* head;
		int rowNumber;
		int size;
		int columnNumber;
};

/* Linked list functions */

node* initRoot()
{
	node* root = new node();
	root->left = root;
	root->right = root;
	return root;
}

node* initHeader(node* root, int number)
{
	node* head = new node();
	head->right = root;
	head->left = root->left;
	head->right->left = head;
	head->left->right = head;
	head->size = 0;
	head->columnNumber = number;
	head->rowNumber = 0;
	head->up = head;
	head->down = head;
	cout << "[initHeader]: &head: " << &head << "\n";
	cout << "[initHeader]: root->right: " << root->right << "\n";
	cout << "[initHeader]: root->left: " << root->left << "\n";
	return head;
}

node* initNode(node* last, node* head, int rowNumber)
{
	node* newnode = new node();
	if (last != NULL)
	{
		newnode->left = last;
		newnode->right = last->right;
		newnode->left->right = newnode;
		newnode->right->left = newnode;
	}
	else
	{
		newnode->left = newnode;
		newnode->right = newnode;
	}
	newnode->head = head;
	cout << "Node head: " << newnode->head << "\n";
	head->size = head->size++;
	newnode->down = head;
	newnode->up = head->up;
	newnode->up->down = newnode;
	newnode->down->up = newnode;
	newnode->rowNumber = rowNumber;
	return newnode;
}


/* Test for linked list */
//Prints an 7 by 7 array (for test)
void print7By7Array(int array[][7])
{
	cout << "Array printing" << endl << endl;
	cout << " -------+-------+-------" << endl;
	for (int x = 0; x < 7; x++) {
		for (int y = 0; y < 7; y++) {
			cout << array[x][y];
		}
		//print dividers between each box
		cout << "\n";
	}
}

void printHeaderSizes(node* root)
{
	for (node* head = root->right; head != root; head = head->right)
	{
		cout << "Column: " << head->columnNumber << " Size: " << head->size << "\n";
	}
}


node* arrayToDLXLinkedList(int array[7][7], int rowSize, int columnSize)
{	

	node* dlxRoot = initRoot();
	node* dlxHead;
	node* last = NULL;

	for (int col = 0; col < 7; col++)
	{
		dlxHead = initHeader(dlxRoot, col);
		cout << "Header" << col << " initialized\n";
	}
	cout << "[arry2dlx]: dlxHead: " << dlxHead << "\n";
	dlxHead = dlxRoot->right;
	cout << "[arry2dlx]: dlxHead columnNum: " << dlxHead->columnNumber << "\n";
	cout << "[arry2dlx]: dlxHead columnNum: " << dlxHead->rowNumber << "\n";
	for (int row = 0; row < 7; row++)
	{
		for (int col = 0; col < 7; col++)
		{
			if (array[row][col] == 1)
			{
				last = initNode(last, dlxHead, row);
			}
			dlxHead = dlxHead->right;
			if (dlxHead == dlxRoot) dlxHead = dlxHead->right;
		}
		last = NULL;
		
	}

	printHeaderSizes(dlxRoot);

	return dlxRoot;	
}

void convertDLX2Array(node* dlxRoot)
{
	int array[7][7] = { 0 };
	for (node* columnHeader = dlxRoot->right; columnHeader != dlxRoot; columnHeader = columnHeader->right)
	{
		for (node* r = columnHeader->down; r != columnHeader; r = r->down)
		{
			array[r->rowNumber][r->head->columnNumber] = 1;
		}
	}
	print7By7Array(array);
}

/* Dancing links algorithms */

// solutions
vector <node*> solutions;

void coverColumn(struct node* columnHeader)
{	
	cout << "Covering column: " << columnHeader->columnNumber << "\n";
	cout << "Column size: " << columnHeader->size << "\n";
	columnHeader->right->left = columnHeader->left;
	columnHeader->left->right = columnHeader->right;

	for (node* i = columnHeader->down; i != columnHeader; i = i->down)
	{
		cout << "Node i: column: " << i->columnNumber << " row: " << i->rowNumber << "\n";
		for (node* j = i->right; j->right != i; j = j->right)
		{
			cout << "Node j: column: " << j->head->columnNumber << " row: " << j->rowNumber << "\n";
			j->down->up = j->up;
			j->up->down = j->down;
			j->head->size--;
		}
		cout << "end of j loop\n";

	}
}

void uncoverColumn(struct node* columnHeader)
{
	for (node* i = columnHeader->up; i != columnHeader; i = i->up)
	{
		for (node* j = i->left; j != i; j = j->left)
		{
			j->head->size++;
			j->down->up = j;
			j->down->up = j;
		}
	}
	columnHeader->right->left = columnHeader;
	columnHeader->left->right = columnHeader;
}

void printSolution()
{
	cout << "*******************Printing solution****************\n";
	for (int i = 0; i < solutions.size(); i++)
	{	
		cout << solutions[i]->head->columnNumber;
		for (node* j = solutions[i]->right; j != solutions[i]; j = j->right)
		{
			j->head->columnNumber;
		}
	}
	cout << "\n";
}

void dlxSolve(node* root, int k)
{
	if (root->right == root)
	{
		printSolution();
		return;
	}

	node* columnHeader = root->right;
	coverColumn(columnHeader);

	for (node* r = columnHeader->down; r != columnHeader; r = r->down)
	{
		solutions.push_back(r);//Add rownode to solution
		for (node* j = r->right; j != r; j = j->right)
		{
			coverColumn(j->head);
		}
		dlxSolve(root, k + 1);
		//r <- Ok: remove rownode from solution
		solutions.pop_back();
		columnHeader = r->head;
		for (node* j = r->left; j != r; j = j->left)
		{
			uncoverColumn(j->head);
		}

	}
	uncoverColumn(columnHeader);

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
	bool solveSudoku(int row, int col);
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
	solveSudoku(0, 0);


	//Clear away as many spaces as the user desired. 
	partClear(blanks);
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
bool Sudoku::solveSudoku(int row, int col) {
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
				if (solveSudoku(row, col + 1)) {
					return true;
				}
			}


		}
		else
		{
			//if there is already a value higher than zero in the space 
			//move on to the next space
			return solveSudoku(row, col + 1);
		}



		//if the guess did not work, make the space blank and continue iterating
		matrix[row][col] = 0;
	}
	//if solution does not work return false and go back

	return false;
}






int main()
{
	int array[7][7] =
	{
	{1, 1, 1, 1, 1, 1, 1},
	{0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 1, 1, 1, 0},
	{0, 1, 1, 1, 0, 1, 0}
	};

	node* root = arrayToDLXLinkedList(array, 7, 7);
	// convertDLX2Array(root);
	dlxSolve(root, 0);

	return 0;
}