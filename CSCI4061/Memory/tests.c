/*
 * tests.c
 *
 *  Created on: Nov 3, 2010
 *      Author: Joe Houlton
 */
#include "mm_public.h"

typedef struct {
	char str[64];
} thing;
int main() {
	mm_t memguy;
	mm_init(&memguy, 100, 64);
	printf("Init memory manager with 100 chunks, 64 bytes each memguy at %d\n",
			(int) &memguy);
	printf("Grabbing memory chunk...\n");
	void* chunk = mm_get(&memguy);
	printf("memory at %d\n", (int) chunk);
	printf("memguy head now %d\n", memguy.head);
	thing* that = (thing*) mm_get(&memguy);
	printf("setting parts of thing\n");
	strcpy(that->str, "this is some long string!!");
	printf("reading from get'd memory... %s\n", that->str);
	printf("returning memory to pool");
	mm_put(&memguy, that);
	return 0;

}
