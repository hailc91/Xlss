#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <deque>
#include <algorithm>
using namespace std;

int main() {
	ofstream test1M("test1M.txt");
	ofstream test10M("test10M.txt");
	ofstream test100M("test100M.txt");
	ofstream test1B("test1B.txt");
	
	// generate 1 million numbers
	for(size_t i = 0; i < 1000000; i++){
		int a = rand()%100;
		test1M << a << " ";
	}
	
	cout << "Finish generate test1M.txt" << endl;
	
	// generate 10 million numbers
	for(size_t i = 0; i < 10000000; i++){
		int a = rand()%100;
		test10M << a << " ";
	}
	
	cout << "Finish generate test10M.txt" << endl;
	
	// generate 100 million numbers
	for(size_t i = 0; i < 100000000; i++){
		int a = rand()%100;
		test100M << a << " ";
	}
	
	cout << "Finish generate test100M.txt" << endl;
	
	// generate 1 billion numbers
	for(size_t i = 0;i < 1000000000; i++){
		int a = rand()%100;
		test1B << a << " ";
	}
	
	cout << "Finish generate test1B.txt" << endl;
	
	test1M.close();
	test10M.close();
	test100M.close();
	test1B.close();
	
	return 0;
}
