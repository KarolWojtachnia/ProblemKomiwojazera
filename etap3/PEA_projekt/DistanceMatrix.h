#pragma once
#include <string.h>
#include <string>
#include <vector>

using namespace std;
class DistanceMatrix
{
public:
	int** tab;
	int size, optimal;
	string name;

	DistanceMatrix();
	DistanceMatrix(int nodes);

	void generate(int nodes);
	void show();
	void addEdge(int x, int y, int dist);

	void loadIt(string fileName);

	void genethicAlgorithm();
	void makeNextGeneration(vector<vector<int>>* population, vector<double>* fitnessValues, double mutationProb, double crossProb);
	void fitnessToProbability(vector<double>* population);
	vector<int> selectRandomFromPopulation(vector<vector<int>>* population);
	vector<int> selectFromPopulationByFitness(vector<vector<int>>* population, vector<double>* probabilities);
	void crossoverOX(vector<int>* individual, vector<int>* individual2);
	bool ifVectorContains(int element, vector<int> elements);
	void transpositionMutation(vector<int>* individual);
	void inversionMutation(vector<int>* individual);
	vector<int> randomPath();
	int calculatePathValue(vector<int> path);


};