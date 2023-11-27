#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESS 100
#define HIGH_PRIORITY 0
#define LOW_PRIORITY 1
#define IO_PRIORITY 2

typedef enum { DISK, PRINTER, TAPE } IOType;
typedef enum { NEW, READY, RUNNING, WAITING, TERMINATED } ProcessStatus;

typedef struct {
    int pid;
    int ppid;
    int priority;
    int remaining_time;
    int io_duration;
    int time_quantum;
    IOType io_type;
    ProcessStatus status;
} PCB;

typedef struct {
    PCB* queue[MAX_PROCESS];
    int front, rear;
} Queue;

void enqueue(Queue* q, PCB* process) {
    q->rear = (q->rear + 1) % MAX_PROCESS;
    q->queue[q->rear] = process;
}

PCB* dequeue(Queue* q) {
    int front = q->front;
    q->front = (q->front + 1) % MAX_PROCESS;
    return q->queue[front];
}

Queue* createQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = -1;
    return q;
}

int isQueueEmpty(Queue* q) {
    return q->front == -1;
}

void initializeProcesses(PCB* processes, int num_processes) {
    // Inicializar os processos conforme necessário
    // ...
}