#include "mpi.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <deque>
#include <algorithm>


#define INIT_PHRASE   -1
#define SORT_PHRASE   -2
#define GATHER_PHRASE -3 
#define END_PHRASE    -4
#define MIN_VALUE      0
#define MAX_VALUE      100

using namespace std;

int minVal = MIN_VALUE;
int maxVal = MAX_VALUE;
string inputFile = "test.txt";
string outputFile = "result.txt";

void writeResultToFile(deque<int> list, ofstream& out){
	deque<int>::const_iterator head;
	deque<int>::const_iterator tail = list.end();
	for(head = list.begin(); head!=tail; ++head){
		out<<*head<<" ";
		//cout<<*head<<" "; 
	}
	//cout <<endl;
}

int sendDataToWorkers(int numBucket) {
	int value;
	int phrase;
	ifstream input(inputFile.c_str());
	
	if (!input.is_open()) {
		cout << "Error when open input files\n";
		MPI::COMM_WORLD.Abort(-1);
		return -1;
	}
	
	int offset = ceil((maxVal - minVal)/(numBucket + 0.0));
	
	// read number and send it to bucket processors
	while (input >> value) {
		int dest = (value - minVal) / offset;
		int tag;
		dest = dest >= numBucket - 1 ? numBucket - 1 : (dest <= 0 ? 0 : dest);
		dest = dest + 1;
		tag = dest;
		//cout <<"bin: "<<dest<<" val: "<<value<<endl;
		
		MPI_Send(&value, 1, MPI_INT,  dest, tag, MPI_COMM_WORLD);
	}
	input.close();
	return 0;
}

int receiveDataFromMaster(int indexBuck) {
	ostringstream fileName;
	fileName << indexBuck - 1;
	string s = fileName.str();
	ofstream output(s.c_str());
	if (!output.is_open()) {
		cout << "Error when open files to write bucket\n";
		MPI::COMM_WORLD.Abort(-1);
		return -1;
	}
	
	int value = INIT_PHRASE;
	MPI_Status status;
	while(value != SORT_PHRASE) {
		MPI_Recv(&value, 1, MPI_INT, 0, indexBuck, MPI_COMM_WORLD, &status);
		output << value << " ";
	}
	
	output.close();
	return 0;
}

int sendDataToMaster(int indexBuck) {
	ostringstream fileName;
	fileName << indexBuck - 1;
	string s = fileName.str();
	ifstream input(s.c_str());
	
	if (!input.is_open()) {
		cout << "Error when open input files\n";
		MPI::COMM_WORLD.Abort(-1);
		return -1;
	}
	
	// wait until getting GATHER_PHRASE signal
	int value = SORT_PHRASE;
	MPI_Status status;
	while(value != GATHER_PHRASE) {
		MPI_Recv(&value, 1, MPI_INT, 0, indexBuck, MPI_COMM_WORLD, &status);
	}
	
	// read number and send it to main processors
	while (input >> value) {
		MPI_Send(&value, 1, MPI_INT,  0, indexBuck, MPI_COMM_WORLD);
	}
	
	// send END_PHRASE signal to main processor
	value = END_PHRASE;
	MPI_Send(&value, 1, MPI_INT,  0, indexBuck, MPI_COMM_WORLD);
	
	input.close();
	return 0;
}

int receiveDataFromWorkers(int indexBuck) {
	
	ofstream output(outputFile.c_str(), ios::app);
	if (!output.is_open()) {
		cout << "Error when open files to write bucket\n";
		MPI::COMM_WORLD.Abort(-1);
		return -1;
	}
	
	int value = SORT_PHRASE;
	deque<int> list;
	MPI_Status status;
	while(value != END_PHRASE) {
		MPI_Recv(&value, 1, MPI_INT, indexBuck, indexBuck, MPI_COMM_WORLD, &status);
		list.push_back(value);
	}

	list.pop_back();
	writeResultToFile(list, output);
	list.clear();
	
	//cout<<"bin: "<<indexBuck<<endl;
	
	output.close();
	return 0;
}

int sortBucket(int indexBuck, int numBucket) {
	ostringstream fileName;
	fileName << indexBuck - 1;
	string s = fileName.str();
	ifstream input(s.c_str());
	
	
	if (!input.is_open()) {
		cout << "Error when open input files\n";
		MPI::COMM_WORLD.Abort(-1);
		return -1;
	}
	
	int value;
	deque<int> sortedList;
	input >> value;
	if(value != SORT_PHRASE) {
		sortedList.push_back(value);
		while (input >> value) {
			sortedList.push_back(value);
		}
	}
	remove(s.c_str());
	ofstream result(s.c_str());
	sort(sortedList.begin(), sortedList.end());
	sortedList.pop_front();
	writeResultToFile(sortedList, result);
	sortedList.clear();
	input.close();
}



int main(int argc, char** argv) {
	int rank, size;
	double startTime, endTime, eslapsedTime;
		
	MPI::Init ( argc, argv );
    rank = MPI::COMM_WORLD.Get_rank();
    size = MPI::COMM_WORLD.Get_size();
	
	if(rank == 0) {
		// send numbers to bucket processors
		startTime = MPI::Wtime();
		//remove(outputFile.c_str());
		sendDataToWorkers(size - 1);
		endTime = MPI::Wtime();
		eslapsedTime = (endTime - startTime) * 1000;
		cout<<"send data time: "<<eslapsedTime<<endl;
		
		// complete initial phrase and send signal to all processors to process sort phrase
		int phrase = SORT_PHRASE;
		
		for(int i = 1; i < size; i++) {
			MPI_Send(&phrase, 1, MPI_INT, i, i, MPI_COMM_WORLD);
		}	
		
		// gather result from processors 
		phrase = GATHER_PHRASE;
		
		startTime = MPI::Wtime();
		for(int i = 1; i < size; i++) {
			MPI_Send(&phrase, 1, MPI_INT, i, i, MPI_COMM_WORLD);
			
			// get result data from bucket processors until getting END_PHRASE signal 
			receiveDataFromWorkers(i);
			
		}	
		endTime = MPI::Wtime();
		eslapsedTime = (endTime - startTime) * 1000;
		cout<<"sort and gather result time: "<<eslapsedTime<<endl;
	}
	else {
		// get numbers from main processor
		receiveDataFromMaster(rank);
		
		// sort each bucket
		sortBucket(rank, size - 1);
		
		// send sorted bucket to main processor
		sendDataToMaster(rank);	
	}
	
	MPI::Finalize();
	return 0;
}
