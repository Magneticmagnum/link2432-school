/*
 * main_mm.c
 *
 *  Created on: Nov 13, 2010
 *      Author: Chaichana Noree
 */

#include "mm_public.h"

int main() {
	void *memtest[500000];
	clock_t start, end;
	double total_time;

	mm_t memguy;

	mm_init(&memguy, 500000, 1024);

	start = clock();

	int i;
	//Allocate the blocks
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

	end = clock();
	total_time = ((double) (end-start))/CLOCKS_PER_SEC;

	printf("Time taken to get 64MB with memory manager %f ms\n", total_time);

	return 0;
}

