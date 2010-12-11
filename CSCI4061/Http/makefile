CC = gcc
CFLAGS = -D_REENTRANT
LDFLAGS = -lpthread -lnsl

all: http.o

http.o:
	${CC} ${CFLAGS} -c http.c

clean:
	rm http.o
