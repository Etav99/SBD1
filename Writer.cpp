#include "Writer.h"
#include "Reader.h"


Writer::Writer(string fileName, int bufferSize) : fileName(fileName), position(0), bufferSize(bufferSize),
buffer(new Record[bufferSize]), writeNo(NULL) {}

Writer::Writer(string fileName, int bufferSize, int* writeNo) : Writer::Writer(fileName, bufferSize) {
	this->writeNo = writeNo;
}

bool Writer::writeBlock() {
	fstream file(fileName, ios::app | ios::binary);
	if (file.write((char*)buffer, sizeof(Record) * (position))) {
		file.close();
		if (writeNo != NULL)
			(*writeNo)++;
		return true;
	}
	clearBuffer();
	return false;
}

void Writer::clearBuffer() {
	position = 0;
}

void Writer::writeRecord(Record record) {
	buffer[position] = record;
	position++;
	if (position == bufferSize) {
		writeBlock();
		position = 0;
	}

}

Writer::~Writer() {
	delete[] buffer;
}