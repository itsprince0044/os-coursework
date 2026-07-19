/* Task 2.1: Paging system with configurable page size */
#include <stdio.h>
#include <stdlib.h>

#define NUM_FRAMES 4          /* physical memory = 4 frames */
#define MAX_PAGES 64          /* size of the page table */

int page_size = 256;          /* configurable: bytes per page */

/* page table: page_table[p] = frame holding page p, or -1 if not in memory */
int page_table[MAX_PAGES];
int frames[NUM_FRAMES];       /* frames[f] = page stored in frame f, -1 = empty */
int faults = 0, hits = 0;

/* translate one virtual address, loading the page if needed */
void access_address(int address) {
    int page   = address / page_size;
    int offset = address % page_size;

    printf("Address %5d -> page %2d, offset %3d : ", address, page, offset);

    if (page_table[page] != -1) {                 /* page already in memory */
        hits++;
        printf("HIT (frame %d)\n", page_table[page]);
        return;
    }

    /* PAGE FAULT: find a free frame */
    faults++;
    for (int f = 0; f < NUM_FRAMES; f++) {
        if (frames[f] == -1) {                    /* free frame found */
            frames[f] = page;
            page_table[page] = f;
            printf("PAGE FAULT -> loaded into free frame %d\n", f);
            return;
        }
    }
    /* no free frame: replacement needed - implemented in next stage (FIFO/LRU) */
    printf("PAGE FAULT -> memory full! (replacement algorithm needed)\n");
}

int main(int argc, char *argv[]) {
    if (argc > 1) page_size = atoi(argv[1]);      /* configurable via argument */

    for (int i = 0; i < MAX_PAGES; i++) page_table[i] = -1;
    for (int f = 0; f < NUM_FRAMES; f++) frames[f] = -1;

    printf("Paging simulator: page size = %d bytes, %d frames\n\n",
           page_size, NUM_FRAMES);

    /* simulated sequence of memory accesses */
    int addresses[] = {100, 300, 550, 100, 900, 1200, 320, 1500, 105, 2000};
    int n = sizeof(addresses) / sizeof(addresses[0]);

    for (int i = 0; i < n; i++)
        access_address(addresses[i]);

    printf("\nResults: %d hits, %d faults, hit ratio = %.1f%%\n",
           hits, faults, 100.0 * hits / (hits + faults));
    return 0;
}
