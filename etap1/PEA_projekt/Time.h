#pragma once
// klasa do mierzenia czasu
#include <Windows.h>
using namespace std;

class Time
{
public:
	double PCFreq = 0.0;
	__int64 CounterStart = 0;
	double time_amount;

	void startCounter(); // rozpoczyna liczenie czasu
	void show(); // wy�wietla czas, jaki min�� od ostatniego startCounter
	double zwroc(); //zwraca czas, jaki min�� od ostatniego startCounter

};

