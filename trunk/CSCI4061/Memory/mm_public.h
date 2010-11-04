#include <sys/time.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#define INTERVAL 0
#define INTERVAL_USEC 50000

/*
 * mm_chnk data type has a chunk and checked out flag
 */
typedef struct {
	void* chunk;
	int inuse;
} mm_chunk;
/* Fill this in */
/* mm_t data type has an array of chunks
 * a count of how many chunks
 * and the size of chunks
 */
typedef struct {
	mm_chunk* chunks;
	int nchunk;
	int szchunk;
	int head;
} mm_t;

/* These will be defined in mm_public.c ... by you */
void mm_init(mm_t *MM, int hm, int sz);
void* mm_get(mm_t *MM);
void mm_put(mm_t *MM, void *chunk);
void mm_release(mm_t *MM);
double comp_time(struct timeval times, struct timeval timee);

