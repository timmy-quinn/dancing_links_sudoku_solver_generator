#ifndef DLX_INCLUDE
#define DLX_INCLUDE

#include <vector>
#include <fstream>
#include <iostream>
#include <random>
#include <unordered_set>
#include <unordered_map>
#include "random_int.h"

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


//TODO add solution class or struct

vector< vector< node* > > getAllSolutions();

void clearAllSolutions();

void coverColumn(node* columnHeader);

void uncoverColumn(node* columnHeader);

void printSolutionRows();

void printRandomEvens();

void dlxSolve(node* root, int k);

void dlxGetOneSolution(node* root, int k);

void dlxGetRandomSolution(node* root, int k);

#endif