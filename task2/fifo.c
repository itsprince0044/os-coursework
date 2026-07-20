/* Task 2.2: FIFO page replacement algorithm */
#include <stdio.h>

#define NUM_FRAMES 3   /* small, so replacement happens often and is visible */

int frames[NUM_FRAMES];
int fifo_index = 0;      /* points to the oldest page - next to be replaced */
int faults = 0, hits = 0;

/* is this page currently in a frame? */
int find_page(int page) {
    for (int f = 0; f < NUM_FRAMES; f++)
        if (frames[f] == page) return f;
    return -1;
}

void print_frames(void) {
    printf("[ ");
    for (int f = 0; f < NUM_FRAMES; f++) {
        if (frames[f] == -1) printf("- ");
        else printf("%d ", frames[f]);
    }
    printf("]");
}

void access_page(int page) {
    printf("Access %d : ", page);

    if (find_page(page) != -1) {          /* page already loaded */
        hits++;
        printf("HIT   ");
        print_frames();
        printf("\n");
        return;
    }

    faults++;                             /* page fault */
    /* place page in the FIFO slot (oldest), advance the pointer round-robin */
    int victim = frames[fifo_index];
    frames[fifo_index] = page;
    fifo_index = (fifo_index + 1) % NUM_FRAMES;

    if (victim == -1) printf("FAULT (loaded)   ");
    else              printf("FAULT (replaced %d) ", victim);
    print_frames();
    printf("\n");
}

int main(void) {
    for (int f = 0; f < NUM_FRAMES; f++) frames[f] = -1;

    /* reference string: the sequence of pages the program touches */
    int refs[] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2};
    int n = sizeof(refs) / sizeof(refs[0]);

    printf("FIFO page replacement, %d frames\n\n", NUM_FRAMES);

    for (int i = 0; i < n; i++)
        access_page(refs[i]);

    printf("\nResults: %d hits, %d faults, hit ratio = %.1f%%\n",
           hits, faults, 100.0 * hits / (hits + faults));
    return 0;
}
