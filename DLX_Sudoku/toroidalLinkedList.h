#ifndef TOROIDAL_LINKED_LIST
#define TOROIDAL_LINKED_LIST

#include "iostream"

using namespace std;


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

node* initRoot();

node* initHeader(node* root, int number);

node* initNode(node* last, node* head, int rowNumber);


/* Test for linked list */
//Prints an 7 by 7 array (for test)
void print7By7Array(int array[][7]);

void printHeaderSizes(node* root);

node* arrayToDLXLinkedList(int array[7][7], int rowSize, int columnSize);

void convertDLX2Array(node* dlxRoot);
#endif

