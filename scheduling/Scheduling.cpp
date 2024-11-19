// perfect code 
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <limits.h>
#include <vector>
#include <queue>
using namespace std;

class Processes {
public:
	string name;
	int arr_time;
	int burst_time;
	int wait_time;
	int turnaround;
	int comp_time;
	int priority;

	Processes() {
    	name = "";
    	arr_time = 0;
    	burst_time = 0;
    	wait_time = 0;
    	turnaround = 0;
    	comp_time = 0;
    	priority = 0;
	}

	void create() {
    	cout << "Enter the Process name: ";
    	cin >> name;
    	cout << "Arrival Time: ";
    	cin >> arr_time;
    	cout << "Burst Time: ";
    	cin >> burst_time;
	}

	friend class Scheduler;
};

class Scheduler {
private:
	Processes *process;
	int n;

public:
	Scheduler() {
    	cout << "Enter the number of processes: ";
    	cin >> n;
    	process = new Processes[n];

    	for (int i = 0; i < n; i++) {
        	process[i].create();
        	cout << endl;
    	}
	}

	void display(Processes process[], int n) {
    	cout << "Process\tAT\tBT\tT_AT\tWT" << endl;

    	for (int i = 0; i < n; i++) {
        	cout << process[i].name << "\t" << process[i].arr_time << "\t"
             	<< process[i].burst_time << "\t" << process[i].turnaround
             	<< "\t" << process[i].wait_time << endl;
    	}
	}

	void average(float avg_tat, float avg_wt) {
    	cout << "\nAverage Turnaround Time:" << avg_tat
         	<< "\nAverage Waiting Time: " << avg_wt;
	}

	void FCFS() {
    	Processes temp[n];

    	for (int i = 0; i < n; i++) {
        	temp[i] = process[i];
    	}

    	for (int i = 0; i < n - 1; i++) {
        	int min = i;
        	for (int j = i + 1; j < n; j++) {
            	if (temp[j].arr_time < temp[min].arr_time) {
                	min = j;
            	}
        	}

        	if (min != i) {
            	swap(temp[i], temp[min]);
        	}
    	}

    	int ct = 0;
    	Processes gantt[n];

    	for (int i = 0; i < n; i++) {
        	gantt[i] = temp[i];
        	ct += gantt[i].burst_time;

        	gantt[i].comp_time = ct;
        	gantt[i].turnaround = gantt[i].comp_time - gantt[i].arr_time;
        	gantt[i].wait_time = gantt[i].turnaround - gantt[i].burst_time;
    	}

    	float avg_tat = 0, avg_wt = 0;
    	for (int i = 0; i < n; i++) {
        	avg_tat += gantt[i].turnaround;
        	avg_wt += gantt[i].wait_time;
    	}
    	avg_tat = avg_tat / n;
    	avg_wt = avg_wt / n;

    	display(gantt, n);
    	average(avg_tat, avg_wt);
	}

//	void SJF_non_preemptive() {
//    	Processes temp[n];
//
//    	for (int i = 0; i < n; i++) {
//        	temp[i] = process[i];
//    	}
//
//    	sort(temp, temp + n, [](const Processes &a, const Processes &b) {
//        	return a.burst_time < b.burst_time;
//    	});
//
//    	int ct = 0;
//    	Processes gantt[n];
//
//    	for (int i = 0; i < n; i++) {
//        	gantt[i] = temp[i];
//        	ct += gantt[i].burst_time;
//
//        	gantt[i].comp_time = ct;
//        	gantt[i].turnaround = gantt[i].comp_time - gantt[i].arr_time;
//        	gantt[i].wait_time = gantt[i].turnaround - gantt[i].burst_time;
//    	}
//
//    	float avg_tat = 0, avg_wt = 0;
//    	for (int i = 0; i < n; i++) {
//        	avg_tat += gantt[i].turnaround;
//        	avg_wt += gantt[i].wait_time;
//    	}
//    	avg_tat = avg_tat / n;
//    	avg_wt = avg_wt / n;
//
//    	display(gantt, n);
//    	average(avg_tat, avg_wt);
//	}

	void SJF_preemptive() {
   	 Processes *v = new Processes[n];
   	 int totalTime = 0;
   	 for (int i = 0; i < n; i++)
   	 {
   		 v[i] = process[i];
   		 totalTime += v[i].burst_time;
   	 }

   	 // Sort (Arrival Time)
   	 sort(v, v + n, [](const Processes &lhs, const Processes &rhs)
   			 { return lhs.arr_time < rhs.arr_time; });

   	 vector<string> gantt;
   	 int aj = 0;
   	 int timeStamp = 0;
   	 vector<Processes> readyQ;

   	 while (timeStamp < totalTime)
   	 {
   		 while (aj < n && v[aj].arr_time <= timeStamp)
   		 {
   			 readyQ.push_back(v[aj]);
   			 aj++;
   		 }

   		 sort(readyQ.begin(), readyQ.end(), [](const Processes &lhs, const Processes &rhs)
   				 { return lhs.burst_time < rhs.burst_time; });

   	         	if (readyQ.size() > 0)
   	         	{
   	             	gantt.push_back(readyQ[0].name);
   	             	readyQ[0].burst_time--;

   	             	if (readyQ[0].burst_time == 0)
   	             	{
   	                 	for (int i = 0; i < n; i++)
   	                 	{
   	                     	if (process[i].name == readyQ[0].name)
   	                     	{
   	                         	process[i].comp_time = timeStamp + 1;
   	                         	process[i].turnaround = process[i].comp_time - process[i].arr_time;
   	                         	process[i].wait_time = process[i].turnaround - process[i].burst_time;
   	                     	}
   	                 	}

   	                 	readyQ.erase(readyQ.begin());
   	             	}
   	         	}

   	         	timeStamp++;
   	     	}

   	     	float avg_tat = 0, avg_wt = 0;

   	     	for (int i = 0; i < n; i++)
   	     	{
   	         	avg_tat += process[i].turnaround;
   	         	avg_wt += process[i].wait_time;
   	     	}

   	     	avg_tat = avg_tat / n;
   	     	avg_wt = avg_wt / n;
   	     	display(process, n);
   	     	average(avg_tat, avg_wt);
   	 	}

	void RoundRobin(int time_quantum){
    	Processes temp[n];
    	for(int i=0; i<n; i++){
        	temp[i] = process[i];
    	}

    	int ct = 0;
    	int remaining_processes = n;
    	queue<int> ready_queue;

    	int current_time = 0;
    	int completed_processes = 0;

    	while(completed_processes < n){
        	for(int i=0; i<n; i++){
            	if(temp[i].burst_time > 0 && temp[i].arr_time <= current_time){
                	ready_queue.push(i);
            	}
        	}

        	if(ready_queue.empty()){
            	current_time++;
            	continue;
        	}

        	int curr_process = ready_queue.front();
        	ready_queue.pop();

        	if(temp[curr_process].burst_time > time_quantum){
            	temp[curr_process].burst_time -= time_quantum;
            	current_time += time_quantum;
        	} else {
            	current_time += temp[curr_process].burst_time;
            	temp[curr_process].burst_time = 0;
            	temp[curr_process].comp_time = current_time;
            	completed_processes++;
        	}
    	}

    	Processes gantt[n];
    	float avg_tat = 0, avg_wt = 0;

    	for(int i=0; i<n; i++){
        	gantt[i] = temp[i];
        	gantt[i].turnaround = gantt[i].comp_time - gantt[i].arr_time;
        	gantt[i].wait_time = gantt[i].turnaround - gantt[i].burst_time;

        	avg_tat += gantt[i].turnaround;
        	avg_wt += gantt[i].wait_time;
    	}

    	avg_tat /= n;
    	avg_wt /= n;

    	display(gantt, n);
    	average(avg_tat, avg_wt);
	}

	void Priority(){
   	 Processes temp[n];

   	     	for (int i = 0; i < n; i++) {
   	         	temp[i] = process[i];
   	     	}

   	     	sort(temp, temp + n, [](const Processes &a, const Processes &b) {
   	         	return a.priority < b.priority;
   	     	});

   	     	int ct = 0;
   	     	Processes gantt[n];

   	     	for (int i = 0; i < n; i++) {
   	         	gantt[i] = temp[i];
   	         	ct += gantt[i].burst_time;

   	         	gantt[i].comp_time = ct;
   	         	gantt[i].turnaround = gantt[i].comp_time - gantt[i].arr_time;
   	         	gantt[i].wait_time = gantt[i].turnaround - gantt[i].burst_time;
   	     	}

   	     	float avg_tat = 0, avg_wt = 0;
   	     	for (int i = 0; i < n; i++) {
   	         	avg_tat += gantt[i].turnaround;
   	         	avg_wt += gantt[i].wait_time;
   	     	}
   	     	avg_tat = avg_tat / n;
   	     	avg_wt = avg_wt / n;

   	     	display(gantt, n);
   	     	average(avg_tat, avg_wt);
	}

	~Scheduler() {
    	delete[] process;
	}
};

int main() {
	Scheduler scheduler;

	cout << "......... Scheduling Algorithm ........." << endl;
	cout << "1. FCFS" << endl;
	scheduler.FCFS();
	/*cout << "2. SJF Non-preemptive" << endl;
	scheduler.SJF_non_preemptive();
	*/cout << "\n2. SJF Preemptive" << endl;
	scheduler.SJF_preemptive();
	cout << "\n3. Round Robin" << endl;
	int time_quantum;
	cout << "\nEnter Time Quantum for Round Robin: ";
	cin >> time_quantum;
	scheduler.RoundRobin(time_quantum);
	
	return 0;

}


