#include"scheduleBuilder.h"
#include "Parameters.h"
#include "readData.h"
#include <vector>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include "Bees.h"
#include "PSO.h"

using namespace std;

int main() {
	srand(time(NULL));

	Jobs jobs;
	jobs.readFile();
	
	//bees(&jobs);
	pso(&jobs);

	system("PAUSE");
	return 0;
}