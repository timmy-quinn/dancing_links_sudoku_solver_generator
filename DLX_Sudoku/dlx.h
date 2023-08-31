#ifndef DLX_INCLUDE
#define DLX_INCLUDE

#include <vector>
#include <fstream>
#include <iostream>
using namespace std;

//TODO do all nodes need columnNumber, size, rowNumber?
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

//TODO is extern necessary?
extern node* initRoot();

extern node* initHeader(node* root, int number);

extern node* initNode(node* last, node* head, int rowNumber);


//TODO solution class

vector< vector< node* > > getAllSolutions();

/* Test for linked list */
//Prints an 7 by 7 array (for test)
void print7By7Array(int array[][7]);

node* arrayToDLXLinkedList(int array[7][7], int rowSize, int columnSize);

void convertDLX2Array(node* dlxRoot);

/* Dancing links algorithms */


void coverColumn(node* columnHeader);

void uncoverColumn(node* columnHeader);

void printSolution();

void printSolutionRows();

void testPrintSolution(node* root);

void testCoverUnCoverColumn(node* root);

void dlxSolve(node* root, int k);

#endif