#pragma once
#include"scheduleBuilder.h"
#include <vector>

struct Interval
{
	float start;
	float end;
	bool isInIntervall(float val) {
		return (val >= start && val <= end);
	}
	int index;
};

void ants(Jobs* jobs);

