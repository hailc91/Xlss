#ifndef _LIB_BUCKET_SORT
#define _LIB_BUCKET_SORT

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <deque>
#include <vector>
#include <algorithm>

using namespace std;
class SequenceBucketSort {
private:
//Name of fileInput
	string fileInput;
//Number bucket need to devide
	unsigned long numBucket;
//List bucket
	deque<int> listBucket[3];
//Max value of array
	int maxVal;
//Min value of array
	int minVal;
//Range value of 1 bucket
	int offset;
//Write Value to Bucket with index
	void PutToCorrectBucket(int value, unsigned long indexBuck);
//Generate original array to seperated bucked
	bool GenerateBucket(int minValue, int maxValue, unsigned long numBuck,string inputFile);
//Sort instance in same bucket
	void sortCurrentBucket(unsigned long indexBuck, ofstream& out);
//Write result to file
	void writeResultToFile(deque<int> list, ofstream& out);
public:
//constructor
	SequenceBucketSort(string fileName, unsigned long numBuc, int max, int min);
//Run function
	void Run();

};

#endif
