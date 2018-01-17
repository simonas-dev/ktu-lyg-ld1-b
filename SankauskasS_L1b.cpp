#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <omp.h>
#include <chrono>
#include <thread>

using namespace std;

const string DATA_FILE_NAME = "SankauskasS_L1b_dat.txt";
const string OUTPUT_FILE_NAME = "SankauskasS_L1b_rez.txt";

struct Worker {
	string name;
	int office;
	double salary;
};

struct QueuedWorker {
	int queueNumber;
	Worker worker;
};

const int THREAD_COUNT = 23; // amount of threads
const int DATA_COUNT = 23;  // amount of data
QueuedWorker QueuedWorkerArr[DATA_COUNT];
int Index = 0;

void readFile(string stringArr[DATA_COUNT], int intArr[DATA_COUNT], double doubleArr[DATA_COUNT]);
void printArrays(string stringArr[DATA_COUNT], int intArr[DATA_COUNT], double doubleArr[DATA_COUNT]);
void startThread(string stringArr[DATA_COUNT], int intArr[DATA_COUNT], double doubleArr[DATA_COUNT]);
void printOutputFile();

int main() {
	string stringArr[DATA_COUNT];
	int intArr[DATA_COUNT];
	double doubleArr[DATA_COUNT];

	for (int i = 0; i < DATA_COUNT; i++) {
		QueuedWorker queuedWorker;
		queuedWorker.queueNumber = -1;
		queuedWorker.worker.name = "";
		queuedWorker.worker.office = -1;
		queuedWorker.worker.salary = -1.0;

		QueuedWorkerArr[i] = queuedWorker;
	}

	readFile(stringArr, intArr, doubleArr);
	printArrays(stringArr, intArr, doubleArr);
	startThread(stringArr, intArr, doubleArr);
	printOutputFile();
}

void readFile(string stringArr[DATA_COUNT], int  intArr[DATA_COUNT], double  doubleArr[DATA_COUNT]) {
	ifstream fd(DATA_FILE_NAME);
	string name;
	int office;
	double salary;
	while (!fd.eof()) {
		for (int i = 0; i < DATA_COUNT; i++) {
			getline(fd, name, ',');
			fd >> ws;
			fd >> office;
			fd >> ws;
			fd >> salary;
			fd >> ws;
			stringArr[i] = name;
			intArr[i] = office;
			doubleArr[i] = salary;
		}
		fd.ignore();
	}
	fd.close();
}

void printArrays(string stringArr[DATA_COUNT], int  intArr[DATA_COUNT], double doubleArr[DATA_COUNT]) {
	ofstream fr(OUTPUT_FILE_NAME);
	fr << "*** Arrays S, I and D Table" << endl;
	fr << "Index, Name, Office, Salary" << endl;
	for (int i = 0; i < DATA_COUNT; i++) {
		fr << i << ", " << stringArr[i] << ", " << intArr[i] << ", " << doubleArr[i] << endl;
	}
	fr << endl;
	fr.close();
}

void startThread(string stringArr[DATA_COUNT], int intArr[DATA_COUNT], double doubleArr[DATA_COUNT]) {

#pragma omp parallel num_threads(THREAD_COUNT) 
	{
		QueuedWorker queuedWorker;
		int i;
		i = omp_get_thread_num();
		queuedWorker.queueNumber = i;
		queuedWorker.worker.name = stringArr[i];
		queuedWorker.worker.office = intArr[i];
		queuedWorker.worker.salary = doubleArr[i];		
		QueuedWorkerArr[Index++] = queuedWorker;
	}
}

void printOutputFile() {
	ofstream fr(OUTPUT_FILE_NAME, ios::app);
	fr << "*** P array Table" << endl;

	fr << "Index, Queue Num, Name, Office, Salary" << endl;
	for (int i = 0; i < DATA_COUNT; i++) {
		QueuedWorker queuedWorker = QueuedWorkerArr[i];
		fr << i << ",  ";
		fr << queuedWorker.queueNumber << ", ";
		fr << queuedWorker.worker.name << ", ";
		fr << queuedWorker.worker.office << ", ";
		fr << queuedWorker.worker.salary << endl;
	}
	fr << endl;
	fr.close();
}