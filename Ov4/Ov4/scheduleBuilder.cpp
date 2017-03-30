#include "scheduleBuilder.h"

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
	for (int i = 0; i < jobs.size(); ++i){
		jobs.current_job_index[i] = 0;
		jobs[i].start_time = 0;
	}


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
