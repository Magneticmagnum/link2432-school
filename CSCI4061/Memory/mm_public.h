#include <sys/time.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#define INTERVAL 0
#define INTERVAL_USEC 50000

/* Fill this in */
typedef struct {
	void* start;
	int head;
	int sz;
	int hm;
	int clrmm;
} mm_t;

/* These will be defined in mm_public.c ... by you */
void mm_init(mm_t *MM, int hm, int sz);
void* mm_get(mm_t *MM);
void mm_put(mm_t *MM, void *chunk);
void mm_release(mm_t *MM);
double comp_time(struct timeval times, struct timeval timee);

