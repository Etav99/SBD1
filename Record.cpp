#include "Record.h"
#include <cmath>



double Record::roots_sum(Record record) {
	double delta = (record.b * record.b) - (4 * record.a * record.c);
	return ((-record.b - sqrt(delta)) / (2 * record.a)) + ((-record.b + sqrt(delta)) / (2 * record.a));
}

bool Record::deltaCheck(Record record) {
	if ((record.b * record.b - 4 * record.a * record.c) >= 0)
		return true;
	return false;
}

bool Record::operator <(const Record& record) {
    if (roots_sum(*this) < roots_sum(record))
        return true;
    return false;
}
bool Record::operator >(const Record& record) {
    if (roots_sum(*this) > roots_sum(record))
        return true;
    return false;
}
bool Record::operator >=(const Record& record) {
    if (roots_sum(*this) >= roots_sum(record))
        return true;
    return false;
}

