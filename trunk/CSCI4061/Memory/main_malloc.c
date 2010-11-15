/*
 * main_malloc.c
 *
 *  Created on: Nov 13, 2010
 *      Author: Chaichana Noree
 */

#include "mm_public.h"

int main(void){
	struct timeval start, end;
	int timer;

	timer = gettimeofday(&start, (void *)NULL);

	void *memtest[500000];

	int i;
	for(i = 0; i<500000; i++){
		memtest[i]=malloc(64);
	}

	for(i = 0; j<500000; j++){
		free(memtest[j]);
	}

	timer = gettimeofday (&end, (void *)NULL);
	fprintf(stderr, "Time taken using malloc: %f ms",
			comp_time (start, end)/1000.0);
	return 0;

}
