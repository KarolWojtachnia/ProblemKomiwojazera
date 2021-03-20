#include "DistanceMatrix.h"
#include "Time.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
//zmienne do przegl¹du zupe³nego
int   vStart, path_length, temp_path_length, temp_path_counter;
int* path, *temp_path;      
bool* visited; 


//sta³a oznaczaj¹ca brak krawêdzi
//const int maxValue = (int)1e9 + 7;
const int maxValue = INT_MAX;

// konstruktor domyslny
DistanceMatrix::DistanceMatrix() {}

struct Node {
    int node_nr;
    int node_bound;
};




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


void DistanceMatrix::startBruteForce() {
    Time time;
        path = new int[size];
        temp_path = new int[size];
        visited = new bool[size];
        for (int i = 0; i < size; i++)
            visited[i] = false;
        temp_path_counter = 0;
        path_length = INT_MAX;
        temp_path_length = 0;
        vStart = 0;
        time.startCounter();
        bruteForce(vStart);
        time.show();
    for (int i = 0; i < size; i++)
        cout << path[i] << "->";
    cout << 0<<endl;


}

void DistanceMatrix::bruteForce(int start){
    
    float prd;
    temp_path[temp_path_counter++] = start;
    if (temp_path_counter < size) {
        visited[start] = true;
        for (int i = size; i --;) 
            if (!visited[i]) {
                temp_path_length += tab[start][i];
                bruteForce(i);
                temp_path_length -= tab[start][i];
            }
        visited[start] = false;  
        temp_path_counter--;
        return;
    }

    temp_path_length += tab[start][0];
    if (temp_path_length < path_length) {
        path_length = temp_path_length;
        prd = 100 * (float)path_length / optimal;
        cout <<endl<<' '<<path_length <<" (" << prd<<"%)";
        for (int k = size; k --;)
            path[k] = temp_path[k];
    }
    temp_path_length -= tab[start][0];
    temp_path_counter--;
}

void DistanceMatrix::startBranchAndBound() {
   
        temp_path = new int[size];
        path = new int[size];
        visited = new bool[size];
        for (int i = 0; i < size; i++)
            visited[i] = false;
        temp_path_counter = 0;
        path_length = INT_MAX;
        temp_path_length = 0;
        vStart = 0;
        Time time;
        time.startCounter();
        int temp_bound = 0;
        for (int i = 0; i < size; i++) {
            temp_bound += minimalIn(i);
            temp_bound += minimalOut(i);
        }
        if (temp_bound % 2 == 0)
            temp_bound /= 2;
        else {
            temp_bound /= 2;
            temp_bound++;
        }
        branchAndBound(vStart, temp_bound);
        time.show();

    for (int i = 0; i < size; i++)
        cout << path[i] << "->";
    cout << 0 << endl;
}


void DistanceMatrix::branchAndBound(int start, int current_bound) {

    temp_path[temp_path_counter++] = start;
    if (temp_path_counter < size){
        visited[start]=true;
        for (int i=0; i < size; i++) {
            if (!visited[i]) {
                int t = current_bound;
                temp_path_length += tab[start][i];
                int reduce = minimalOut(start) + minimalIn(i);
                if (reduce % 2 == 1) reduce /= 2;
                else {
                    reduce /= 2;
                reduce++;
                }
                current_bound -= reduce;
                if (temp_path_length + current_bound < path_length) {
                    branchAndBound(i, current_bound);
                }
                temp_path_length -= tab[start][i];
                current_bound = t;


            }

        }
        visited[start] = false;
        temp_path_counter--;
        return;
    }

    float prd;
    temp_path_length += tab[start][0];
    if (temp_path_length < path_length) {
        path_length = temp_path_length;
        prd = 100 * (float)path_length / optimal;
        cout << endl << ' ' << path_length << " (" << prd << "%)";
        for (int k = size; k--;)
            path[k] = temp_path[k];
    }
    temp_path_length -= tab[start][0];
    temp_path_counter--;
  }

int DistanceMatrix::minimalOut(int vertex){
    int min = INT_MAX;
    for (int i = 0; i<size;i++) {
        if ( i != vertex && tab[vertex][i] < min) {
            min = tab[vertex][i];
        }
    }
    return min;
}

int DistanceMatrix::minimalIn(int vertex) {
    int min = INT_MAX;
    for (int i = 0; i<size;i++) {
        if ( i != vertex && (tab[i][vertex] < min)) {
            min = tab[i][vertex];
        }
    }
    return min;
}

