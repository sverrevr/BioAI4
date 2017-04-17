#pragma once
#include <vector>
#include "Parameters.h"
#include "scheduleBuilder.h"
struct Particle {
	std::vector<std::vector<float>> state;
	std::vector<std::vector<float>> velocity;
	double makespan;
	std::vector<std::vector<float>> pb;
	double pb_makespan;
	Jobs* jobs;
	void calcMakespan();
	Particle(Jobs* jobs) {
		for (int i = 0; i < jobs->size(); ++i) {
			state.push_back(std::vector<float>());
			velocity.push_back(std::vector<float>());
			for (int j = 0; j < jobs->size(i); ++j) {
				state[i].push_back(rand() % (int)MAX_VAL);
				velocity[i].push_back((rand() % (int)MAX_VAL/20) - (int)MAX_VAL / 40);
			}
		}
		pb = state;
		makespan = INFINITY;
		pb_makespan = INFINITY;
		this->jobs = jobs;
	}
};

void makeGenom(std::vector<char> *genom, std::vector<std::vector<float>> state);

void pso(Jobs* jobs);

struct Poppulation{
	std::vector<Particle> particles;
	std::vector<std::vector<float>> gb;
	double gb_makespan;


	Poppulation(Jobs* jobs) {
		for (int i = 0; i < NUM_PARTICLES; ++i) {
			particles.push_back(Particle(jobs));
		}
		gb = particles[0].state;
		gb_makespan = INFINITY;
	}

	void update();
};

std::vector<std::vector<float>>& operator+=(std::vector<std::vector<float>>& lhs, std::vector<std::vector<float>>& rhs);
std::vector<std::vector<float>> operator+(std::vector<std::vector<float>>& lhs, std::vector<std::vector<float>>& rhs);
std::vector<std::vector<float>>& operator*=(std::vector<std::vector<float>>& lhs, float& rhs);
std::vector<std::vector<float>> operator*(float lhs, std::vector<std::vector<float>>& rhs);
std::vector<std::vector<float>>& operator-=(std::vector<std::vector<float>>& lhs, std::vector<std::vector<float>>& rhs);
std::vector<std::vector<float>> operator-(std::vector<std::vector<float>>& lhs, std::vector<std::vector<float>>& rhs);