#include "mm_public.h"

int main(void){
	clock_t start, end;
	double total_time;

	start = clock();
	void *memtest[500000];

	int i;
	for(i = 0; i<500000; i++){
		memtest[i]=malloc(64);
	}

	int j;
	for(j = 0; j<500000; j++){
		free(memtest[j]);
	}

	end = clock();
	total_time = ((double) (end-start))/CLOCKS_PER_SEC;
	fprintf(stderr, "Time taken using malloc: %f seconds", total_time);
	return 0;

}
