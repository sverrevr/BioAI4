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
	const int m = jobs->read_numMachines();
	const int K_ANTS = n / 2;

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
	Jobs job_copy = *jobs; //ToDo, sjekk om nødvendig... 
	int itterations = 0;
	int maxNumItterations = 0;
	int input;
	while (1) {
		vector<bool> dec_rule;
		dec_rule.reserve(K_ANTS);

		for (int k = 0; k < K_ANTS; ++k) {
			Jobs tabu = job_copy; //copy so start-times can be set, bruke reset?
			
			colony[k].gene.clear();
			
			//3.1 Random assignment of first operation
			//Add random task
			colony[k].gene.push_back(rand() % n);
			tabu.increment(colony[k].gene[0]);

			//3.2 Define decidabilty rule for each ant
			// Random choice, longest or shortest processing time
			bool dec_rule= rand() % 2;
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

				float bestVal = (dec_rule ? INFINITY : 0) ;
				int best_id = -1;
				vector<float> tau_ij(n, 0.0);
				float tau_sum = 0.0;
				for (int i = 0; i < tabu.size(); ++i) {				
					if (!tabu.canIndex(i)) continue;
					tau_ij[i] += phi[i][tabu.current_job_index[i]][i];
					tau_sum += pow(tau_ij[i], ALPHA)*pow(tabu[i].process_time, BETHA);
				}
				for (int i = 0; i < tabu.size(); ++i) {
					if (!tabu.canIndex(i)) continue;
					float p_ij = pow(tau_ij[i], ALPHA)*pow(tabu[i].process_time, BETHA)/tau_sum;
					if (dec_rule ? (p_ij < bestVal) : (p_ij > bestVal) ){
						bestVal = p_ij;
						best_id = i;
					}
				}
				schedule.at(tabu[best_id].machine_id).push_back(tabu[best_id]);
				tabu.increment(best_id);
				colony[k].gene.push_back(best_id);
			}
			calcFinishTime(&colony[k],&schedule,&tabu);
			
			//3.4 check if best, update pheromone
			if (colony[k] < bestSolution) {
				bestSolution = colony[k];
			}
			vector<char> current_job_index(n,0);
			for (int i = 0; i < colony[k].gene.size(); ++i) {
				
				for (int j = 0; j < n; ++j) {
					if (current_job_index[j]>= m) continue;  //ToDo: sjekk om dette er rett 
					delta_phi[j] [current_job_index[j]] [colony[k].gene[i]] += Q / colony[k].finish_time; //Eq. 3
				}
				current_job_index[ colony[k].gene[i] ]++;
			}
		}

		//3.5 Update pheromone
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < m; ++j) {
				for (int edge_index = 0; edge_index < n; ++edge_index) {
					phi[i][j][edge_index] = delta_phi[i][j][edge_index] + RHO*phi[i][j][edge_index]; //Eq. 2
					delta_phi[i][j][edge_index] = 0; //reset delta_phi
				}
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

