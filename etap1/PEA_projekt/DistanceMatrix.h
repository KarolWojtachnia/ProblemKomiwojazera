#pragma once
#include <string.h>
#include <string>
class DistanceMatrix
{
public:
	int** tab;
	int size, optimal;
	std::string name;


	DistanceMatrix();
	DistanceMatrix(int nodes);

	void generate(int nodes);
	void show();
	void addEdge(int x, int y, int dist);

	void loadIt(std::string fileName);
	void startBruteForce();
	void bruteForce(int start);
	void startBranchAndBound();
    void branchAndBound(int start, int current_bound);
	int	 minimalIn(int vertex);
	int  minimalOut(int vertex);
};


