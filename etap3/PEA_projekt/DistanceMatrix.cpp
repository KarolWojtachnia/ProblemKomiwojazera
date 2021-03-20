#include "DistanceMatrix.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include "countTime.h"
#include <algorithm>

using namespace std;

//sta³a oznaczaj¹ca brak krawêdzi
//const int maxValue = (int)1e9 + 7;
const int maxValue = INT_MAX;

// konstruktor domyslny
DistanceMatrix::DistanceMatrix() {}

// ustawia wszystkie atrybuty pod nowy graf
void DistanceMatrix::generate(int nodes) {
    tab = new int* [nodes];
    for (int i = 0; i < nodes; i++)
        tab[i] = new int[nodes];
    for (int i = 0; i < nodes; i++)
        for (int j = 0; j < nodes; j++)
            tab[i][j] = maxValue;
}

// konstruktor ktoremu podajemy liczbe wierzcholkow grafu
DistanceMatrix::DistanceMatrix(int nodes) {
    size = nodes;
    generate(nodes);
}

// metoda ktora pokazuje graf
void DistanceMatrix::show() {
    cout << "### MATRIX REPRESENTATION ###\n     ";
    for (int i = 0; i < size; i++) 
        printf("%4d ", i);
    std::cout << endl<<endl;
    for (int i = 0; i < size; i++) {
        printf("%3d  ", i);
        for (int j = 0; j < size; j++) {
            printf("%4d ", tab[i][j]);

        }
        cout << endl;
    }

}

// metoda dodajaca wierzcholek do grafu
void DistanceMatrix::addEdge(int x, int y, int dist) {
    tab[x][y] = dist;
}


// metoda wczytujaca graf 
void DistanceMatrix::loadIt(string fileName) {
    fstream f;
    f.open(fileName, ios::in);
    if (!f.good()) {
        cout << "nie mozna otworzyc pliku!" << endl;
        exit(1);
    }

    f >> name;
    f >> size;

    generate(size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
        int d;
        f >> d;
        addEdge(i, j, d);
    }
    }
    f >> optimal;
    f.close();
}

void DistanceMatrix::genethicAlgorithm() {
    
    
    srand(time(NULL));
    int iteration = 1;
    CountTime countTime;
    double maxTime = 60000.0;
    double mutationProbability = 0.5;
    double crossoverProbability = 0.85;
    int populationSize =  200
        ;
    vector <vector<int>> population;
    vector <double> populationFitnessValues;

    vector <int> bestPath;
    int bestPathValue = maxValue;
    double prd;

    //vector <int> bestInPopulation;
    //int bestInPopulationValue=maxValue;
    //double prdPopulation;


    countTime.startCounter();
    for (int i = 0; i < populationSize; i++) {   
        population.push_back(randomPath());
        int pathLength = calculatePathValue(population[i]);  
        if (pathLength < bestPathValue) {
            bestPathValue = pathLength;
            bestPath = population[i];
        }
        populationFitnessValues.push_back(1. / pathLength);
        
    }

    prd = 100 * (double)bestPathValue / optimal;
    cout << iteration << " " << bestPathValue << " " << prd << "%\n";


    while (countTime.giveTime() < maxTime) {
        makeNextGeneration(&population,&populationFitnessValues, mutationProbability, crossoverProbability);
        vector<double> newFitness;
        for (int i = 0; i < populationSize; i++) {
            int pathLength = calculatePathValue(population[i]);
            if (pathLength < bestPathValue) {
                bestPathValue = pathLength;
                bestPath = population[i];
                prd = 100 * (double)bestPathValue / optimal;
                cout << iteration << " " << bestPathValue << " " << prd << "%\n";
            }
            //if (pathLength < bestInPopulationValue) {
             //   bestInPopulation = population[i];
             //   bestInPopulationValue = pathLength;
              //  prdPopulation = 100 * (double)bestPathValue / optimal;
            
            //}
            newFitness.push_back(1. / pathLength);
        }
        //cout << "best in population number: " << iteration << " " << bestPathValue << " " << prd << "%\n";
        populationFitnessValues = newFitness;
        //fitnessToProbability(&populationFitnessValues);
        //bestInPopulationValue = maxValue;
        iteration++;
    }
    
    cout << endl;
    cout << bestPath[0];
    for (int i = 1; i < size; i++) {
        cout << "->" << bestPath[i];
    }
    cout << endl;

}


void DistanceMatrix:: makeNextGeneration(vector<vector<int>>* population, vector<double>* fitnessValues, double mutationProb, double crossProb) {

    vector<vector<int>> nextPopulation;
    int tenPercent = ceil((*population).size() / 10);
    vector<pair<double, int>> help;
    for (int i = 0; i < (*fitnessValues).size(); i++) {
        help.push_back(make_pair((*fitnessValues)[i], i));
    }
    sort(help.rbegin(), help.rend());
    for (int i = 0; i < tenPercent; i++) {
        nextPopulation.push_back((*population)[help[i].second]);
    }
    for (int i = tenPercent; i < (*population).size(); i++) {
        vector<int> newIndividual = selectRandomFromPopulation(population);
        double random = (double)rand() / RAND_MAX;
        if (random < crossProb) {
            vector<int> newIndividual2 = selectRandomFromPopulation(population);
            crossoverOX(&newIndividual, &newIndividual2);
        }
        double random2 = (double)rand() / RAND_MAX;
        if (random2 < mutationProb)
            inversionMutation(&newIndividual);
        nextPopulation.push_back(newIndividual);
    }
    
    *population = nextPopulation;


}

void DistanceMatrix::fitnessToProbability(vector<double>* fitnessValues) {
    
    double sum = 0;
    for (int i = 0; i < (*fitnessValues).size(); i++) {
        sum += (*fitnessValues)[i];
    }
    for (int i = 0; i < (*fitnessValues).size(); i++) {
        (*fitnessValues)[i]/= sum;
    }
}

vector<int> DistanceMatrix::selectRandomFromPopulation(vector<vector<int>>* population) {

    int random = rand() % (*population).size();
    return (*population)[random];

}

//below requires calling of DistanceMatrix::fitnessToProbability() before calling
vector<int> DistanceMatrix::selectFromPopulationByFitness(vector<vector<int>>* population, vector<double>* probabilities) {
    
    int number = 0;
    double random = (double)rand() / RAND_MAX;

    while (random >= 0) {
        if (number == (*population).size()) {
            break;
        }
        random -= (*probabilities)[number];
        number++;
    }
    
    number--;
    return (*population)[number];

}

void DistanceMatrix::crossoverOX(vector<int>* individual, vector<int>* individual2) {

    int one = rand() % (*individual).size();
    int two;
    do {
        two = rand() % (*individual).size();
    } while (two == one);
    if (one > two) {
        int temp = two;
        two = one;
        one = temp;
    }

    vector<int> child1;
    vector<int> child2;
    for (int i = 0; i < (*individual).size(); i++) {
        if (i >= one && i <= two) {
            child1.push_back((*individual2)[i]);
            child2.push_back((*individual)[i]);
        }
        else {
            child1.push_back(-1);
            child2.push_back(-1);
        }
    }

    
    int remaining1 = (*individual).size() - two + one - 1;
    int remaining2 = (*individual).size() - two + one - 1;
    int parentIndex = two + 1;
    int child1Index = two + 1;
    int child2Index = two + 1;
    if (parentIndex == (*individual).size()) {
        parentIndex = 0;
        child1Index = 0;
        child2Index = 0;
    }
    
    while (remaining1 || remaining2) {

        if (remaining1) {
            if (!ifVectorContains((*individual)[parentIndex], child1)) {
                child1[child1Index] = (*individual)[parentIndex];
                if (child1Index == (*individual).size() - 1)
                    child1Index = 0;
                else child1Index++;
                remaining1--;
            }
        }
        if (remaining2) {
            if (!ifVectorContains((*individual2)[parentIndex], child2)) {
                child2[child2Index] = (*individual2)[parentIndex];
                remaining2--;
                if (child2Index == (*individual).size() - 1)
                    child2Index = 0;
                else child2Index++;
            }
        }

        if (parentIndex == (*individual).size()-1)
            parentIndex = 0;
        else parentIndex++;
        


    }
    
    if (calculatePathValue(child1) > calculatePathValue(child2))
        (*individual) = child1;
    else (*individual) = child2;
    
}
bool DistanceMatrix::ifVectorContains(int element, vector<int> elements) {
    for (int i = 0; i < elements.size(); i++) {
        if (elements[i] == element)
            return true;
    }
    return false;
}

void DistanceMatrix::transpositionMutation(vector<int>* individual) {
    int one = rand() % (*individual).size();
    int two;
    do {
        two = rand() % (*individual).size();
    } while (two == one);
    int help = (*individual)[one];
    (*individual)[one] = (*individual)[two];
    (*individual)[two] = help;
}

void DistanceMatrix::inversionMutation(vector<int>* individual) {
    int one = rand() % (*individual).size();
    int two;
    do {
        two = rand() % (*individual).size();
    } while (two == one);
    if (one > two) {
        int temp = two;
        two = one;
        one = temp;
    }

    while (one < two) {
        int temp = (*individual)[two];
        (*individual)[two] = (*individual)[one];
        (*individual)[one] = temp;
        one++;
        two--;
    }

}

vector<int> DistanceMatrix::randomPath() {

    int remaining = size;
    vector<int> verticies;
    vector<int> randomRoute;
    for (int i = 0; i < remaining;i++)
        verticies.push_back(i);
  
    while (remaining) {
        int randomNumber = rand() % remaining;
        randomRoute.push_back(verticies[randomNumber]);
        verticies.erase(verticies.begin()+randomNumber);
        remaining--;
    }
    return randomRoute;
}

int DistanceMatrix::calculatePathValue(vector<int> path) {
    
    int pathValue = 0;
    for (int i = 0; i < size - 1; i++) {
        pathValue += tab[path[i]][path[i + 1]];
    }
    pathValue += tab[path[size - 1]][path[0]] ;
    return pathValue;
}

