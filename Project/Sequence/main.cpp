#include "SequenceBucketSort.h"
#include <ctime>
int main() {
	/*ofstream test("test.txt");
	for(size_t i =0;i<1000000; i++){
		int a = rand()%100;
		test<<a<<" ";
	}*/
	SequenceBucketSort* sq = new SequenceBucketSort("test.txt",20,100,0);
	clock_t begin = clock();
	sq->Run();
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	cout<<"Time: "<<elapsed_secs<<endl;
	return 0;
}
