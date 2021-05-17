#include "Tape.h"


Tape::Tape(string filename, int bufferSize) : filename(filename), reader(new Reader(filename, bufferSize)),
writer(new Writer(filename, bufferSize)), index(-1), writeIndex(-1), dummyRuns(0), runs(0), writeNo(NULL),
readNo(NULL) {}

Tape::Tape(string filename, int bufferSize, int* readNo, int* writeNo) : writeNo(writeNo), readNo(readNo),
filename(filename), reader(new Reader(filename, bufferSize, readNo)), writer(new Writer(filename, bufferSize, writeNo)),
index(-1), writeIndex(-1), dummyRuns(0), runs(0) {}

void Tape::startReading() {
	index = 0;
	endOfRun = false;
	if (reader->readRecord(index) != NULL) {
		endOfTape = false;
		currentRecord = *(reader->readRecord(index));
	}
	else
		endOfTape = true;
}

void Tape::nextRecord() {
	Record* record = reader->readRecord(++index);
	if (record == NULL) {
		endOfTape = true;
		runs--;
		return;
	}

	previousRecord = currentRecord;
	currentRecord = *record;
	if (currentRecord < previousRecord) {
		endOfRun = true;
		runs--;
	}
}

void Tape::newRun() {
	endOfRun = false;
}

void Tape::addRecord(Record record) {
	previousRecord = record;
	writer->writeRecord(record);
	writeIndex++;
}

void Tape::finishWriting() {
	index = -1;
	writeIndex = -1;
	writer->writeBlock();
	reader->updateSize();
	writer->clearBuffer();
}

void Tape::printTape() {
	cout << reader->fileName << " runs: " << runs << "  dummyRuns: " << dummyRuns << endl;
	for (int i = 0; i <= reader->lastRecordIndex; i++)
		cout << "[" << Record::roots_sum(*reader->readRecord(i)) << "] ";
	cout << endl << endl;
}

void Tape::clear() {
	remove(filename.c_str());
	reader->updateSize();
	writer->clearBuffer();
}

void Tape::switchAndDelete(Tape* tape) {
	remove(filename.c_str());
	rename(tape->filename.c_str(), filename.c_str());
	index = -1;
	endOfTape = 0;
	reader->updateSize();
	writer->clearBuffer();
}

Tape::~Tape() {
	delete reader;
	delete writer;
	remove(filename.c_str());
}