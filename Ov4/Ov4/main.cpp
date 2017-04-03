#include"scheduleBuilder.h"
#include "Parameters.h"
#include "readData.h"
#include <vector>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include "Bees.h"

using namespace std;

int main() {
	srand(time(NULL));

	Jobs jobs;
	jobs.readFile();
	
	bees(&jobs);

	vector<char> genom;
	for (int i = 0; i < 6; ++i) {
		for (int j = 0; j < 6; ++j) {
			genom.push_back(j);
		}
	}

	printSchedule(genom, jobs);
	system("PAUSE");
	return 0;
}