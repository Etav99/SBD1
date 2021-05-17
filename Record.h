#pragma once
#include <iostream>
using namespace std;

struct Record {
	double a, b, c;

	static double roots_sum(Record record);
    static bool deltaCheck(Record record);
    bool operator <(const Record& record);
    bool operator >(const Record& record);
	bool operator >=(const Record& record);

};