// klasa do mierzenia czasu
#include "countTime.h"
#include <windows.h>
#include <iostream>

using namespace std;

void CountTime::startCounter() {
    LARGE_INTEGER li;
    if (!QueryPerformanceFrequency(&li))
        cout << "QueryPerformanceFrequency failed!\n";

    PCFreq = double(li.QuadPart) / 1000.0;

    QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;
}

void CountTime::showTime()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    time_amount = double(li.QuadPart - CounterStart) / PCFreq;

    cout << "\nTime (in ms): " << time_amount << endl << endl;
}

double CountTime::giveTime()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    time_amount = double(li.QuadPart - CounterStart) / PCFreq;
    return time_amount;
}