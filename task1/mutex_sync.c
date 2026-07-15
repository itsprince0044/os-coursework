/* Task 1.2: Race condition demo and fix using a mutex */
#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 3
#define INCREMENTS 1000000

long counter = 0;                 /* shared data - all threads touch this */
pthread_mutex_t lock;             /* the mutex protecting counter */
int use_mutex = 0;                /* 0 = unsafe run, 1 = safe run */

void *increment(void *arg) {
    for (int i = 0; i < INCREMENTS; i++) {
        if (use_mutex) {
            pthread_mutex_lock(&lock);    /* only one thread past here */
            counter++;                    /* critical section */
            pthread_mutex_unlock(&lock);  /* release for others */
        } else {
            counter++;                    /* unprotected - race! */
        }
    }
    return NULL;
}

void run_test(int with_mutex) {
    pthread_t threads[NUM_THREADS];
    counter = 0;
    use_mutex = with_mutex;

    for (int i = 0; i < NUM_THREADS; i++)
        pthread_create(&threads[i], NULL, increment, NULL);
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    printf("%s: expected %d, got %ld %s\n",
           with_mutex ? "WITH mutex   " : "WITHOUT mutex",
           NUM_THREADS * INCREMENTS, counter,
           counter == NUM_THREADS * INCREMENTS ? "(correct)" : "(WRONG - race condition!)");
}

int main(void) {
    pthread_mutex_init(&lock, NULL);

    run_test(0);   /* demonstrate the race */
    run_test(1);   /* demonstrate the fix */

    pthread_mutex_destroy(&lock);
    return 0;
}
