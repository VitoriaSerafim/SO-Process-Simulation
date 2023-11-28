#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PROCESS 100
#define HIGH_PRIORITY 0
#define LOW_PRIORITY 1

#define DISK_IO_PRIORITY 2
#define TAPE_IO_PRIORITY 3
#define PRINTER_IO_PRIORITY 4

#define DISK_TIME 3
#define TAPE_TIME 2
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
  /*
  srand(time(NULL));
  int IO[] = {DISK_IO_PRIORITY, TAPE_IO_PRIORITY, PRINTER_IO_PRIORITY};

  for (int i = 0; i < n; i++) {
    processes[i].id = i + 1;
    processes[i].arrivalTime = rand() % 10; // Random arrival time (0 to 9)
    processes[i].burstTime = rand() % 20 + 1; // Random burst time (1 to 20)
    processes[i].remainingTime = processes[i].burstTime;
    processes[i].completionTime = 0;
    processes[i].turnaroundTime = 0;
    processes[i].waitingTime = 0;
    processes[i].priority = HIGH_PRIORITY; // New processes start with high priority
    processes[i].ioType = IO[rand() % 3]; // Random I/O type
  }
  */

  processes[0].id = 1;
  processes[0].arrivalTime = 0; // Random arrival time (0 to 9)
  processes[0].burstTime = 4;   // Random burst time (1 to 20)
  processes[0].remainingTime = 4;
  processes[0].completionTime = 0;
  processes[0].turnaroundTime = 0;
  processes[0].waitingTime = 0;
  processes[0].priority = HIGH_PRIORITY; // New processes start with high priority
  processes[0].ioType = 2;               // Random I/O type

  processes[1].id = 2;
  processes[1].arrivalTime = 1; // Random arrival time (0 to 9)
  processes[1].burstTime = 5;   // Random burst time (1 to 20)
  processes[1].remainingTime = processes[1].burstTime;
  processes[1].completionTime = 0;
  processes[1].turnaroundTime = 0;
  processes[1].waitingTime = 0;
  processes[1].priority = HIGH_PRIORITY; // New processes start with high priority
  processes[1].ioType = 3;               // Random I/O type

  processes[2].id = 3;
  processes[2].arrivalTime = 2; // Random arrival time (0 to 9)
  processes[2].burstTime = 2;   // Random burst time (1 to 20)
  processes[2].remainingTime = processes[2].burstTime;
  processes[2].completionTime = 0;
  processes[2].turnaroundTime = 0;
  processes[2].waitingTime = 0;
  processes[2].priority = HIGH_PRIORITY; // New processes start with high priority
  processes[2].ioType = 4;               // Random I/O type
}

void treatIO(struct Process process[], int n, struct Queue lowPriorityQueue)
{
  static int disc = 0, print = 0, tape = 0;

  if (process->ioType == 2)
  {
    disc = +1;
    enqueue(&lowPriorityQueue, process);
  }
}

void calculateTimes(struct Process processes[], int n)
{
  struct Queue highPriorityQueue = {.front = 0, .rear = -1};
  struct Queue lowPriorityQueue = {.front = 0, .rear = -1};
  struct Queue diskIOQueue = {.front = 0, .rear = -1};
  struct Queue tapeIOQueue = {.front = 0, .rear = -1};
  struct Queue printerIOQueue = {.front = 0, .rear = -1};

  int currentTime = 0;
  int allDone = 0;
  static int disc = 0, tape = 0, printer = 0;

  while (!allDone)
  {
    allDone = 1;

    for (int i = 0; i < n; i++)
    {

      if (processes[i].arrivalTime <= currentTime && processes[i].remainingTime > 0)
      {
        allDone = 0;
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
    while (!isEmpty(&highPriorityQueue))
    {
      struct Process *process = dequeue(&highPriorityQueue);
      // int slice = TIME_QUANTUM < process->remainingTime ? TIME_QUANTUM : process->remainingTime;
      // currentTime += slice;
      // process->remainingTime -= slice;
      if (process->remainingTime > 0)
      {

        if (process->remainingTime > TIME_QUANTUM)
        {
          currentTime = currentTime + TIME_QUANTUM;
          process->remainingTime = process->remainingTime - TIME_QUANTUM;
        }
        else
        {
          currentTime = currentTime + process->remainingTime;
          processes->completionTime = currentTime;
          process->remainingTime = 0;
        }
      }
      // if (process->remainingTime > 0) {
      //   // process->ioType = rand() % 3 + 2; // 2: Disk, 3: Tape, 4: Printer
      //   process->priority = process->ioType == DISK_IO_PRIORITY ? LOW_PRIORITY : HIGH_PRIORITY;
      // }
    }

    // Adiciona processos à fila de baixa prioridade
    if (!isEmpty(&diskIOQueue))
    {
      disc += 1;
      if (disc % DISK_TIME == 0)
      {
        struct Process *process = dequeue(&diskIOQueue);
         process->ioType = 0; // Fim da operação de I/O
        enqueue(&lowPriorityQueue, process);
      }
    }

    // Adiciona processos à fila de alta prioridade
    if (!isEmpty(&tapeIOQueue))
    {
      tape += 1;
      if (tape % TAPE_TIME == 0)
      {
        struct Process *process = dequeue(&tapeIOQueue);
         process->ioType = 0; // Fim da operação de I/O
        enqueue(&highPriorityQueue, process);
      }
    }

    // Adiciona processos à fila de alta prioridade
    if (!isEmpty(&printerIOQueue))
    {
      printer += 1;
      if (printer % PRINTER_TIME == 0)
      {
        struct Process *process = dequeue(&printerIOQueue);
         process->ioType = 0; // Fim da operação de I/O
        enqueue(&highPriorityQueue, process);
      }
    }

    // Executa processos da fila de baixa prioridade
    while (!isEmpty(&lowPriorityQueue))
    {
      struct Process *process = dequeue(&lowPriorityQueue);
      // int slice = TIME_QUANTUM < process->remainingTime ? TIME_QUANTUM : process->remainingTime;
      // currentTime += slice;
      // process->remainingTime -= slice;

      if (process->remainingTime > 0)
      {

        if (process->remainingTime > TIME_QUANTUM)
        {
          currentTime = currentTime + TIME_QUANTUM;
          process->remainingTime = process->remainingTime - TIME_QUANTUM;
        }
        else
        {
          currentTime = currentTime + process->remainingTime;
          processes->completionTime = currentTime;
          process->remainingTime = 0;
        }
      }
      // if (process->remainingTime == 0)
      // {
      //   process->turnaroundTime = currentTime - process->arrivalTime;
      // }

      // // Simula I/O aleatório
      // if (process->remainingTime > 0 && rand() % 2 == 0) {
      //   process->ioType = rand() % 3 + 1; // 1: Disk, 2: Tape, 3: Printer
      //   process->priority = process->ioType == DISK_IO_PRIORITY ? LOW_PRIORITY : HIGH_PRIORITY;
      // }
    }
  }

  // // Calcula turnaround e waiting time
  // for (int i = 0; i < n; i++)
  // {
  //   // processes[i].turnaroundTime = processes[i].completionTime - processes[i].arrivalTime;
  //   processes[i].completionTime = processes[i].arrivalTime + processes[i].turnaroundTime;
  //   processes[i].waitingTime = processes[i].turnaroundTime - processes[i].burstTime;
  // }
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
         "----------------------------------------------------------------\n");
  printf("| Processo | Tempo de ativacao | Tempo de execucao | Tempo de conclusao | "
         "Turnaround | Tempo de espera | Prioridade | Tipo de I/O |\n");
  printf("--------------------------------------------------------------------"
         "----------------------------------------------------------------\n");
  for (int i = 0; i < n; i++)
  {
    printf("|    %d     |          %d        |       %d        |        %d        |        %d         |      %d      |    %d    |     %d    |\n",
           processes[i].id, processes[i].arrivalTime, processes[i].burstTime,
           processes[i].completionTime, processes[i].turnaroundTime,
           processes[i].waitingTime, processes[i].priority, processes[i].ioType);
  }
  printf("--------------------------------------------------------------------"
         "----------------------------------------------------------------\n");
}

int main()
{
  int n = 3;
  // printf("Quantidade de Processos: ");
  // scanf("%d", &n);

  struct Process processes[MAX_PROCESS];
  initializeProcesses(processes, 3);

  calculateTimes(processes, n);
  // calculateTurnaroundTime(processes, n);
  // calculateWaitingTime(processes, n);

  printf("\n Escalonamento Round Robin:\n");
  printTable(processes, n);

  return 0;
}