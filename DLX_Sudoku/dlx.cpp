#include "dlx.h"

using namespace std;

// solutions and functions
vector <node*> solution;
vector <vector <node*>> allSolutions;
vector <node*> columns;
vector <node*> columnsRandom;
unordered_map <int, node*> map; 
vector<int> freeColumns; 

int freeColumnsSize()
{
	return freeColumns.size();
}

vector<vector<node*>> getAllSolutions()
{
	return allSolutions;
}

/* Linked list functions */

extern node* initRoot()
{
	node* root = new node();
	root->left = root;
	root->right = root;
	return root;
}

extern node* initHeader(node* root, int number)
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
	columns.push_back(head);
	freeColumns.push_back(1);
	return head;
}

extern node* initNode(node* last, node* head, int rowNumber)
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
	cout << " -----------------" << endl;
	for (int x = 0; x < 7; x++)
	{
		for (int y = 0; y < 7; y++)
		{
			cout << array[x][y];
		}
		cout << "\n";
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
	}
	dlxHead = dlxRoot->right;
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


void coverColumn(node* columnHeader)
{
	columnHeader->right->left = columnHeader->left;
	columnHeader->left->right = columnHeader->right;

	for (node* i = columnHeader->down; i != columnHeader; i = i->down)
	{
		for (node* j = i->right; j != i; j = j->right)
		{
			j->down->up = j->up;
			j->up->down = j->down;
			j->head->size--;
		}

	}
}

void uncoverColumn(node* columnHeader)
{
	for (node* i = columnHeader->up; i != columnHeader; i = i->up)
	{
		for (node* j = i->left; j != i; j = j->left)
		{
			j->head->size = j->head->size + 1;
			j->down->up = j;
			j->up->down = j;
		}
	}
	columnHeader->right->left = columnHeader;
	columnHeader->left->right = columnHeader;
}

//void printSolution()
//{
//	cout << "*******************Printing solution*****************\n";
//	for (int i = 0; i < solution.size(); i++)
//	{
//		cout << solution[i]->head->columnNumber;
//		for (node* j = solution[i]->right; j != solution[i]; j = j->right)
//		{
//			cout << j->head->columnNumber;
//		}
//		cout << "\n";
//	}
//}

void printSolutionRows()
{
	cout << "***********************Printing Solution Rows**********************" << "\n";
	for (int i = 0; i < solution.size(); i++)
	{
		cout << solution[i]->rowNumber << "\n";
	}
}
//
//void testPrintSolution(node* root)
//{
//	solution.push_back(root->right->down);
//	printSolution();
//}

void testCoverUnCoverColumn(node* root)
{
	node* column = root->right;
	coverColumn(column);
	convertDLX2Array(root);
	uncoverColumn(column);
	convertDLX2Array(root);
}

void printColumnNumbers()
{
	for (int i = 0; i < columns.size(); i++)
	{
		cout << columns[i]->columnNumber << "\n";
	}
}
void randomizeColumnOrder()
{
	// Initialize the random_device
	random_device rd;

	// Seed the engine
	mt19937_64 generator(rd());
	
	columnsRandom = columns;

	shuffle(columnsRandom.begin(), columnsRandom.end(), generator);
}

node* getShortestColumn(node* root)
{
	node* shortestCol = root->right; 
	for (node* r = root->right; r != root; r = r->right)
	{
		if (r->size < shortestCol->size) shortestCol = r; 
	}
	return shortestCol; 
}

node* getRandomUncoveredColumn(node * root)
{
	// Initialize the random_device
	random_device rd;

	// Seed the engine
	mt19937_64 generator(rd());
	vector<int> tempColumns = freeColumns;
	tempColumns.resize(81); 
	

	// Specify the weighted value of numbers to generate
	//discrete_distribution<> dist{freeColumns.begin(), freeColumns.end()};
	discrete_distribution<> dist{freeColumns.begin(), freeColumns.end() };

	int index = dist(generator); 
	// cout << "Chosen: " << index << "\n";
	return columns[index];
}

void printRandomEvens()
{
	vector <int> evens; 
	for (int i = 0; i < 30; i++)
	{
		if (i % 2 == 0)
		{
			evens.push_back(1);
		}
		else
		{
			evens.push_back(0);
		}
	}

	// Initialize the random_device
	random_device rd;

	// Seed the engine
	mt19937_64 generator(rd());

	// Specify the weighted value of numbers to generate
	discrete_distribution<> dist{evens.begin(), evens.end()};

	int index = dist(generator);

	// cout << "Random index: " << index << "\n";
	cout << "Printing random numbers: \n";
	for (int i = 0; i < 10; i++)
	{
		cout << "random: " << dist(generator) << "\n";
	}
}

//TODO choose minimum coloumn
void dlxSolve(node* root, int k)
{
	if (root->right == root)
	{
		allSolutions.push_back(solution);
		cout << " *****************************************Solution found*********************************************\n";
		return;
	}

	node* columnHeader = getShortestColumn(root);
	if (columnHeader->columnNumber == 80) cout << "column 80 \n";
	if (columnHeader->columnNumber > 80) cout << "column: " << columnHeader->columnNumber << " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
	// cout << "chosen: " << columnHeader->columnNumber << "\n";
	coverColumn(columnHeader);

	for (node* r = columnHeader->down; r != columnHeader; r = r->down)
	{
		solution.push_back(r);//Add rownode to solution
		for (node* j = r->right; j != r; j = j->right)
		{
			coverColumn(j->head);
		}
		dlxSolve(root, k + 1);
		//r <- Ok: remove rownode from solution
		solution.pop_back();
		columnHeader = r->head;
		for (node* j = r->left; j != r; j = j->left)
		{
			uncoverColumn(j->head);
		}

	}
	uncoverColumn(columnHeader);
}

void dlxGetOneSolution(node* root, int k)
{
	if (root->right == root)
	{
		allSolutions.push_back(solution);
		return;
	}

	node* columnHeader = getShortestColumn(root);
	coverColumn(columnHeader);
	// cout << "column: " << columnHeader->columnNumber << "\n";

	for (node* r = columnHeader->down; r != columnHeader && allSolutions.size() < 1; r = r->down)
	{
		solution.push_back(r);//Add rownode to solution
		for (node* j = r->right; j != r; j = j->right)
		{
			coverColumn(j->head);
		}
		dlxGetOneSolution(root, k + 1);
		//r <- Ok: remove rownode from solution
		solution.pop_back();
		columnHeader = r->head;
		for (node* j = r->left; j != r; j = j->left)
		{
			uncoverColumn(j->head);
		}

	}
	uncoverColumn(columnHeader);
	// cout << "*************** dead end backtracking: *******************" << "\n";
}


//This may only work for sudoku ECM
void dlxGetRandomSolution(node* root, int k)
{
	if (root->right == root)
	{
		cout << "******************************one solution found**********************\n";
		printSolutionRows();
		allSolutions.push_back(solution);
		return;
	}

	node* columnHeader = getShortestColumn(root);
	// cout << " Got column number: " << columnHeader->columnNumber << "\n";
	// cout << "Got column header: " << columnHeader->columnNumber << "\n";
	coverColumn(columnHeader);
	freeColumns[columnHeader->columnNumber] = 0;

	for (node* r = columnHeader->down; r != columnHeader && allSolutions.size() < 1; r = r->down)
	{
		solution.push_back(r);		//Add rownode to solution
		for (node* j = r->right; j != r; j = j->right)
		{
			coverColumn(j->head);
			freeColumns[j->head->columnNumber] = 0;
		}
		dlxGetRandomSolution(root, k + 1);
		//r <- Ok: remove rownode from solution
		solution.pop_back();
		columnHeader = r->head;
		for (node* j = r->left; j != r; j = j->left)
		{
			uncoverColumn(j->head);
			freeColumns[j->head->columnNumber] = 1;
		}

	}
	uncoverColumn(columnHeader);
	freeColumns[columnHeader->columnNumber] = 1; 

	/*for (int i = 0; i < freeColumns.size(); i++)
	{
		cout << " " << freeColumns[i] << " "; 
	}
	cout << " \n";*/
}