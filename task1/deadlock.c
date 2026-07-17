/* Task 1.4: Deadlock demonstration and prevention via lock ordering */
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t lockA = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lockB = PTHREAD_MUTEX_INITIALIZER;
int bad_mode = 0;   /* 1 = demonstrate deadlock, 0 = prevention */

void *thread1(void *arg) {
    printf("Thread 1: locking A\n");
    pthread_mutex_lock(&lockA);
    sleep(1);   /* give thread 2 time to grab its first lock */

    printf("Thread 1: locking B\n");
    pthread_mutex_lock(&lockB);

    printf("Thread 1: has both locks, working\n");
    pthread_mutex_unlock(&lockB);
    pthread_mutex_unlock(&lockA);
    return NULL;
}

void *thread2(void *arg) {
    if (bad_mode) {
        /* BAD: opposite order to thread 1 -> circular wait -> deadlock */
        printf("Thread 2: locking B\n");
        pthread_mutex_lock(&lockB);
        sleep(1);
        printf("Thread 2: locking A\n");
        pthread_mutex_lock(&lockA);
    } else {
        /* GOOD: same order as thread 1 (A then B) -> no deadlock */
        printf("Thread 2: locking A\n");
        pthread_mutex_lock(&lockA);
        sleep(1);
        printf("Thread 2: locking B\n");
        pthread_mutex_lock(&lockB);
    }

    printf("Thread 2: has both locks, working\n");
    pthread_mutex_unlock(&lockB);
    pthread_mutex_unlock(&lockA);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "bad") == 0) bad_mode = 1;

    printf("Mode: %s\n\n", bad_mode ?
        "BAD (opposite lock order - will deadlock, Ctrl+C to exit)" :
        "GOOD (consistent lock order - deadlock prevented)");

    pthread_t t1, t2;
    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("\nBoth threads finished - no deadlock.\n");
    return 0;
}
