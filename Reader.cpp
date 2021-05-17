#include "Reader.h"


Reader::Reader(string fileName, int bufferSize) : fileName(fileName), blockStartIndex(0), blockEndIndex(0), bufferEmpty(true),
bufferSize(bufferSize), buffer(new Record[bufferSize]), readNo(NULL) {
	fstream file(fileName, ios::in | ios::binary | ios::ate);
	this->lastRecordIndex = file.tellg() / sizeof(Record) - 1;
	file.close();
}

Reader::Reader(string fileName, int bufferSize, int* readNo) : Reader::Reader(fileName, bufferSize) {
	this->readNo = readNo;
}

void Reader::updateSize() {
	bufferEmpty = true;
	fstream file(fileName, ios::in | ios::binary | ios::ate);
	this->lastRecordIndex = (file.tellg() / sizeof(Record)) - 1;
	file.close();
}

bool Reader::loadBlock(int blockIndex) {
	fstream file(fileName, ios::in | ios::binary);
	if (!file.good()) {
		cout << "Blad otwierania pliku";
		this->blockStartIndex = 0;
		this->blockEndIndex = 0;
		return false;
	}
	blockStartIndex = blockIndex * bufferSize;
	file.seekg(blockStartIndex * sizeof(Record), ios::beg);

	int recordNo;
	for (recordNo = 0; recordNo < bufferSize; recordNo++) {
		if (file.read((char*)&buffer[recordNo], sizeof(Record)));
		else break;
	}
	blockEndIndex = blockStartIndex + recordNo - 1;
	file.close();
	this->bufferEmpty = false;
	if (readNo != NULL)
		(*readNo)++;
	return true;
}

Record* Reader::readRecord(int recordNumber) {
	if (recordNumber > lastRecordIndex)
		return NULL;
	if (bufferEmpty || !(recordNumber >= blockStartIndex && recordNumber <= blockEndIndex))
	{
		int blockIndex = recordNumber / bufferSize;
		if (loadBlock(blockIndex))
			return &buffer[recordNumber % bufferSize];
		else
			return NULL;
	}
	else return &buffer[recordNumber % bufferSize];
}

Reader::~Reader() {
	delete[] buffer;
}