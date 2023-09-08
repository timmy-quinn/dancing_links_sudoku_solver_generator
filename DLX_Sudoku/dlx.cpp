#include "dlx.h"

using namespace std;

// solutions vectors and functions
vector <node*> solution;
vector <vector <node*>> allSolutions;

vector<vector<node*>> getAllSolutions()
{
	return allSolutions;
}
void clearAllSolutions()
{
	allSolutions.clear(); 
}


/* ****Linked list functions*** */

//*** initialization functions ***

//Initialize the ECM root
extern node* initRoot()
{
	node* root = new node();
	root->left = root;
	root->right = root;
	return root;
}

//Initialize headers
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

//Initialize each node
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

void printSolutionRows()
{
	cout << "***********************Printing Solution Rows**********************" << "\n";
	for (int i = 0; i < solution.size(); i++)
	{
		cout << solution[i]->rowNumber << "\n";
	}
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

node* getRandomShortestColumn(node* root)
{
	vector<node*> shortestCols; 
	shortestCols.push_back(root->right); 
	for (node* c = root->right; c != root; c = c->right)
	{
		if (c->size == shortestCols[0]->size) shortestCols.push_back(c);
		else if (c->size < shortestCols[0]->size)
		{
			shortestCols.clear(); 
			shortestCols.push_back(c);
		}
	}

	return shortestCols[getRandomInt(0, shortestCols.size() - 1)];
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
		return;
	}

	node* columnHeader = getShortestColumn(root);
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
		allSolutions.push_back(solution);
		return;
	}

	node* columnHeader = getRandomShortestColumn(root);
	// cout << " Got column number: " << columnHeader->columnNumber << "\n";
	// cout << "Got column header: " << columnHeader->columnNumber << "\n";
	coverColumn(columnHeader);

	for (node* r = columnHeader->down; r != columnHeader && allSolutions.size() < 1; r = r->down)
	{
		solution.push_back(r);		//Add rownode to solution
		for (node* j = r->right; j != r; j = j->right)
		{
			coverColumn(j->head);
		}
		dlxGetRandomSolution(root, k + 1);
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