#include <iostream>
#include <stdio.h>
#include "DistanceMatrix.h"

std::string fileName;

void showMenu(void) {
    std::cout << "\n\n----PROBLEM KOMIWOJAZERA----\n";
    std::cout << "   Wybierz odpowiednia opcje:\n";
    std::cout << "1. Wczytaj plik konfiguracyjny\n";
    std::cout << "2. Wyswietl macierz sasiedztwa\n";
    std::cout << "3. Oblicz dlugosc cyklu Hamiltona od pierwszego wierzcholka\n";
    std::cout << "4. Rozpocznij algorytm zachlanny\n";
    std::cout << "5. Rozpocznij brute force\n";
    std::cout << "0. Zamknij program\n";
}

int main(void)
{
    DistanceMatrix matrix;
    while (true) {
        showMenu();
        int wybor;
        std::cin >> wybor;
        switch (wybor) {
        case 1:
            std::cout << "Podaj sciezke do pliku\n";
            std::cin >> fileName;
            matrix.loadIt(fileName);
            std::cout << "Wczytano pomyslnie !\n";
            break;
        case 2:
            matrix.show();
            break;
        case 3:
            matrix.hamiltonianCycle();
            break;
        case 4:
            matrix.nearestNeighbourAlgorithm();
            break;
        case 5:
            matrix.startBruteForce();
            break;
        case 0:
            exit(0);
        }
    }
    return 0;
}



