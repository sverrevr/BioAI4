#include"scheduleBuilder.h"
#include "Parameters.h"
#include "readData.h"
#include <vector>
#include <iostream>

using namespace std;

int main() {
	Jobs jobs;
	readFile(&jobs);
	
	vector<char> genom;
	for (int i = 0; i < 6; ++i) {
		for (int j = 0; j < 6; ++j) {
			genom.push_back(j);
		}
	}

	vector<vector<Task>> schedule(jobs.jobs.size(), vector<Task>());

	scheduleBuilder(jobs, genom, &schedule);
	int t;
	for (int i = 0; i < 6; ++i) {
		t = 0;
		for (int j = 0; j < 6; ++j) {
			while (t < schedule[i][j].start_time) {
				cout << ' ';
				t++;
			}
			while (t < schedule[i][j].start_time + schedule[i][j].process_time) {
				t++;
				cout << (int)schedule[i][j].job_id;
			}
		}
		cout << endl;
	}

	system("PAUSE");
	return 0;
}