/*
 * main_mm.c
 *
 *  Created on: Nov 13, 2010
 *      Author: Chaichana Noree
 */

#include "mm_public.h"

int main() {
	mm_t memguy;
	struct timeval timestart, timeend;
	mm_init(&memguy, 1024 * 64, 1024);
	gettimeofday(&timestart, NULL);
	int i;
	for (i = 0; i < 1024 * 64; i++) {
		mm_get(&memguy);
	}
	gettimeofday(&timeend, NULL);
	double tm = comp_time(timestart, timeend);
	printf("Time taken to get 64MB with memory manager %f ms\n", tm / 1000.0);
	mm_release(&memguy);

	return 0;
}

