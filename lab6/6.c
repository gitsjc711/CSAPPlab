#include <stdio.h>
#include <string.h>
#include<stdlib.h>
#define MAX    1024 
void copy(char* dest,char* src) {
    char* p = dest;
    char* q = src;
    int i = 4;
    while (q[i] != '\0') {
        p[i-4] = q[i];
        i++;
    }
}
int judge(char* cmd) {
    char help[] = "help";
    char xuehao[] = "xuehao";
    char exit[] = "exit";
    char exec[5] = "exec";
    if (strcmp(cmd, help) == 0) {
        return 1;
    }
    else if (strcmp(cmd, xuehao) == 0) {
        return 2;
    }
    else if(strcmp(cmd, exit) == 0) {
        return 3;
    }
    else {
        char first[5];
        memcpy(first, cmd, 4);
        first[4] = '\0';
        if (strcmp(first, exec) == 0) {
            return 4;
        }
        else {
            return 5;
        }

    }
}
void execshell(char* cmd) {
    char exec[MAX];
    copy(exec, cmd);
    system(exec);
}
void help() {
    printf("------------\n");
    printf("内部命令:\n");
    printf("help:打印帮助信息\n");
    printf("xuehao:打印学号\n");
    printf("exit:退出\n");
    printf("------------\n");
    printf("外部命令:\n");
    printf("exec+命令\n");
    printf("------------\n");
}
void xuehao() {
    printf("2021214028\n");
}
int main() {
	printf("welcome to  my shell!\n");
    char cmd[MAX];
    printf("myshell>");
    while (scanf("%[^\n]",cmd)) {
        int flag=judge(cmd);
        switch (flag)
        {
        case 1:
            help();
            break;
        case 2:
            xuehao();
            break;
        case 3:
            printf("bye\n");
            return 0;
        case 4:
            execshell(cmd);
            break;
        case 5:
            printf("无效的命令\n");
            break;
        }
        getchar();
        printf("myshell>");
    }
    return 0;
}
