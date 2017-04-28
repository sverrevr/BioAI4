#include <iostream>
#include <stdlib.h>


#include "Ant.h"
#include "Parameters.h"
#include "scheduleBuilder.h"
#include "Bees.h" //for calcFinishTime og Solution 

using namespace std;

void ants(Jobs* jobs) {
	Solution feasableSolution;

	const int n = jobs->size();
	int total_operations = 0;
	vector<vector<vector<float>>> phi; //pheromone
	vector<vector<vector<float>>> delta_phi; //pheromone


	for (char job = 0; job < n; ++job) {
		vector<vector<float>> pher_i;
		vector<vector<float>> delta_i;
		for (int task = 0; task < jobs->size(job); ++task) {
			feasableSolution.gene.push_back(job); //Bees
			vector<float> pher_j;
			vector<float> delta_j;
			total_operations = n*jobs->size(job);
			for (int j = 0; j < n; ++j) {
				pher_j.push_back(((double)rand() / (RAND_MAX / PHERO_MAX)));
				delta_j.push_back(0);
			}
			pher_i.push_back(pher_j);
			delta_i.push_back(delta_j);
		}
		phi.push_back(pher_i);
		delta_phi.push_back(delta_i);
	}

	vector<Solution> colony(K_ANTS, feasableSolution); //Bees men må lages
	
	//init random poppulation
	for (int i = 0; i < K_ANTS; ++i) {
		random_shuffle(colony[i].gene.begin(), colony[i].gene.end()); //Bees
	}
	//calcPoppulationFinishTime(colony, jobs);
	//sort(colony.begin(), colony.end()); //ToDo: remove, doesn't make sense


	int itterations = 0;
	int maxNumItterations = 0;
	int input;
	while (1) {
		//3.1 Random assignment of first operation
		bool dec_rule[K_ANTS];

		for (int k = 0; k < K_ANTS; ++k) {
			Jobs tabu = *jobs; //copy so start-times can be set
			//vector<int> tabu(n,0);
			
			colony[k].gene.clear();
			colony[k].gene.push_back(rand() % n);
			tabu.increment(colony[k].gene[0]);

			//3.2 Define decidabilty rule for each ant
			// Random choice, longest or shortest processing time
			dec_rule[k] = rand() % 2;
			//3.3 while tabu_k not full
			while (colony[k].gene.size() < total_operations) {
				continue;
			}

		}

		

		//----------------skriv ut og sånt her.------------------------
		cout << colony[0].finish_time << ' ';
		if (itterations >= maxNumItterations) {
			cout << "\n\n" << colony[0].finish_time << "\n\n";
			cout << "[0]Print solution, [1, ...] more itterations: ";
			cin >> input;
			if (input == 0) {
				printSchedule(colony[0].gene, *jobs);
			}
			else {
				maxNumItterations = itterations + input;
			}
		}
		itterations++;
		//-------------------------------------------------------------
	}
}
