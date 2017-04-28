#include <iostream>
#include <stdlib.h>
#include <cmath>

#include "Ant.h"
#include "Parameters.h"
#include "scheduleBuilder.h"
#include "Bees.h" //for calcFinishTime og Solution 

using namespace std;

void ants(Jobs* jobs) {
	Solution feasableSolution; //Bees
	
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
	Solution bestSolution;
	bestSolution.finish_time = INFINITY;
	Jobs job_copy = *jobs;
	int itterations = 0;
	int maxNumItterations = 0;
	int input;
	while (1) {
		//3.1 Random assignment of first operation
		bool dec_rule[K_ANTS];

		for (int k = 0; k < K_ANTS; ++k) {
			Jobs tabu = job_copy; //copy so start-times can be set, bruke reset?
			//vector<int> tabu(n,0);
			
			colony[k].gene.clear();
			colony[k].gene.push_back(rand() % n);
			tabu.increment(colony[k].gene[0]);

			//3.2 Define decidabilty rule for each ant
			// Random choice, longest or shortest processing time
			dec_rule[k] = rand() % 2;
			//3.3 while tabu_k not full
			
			schedule_t schedule(tabu.read_numMachines(), vector<Task>());
			while (colony[k].gene.size() < total_operations) {
				//kalkuler start_time
				for (int i = 0; i < tabu.size(); ++i) {
					//hvis vi har fullført denne jobben, hopp til neste
					if (!tabu.canIndex(i)) continue;

					float job_end_time = 0;
					float machine_end_time = 0;
					//Hvis det er oppgaver før denne i denne jobben
					if (tabu.canIndexPrev(i)) {
						//Skriv ned når den fullførte
						job_end_time = tabu.getPrevTask(i).start_time + tabu.getPrevTask(i).process_time;
					}
					//hvis det er jobber lagt til på denne maskinen
					int machine_size = schedule.at(tabu[i].machine_id).size();
					if (machine_size) {
						//skriv ned når maskinen blir ledig
						machine_end_time = schedule.at(tabu[i].machine_id)[machine_size - 1].start_time +
							schedule.at(tabu[i].machine_id)[machine_size - 1].process_time;
					}
					tabu[i].start_time = max(job_end_time, machine_end_time);
				}

				//ToDo: Endre så det veksler mellom short og long
				float bestVal = INFINITY;
				int best_id = -1;
				vector<float> tau_ij(n, 0.0);
				float total = 0.0;
				for (int i = 0; i < tabu.size(); ++i) {				
					if (!tabu.canIndex(i)) continue;
					tau_ij[i] += phi[i][tabu.current_job_index[i]][i];
					total += pow(tau_ij[i], ALPHA)*pow(tabu[i].process_time, BETHA);
				}
				for (int i = 0; i < tabu.size(); ++i) {
					if (!tabu.canIndex(i)) continue;
					float p_ij = pow(tau_ij[i], ALPHA)*pow(tabu[i].process_time, BETHA)/total;
					if (p_ij < bestVal) {
						bestVal = p_ij;
						best_id = i;
					}
				}
				schedule.at(tabu[best_id].machine_id).push_back(tabu[best_id]);
				tabu.increment(best_id);
				colony[k].gene.push_back(best_id);
			}
		
		}

		
 		calcPoppulationFinishTime(colony, jobs);
		
		for (int k = 0; k < K_ANTS; ++k) {
			if (colony[k] < bestSolution) {
				bestSolution = colony[k];
			}
		}
		
		//----------------skriv ut og sånt her.------------------------
		cout << bestSolution.finish_time << ' ';
		if (itterations >= maxNumItterations) {
			cout << "\n\n" << bestSolution.finish_time << "\n\n";
			cout << "[0]Print solution, [1, ...] more itterations: ";
			cin >> input;
			if (input == 0) {
				printSchedule(bestSolution.gene, *jobs);
			}
			else {
				maxNumItterations = itterations + input;
			}
		}
		itterations++;
		//-------------------------------------------------------------
	}
}


