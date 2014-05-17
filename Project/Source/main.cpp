#include "SequenceBucketSort.h"
#include <ctime>
int main() {
	SequenceBucketSort* sq = new SequenceBucketSort("test100M.txt",2,100,0);
	clock_t begin = clock();
	sq->Run();
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	cout<<"Time: "<<elapsed_secs<<endl;
	return 0;
}
