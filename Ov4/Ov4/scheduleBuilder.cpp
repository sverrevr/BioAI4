#include "scheduleBuilder.h"
#include <iostream>
#include <Windows.h>

using namespace std;


//Jobs må være constructed med current_job_index
//schedule må være constructed til å ha riktig antall maskiner
void scheduleBuilder(Jobs& jobs, vector<char> genom, vector<vector<Task>>* schedule) {
	// Vi skal endre på genomet lokalt så må taes inn som copy

	//tømmer schedulen
	for (auto it = schedule->begin(); it != schedule->end(); ++it) {
		it->clear();
	}

	//resetter jobs
	jobs.reset();


	while (!jobs.isFinished()) {
		//kalkuler start_time
		for (int i = 0; i < jobs.size(); ++i) {
			//hvis vi har fullført denne jobben, hopp til neste
			if (!jobs.canIndex(i)) continue;

			float job_end_time = 0;
			float machine_end_time = 0;
			//Hvis det er oppgaver før denne i denne jobben
			if (jobs.canIndexPrev(i)) {
				//Skriv ned når den fullførte
				job_end_time = jobs.getPrevTask(i).start_time + jobs.getPrevTask(i).process_time;
			}
			//hvis det er jobber lagt til på denne maskinen
			int machine_size = schedule->at(jobs[i].machine_id).size();
			if (machine_size) {
				//skriv ned når maskinen blir ledig
				machine_end_time = schedule->at(jobs[i].machine_id)[machine_size - 1].start_time +
					schedule->at(jobs[i].machine_id)[machine_size - 1].process_time;
			}
			jobs[i].start_time = max(job_end_time, machine_end_time);
		}

		//finn jobben som fullfører først
		char earliest_job_id = 0;
		float earliest_job_end = INFINITY;
		for (int i = 0; i < jobs.size(); ++i) {
			//hvis vi har fullført denne jobben, hopp til neste
			if (!jobs.canIndex(i)) continue;

			if (jobs[i].start_time + jobs[i].process_time < earliest_job_end) {
				earliest_job_id = i;
				earliest_job_end = jobs[i].start_time + jobs[i].process_time;
			}
		}

		//finn alle jobbene som kræsjer på maskin og tidspunkt
		//inklusive den beste
		vector<char> colliding_jobs_id;
		for (int i = 0; i < jobs.size(); ++i) {
			//hvis vi har fullført denne jobben, hopp til neste
			if (!jobs.canIndex(i)) continue;

			//drop jobs that are on another machine
			if (jobs[i].machine_id != jobs[earliest_job_id].machine_id) continue;

			//add the job if it wants to start before earliest_job_end
			if (jobs[i].start_time < earliest_job_end) {
				colliding_jobs_id.push_back(i);
			}
		}

		//finn den jobben blant colliding som forekommer først i genomet
		int genoId = 0;
		for (; genoId < genom.size(); ++genoId) {
			if (find(colliding_jobs_id.begin(), colliding_jobs_id.end(), genom[genoId]) != colliding_jobs_id.end()) {
				break;
				//TEST er det noen tilfeller denne aldri breaker, og genoID bare blir siste mulige?
			}
		}

		//legg til tasken i schedulen
		schedule->at(jobs[genom[genoId]].machine_id).push_back(jobs[genom[genoId]]);

		//inkrementer jobben som ble lagt til
		jobs.increment(genom[genoId]);

		//fjern indexen fra genomet
		genom.erase(genom.begin() + genoId);

	}

}

void printSchedule(vector<char> genom,Jobs& jobs) {
	vector<vector<Task>> schedule(jobs.read_numMachines(), vector<Task>());
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	scheduleBuilder(jobs, genom, &schedule);

	float worstFinishTime = 0;
	for (auto machine = schedule.begin(); machine != schedule.end(); ++machine) {
		if (machine->at(machine->size()-1).start_time + machine->at(machine->size()-1).process_time > worstFinishTime) {
			worstFinishTime = machine->at(machine->size()-1).start_time + machine->at(machine->size()-1).process_time;
		}
	}

	cout << "Finish time: " << worstFinishTime << endl;

	int t;
	int dt=INT_MAX;

	//finn minste tidssteg som korteste duration
	/*for (int i = 0; i < schedule.size(); ++i) {
		for (int j = 0; j < schedule[i].size(); ++j) {
			if (schedule[i][j].process_time < dt) {
				dt = schedule[i][j].process_time;
			}
		}
	}
	*/
	dt = 1; //blir domt med partall oddetall mismatch

	char out;
	for (int i = 0; i < schedule.size(); ++i) {
		t = 0;
		for (int j = 0; j < schedule[i].size(); ++j) {
			while (t < schedule[i][j].start_time) {
				SetConsoleTextAttribute(hConsole, 15);
				cout << "  ";
				t+=dt;
			}
			while (t < schedule[i][j].start_time + schedule[i][j].process_time) {
				t+=dt;
				SetConsoleTextAttribute(hConsole, 15 - (schedule[i][j].job_id % 15));
				if (schedule[i][j].job_id <= 9) {
					out = schedule[i][j].job_id + '0';
				}
				else {
					out = schedule[i][j].job_id - 10 + 'a';
				}
				cout << out << ' ';
			}
		}
		cout << endl;
	}
	SetConsoleTextAttribute(hConsole, 15);
}
