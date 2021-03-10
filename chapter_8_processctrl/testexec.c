#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

char* env_init[] = {"USER=unknow", "PATH=/temp", NULL};

int main(void) {
	pid_t pid;

	if ((pid = fork()) < 0)
		printf("fork error");
	else if (pid == 0) {
		if (execle("/home/sar/bin/echoall", "echoall", "echoall", "myarg1",
				   "MY ARG2", (char*)0, env_init) < 0)
			printf("execle error");
	}

	if (waitpid(pid, NULL, 0) < 0)
		fprintf(stderr, "wait error");

	if ((pid = fork()) < 0) {
		fprintf(stderr, "fork error");
	} else if (pid == 0) {
		if (execlp("echoall", "echoall", "only 1 arg", (char*)0) < 0)
			fprintf(stderr, "execlp error");
	}
	exit(0);
}