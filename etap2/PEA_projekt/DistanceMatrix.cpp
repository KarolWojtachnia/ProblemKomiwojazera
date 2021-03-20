#include "DistanceMatrix.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include "countTime.h"

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

void DistanceMatrix::tabuSearch() {

    CountTime time;
    double maxTime = 60000.0;

    int** tabu;
    tabu = new int* [size - 1];
    for (int i = 0; i < size; i++)
        tabu[i] = new int[size - i - 1];
    for (int i = 0; i < size - 1; i++)
        for (int j = i + 1; j < size; j++)
            tabu[i][j - i - 1] = 0;

    int iteration = 1;
    double prd;

    vector<int> bestPath;
    int bestPathValue = 0;

    vector<int> currentPath;
    int currentPathValue = 0;

    vector<int> newPath;
    int newPathValue = 0;

    int stagnation = 0;

    currentPath = randomPath();
    currentPathValue = calculatePathValue(currentPath);
    bestPath = currentPath;
    bestPathValue = currentPathValue;
    prd = 100 * (double)bestPathValue / optimal;
    cout << "start " << bestPathValue << " " << prd << "%\n";

    time.startCounter();
    do {
        
        if (stagnation > 0) {
            currentPath = randomPath();
            currentPathValue = calculatePathValue(currentPath);
            if (currentPathValue < bestPathValue) {
                bestPath = currentPath;
                bestPathValue = currentPathValue;
                prd = 100 * (double)bestPathValue / optimal;
                cout << iteration << " " << bestPathValue << " " << prd << "%\n";
            }
            stagnation = 0;
        }
        
        int bestSwap1 = 0;
        int bestSwap2 = 0;
        for (int i = 0; i < size - 1; i++) {
            for (int j = i + 1; j < size; j++) {
                newPath = currentPath;
                int help = newPath[i];
                newPath[i] = newPath[j];
                newPath[j] = help;
                newPathValue = calculatePathValue(newPath);

                if (newPathValue < bestPathValue) {
                    bestSwap1 = i;
                    bestSwap2 = j;
                    bestPathValue = newPathValue;
                    bestPath = newPath;
                    prd = 100 * (double)bestPathValue / optimal;
                    cout << iteration << " " << bestPathValue << " " << prd << "%\n";
                }
                else if (newPathValue < currentPathValue && tabu[i][j - i - 1] == 0) {
                    bestSwap1 = i;
                    bestSwap2 = j;
                    currentPathValue = newPathValue;
                }
            }
        }
        if ((bestSwap1 | bestSwap2) != 0) {
            newPath = currentPath;
            int help = newPath[bestSwap1];
            newPath[bestSwap1] = newPath[bestSwap2];
            newPath[bestSwap2] = help;
            currentPath = newPath;
            currentPathValue = calculatePathValue(currentPath);
            tabu[bestSwap1][bestSwap2 - bestSwap1 - 1] = size*10;
            stagnation = 0;
        }
        else stagnation++;

        for (int i = 0; i < size - 1; i++)
            for (int j = i + 1; j < size; j++)
                if(tabu[i][j - i - 1] != 0)
                    tabu[i][j-i-1]--;

        iteration++;
    } while (time.giveTime() < maxTime);

    cout << bestPath[0];
    for (int i = 1; i < size; i++) {
        cout << "->" << bestPath[i];
    }
    cout << endl;

}



void DistanceMatrix::simulatedAnnealing() {

    srand(time(NULL));

    int iteration = 1;
    double prd;

    int eraLength=size/4;

    float temperature = 100;
    float minimalTemp = 0.0001;
    
    vector<int> bestPath;
    int bestPathValue = 0;
    
    vector<int> currentPath;
    int currentPathValue = 0;
    
    vector<int> newPath;
    int newPathValue = 0;
    
    int stagnation=0;
    int stagnationMax = 100 * size;
    
    currentPath = randomPath();
    currentPathValue = calculatePathValue(currentPath);
    bestPath = currentPath;
    bestPathValue = currentPathValue;
    prd = 100 * (double)bestPathValue / optimal;
    cout << "start " << bestPathValue << " " << prd << "%\n";

    while (temperature > minimalTemp) {
        
        for (int i = 0; i<eraLength;i++) {
            
            if (stagnation > stagnationMax) {
                currentPath = randomPath();
                currentPathValue = calculatePathValue(currentPath);
                if (currentPathValue < bestPathValue) {
                    bestPath = currentPath;
                    bestPathValue = currentPathValue;
                    prd = 100 * (double)bestPathValue / optimal;
                    cout << iteration << " " << bestPathValue << " " << prd << "%\n";
                }
                stagnation = 0;
            }

            int randomOne = rand() % size;
            int randomTwo = randomOne;
            while (randomOne == randomTwo)
                randomTwo = rand() % size;
            newPath = currentPath;
            int help = newPath[randomOne];
            newPath[randomOne] = newPath[randomTwo];
            newPath[randomTwo] = help;
            newPathValue = calculatePathValue(newPath);

            if (newPathValue < currentPathValue) {
                currentPath = newPath;
                currentPathValue = newPathValue;
                stagnation = 0;
                if (currentPathValue < bestPathValue) {
                    bestPath = currentPath;
                    bestPathValue = currentPathValue;
                    prd = 100 * (double)bestPathValue / optimal;
                    cout << iteration << " " << bestPathValue << " " << prd << "%\n";
                }
            }
            else if ((float)rand() / RAND_MAX < exp(-(newPathValue - currentPathValue) / temperature)) {
                currentPath = newPath;
                currentPathValue = newPathValue;
                stagnation = 0;
            }
            else stagnation++;

        }
        iteration++;
        temperature *= 0.99999;

    }

    cout << bestPath[0];
    for (int i = 1; i < size; i++) {
        cout << "->" << bestPath[i];
    }
    cout << endl;
    cout << iteration;



}

vector<int> DistanceMatrix::randomPath() {
    srand(time(NULL));
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

