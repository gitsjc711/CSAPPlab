#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
int main(int argc,char* argv[],char* envp[])
{
	if (execve("/bin/ls", argv, envp)==-1)
	{
		printf("mylsµ÷ÓÃÊ§°Ü\n");
		exit(1);
	}
	return 0;
}