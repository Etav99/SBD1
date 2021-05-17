#pragma once
#include "Record.h"
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

struct Writer {
	string fileName;
	int position;
	int bufferSize;
	Record* buffer;
	int* writeNo;

	Writer(string fileName, int bufferSize);
	Writer(string fileName, int bufferSize, int* writeNo);
	bool writeBlock();
	void writeRecord(Record record);
	void clearBuffer();
	~Writer();
};