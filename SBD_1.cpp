#include <iostream>
#include <iomanip> 
#include "Tape.h"
#include "Record.h"
#include "Reader.h"
#include "Writer.h"
#include <cstdlib>
#include <ctime>

#define BLOCK_SIZE 200
#define LOWER_RAND -25.0
#define UPPER_RAND 25.0

bool showEachPhase = 0, display = 1;
int phaseNo = 0, writeBlockNo = 0, readBlockNo = 0;
int runsTest;

using namespace std;

bool fibDistribute(Tape* input, Tape* tape1, Tape* tape2);
bool fibMerge(Tape* tape1, Tape* tape2, Tape* tape3);
void generate(Tape* inputTape, int recordsNumber);
void fibSort(Tape* inputTape);
double gen_number();



bool fibDistribute(Tape* input, Tape* tape1, Tape* tape2) {
	tape1->clear();
	tape2->clear();
	Tape* tape[2] = { tape1, tape2 };
	bool activeTape = 0;
	int totalRuns = 0;
	int prevFib = 0, fib = 1, temp;


	input->startReading();
	if (input->endOfTape)
		return true;
	tape[activeTape]->addRecord(input->currentRecord);
	input->nextRecord();

	while (!(input->endOfTape)) {

		if (input->currentRecord < tape[activeTape]->previousRecord) {
			totalRuns++;
			tape[activeTape]->runs++;
		}
		if (tape[activeTape]->runs == fib) {
			activeTape = !activeTape;
			temp = fib;
			fib = prevFib + fib;
			prevFib = temp;
			if (tape[activeTape]->writeIndex != -1 && input->currentRecord >= tape[activeTape]->previousRecord)
				tape[activeTape]->runs--;
		}
		tape[activeTape]->addRecord(input->currentRecord);

		input->nextRecord();
	}
	totalRuns++;
	tape[activeTape]->runs++;

	if (tape[activeTape]->runs < fib) {
		tape[activeTape]->dummyRuns = fib - tape[activeTape]->runs;
	}
	tape1->finishWriting();
	tape2->finishWriting();
	input->runs = totalRuns;

	if ((tape1->runs == 1 && tape2->runs == 0) || (tape2->runs == 1 && tape1->runs == 0))
		return true;
	return false;

}


bool fibMerge(Tape* tape1, Tape* tape2, Tape* tape3) {
	tape1->startReading();
	tape2->startReading();

	if (showEachPhase) {
		cout << "________________________________________: " << endl;
		cout << "Przed Merge: " << endl;
		tape1->printTape();
		tape2->printTape();
		tape3->printTape();
	}
	tape3->runs = (tape1->runs + tape1->dummyRuns) < (tape2->runs + tape2->dummyRuns) ? tape1->runs : tape2->runs;

	while (true) {
		if (!(tape1->endOfTape) || !(tape2->endOfTape)) {
			if (tape1->dummyRuns > 0)
				tape1->endOfRun = true;
			else if (tape2->dummyRuns > 0)
				tape2->endOfRun = true;

			// If end of series on tape 1
			if (tape1->endOfRun || tape1->endOfTape) {
				// move series from tape 2 to tape 3
				while (!(tape2->endOfTape) && !(tape2->endOfRun)) {
					tape3->addRecord(tape2->currentRecord);
					tape2->nextRecord();
				}
				tape1->newRun();
				tape2->newRun();

				if (tape1->dummyRuns > 0) {
					tape1->dummyRuns--;
				}
				if (tape1->endOfTape)
					break;
				if (tape1->dummyRuns == 0 && tape2->endOfTape)
					break;
			}
			// If end of series on tape 2
			else if (tape2->endOfRun || tape2->endOfTape) {
				// move series from tape 1 to tape 3
				while (!(tape1->endOfTape) && !(tape1->endOfRun)) {
					tape3->addRecord(tape1->currentRecord);
					tape1->nextRecord();
				}
				tape1->newRun();
				tape2->newRun();

				if (tape2->dummyRuns > 0)
					tape2->dummyRuns--;
				if (tape2->endOfTape)
					break;
				if (tape2->dummyRuns == 0 && tape1->endOfTape)
					break;
			}
			else {
				if (tape1->currentRecord < tape2->currentRecord) {
					tape3->addRecord(tape1->currentRecord);
					tape1->nextRecord();
				}
				else {
					tape3->addRecord(tape2->currentRecord);
					tape2->nextRecord();
				}

			}
		}
		else if (tape1->endOfTape)
			break;
		else if (tape2->endOfTape)
			break;
	}
	tape3->finishWriting();


	if (tape1->runs == 0)
		tape1->clear();
	if (tape2->runs == 0)
		tape2->clear();


	Tape* copy = NULL;
	if (!(tape1->endOfTape))
		copy = tape1;
	else if (!(tape2->endOfTape))
		copy = tape2;
	if (copy != NULL) {
		Tape* tapeRest = new Tape("tapeCopy", BLOCK_SIZE);
		int* tempOp = copy->readNo;
		copy->reader->readNo = NULL;
		int temp = copy->runs;
		while (!(copy->endOfTape)) {
			tapeRest->addRecord(copy->currentRecord);
			copy->nextRecord();
		}
		tapeRest->finishWriting();
		copy->switchAndDelete(tapeRest);
		copy->runs = temp;
		if (copy == tape1)
			tape2->clear();
		else
			tape1->clear();
		copy->reader->readNo = tempOp;
		delete tapeRest;
	}


	if (showEachPhase) {
		cout << "Po Merge: " << endl << endl;
		tape1->printTape();
		tape2->printTape();
		tape3->printTape();
		cout << "________________________________________: " << endl;
	}

	if (tape3->runs == 1 && tape1->endOfTape && tape2->endOfTape)
		return true;
	return false;
}


void generate(Tape* inputTape, int recordsNumber) {
	remove(inputTape->filename.c_str());
	srand(time(NULL));
	int i = 0;
	Record record;
	while (i < recordsNumber) {
		do {
			record.a = gen_number();
			record.b = gen_number();
			record.c = gen_number();
		} while (!Record::deltaCheck(record));
		inputTape->addRecord(record);
		i++;
	}
	inputTape->finishWriting();
	writeBlockNo = 0;
}

double gen_number() {
	return LOWER_RAND + static_cast <double> (rand()) / (static_cast <double> (RAND_MAX / (UPPER_RAND - LOWER_RAND)));
}


void fibSort(Tape* inputTape) {
	phaseNo = 0;
	remove("tape1");
	remove("tape2");
	remove("tape3");
	Tape *tape1 = new Tape("tape1", BLOCK_SIZE, &readBlockNo, &writeBlockNo);
	Tape *tape2 = new Tape("tape2", BLOCK_SIZE, &readBlockNo, &writeBlockNo);
	Tape *tape3 = new Tape("tape3", BLOCK_SIZE, &readBlockNo, &writeBlockNo);
	bool sorted = 0;
	phaseNo++;
	sorted = fibDistribute(inputTape, tape1, tape2);
	int runs = inputTape->runs;
	runsTest = runs;

	if (display) {
		cout << "Plik wejsciowy:\n";
		inputTape->printTape();
	}

	if(!sorted)
		sorted = fibMerge(tape1, tape2, tape3);
	while (!sorted) {
		phaseNo++;
		if(tape1->runs==0)
			sorted = fibMerge(tape2, tape3, tape1);
		else if(tape2->runs == 0)
			sorted = fibMerge(tape3, tape1, tape2);
		else
			sorted = fibMerge(tape1, tape2, tape3);
	}

	if (display) {
		cout << "\nIlosc serii w pliku poczatkowym: " << runs << endl;
		cout << "\nIlosc faz: " << phaseNo << endl;
		cout << "Ilosc odczytow: " << readBlockNo << endl;
		cout << "Ilosc zapisow: " << writeBlockNo << endl;
		cout << "Ilosc operacji odczytu/zapisu: " << writeBlockNo + readBlockNo << endl << endl;


		cout << "Plik posortoway:\n";
		if (tape1->runs == 1) {
			tape1->printTape();
			rename(tape1->filename.c_str(), "output");
		}
		else if (tape2->runs == 1) {
			tape2->printTape();
			rename(tape2->filename.c_str(), "output");
		}
		else {
			tape3->printTape();
			rename(tape3->filename.c_str(), "output");
		}
	}
	delete tape1;
	delete tape2;
	delete tape3;
}



int main()
{
	char in;
	int recordsNumber;
	Record inputRecord;
	Tape* inputTape = NULL;
	string filename;
	do {
		remove("generated");
		remove("keyboardInput");
		system("cls");
		cout << "Maciej Zakrzewski\nNr albumu: 175573\n\nSortowanie polifazowe(Fibonacciego) z uzyciem 3 tasm\n\nRekordy pliku: wspolczynniki wielomianow kwadratowych\nSortowanie rosnaco wzgledem sumy pierwiastkow wielomianow\n\n";
		cout << "Menu:\n1. Sortuj wygenerowane dane\n2. Sortuj dane wprowadzone z klawiatury\n3. Sortuj dane z pliku testowego\n4. Testowanie\nf. Wyswietlaj po kazdej fazie (" << showEachPhase <<")\nq. Wyjscie\nWybor: ";
		cin >> in;
		switch (in) {
		case '1':
			display = 1;
			do {
				cout << "\nPodaj liczbe rekordow: ";
				cin >> recordsNumber;
			} while (cin.fail());
			inputTape = new Tape("generated", BLOCK_SIZE, &readBlockNo, &writeBlockNo);
			generate(inputTape, recordsNumber);
			fibSort(inputTape);
			delete inputTape;
			system("pause");
			break;
		case '2':
			do {
				cout << "\nPodaj liczbe rekordow: ";
				cin >> recordsNumber;
			} while (cin.fail());
			inputTape = new Tape("keyboardInput", BLOCK_SIZE, &readBlockNo, &writeBlockNo);
			cout << "Wprowadzaj po trzy wspolczynniki oddzielone spacja\n\n";
			for (int i = 1; i <= recordsNumber;) {
				cout << "Rekord " << i << ":";
				cin >> inputRecord.a;
				if (cin.fail()) {
					cout << "Nieprawidlowa wartosc, sprobuj ponownie.";
					continue;
				}
				cin >> inputRecord.b;
				if (cin.fail()) {
					cout << "Nieprawidlowa wartosc, sprobuj ponownie.";
					continue;
				}
				cin >> inputRecord.c;
				if (cin.fail()) {
					cout << "Nieprawidlowa wartosc, sprobuj ponownie.";
					continue;
				}
				i++;
				inputTape->addRecord(inputRecord);
			}
			inputTape->finishWriting();
			fibSort(inputTape);
			delete inputTape;
			system("pause");
			break;
		case '3':
			do {
				cout << "\nPodaj nazwe pliku testowego: ";
				cin >> filename;
			} while (cin.fail());
			inputTape = new Tape(filename, BLOCK_SIZE);
			fibSort(inputTape);
			delete inputTape;
			system("pause");
			break;
		case '4':
			display = 0;
			inputTape = new Tape("generated", BLOCK_SIZE, &readBlockNo, &writeBlockNo);
			for (int n = 100; n <= 312500; n *= 5) {
				writeBlockNo = 0;
				readBlockNo = 0;
				phaseNo = 0;
				generate(inputTape, n);
				fibSort(inputTape);
				cout << "N = " << n << "\t\tr = " << runsTest << "\tF = " << phaseNo << "\tO = " << writeBlockNo + readBlockNo  << "\tR = " << readBlockNo << "\tW = " << writeBlockNo << endl << endl;
			}
			delete inputTape;
			cout << '\a';
			system("pause");
			break;
		case 'f':
			showEachPhase = !showEachPhase;
			break;
		case 'q':
			break;
		default:
			break;
		}
	} while (in != 'q');


}
