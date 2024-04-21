#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/types.h>
#include <wait.h>
int main() {
	int status;
	for (int i = 0; i < 10; i++) {
		pid_t pid = fork();
		if (pid == 0) {
			
			printf("hello %d\n", getpid());
			break;
		}
		else if (pid < 0) {
			printf("error!");
		}
		else {
			wait(&status);
			printf("word %d\n", getpid());
		}
	}
	exit(0);
}
