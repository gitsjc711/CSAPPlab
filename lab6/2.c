#include<stdio.h>
#include<unistd.h>
int main(){
	for(int i=0;i<6;i++){
		pid_t pid=fork();
		if(pid==0){
			break;
		}else if(pid<0){
			printf("error!");
		}else{
			
		}
	}
	getchar();
}

