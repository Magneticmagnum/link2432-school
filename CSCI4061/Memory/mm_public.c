#include "mm_public.h"

/* Return usec */
double comp_time(struct timeval times, struct timeval timee) {

	double elap = 0.0;

	if (timee.tv_sec > times.tv_sec) {
		elap += (((double) (timee.tv_sec - times.tv_sec - 1)) * 1000000.0);
		elap += timee.tv_usec + (1000000 - times.tv_usec);
	} else {
		elap = timee.tv_usec - times.tv_usec;
	}
	return ((unsigned long) (elap));

}

/* Write these ... */
// Init the mm_t memory manager
// allocates hm number of chunks
// sets inuse of each chunk to 0 and allocates sz bytes of raw memory
void mm_init(mm_t *MM, int hm, int sz) {
	MM->nchunk = hm;
	MM->szchunk = sz;
	MM->head = 0;
	MM->chunks = malloc(sizeof(mm_chunk) * hm);
	int i;
	for (i = 0; i < hm; i++) {
		MM->chunks[i].inuse = 0;
		MM->chunks[i].chunk = malloc(sz);
	}
}

// if available, returns a raw section of memory
void* mm_get(mm_t *MM) {
	int nch = MM->nchunk;
	int head = MM->head;
	void* raw;
	if (head < nch) {
		raw = MM->chunks[head].chunk;
		MM->chunks[head].inuse = 1;
		head++;
		MM->head = head;
	}
	return raw;
}

// put a raw section of memory back
void mm_put(mm_t *MM, void *chunk) {
	int head = MM->head;
	head--;
	if (head > -1) {
		MM->chunks[head].chunk = chunk;
		MM->chunks[head].inuse = 0;
		MM->head = head;
	}
}

void mm_release(mm_t *MM) {
	int nch = MM->nchunk;
	int i;
	for (i = 0; i < nch; i++) {
		free(MM->chunks[i].chunk);
	}
	free(MM->chunks);
	free(MM);
}

void timer_example() {
	struct timeval time_start, time_end;
	int j;

	/* start timer */
	j = gettimeofday(&time_start, (void *) NULL);

	/* code you wish to time goes here */

	j = gettimeofday(&time_end, (void *) NULL);

	fprintf(stderr, "Time taken =  %f msec\n", comp_time(time_start, time_end)
			/ 1000.0);
}

