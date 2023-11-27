#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PROCESS 100
#define HIGH_PRIORITY 0
#define LOW_PRIORITY 1
#define DISK_IO_PRIORITY 1
#define TAPE_IO_PRIORITY 0
#define PRINTER_IO_PRIORITY 0
#define TIME_QUANTUM 5

struct Process {
  int id;
  int arrivalTime;
  int burstTime;
  int remainingTime;
  int completionTime;
  int turnaroundTime;
  int waitingTime;
  int priority;
  int ioType;  // 0: None, 1: Disk, 2: Tape, 3: Printer
};

struct Queue {
  struct Process *array[MAX_PROCESS];
  int front, rear;
};

void enqueue(struct Queue *q, struct Process *process) {
  q->rear++;
  q->array[q->rear] = process;
}

struct Process *dequeue(struct Queue *q) {
  struct Process *process = q->array[q->front];
  q->front++;
  return process;
}

int isEmpty(struct Queue *q) {
  return q->front > q->rear;
}

void initializeProcesses(struct Process processes[], int n) {
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
    processes[i].ioType = rand() % 4; // Random I/O type (0 to 3)
  }
}

void calculateTimes(struct Process processes[], int n) {
  struct Queue highPriorityQueue = { .front = 0, .rear = -1 };
  struct Queue lowPriorityQueue = { .front = 0, .rear = -1 };
  struct Queue diskIOQueue = { .front = 0, .rear = -1 };
  struct Queue tapeIOQueue = { .front = 0, .rear = -1 };
  struct Queue printerIOQueue = { .front = 0, .rear = -1 };

  int currentTime = 0;
  int allDone = 0;

  while (!allDone) {
    allDone = 1;

    // Adiciona processos à fila de alta prioridade
    for (int i = 0; i < n; i++) {
      if (processes[i].arrivalTime <= currentTime && processes[i].remainingTime > 0) {
        allDone = 0;
        if (processes[i].priority == HIGH_PRIORITY) {
          enqueue(&highPriorityQueue, &processes[i]);
        } else if (processes[i].priority == LOW_PRIORITY) {
          enqueue(&lowPriorityQueue, &processes[i]);
        } else if (processes[i].priority == DISK_IO_PRIORITY) {
          enqueue(&diskIOQueue, &processes[i]);
        } else if (processes[i].priority == TAPE_IO_PRIORITY) {
          enqueue(&tapeIOQueue, &processes[i]);
        } else if (processes[i].priority == PRINTER_IO_PRIORITY) {
          enqueue(&printerIOQueue, &processes[i]);
        }
      }
    }

    // Executa processos da fila de alta prioridade
    while (!isEmpty(&highPriorityQueue)) {
      struct Process *process = dequeue(&highPriorityQueue);
      int slice = TIME_QUANTUM < process->remainingTime ? TIME_QUANTUM : process->remainingTime;
      currentTime += slice;
      process->remainingTime -= slice;

      // Simula I/O aleatório
      if (process->remainingTime > 0 && rand() % 2 == 0) {
        process->ioType = rand() % 3 + 1; // 1: Disk, 2: Tape, 3: Printer
        process->priority = process->ioType == DISK_IO_PRIORITY ? LOW_PRIORITY : HIGH_PRIORITY;
      }
    }

    // Adiciona processos à fila de baixa prioridade
    while (!isEmpty(&diskIOQueue)) {
      struct Process *process = dequeue(&diskIOQueue);
      process->ioType = 0; // Fim da operação de I/O
      enqueue(&lowPriorityQueue, process);
    }

    // Adiciona processos à fila de alta prioridade
    while (!isEmpty(&tapeIOQueue)) {
      struct Process *process = dequeue(&tapeIOQueue);
      process->ioType = 0; // Fim da operação de I/O
      enqueue(&highPriorityQueue, process);
    }

    // Adiciona processos à fila de alta prioridade
    while (!isEmpty(&printerIOQueue)) {
      struct Process *process = dequeue(&printerIOQueue);
      process->ioType = 0; // Fim da operação de I/O
      enqueue(&highPriorityQueue, process);
    }

    // Executa processos da fila de baixa prioridade
    while (!isEmpty(&lowPriorityQueue)) {
      struct Process *process = dequeue(&lowPriorityQueue);
      int slice = TIME_QUANTUM < process->remainingTime ? TIME_QUANTUM : process->remainingTime;
      currentTime += slice;
      process->remainingTime -= slice;

      // Simula I/O aleatório
      if (process->remainingTime > 0 && rand() % 2 == 0) {
        process->ioType = rand() % 3 + 1; // 1: Disk, 2: Tape, 3: Printer
        process->priority = process->ioType == DISK_IO_PRIORITY ? LOW_PRIORITY : HIGH_PRIORITY;
      }
    }
  }

  // Calcula turnaround e waiting time
  for (int i = 0; i < n; i++) {
    processes[i].turnaroundTime = processes[i].completionTime - processes[i].arrivalTime;
    processes[i].completionTime = processes[i].arrivalTime + processes[i].turnaroundTime;
    processes[i].waitingTime = processes[i].turnaroundTime - processes[i].burstTime;
  }
}

void printTable(struct Process processes[], int n) {
  printf("--------------------------------------------------------------------"
          "----------------------------------------------------------------\n");
  printf("| Processo | Tempo de ativação | Tempo de execução | Tempo de serviço | "
          "Turnaround | Tempo de espera | Prioridade | Tipo de I/O |\n");
  printf("--------------------------------------------------------------------"
          "----------------------------------------------------------------\n");
  for (int i = 0; i < n; i++) {
    printf("|    %d     |          %d        |       %d        |        %d        |        %d         |      %d      |    %d    |     %d    |\n",
           processes[i].id, processes[i].arrivalTime, processes[i].burstTime,
           processes[i].completionTime, processes[i].turnaroundTime,
           processes[i].waitingTime, processes[i].priority, processes[i].ioType);
  }
   printf("--------------------------------------------------------------------"
          "----------------------------------------------------------------\n");
}

int main() {
  int n;
  printf("Quantidade de Processos: ");
  scanf("%d", &n);

  struct Process processes[MAX_PROCESS];
  initializeProcesses(processes, n);

  calculateTimes(processes, n);

  printf("\n Escalonamento Round Robin:\n");
  printTable(processes, n);

  return 0;
}