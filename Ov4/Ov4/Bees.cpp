#include "Bees.h"
#include <algorithm>
#include <iostream>;

using namespace std;

void bees(Jobs* jobs) {

	Solution feasableSolution;
	for (char job = 0; job < jobs->size(); ++job) {
		for (int task = 0; task < jobs->size(job); ++task) {
			feasableSolution.gene.push_back(job);
		}
	}

	vector<Solution> poppulation(POPPULATION_SIZE,feasableSolution);

	//init random poppulation
	for (int i = 0; i < POPPULATION_SIZE; ++i) {
		random_shuffle(poppulation[i].gene.begin(), poppulation[i].gene.end());
	}

	//beregn fitness
	calcPoppulationFinishTime(poppulation, jobs);

	//sorter
	sort(poppulation.begin(), poppulation.end());


	int itterations = 0;
	int maxNumItterations = 0;
	int input;
	while (1) {
		//randomize de dårligste
		for (int i = SURVIVOR_SIZE; i < POPPULATION_SIZE; ++i) {
			random_shuffle(poppulation[i].gene.begin(), poppulation[i].gene.end());
		}

		//kjør randomsøk utifra hvor bra løsningen er
		for (int i = 0; i < POPPULATION_SIZE; ++i) {
			localSearch(&(poppulation[i]), ceil(MAX_LOCALSEARCHES*(1 - i / POPPULATION_SIZE)), jobs);
		}

		//beregn fitness
		calcPoppulationFinishTime(poppulation, jobs);

		//sorter
		sort(poppulation.begin(), poppulation.end());

		//skriv ut og sånt her.
		cout << poppulation[0].finish_time << ' ';
		if (itterations >= maxNumItterations) {
			cout << "\n\n"<< poppulation[0].finish_time << "\n\n";
			cout << "[0]Print solution, [1, ...] more itterations: ";
			cin >> input;
			if (input == 0) {
				printSchedule(poppulation[0].gene, *jobs);
			}
			else {
				maxNumItterations = itterations +input;
			}
		}


		itterations++;
	}
}


void calcPoppulationFinishTime(vector<Solution>& poppulation, Jobs* jobs) {
	schedule_t schedule(jobs->read_numMachines(), vector<Task>());

	for (int i = 0; i < poppulation.size(); ++i) {
		calcFinishTime(&(poppulation[i]), &schedule, jobs);
	}
}

void calcFinishTime(Solution* sol, schedule_t* schedule, Jobs* jobs) {
	float worstFinishTime;
	scheduleBuilder(*jobs, sol->gene, schedule);
	worstFinishTime = 0;
	for (auto machine = schedule->begin(); machine != schedule->end(); ++machine) {
		if (machine->at(machine->size()-1).start_time + machine->at(machine->size()-1).process_time > worstFinishTime) {
			worstFinishTime = machine->at(machine->size()-1).start_time + machine->at(machine->size()-1).process_time;
		}
	}
	sol->finish_time = worstFinishTime;
}


void localSearch(Solution* sol, int num, Jobs* jobs) {
	//TODO skal vi gjøre noe som innfører at flowerpatchen endrer størrelse?
	schedule_t schedule(jobs->read_numMachines(), vector<Task>());
	float prevFinishTime = sol->finish_time;
	int r1;
	int r2;	
	for (int i = 0; i < num; ++i) {
		r1 = rand() % sol->gene.size();
		r2 = rand() % sol->gene.size();
		while (sol->gene[r1] == sol->gene[r2]) {
			r2 = rand() % sol->gene.size();
		}
		swap(sol->gene[r1], sol->gene[r2]);
		
		calcFinishTime(sol, &schedule, jobs);
		if (sol->finish_time <= prevFinishTime) {
			//keep good changes
			prevFinishTime = sol->finish_time;
		}
		else {
			//undo bad changes
			swap(sol->gene[r1], sol->gene[r2]);
			sol->finish_time = prevFinishTime;
		}
	}
}