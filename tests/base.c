#include <stdio.h>

#define MAX_PROCESS 100
#define HIGH_PRIORITY 0
#define LOW_PRIORITY 1
#define IO_PRIORITY 2

struct Process {
  int id;
  int arrivalTime;
  int burstTime;
  int completionTime;
  int turnaroundTime;
  int waitingTime;
};

void calculateTimes(struct Process processes[], int n, int quantum) {
  int remainingTime[n];
  for (int i = 0; i < n; i++) {
    remainingTime[i] = processes[i].burstTime;
  }
  int currentTime = 0;
  int allDone = 0;
  while (!allDone) {
    allDone = 1;
    for (int i = 0; i < n; i++) {
      if (remainingTime[i] > 0) {
        allDone = 0;
        if (remainingTime[i] > quantum) {
          currentTime = currentTime + quantum;
          remainingTime[i] = remainingTime[i] - quantum;
        } else {
          currentTime = currentTime + remainingTime[i];
          processes[i].completionTime = currentTime;
          remainingTime[i] = 0;
        }
      }
    }
  }
}

void calculateTurnaroundTime(struct Process processes[], int n) {
  for (int i = 0; i < n; i++)
    processes[i].turnaroundTime = processes[i].completionTime - processes[i].arrivalTime;
}

void calculateWaitingTime(struct Process processes[], int n) {
  for (int i = 0; i < n; i++)
    processes[i].waitingTime = processes[i].turnaroundTime - processes[i].burstTime;
}

void printTable(struct Process processes[], int n) {
  printf("--------------------------------------------------------------------"
          "----------------------\n");
  printf("| Process | Arrival Time | Burst Time | Completion Time | "
          "Turnaround Time | Waiting Time |\n");
  printf("--------------------------------------------------------------------"
          "----------------------\n");
  for (int i = 0; i < n; i++) {
    printf("|    %d    |      %d      |     %d     |        %d        |        %d         |      %d      |\n",
           processes[i].id, processes[i].arrivalTime, processes[i].burstTime,
           processes[i].completionTime, processes[i].turnaroundTime,
           processes[i].waitingTime);
  }
  printf("--------------------------------------------------------------------"
          "----------------------\n");
}

int main() {
  int n, quantum;
  printf("Enter The Number of Process: ");
  scanf("%d", &n);
  printf("Enter The Time Quantum: ");
  scanf("%d", &quantum);

  struct Process processes[n];
  printf("Enter process details:\n");
  for (int i = 0; i < n; i++) {
    printf("Process %d:\n", i + 1);
    processes[i].id = i + 1;
    printf("   Arrival Time: ");
    scanf("%d", &processes[i].arrivalTime);
    printf("   Burst Time: ");
    scanf("%d", &processes[i].burstTime);
  }

  calculateTimes(processes, n, quantum);
  calculateTurnaroundTime(processes, n);
  calculateWaitingTime(processes, n);

  printf("\nRound Robin Scheduling Results:\n");
  printTable(processes, n);

  return 0;
}
