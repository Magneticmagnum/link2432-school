CC = gcc
CFLAGS = -D_REENTRANT
LDFLAGS = -lpthread lsocket

all: util_http

util_http: http util-http.o
	${CC} ${CFLAGS} -c http.c

clean:
	rm http.o
