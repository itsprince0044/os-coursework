/* Task 1.1: Creating multiple threads performing concurrent tasks */
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 3

void *worker(void *arg) {
    int id = *(int *)arg;
    for (int i = 1; i <= 3; i++) {
        printf("Thread %d: doing work, step %d\n", id, i);
        sleep(1);
    }
    printf("Thread %d: finished\n", id);
    return NULL;
}

int main(void) {
    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];

    printf("Main: creating %d threads\n", NUM_THREADS);

    for (int i = 0; i < NUM_THREADS; i++) {
        ids[i] = i + 1;
        pthread_create(&threads[i], NULL, worker, &ids[i]);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Main: all threads completed\n");
    return 0;
}
