/*
 * pipes.c
 *
 *  Created on: Oct 19, 2010
 *      Author: hoult015
 */
#include "stdio.h"
#include "stdlib.h"
int main(void) {

	int ends[2];
	char buff[1024];
	pid_t pid;

	if (pipe(ends) == -1) {
		perror("pipe!");
		exit(1);
	}

	pid = fork();
	if (pid == 0) {
		close(ends[0]);
		write(ends[1], "Child writing something to parent\n", 1024);
		write(ends[1], "Child again writing to parent\n", 1024);
		close(ends[1]);

	} else if (pid > 0) {
		close(ends[1]);
		read(ends[0], buff, 1024);
		printf("%s", buff);
		read(ends[0], buff, 1024);
		printf("%s", buff);
		close(ends[0]);
	} else {
		perror("error fork!");
		exit(1);
	}

	return 0;

}
