#include<stdio.h>
#include<unistd.h>
int main() {
	for (int i = 0; i < 6; i++) {
		pid_t pid = fork();
		if (pid < 0) {
			printf("error!");
		}
		if (pid == 0) {
			break;
		}
		
		if(pid>0) {
			continue;
		}
	}
	getchar();
	return 0;
}

