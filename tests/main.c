#include <stdio.h>
#include <stdlib.h>

// Define process types
enum ProcessType {
    CPU_BOUND,
    IO_BOUND
};

enum IOType{
    DISC,
    PRINTER,
    TAPE
};
 struct Process
 {
    int pid;
    int timeSlice;
    int serviceTime;
    int durationTime;
 };
 
// Process Control Block (PCB) structure
// struct PCB {
//     int pid;             // Process ID
//     int priority;        // Priority of the process
//     int timeSlice;       // Time slice for CPU-bound processes
//     enum ProcessType type; // Type of process (CPU or I/O bound)
// };

struct PCB{
    int processId;
    int priority;
    int ppid;
    int status;
};

// Node structure for the priority queue
struct Node {
    struct PCB data;
    struct Node* next;
};

// Priority Queue structure
struct PriorityQueue {
    struct Node* front;
};

// Function to create a new node
struct Node* createNode(struct PCB process) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = process;
    newNode->next = NULL;
    return newNode;
}

// Function to initialize an empty priority queue
struct PriorityQueue* initPriorityQueue() {
    struct PriorityQueue* queue = (struct PriorityQueue*)malloc(sizeof(struct PriorityQueue));
    queue->front = NULL;
    return queue;
}

// Function to insert a process into the priority queue based on priority
void enqueue(struct PriorityQueue* queue, struct PCB process) {
    struct Node* newNode = createNode(process);

    if (queue->front == NULL || process.priority > queue->front->data.priority) {
        newNode->next = queue->front;
        queue->front = newNode;
    } else {
        struct Node* current = queue->front;
        while (current->next != NULL && current->next->data.priority >= process.priority) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
}

// Function to remove and return the highest priority process from the queue
struct PCB dequeue(struct PriorityQueue* queue) {
    if (queue->front == NULL) {
        // Queue is empty, return an empty PCB
        struct PCB emptyPCB = { -1, 0, 0, CPU_BOUND };
        return emptyPCB;
    }

    struct Node* temp = queue->front;
    struct PCB dequeuedProcess = temp->data;
    queue->front = temp->next;
    free(temp);
    return dequeuedProcess;
}
int main() {
    int n, i, qt, count = 0, temp, sq = 0, bt[10], tat[10], wt[10];
    float awt = 0, atat = 0;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter Burst time:\n");
    for (i = 0; i < n; i++) {
        scanf("%d", &bt[i]);
    }

    printf("Enter Quantum Time: ");
    scanf("%d", &qt);

    for (i = 0; i < n; i++) {
        wt[i] = 0;
    }

    printf("\nGantt Chart:\n");
    printf("  ");
    for (i = 0; i < n; i++) {
        printf("P%d ", i + 1);
    }
    printf("\n");

    while (1) {
        int flag = 1;  // Flag para verificar se todos os processos foram concluídos

        for (i = 0, count = 0; i < n; i++) {
            temp = qt;
            if (bt[i] == 0) {
                count++;
                continue;
            }
            if (bt[i] > qt) {
                printf("|   ");
                bt[i] = bt[i] - qt;
            } else {
                temp = bt[i];
                bt[i] = 0;
                printf("|  ");
            }
            sq = sq + temp;
            tat[i] = sq;
        }

        if (n == count)
            flag = 0;

        if (!flag)
            break;
    }

    printf("\n\nProcess\tBurst time\tTurnaround time\tWaiting time\n");
    for (i = 0; i < n; i++) {
        wt[i] = tat[i] - bt[i];
        awt = awt + wt[i];
        atat = atat + tat[i];
        printf("%d\t\t%d\t\t%d\t\t%d\n", i + 1, bt[i], tat[i], wt[i]);
    }

    awt = awt / n;
    atat = atat / n;

    printf("\nAverage waiting time = %f\n", awt);
    printf("Average turnaround time = %f\n", atat);

    return 0;
}

// int main() {
//     // Example usage

//     // Initialize a priority queue
//     struct PriorityQueue* processQueue = initPriorityQueue();

//     // Example processes
//     struct PCB process1 = {1, 2, 10, CPU_BOUND};
//     struct PCB process2 = {2, 1, 5, IO_BOUND};
//     struct PCB process3 = {3, 3, 8, CPU_BOUND};

//     // Enqueue processes
//     enqueue(processQueue, process1);
//     enqueue(processQueue, process2);
//     enqueue(processQueue, process3);

//     // Dequeue and process each process in order of priority
//     while (processQueue->front != NULL) {
//         struct PCB currentProcess = dequeue(processQueue);

//         // Simulate processing the current process
//         printf("Processing Process ID %d with priority %d\n", currentProcess.processId, currentProcess.priority);

//         // Additional simulation logic can be added here

//         // For simplicity, let's assume each process is processed for its time slice
//         // For IO-bound processes, you may simulate IO operations

//         // Simulate processing time
//         printf("Process ID %d completed its time slice or IO operation\n", currentProcess.processId);
//     }

//     return 0;
// }
