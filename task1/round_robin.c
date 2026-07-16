/* Task 1.3: Round-robin CPU scheduling simulation */
#include <stdio.h>

#define NUM_PROCESSES 4
#define QUANTUM 3   /* time slice each process gets per turn */

typedef struct {
    int pid;         /* process ID */
    int burst;       /* total CPU time needed */
    int remaining;   /* time still needed */
    int finish;      /* time when process completed */
} Process;

int main(void) {
    Process p[NUM_PROCESSES] = {
        {1, 8, 8, 0},
        {2, 4, 4, 0},
        {3, 9, 9, 0},
        {4, 5, 5, 0},
    };
    int time = 0;        /* simulation clock */
    int done = 0;        /* how many processes finished */

    printf("Round-Robin scheduling, quantum = %d\n\n", QUANTUM);

    /* keep cycling through the "queue" until all processes finish */
    while (done < NUM_PROCESSES) {
        for (int i = 0; i < NUM_PROCESSES; i++) {
            if (p[i].remaining == 0) continue;   /* already finished */

            /* run this process for one quantum (or less if nearly done) */
            int slice = p[i].remaining < QUANTUM ? p[i].remaining : QUANTUM;
            printf("Time %2d-%2d: P%d runs (%d left after)\n",
                   time, time + slice, p[i].pid, p[i].remaining - slice);

            time += slice;
            p[i].remaining -= slice;

            if (p[i].remaining == 0) {   /* process just completed */
                p[i].finish = time;
                done++;
                printf("          P%d FINISHED at time %d\n", p[i].pid, time);
            }
        }
    }

    /* stats: turnaround = finish - arrival(0), waiting = turnaround - burst */
    printf("\nPID  Burst  Turnaround  Waiting\n");
    float avg_tat = 0, avg_wait = 0;
    for (int i = 0; i < NUM_PROCESSES; i++) {
        int tat = p[i].finish;
        int wait = tat - p[i].burst;
        avg_tat += tat; avg_wait += wait;
        printf("P%d   %5d  %10d  %7d\n", p[i].pid, p[i].burst, tat, wait);
    }
    printf("Average turnaround: %.2f, average waiting: %.2f\n",
           avg_tat / NUM_PROCESSES, avg_wait / NUM_PROCESSES);
    return 0;
}
