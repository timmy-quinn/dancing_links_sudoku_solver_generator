#include "toroidalLinkedList.h"
#include "iostream"

using namespace std;


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
	for (int x = 0; x < 7; x++)
	{
		for (int y = 0; y < 7; y++)
		{
			cout << array[x][y];
		}
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

