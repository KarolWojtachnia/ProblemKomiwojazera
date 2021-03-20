#pragma once
// klasa do mierzenia czasu
#include <Windows.h>
using namespace std;

class CountTime
{
public:
	double PCFreq = 0.0;
	__int64 CounterStart = 0;
	double time_amount;

	void startCounter(); // rozpoczyna liczenie czasu
	void showTime(); // wyœwietla czas, jaki min¹³ od ostatniego startCounter
	double giveTime(); //zwraca czas, jaki min¹³ od ostatniego startCounter

};



