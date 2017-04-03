#pragma once
#include <vector>
#include "scheduleBuilder.h"

#define POPPULATION_SIZE 30
#define SURVIVOR_SIZE 5
#define MAX_LOCALSEARCHES 6

struct Solution{
	std::vector<char> gene;
	float finish_time;

	bool operator <(const Solution& rhs) {
		return finish_time < rhs.finish_time;
	}
};

void bees(Jobs* jobs);
void calcPoppulationFinishTime(std::vector<Solution>& poppulation, Jobs* jobs);
void calcFinishTime(Solution* sol, std::vector<std::vector<Task>>* schedule, Jobs* jobs);
void localSearch(Solution* sol, int num, Jobs* jobs);