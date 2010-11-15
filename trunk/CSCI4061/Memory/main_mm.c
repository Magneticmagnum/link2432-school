/*
 * main_mm.c
 *
 *  Created on: Nov 13, 2010
 *      Author: Chaichana Noree
 */

#include "mm_public.h"

int main() {
	struct timeval start, end;
	int timer;

	timer = gettimeofday(&start, (void *)NULL);

	void *memtest[500000];

	mm_t memguy;

	mm_init(&memguy, 500000, 1024);

	start = clock();

	//Allocate the blocks
	int i;
	for (i = 0; i < 500000; i++) {
		if ((memtest[i] = mm_get(&memguy)) == NULL) {
			fprintf(stderr, "mm_get failed");
		}
	}
	//Free the blocks
	for (i = 0; i < 500000; i++){
		mm_put(&memguy, memtest[i]);
	}
	mm_release(&memguy);

	timer = gettimeofday(&end, (void *)NULL);
	fprintf("Time taken to get 64MB with memory manager %f ms\n", comp_time(start, end)/1000.0);

	return 0;
}

