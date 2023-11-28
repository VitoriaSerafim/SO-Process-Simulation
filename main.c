#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PROCESS 100
#define HIGH_PRIORITY 0
#define LOW_PRIORITY 1

#define DISK_IO_PRIORITY 2
#define TAPE_IO_PRIORITY 3
#define PRINTER_IO_PRIORITY 4

#define DISK_TIME 2
#define TAPE_TIME 3
#define PRINTER_TIME 4

#define TIME_QUANTUM 3

struct Process
{
  int id;
  int arrivalTime;
  int burstTime;
  int remainingTime;
  int completionTime;
  int turnaroundTime;
  int waitingTime;
  int priority;
  int ioType; // 0: Nenhum, 1: Disco, 2: Fita, 3: Impressora
};

struct Queue
{
  struct Process *array[MAX_PROCESS];
  int front, rear;
};

void enqueue(struct Queue *q, struct Process *process)
{
  q->rear++;
  q->array[q->rear] = process;
}

struct Process *dequeue(struct Queue *q)
{
  struct Process *process = q->array[q->front];
  q->front++;
  return process;
}

int isEmpty(struct Queue *q)
{
  return q->front > q->rear;
}

void initializeProcesses(struct Process processes[], int n)
{
  
  srand(time(NULL));

  for (int i = 0; i < n; i++) {
    processes[i].id = i + 1;
    processes[i].arrivalTime = rand() % 10; // Random arrival time (0 to 9)
    processes[i].burstTime = rand() % 20 + 1; // Random burst time (1 to 20)
    processes[i].remainingTime = processes[i].burstTime;
    processes[i].completionTime = 0;
    processes[i].turnaroundTime = 0;
    processes[i].waitingTime = 0;
    processes[i].priority = HIGH_PRIORITY; // New processes start with high priority
    processes[i].ioType = rand() % 3 + 1; // Random I/O type
  }
  

  // processes[0].id = 1;
  // processes[0].arrivalTime = 0; // Random arrival time (0 to 9)
  // processes[0].burstTime = 4;   // Random burst time (1 to 20)
  // processes[0].remainingTime = 4;
  // processes[0].completionTime = 0;
  // processes[0].turnaroundTime = 0;
  // processes[0].waitingTime = 0;
  // processes[0].priority = HIGH_PRIORITY; // New processes start with high priority
  // processes[0].ioType = 2;               // Random I/O type

  // processes[1].id = 2;
  // processes[1].arrivalTime = 1; // Random arrival time (0 to 9)
  // processes[1].burstTime = 5;   // Random burst time (1 to 20)
  // processes[1].remainingTime = processes[1].burstTime;
  // processes[1].completionTime = 0;
  // processes[1].turnaroundTime = 0;
  // processes[1].waitingTime = 0;
  // processes[1].priority = HIGH_PRIORITY; // New processes start with high priority
  // processes[1].ioType = 3;               // Random I/O type

  // processes[2].id = 3;
  // processes[2].arrivalTime = 2; // Random arrival time (0 to 9)
  // processes[2].burstTime = 2;   // Random burst time (1 to 20)
  // processes[2].remainingTime = processes[2].burstTime;
  // processes[2].completionTime = 0;
  // processes[2].turnaroundTime = 0;
  // processes[2].waitingTime = 0;
  // processes[2].priority = HIGH_PRIORITY; // New processes start with high priority
  // processes[2].ioType = 4;               // Random I/O type
}

void calculateTimes(struct Process processes[], int n, int quantum)
{
  struct Queue highPriorityQueue = {.front = 0, .rear = -1};
  struct Queue lowPriorityQueue = {.front = 0, .rear = -1};
  struct Queue diskIOQueue = {.front = 0, .rear = -1};
  struct Queue tapeIOQueue = {.front = 0, .rear = -1};
  struct Queue printerIOQueue = {.front = 0, .rear = -1};

  int remainingTime[n];
  
  static int disc = 0, tape = 0, printer = 0;
  
  for (int i = 0; i < n; i++)
  {
    remainingTime[i] = processes[i].burstTime;
  }
  
  int currentTime = 0;
  int allDone = 0;
  
  struct Process *process;
  
  while (!allDone)
  {
    allDone = 1;

    for (int i = 0; i < n; i++)
    {
      if (remainingTime[i] > 0)
      {
        allDone = 0;
        if (remainingTime[i] > 0)
        {

          if (processes[i].arrivalTime <= currentTime && processes[i].remainingTime > 0)
          {
            if (processes[i].ioType == DISK_IO_PRIORITY)
            {
              enqueue(&diskIOQueue, &processes[i]);
            }
            else if (processes[i].ioType == TAPE_IO_PRIORITY)
            {
              enqueue(&tapeIOQueue, &processes[i]);
            }
            else if (processes[i].ioType == PRINTER_IO_PRIORITY)
            {
              enqueue(&printerIOQueue, &processes[i]);
            }
            else if (processes[i].priority == HIGH_PRIORITY)
            {
              enqueue(&highPriorityQueue, &processes[i]);
            }
            else if (processes[i].priority == LOW_PRIORITY)
            {
              enqueue(&lowPriorityQueue, &processes[i]);
            }
          }
        }
        // Executa processos da fila de alta prioridade
        if (!isEmpty(&highPriorityQueue))
        {
          process = dequeue(&highPriorityQueue);
        }
        else if (!isEmpty(&diskIOQueue))
        {
          disc += 1;
          if (disc % DISK_TIME == 0)
          {
            process = dequeue(&diskIOQueue);
            process->ioType = 0; // Fim da operação de I/O
            enqueue(&lowPriorityQueue, process);
          }
        }
        else if (!isEmpty(&tapeIOQueue))
        {
          tape += 1;
          if (tape % TAPE_TIME == 0)
          {
            process = dequeue(&tapeIOQueue);
            process->ioType = 0; // Fim da operação de I/O
            enqueue(&highPriorityQueue, process);
          }
        }
        else if (!isEmpty(&printerIOQueue))
        {
          printer += 1;
          if (printer % PRINTER_TIME == 0)
          {
            process = dequeue(&printerIOQueue);
            process->ioType = 0; // Fim da operação de I/O
            enqueue(&highPriorityQueue, process);
          }
        }
        else if (!isEmpty(&lowPriorityQueue))
        {
          process = dequeue(&lowPriorityQueue);
        }

        if (remainingTime[i] > quantum)
        {
          currentTime = currentTime + quantum;
          remainingTime[i] = remainingTime[i] - quantum;
        }
        else
        {
          currentTime = currentTime + remainingTime[i];
          processes[i].completionTime = currentTime;
          remainingTime[i] = 0;
        }
      }
    }
  }
}


void calculateTurnaroundTime(struct Process processes[], int n)
{
  for (int i = 0; i < n; i++)
    processes[i].turnaroundTime = processes[i].completionTime - processes[i].arrivalTime;
}

void calculateWaitingTime(struct Process processes[], int n)
{
  for (int i = 0; i < n; i++)
    processes[i].waitingTime = processes[i].turnaroundTime - processes[i].burstTime;
}

void printTable(struct Process processes[], int n)
{
  printf("--------------------------------------------------------------------"
         "--------------------------------------- \n");
  printf("| Processo | Tempo de ativacao | Tempo de execucao | Tempo de conclusao | "
         "Turnaround | Tempo de espera |\n");
 printf("--------------------------------------------------------------------"
         "--------------------------------------- \n");
  for (int i = 0; i < n; i++)
  {
    printf("|    %d     |          %-2d        |         %-2d        |        %-2d        |        %-2d         |      %-2d      |\n",
           processes[i].id, processes[i].arrivalTime, processes[i].burstTime,
           processes[i].completionTime, processes[i].turnaroundTime,
           processes[i].waitingTime);
  }
 printf("--------------------------------------------------------------------"
         "--------------------------------------- \n");
}

int main()
{
  int n;
  printf("Quantidade de Processos: ");
  scanf("%d", &n);

  struct Process processes[MAX_PROCESS];
  initializeProcesses(processes, n);

  calculateTimes(processes, n, TIME_QUANTUM);
  calculateTurnaroundTime(processes, n);
  calculateWaitingTime(processes, n);

  printf("\n Escalonamento Round Robin:\n");
  printTable(processes, n);

  return 0;
}