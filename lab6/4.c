#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
int main() {
	for (int i = 0; i < 3; i++) {
		pid_t pid = fork();
		if (pid == 0) {
			for (int i = 0; i < 3; i++) {
				pid_t pid = fork();
				if (pid == 0) {
					break;
				}
				else if (pid < 0) {
					printf("error!");
				}
				else {

				}
			}
			break;
		}
		else if (pid < 0) {
			printf("error!");
		}
		else {

		}
	}
	pause();
}
