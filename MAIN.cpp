#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>

using namespace std;

struct Job
{
	int id;
	vector<int> p;
	int d;
};

/* loadData(configs, demand): load data from file */
void loadData(int& m, vector<Job>& jobs) {
	FILE* file;
	Job job;
	int id = 0;

	fopen_s(&file, "jobs.dat", "rt");
	fscanf_s(file, "%d\n", &m);
	job.p = vector<int>( m, 0 );
	job.d = 0;

	while (!feof(file))
	{
		job.id = id;
		jobs.push_back(job);
		for (int i = 0; i < m; i++)
		{
			fscanf_s(file, "%d ", &jobs.back().p[i]);
		}
		fscanf_s(file, "%d\n", &jobs.back().d);
		id++;
	}
	fclose(file);
}

/* printJobs()jobs */
void printJobs(int m, vector<Job> jobs) {
	for (int i = 0; i < jobs.size(); i++)
	{
		printf_s("job %d - pt: ", jobs[i].id);
		for (int k = 0; k < m; k++)
		{
			printf_s("%3d ", jobs[i].p[k]);
		}
		printf_s(", d: %4d\n", jobs[i].d);
	}
}

/* computeTT(jobs, makespan): computes schedule's total tardiness */
int computeTT(int m, vector<Job> jobs) {
	vector<vector<int>> D = vector<vector<int>>(m - 1, vector<int>(jobs.size(), 0));
	int makespan = 0;
	int TT = 0;
	int C;
	
	//fill D and compute makespan
	for (int i = 0; i < m - 1; i++)
	{
		D[i][0] = jobs[0].p[i + 1];
		for (int k = 1; k < jobs.size(); k++)
		{
			D[i][k] = max(D[i][k - 1] - jobs[k].p[i], 0) + jobs[k].p[i + 1];
			if (k == jobs.size() - 1)
				makespan += D[i][k];
		}
	}
	for (int i = 0; i < jobs.size(); i++)
	{
		makespan += jobs[i].p[0];
	}

	// compute total tardiness
	C = makespan;
	for (int i = jobs.size() - 1; i >= 0; i--)
	{
		TT += max(C - jobs[i].d, 0);
		C -= jobs[i].p[m - 1];
	}
	return TT;
}

bool comparatorSPT(const Job& l, const Job& r) {
	int m = l.p.size() - 1;
	return l.p[m] < r.p[m];
}

bool comparatorEDD(const Job& l, const Job& r) {
	int m = l.p.size() - 1;
	return l.d < r.d;
}

/****   MAIN: load data and solve the problem              ******/
void main(void)
{
	int m = 0, makespan = 0;
	vector<Job> jobs;
	vector<vector<int>> D; // matrix [machine i to i+1][job j] of minimum completion time difference of jobs from one machine to the other

	loadData(m, jobs);

	printJobs(m, jobs);

	printf_s("Total Tardiness: %d\n", computeTT(m, jobs));

	sort(jobs.begin(), jobs.end(), &comparatorEDD);

	printJobs(m, jobs);

	printf_s("Total Tardiness: %d\n", computeTT(m, jobs));

	sort(jobs.begin(), jobs.end(), &comparatorSPT);

	printJobs(m, jobs);

	printf_s("Total Tardiness: %d\n", computeTT(m, jobs));

}