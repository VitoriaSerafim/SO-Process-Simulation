#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Algumas premissas usadas na construção do código
#define MAX_PROCESS 50
#define HIGH_PRIORITY 0
#define LOW_PRIORITY 1

// Valores para identificar o tipo de prioridade de cada I/O
#define DISK_IO_PRIORITY 2
#define TAPE_IO_PRIORITY 3
#define PRINTER_IO_PRIORITY 4

// Tempo de cada I/O
#define DISK_TIME 2
#define TAPE_TIME 3
#define PRINTER_TIME 4

// Tempo de quantum fixo
#define TIME_QUANTUM 3

// Definição da estrutura dos processos
struct Process
{
  int id;             // PID
  int arrivalTime;    // Tempo de ativação
  int burstTime;      // Tempo de execução
  int remainingTime;  // Tempo de execução restante do processo
  int completionTime; // Tempo de conclusão
  int turnaroundTime; // Turnaround do processo
  int waitingTime;    // Tempo de espera
  int priority;       // Prioridade de execução
  int ioType;         // Tipo de I/O --> 0: Nenhum, 1: Disco, 2: Fita, 3: Impressora
};

// Definição das filas
struct Queue
{
  struct Process *array[MAX_PROCESS];
  int front, rear;
};

// Estrutura para enfilar os processos
void enqueue(struct Queue *q, struct Process *process)
{
  q->rear++;
  q->array[q->rear] = process;
}

// Estrutura para desenfilar os processos
struct Process *dequeue(struct Queue *q)
{
  struct Process *process = q->array[q->front];
  q->front++;
  return process;
}

// Verificação para checar se as filas realmente estão vazias
int isEmpty(struct Queue *q)
{
  return q->front > q->rear;
}

// Função para inicializar os processos
void initializeProcesses(struct Process processes[], int n)
{
  // Semente para a função random
  srand(time(NULL));

  for (int i = 0; i < n; i++) {
    processes[i].id = i;
    processes[i].arrivalTime = rand() % 10; // Tempo de ativação aleatório (entre 0 a 9)
    processes[i].burstTime = rand() % 10 + 1; // Tempo de execução aleatório (entre 1 a 20)
    processes[i].remainingTime = processes[i].burstTime;
    processes[i].completionTime = 0;
    processes[i].turnaroundTime = 0;
    processes[i].waitingTime = 0;
    processes[i].priority = HIGH_PRIORITY; // Novos processo sempre começam com prioridade alta
    processes[i].ioType = rand() % 4; // Tipo de I/O aleatório: 0: Nenhum, 1: Disco, 2: Fita, 3: Impressora
  }
}

// Função para simular o escalonamento dos processos
void processScheduling(struct Process processes[], int n, int quantum)
{
  struct Queue highPriorityQueue = {.front = 0, .rear = -1};
  struct Queue lowPriorityQueue = {.front = 0, .rear = -1};
  struct Queue diskIOQueue = {.front = 0, .rear = -1};
  struct Queue tapeIOQueue = {.front = 0, .rear = -1};
  struct Queue printerIOQueue = {.front = 0, .rear = -1};

  int remainingTime[n];

  // varáveis estáticas para calcular o tempo de bloqueio para cada tipo de I/O
  static int disc = 0, tape = 0, printer = 0;

  // variável auxliar
  for (int i = 0; i < n; i++)
  {
    remainingTime[i] = processes[i].remainingTime;
  }

  int currentTime = 0;
  int allDone = 0;

 //inicializa a estrutura dos processos
  struct Process *process;

  while (!allDone)
  {
    allDone = 1;

    for (int i = 0; i < n; i++)
    {
      if (remainingTime[i] > 0)
      {
        allDone = 0;

        //Verificamos o tempo de ativação de cada processo e se ainda precisa executar
        // Se precisar executar, alocamos o processo na fila de prioridade correta
        if ((processes[i].arrivalTime <= currentTime && processes[i].remainingTime > 0) && remainingTime[i] > 0)
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

        // Executa processos da fila de alta prioridade
        if (!isEmpty(&highPriorityQueue))
        {
          process = dequeue(&highPriorityQueue);
        }// Verifica se existe processo para fazer I/O de disco
        else if (!isEmpty(&diskIOQueue))
        {
          disc += 1;
          if (disc % DISK_TIME == 0)
          {
            process = dequeue(&diskIOQueue);
            process->ioType = 0; // Fim da operação de I/O
            enqueue(&lowPriorityQueue, process);
          }
        }  // Verifica se existe processo para fazer I/O de fita
        else if (!isEmpty(&tapeIOQueue))
        {
          tape += 1;
          if (tape % TAPE_TIME == 0)
          {
            process = dequeue(&tapeIOQueue);
            process->ioType = 0; // Fim da operação de I/O
            enqueue(&highPriorityQueue, process);
          }
        } // Verifica se existe processo para fazer I/O de impressora
        else if (!isEmpty(&printerIOQueue))
        {
          printer += 1;
          if (printer % PRINTER_TIME == 0)
          {
            process = dequeue(&printerIOQueue);
            process->ioType = 0; // Fim da operação de I/O
            enqueue(&highPriorityQueue, process);
          }
        } // Verifica se existe processo na fila de baixa prioridade -> última a ser executada
        else if (!isEmpty(&lowPriorityQueue))
        {
          process = dequeue(&lowPriorityQueue);
        }

        // Se o tempo de execução for que um quantum, avançamos na execução e calculamos o tempo restante para o processo acabar
        if (remainingTime[i] > quantum)
        {
          currentTime = currentTime + quantum;
          remainingTime[i] = remainingTime[i] - quantum;
        }// Se o processo tiver acabado, salvamos esse valor e encerramos a execução
        else
        {
          currentTime = currentTime + quantum;
          processes[i].completionTime = currentTime;
          remainingTime[i] = 0;
        }
      }
    }
  }
}


//Função para calcular o Turnaround
void calculateTurnaroundTime(struct Process processes[], int n)
{
  for (int i = 0; i < n; i++)
    processes[i].turnaroundTime = processes[i].completionTime - processes[i].arrivalTime;
}

//Função para calcular o tempo de espera de cada processo
void calculateWaitingTime(struct Process processes[], int n)
{
  for (int i = 0; i < n; i++)
    processes[i].waitingTime = processes[i].turnaroundTime - processes[i].burstTime;
}


//Aqui imprimimos a tabela resultado do escalonamento dos processos
void printTable(struct Process processes[], int n)
{
  printf("--------------------------------------------------------------------"
         "--------------------------------------- \n");
  printf("| Processo | Tempo de ativacao | Tempo de execucao | Tempo de conclusao | "
         "   Turnaround   | Tempo de espera |\n");
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

  processScheduling(processes, n, TIME_QUANTUM);
  calculateTurnaroundTime(processes, n);
  calculateWaitingTime(processes, n);

  printf("\n Escalonamento Round Robin:\n");
  printTable(processes, n);

  return 0;
}