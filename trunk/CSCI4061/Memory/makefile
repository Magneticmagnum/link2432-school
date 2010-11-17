#makefile for 4061 Operating Systems Project 3
#Chaichana Noree 	ID: 4295362
#Brian Norman    	ID:
#Joe Houlton     	ID:

CFLAGS = -g -Wall -03

all : main_mm main_malloc packet

main_malloc : main_malloc.c mm_public.o
	gcc ${CFLAGS} -o main_malloc main_malloc.c mm_public.o

main_mm : main_mm.c mm_public.o
	gcc ${CFLAGS} -o main_mm main_mm.c mm_public.o

mm_public.o : mm_public.c mm_public.h
	gcc ${CFLAGS} -c mm_public.c
	
packet : packet_public.c mm_public.o
	gcc ${CFLAGS} -o packet packet_public.c mm_public.o
	
debug: mm_public.o
	gcc ${CFLAGS} -o main_malloc main_malloc.c mm_public.o -DDEBUG
	gcc ${CFLAGS} -o main_mm main_mm.c mm_public.o -DDEBUG

clean:
	rm -rf mm_public.o main_mm main_malloc packet