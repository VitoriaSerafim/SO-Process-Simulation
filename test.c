#include <stdio.h>

// Algumas definições feitas pelo grupo
#define MAX_PROCESS 100 // máximo de 100 processos para o escalonador
#define HIGH_PRIORITY 0
#define LOW_PRIORITY 1
#define DISK_IO_PRIORITY 2
#define TAPE_IO_PRIORITY 3
#define PRINTER_IO_PRIORITY 4

struct Process {
  int id; // PID
  int arrivalTime; // tempo de chegada
  int burstTime; // tempo de execução
  int completionTime;
  int turnaroundTime;
  int waitingTime; //tempo de espera
  int priority; //prioridade do processo
};

// Estrutura para representar uma fila de processos
struct Fila {
  struct Process *array[MAX_PROCESS];
  int prox, ultimo;
};

// Funções para manipulação de filas
void Enfila(struct Fila *f, struct Process *process) {
  f->ultimo++;
  f->array[f->ultimo] = process;
}

struct Process *enfila(struct Fila *f) {
  struct Process *process = f->array[f->prox];
  f->prox++;
  return process;
}

int isEmpty(struct Fila *f) {
  return f->prox > f->ultimo;
}

void calculateTimes(struct Process processes[], int n, int fuantum) {
  struct Fila highPriorityFila = { .prox = 0, .ultimo = -1 };
  struct Fila lowPriorityFila = { .prox = 0, .ultimo = -1 };
  struct Fila diskIOFila = { .prox = 0, .ultimo = -1 };
  struct Fila tapeIOFila = { .prox = 0, .ultimo = -1 };
  struct Fila printerIOFila = { .prox = 0, .ultimo = -1 };

  int remainingTime[n];
  for (int i = 0; i < n; i++) {
    remainingTime[i] = processes[i].burstTime;
  }

  int currentTime = 0;
  int allDone = 0;

  while (!allDone) {
    allDone = 1;

    // Adiciona processos à fila de alta prioridade
    for (int i = 0; i < n; i++) {
      if (processes[i].arrivalTime <= currentTime && remainingTime[i] > 0) {
        allDone = 0;
        if (processes[i].priority == HIGH_PRIORITY) {
          Enfila(&highPriorityFila, &processes[i]);
        } else if (processes[i].priority == LOW_PRIORITY) {
          Enfila(&lowPriorityFila, &processes[i]);
        } else if (processes[i].priority == DISK_IO_PRIORITY) {
          Enfila(&diskIOFila, &processes[i]);
        } else if (processes[i].priority == TAPE_IO_PRIORITY) {
          Enfila(&tapeIOFila, &processes[i]);
        } else if (processes[i].priority == PRINTER_IO_PRIORITY) {
          Enfila(&printerIOFila, &processes[i]);
        }
      }
    }

    // Executa processos da fila de alta prioridade
    while (!isEmpty(&highPriorityFila)) {
      struct Process *process = enfila(&highPriorityFila);
      int slice = fuantum < remainingTime[process->id - 1] ? fuantum : remainingTime[process->id - 1];
      currentTime += slice;
      remainingTime[process->id - 1] -= slice;
    }

    // Adiciona processos à fila de baixa prioridade
    while (!isEmpty(&diskIOFila)) {
      struct Process *process = enfila(&diskIOFila);
      Enfila(&lowPriorityFila, process);
    }

    // Adiciona processos à fila de alta prioridade
    while (!isEmpty(&tapeIOFila)) {
      struct Process *process = enfila(&tapeIOFila);
      Enfila(&highPriorityFila, process);
    }

    // Adiciona processos à fila de alta prioridade
    while (!isEmpty(&printerIOFila)) {
      struct Process *process = enfila(&printerIOFila);
      Enfila(&highPriorityFila, process);
    }

    // Executa processos da fila de baixa prioridade
    while (!isEmpty(&lowPriorityFila)) {
      struct Process *process = enfila(&lowPriorityFila);
      int slice = fuantum < remainingTime[process->id - 1] ? fuantum : remainingTime[process->id - 1];
      currentTime += slice;
      remainingTime[process->id - 1] -= slice;
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
  int n, fuantum;
  printf("Quantidade de Processos: ");
  scanf("%d", &n);
  printf("Tempo de Quantum: ");
  scanf("%d", &fuantum);

  struct Process processes[n];
  printf("Detalhes do processo:\n");
  for (int i = 0; i < n; i++) {
    printf("Processo %d:\n", i + 1);
    processes[i].id = i + 1;
    printf("   Tempo de ativação(chegada): ");
    scanf("%d", &processes[i].arrivalTime);
    printf("   Tempo de execução: ");
    scanf("%d", &processes[i].burstTime);
    printf("   Prioridade (0 for Alta, 1 for Baixa, 2 for Disco, 3 for Fita, 4 for Impressora): ");
    scanf("%d", &processes[i].priority);
  }

  calculateTimes(processes, n, fuantum);
  calculateTurnaroundTime(processes, n);
  calculateWaitingTime(processes, n);

  printf("\nRound Robin Scheduling Results:\n");
  printTable(processes, n);

  return 0;
}
