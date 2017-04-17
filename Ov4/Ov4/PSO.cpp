#include "PSO.h"
#include "Parameters.h"
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include "scheduleBuilder.h"
#include <iostream>

using namespace std;

void pso(Jobs* jobs) {

	Poppulation poppulation(jobs);


	int itterations = 0;
	int maxNumItterations = 0;
	int input;
	vector<char> genom;
	while (1) {
		poppulation.update();

		//skriv ut og sånt her.
		cout << poppulation.gb_makespan << ' ';
		if (itterations >= maxNumItterations) {
			cout << "\n\n" << poppulation.gb_makespan << "\n\n";
			cout << "[0]Print solution, [1, ...] more itterations: ";
			cin >> input;
			if (input == 0) {
				makeGenom(&genom, poppulation.gb);
				printSchedule(genom, *jobs);
			}
			else {
				maxNumItterations = itterations + input;
			}
		}


		itterations++;
	}
}


void Poppulation::update() {
	for (auto it = particles.begin(); it != particles.end(); ++it) {
		float rand_val = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		it->velocity += (rand_val*PB_SCALE * (it->pb - it->state));
		it->velocity += (rand_val*GB_SCALE * (gb - it->state));

		it->state += DT * it->velocity;

		for (auto jt = it->state.begin(); jt != it->state.end(); ++jt) {
			for (auto ot = jt->begin(); ot != jt->end(); ++ot) {
				*ot = max(*ot, MIN_VAL);
				*ot = min(*ot, MAX_VAL);
			}
		}


		it->calcMakespan();

		if (it->makespan < it->pb_makespan) {
			it->pb = it->state;
			it->pb_makespan = it->makespan;

			if (it->makespan < gb_makespan) {
				gb = it->state;
				gb_makespan = it->makespan;
			}

		}
	}
}

void Particle::calcMakespan() {
	vector<char> genom;
	makeGenom(&genom,state);

	std::vector<std::vector<Task>> schedule(jobs->read_numMachines(), vector<Task>());
	scheduleBuilder(*jobs, genom, &schedule);

	makespan = getMakespan(schedule, *jobs);

}

void makeGenom(std::vector<char> *genom, std::vector<std::vector<float>> state){

	for (auto it = state.begin(); it != state.end(); ++it) {
		sort(it->begin(), it->end());
	}
	vector<int> index(state.size(), 0);

	float min;
	char job;
	bool finished = 0;
	while (!finished) {
		finished = 1;
		min = INFINITY;
		for (char i = 0; i < index.size(); ++i) {
			if (index[i] != state.size()) {
				finished = 0;
				if (state[i][index[i]] < min) {
					min = state[i][index[i]];
					job = i;
				}
			}
		}
		genom->push_back(job);
		index[job]++;
	}
}

std::vector<std::vector<float>>& operator+=(std::vector<std::vector<float>>& lhs, std::vector<std::vector<float>>& rhs) {
	for (int i = 0; i < lhs.size(); ++i) {
		for (int j = 0; j < lhs[i].size(); ++j) {
			lhs[i][j] += rhs[i][j];
		}
	}
	return lhs;
}

std::vector<std::vector<float>> operator+(std::vector<std::vector<float>>& lhs, std::vector<std::vector<float>>& rhs){
	std::vector<std::vector<float>> res = lhs;
	res += rhs;
	return res;
}



std::vector<std::vector<float>>& operator*=(std::vector<std::vector<float>>& lhs, float& rhs) {
	for (int i = 0; i < lhs.size(); ++i) {
		for (int j = 0; j < lhs[i].size(); ++j) {
			lhs[i][j] *= rhs;
		}
	}
	return lhs;
}

std::vector<std::vector<float>> operator*(float lhs, std::vector<std::vector<float>>& rhs) {
	auto res = rhs;
	res *= lhs;
	return res;
}

std::vector<std::vector<float>>& operator-=(std::vector<std::vector<float>>& lhs, std::vector<std::vector<float>>& rhs) {
	for (int i = 0; i < lhs.size(); ++i) {
		for (int j = 0; j < lhs[i].size(); ++j) {
			lhs[i][j] -= rhs[i][j];
		}
	}
	return lhs;
}

std::vector<std::vector<float>> operator-(std::vector<std::vector<float>>& lhs, std::vector<std::vector<float>>& rhs) {
	auto res = lhs;
	res -= rhs;
	return res;
}