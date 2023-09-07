#include "random_int.h"

using namespace std;

int getRandomInt(int min, int max)
{
	// Initialize the random_device
	random_device rd;

	// Seed the engine
	mt19937_64 generator(rd());

	// Specify the range of numbers to generate, in this case [min, max]
	uniform_int_distribution<int> dist{min, max};

	return dist(generator);
}