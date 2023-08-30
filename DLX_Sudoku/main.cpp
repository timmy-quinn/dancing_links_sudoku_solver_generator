#include "dlx.h"

using namespace std;

int main()
{
	int array[7][7] =
	{
	{0, 0, 0, 0, 1, 1, 1},
	{1, 0, 1, 0, 1, 1, 1},
	{0, 1, 0, 1, 0, 0, 0},
	{1, 0, 1, 0, 0, 0, 0},
	{0, 0, 0, 1, 1, 1, 1},
	{1, 1, 1, 0, 0, 0, 0},
	{0, 0, 0, 1, 1, 1, 1}
	};

	node* root = arrayToDLXLinkedList(array, 7, 7);
	convertDLX2Array(root);
	//testCoverUnCoverColumn(root);
	// testPrintSolution(root);
	dlxSolve(root, 0);
	//convertDLX2Array(root);


	return 0;
}