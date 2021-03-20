#include "DistanceMatrix.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
//zmienne do przegl¹du zupe³nego
int   vStart, path_length, temp_path_length, stos_pom_wsk;
int* path, *temp_path;      
bool* visited; 
unsigned long long int nr;
//

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

void DistanceMatrix::hamiltonianCycle() {
 
    int path = 0;
    for (int i = 1; i < size; i++)
        path += tab[i - 1][i];

    path += tab[size - 1][0];
    std::cout << "Dlugosc optymalnego cyklu Hamiltona:  " << optimal << std::endl;
    std::cout << "Dlugosc cyklu Hamiltona liczona po kolejnych wierzcholkach od 0: " << path << std::endl;
    if (path == optimal)
        std::cout << "D³ugosc zgodna z optymalna\n";
    else
        std::cout << "Dlugosc niezgodna z optymalna";


}

void DistanceMatrix::nearestNeighbourAlgorithm() {
    int remaining = size-1;
    int dist=maxValue;
    int path_length=0;
    int curr_vertex = 0;
    int to_erase=0;
    std::vector<int> vertices;
    std::string path;
    path.append("0->");
    for (int i = 1; i < size; i++) 
        vertices.push_back(i);
    while(remaining) {
        int curr_path_length = maxValue;
        for (int i = 0; i < remaining; i++) {
            dist = tab[curr_vertex][vertices[i]];
            if (dist<curr_path_length) {
                curr_path_length = dist;
                to_erase = i;
            }
        }
        curr_vertex = vertices[to_erase];
        path_length += curr_path_length;
        remaining--;
        vertices.erase(vertices.begin()+to_erase);
        path.append(std::to_string(curr_vertex) + "->");
    }
    path_length += tab[curr_vertex][0];
    path.append("0\n");
    std::cout << "\nDroga: " << path;
    int prd = 100 * path_length / optimal;
    std::cout << "\nDlugosc: " << path_length << "   PRD: " << prd <<"%"<< endl;

}

void DistanceMatrix::startBruteForce() {
    nr = 0;
    path = new int[size];
    temp_path = new int[size];
    visited = new bool[size];
    for (int i = 0; i < size; i++)
        visited[i] = false;
    stos_pom_wsk = 0;
    path_length = INT_MAX;
    temp_path_length = 0;
    vStart = 0;
    bruteForce(vStart);
    cout << endl<<nr<<endl;
    for (int i = 0; i < size; i++)
        cout << path[i] << "->";
    cout << 0<<endl;


}

void DistanceMatrix::bruteForce(int start){
    
    float prd;
    temp_path[stos_pom_wsk++] = start;
    if (stos_pom_wsk < size) {
        visited[start] = true;
        for (int i = size; i --;) 
            if (!visited[i]) {
                temp_path_length += tab[start][i];
                bruteForce(i);
                temp_path_length -= tab[start][i];
            }
        visited[start] = false;  
    }
    else {
        nr++;
        temp_path_length += tab[start][0];
        if (temp_path_length < path_length) {
            path_length = temp_path_length;
            prd = 100 * (float)path_length / optimal;
            cout <<endl<<nr<<' '<<path_length <<" (" << prd<<"%)";
            for (int k = size; k --;)
                path[k] = temp_path[k];
        }
        temp_path_length -= tab[start][0];
    }
    stos_pom_wsk--;
}



