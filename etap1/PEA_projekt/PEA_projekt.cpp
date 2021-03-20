#include <iostream>
#include <stdio.h>
#include "DistanceMatrix.h"


using namespace std;
string fileName;

void showMenu(void) {
    cout << "\n\n----PROBLEM KOMIWOJAZERA----\n";
    cout << "   Wybierz odpowiednia opcje:\n";
    cout << "1. Wczytaj plik konfiguracyjny\n";
    cout << "2. Wyswietl macierz sasiedztwa\n";
    cout << "3. Rozpocznij brute force\n";
    cout << "4. Rozpocznij branch and bound\n";
    cout << "0. Zamknij program\n";
}

int main(void)
{
    DistanceMatrix matrix;
    while (true) {
        showMenu();
        int wybor;
        cin >> wybor;
        switch (wybor) {
        case 1:
            cout << "Podaj sciezke do pliku\n";
            cin >> fileName;
            matrix.loadIt(fileName);
            cout << "Wczytano pomyslnie !\n";
            break;
        case 2:
            matrix.show();
            break;
        case 3:
            matrix.startBruteForce();
            break;
        case 4:
            matrix.startBranchAndBound();
            break;
        case 0:
            exit(0);
        }
    }
    return 0;
}




