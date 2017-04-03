#pragma once
#include <vector>
#include <algorithm>

struct Task {
	//for å bruke Tasks i jobs og schedule har vi med machine, jobb og task
	float start_time;
	const float process_time;
	const char machine_id;
	const char job_id;
	const char task_id;
	Task(char machine_id_inn, char jobb_id_inn, char task_id_inn, float process_time_inn) :
		machine_id(machine_id_inn),
		job_id(jobb_id_inn),
		task_id(task_id_inn),
		process_time(process_time_inn) {}
	Task(const Task& rhs) :Task(rhs.machine_id, rhs.job_id, rhs.task_id, rhs.process_time) {
		start_time = rhs.start_time;
	}
};

struct Jobs {
private:
	std::vector<std::vector<Task>> jobs;
	std::vector<char> current_job_index;

	int numMachines;
	int numJobs;
public:
	int read_numMachines() { return numMachines; }

	Task& operator[](char job_index) {
		return jobs[job_index][current_job_index[job_index]];
	}
	Task& getPrevTask(char job_index) {
		return jobs[job_index][current_job_index[job_index]-1];
	}
	bool canIndex(char job_index) {
		return  !(current_job_index[job_index] == jobs[job_index].size());
	}
	bool canIndexPrev(char job_index) {
		return current_job_index[job_index];
	}
	void increment(char job_index) {
		current_job_index[job_index]++;
	}

	void reset() {
		for (int i = 0; i < jobs.size(); ++i) {
			current_job_index[i] = 0;
			jobs[i][0].start_time = 0;
		}
	}

	int size() {
		return jobs.size();
	}
	int size(int i) {
		return jobs[i].size();
	}
	bool isFinished() {
		for (int i = 0; i < jobs.size(); ++i) {
			if (current_job_index[i] < jobs[i].size()) {
				return false;
			}
		}
		return true;
	}

	void readFile();

};


void scheduleBuilder(Jobs& jobs, std::vector<char> genom, std::vector<std::vector<Task>>* schedule);
void printSchedule(std::vector<char> genom, Jobs& jobs);