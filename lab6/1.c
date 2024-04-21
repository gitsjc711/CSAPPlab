#include<stdio.h>
#include<unistd.h>
int main(){
	pid_t pid=fork();
	if(pid<0) printf("error!");
	else if(pid==0) getchar();
	else getchar();
	return 0;
}
