/*
 * tests.c
 *
 *  Created on: Nov 3, 2010
 *      Author: Joe Houlton
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
	gettimeofday(&timestart, NULL);
	for (i = 0; i < 1024 * 64; i++) {
		malloc(1024);
	}
	gettimeofday(&timeend, NULL);
	tm = comp_time(timestart, timeend);
	printf("Time taken to allocate 64MB with malloc %f ms\n", tm / 1000.0);
	return 0;

}
