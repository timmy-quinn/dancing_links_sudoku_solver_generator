#include "dlx.h"

using namespace std;


// solutions and functions
vector <node*> solution;
vector<vector <node*>> allSolutions;

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

void printSolution()
{
	cout << "*******************Printing solution*****************\n";
	for (int i = 0; i < solution.size(); i++)
	{
		cout << solution[i]->head->columnNumber;
		for (node* j = solution[i]->right; j != solution[i]; j = j->right)
		{
			cout << j->head->columnNumber;
		}
		cout << "\n";
	}
}

void printSolutionRows()
{
	cout << "***********************Printing Solution Rows**********************" << "\n";
	for (int i = 0; i < solution.size(); i++)
	{
		cout << solution[i]->rowNumber << "\n";
	}
}

void testPrintSolution(node* root)
{
	solution.push_back(root->right->down);
	printSolution();
}

void testCoverUnCoverColumn(node* root)
{
	node* column = root->right;
	coverColumn(column);
	convertDLX2Array(root);
	uncoverColumn(column);
	convertDLX2Array(root);
}

//TODO choose minimum coloumn
void dlxSolve(node* root, int k)
{
	if (root->right == root)
	{
		// printSolution();
		// printSolutionRows();
		allSolutions.push_back(solution);
		return;
	}

	node* columnHeader = root->right;
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

bool dlxGetOneSolution(node* root, int k)
{
	if (root->right == root)
	{
		allSolutions.push_back(solution);
		return true;
	}

	node* columnHeader = root->right;
	coverColumn(columnHeader);

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
}