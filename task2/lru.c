/* Task 2.3: LRU (Least Recently Used) page replacement algorithm */
#include <stdio.h>

#define NUM_FRAMES 3

int frames[NUM_FRAMES];
int last_used[NUM_FRAMES];   /* timestamp of the most recent access per frame */
int clock = 0;               /* global time, ticks up every access */
int faults = 0, hits = 0;

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
    clock++;
    printf("Access %d : ", page);

    int idx = find_page(page);
    if (idx != -1) {                 /* HIT: refresh its last-used time */
        hits++;
        last_used[idx] = clock;
        printf("HIT   ");
        print_frames();
        printf("\n");
        return;
    }

    faults++;                        /* page fault */

    /* find a free frame first */
    int target = -1;
    for (int f = 0; f < NUM_FRAMES; f++)
        if (frames[f] == -1) { target = f; break; }

    /* no free frame: pick the least recently used (smallest timestamp) */
    if (target == -1) {
        target = 0;
        for (int f = 1; f < NUM_FRAMES; f++)
            if (last_used[f] < last_used[target]) target = f;
    }

    int victim = frames[target];
    frames[target] = page;
    last_used[target] = clock;

    if (victim == -1) printf("FAULT (loaded)   ");
    else              printf("FAULT (replaced %d) ", victim);
    print_frames();
    printf("\n");
}

int main(void) {
    for (int f = 0; f < NUM_FRAMES; f++) { frames[f] = -1; last_used[f] = 0; }

    /* SAME reference string as FIFO - required for a fair comparison */
    int refs[] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2};
    int n = sizeof(refs) / sizeof(refs[0]);

    printf("LRU page replacement, %d frames\n\n", NUM_FRAMES);

    for (int i = 0; i < n; i++)
        access_page(refs[i]);

    printf("\nResults: %d hits, %d faults, hit ratio = %.1f%%\n",
           hits, faults, 100.0 * hits / (hits + faults));
    return 0;
}
