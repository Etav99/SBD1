#pragma once
#include "Reader.h"
#include "Writer.h"
#include "string.h"
#include <iostream>

using namespace std;

struct Tape {
	int* readNo;
	int* writeNo;
	string filename;
	Reader* reader;
	Writer* writer;
	int runs;
	int dummyRuns;
	int index, writeIndex;
	bool endOfRun, endOfTape;
	Record previousRecord, currentRecord;


	Tape(string filename, int bufferSize);
	Tape(string filename, int bufferSize, int* readNo, int* writeNo);
	void startReading();
	void nextRecord();
	void newRun();
	void addRecord(Record record);
	void finishWriting();
	void printTape();
	void clear();
	void switchAndDelete(Tape* tape);

	~Tape();
};