#include "csapp.h"
void echo(int connfd,char* client_hostname,char* client_port);
void sigchld_handler(int sig){
    while(waitpid(-1,0,WNOHANG)>0)
	;
    return;
}
int main(int argc, char **argv)
{
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    char client_hostname[MAXLINE],client_port[MAXLINE];
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }
    Signal(SIGCHLD,sigchld_handler);

    listenfd = Open_listenfd(argv[1]);
    while (1) {
        clientlen = sizeof(struct sockaddr_storage);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
	if(Fork()==0){
	     Close(listenfd);
	     Getnameinfo((SA *) &clientaddr,clientlen,client_hostname,MAXLINE,client_port,MAXLINE,0);
             echo(connfd,client_hostname,client_port);
             Close(connfd);
	     printf("the server of (%s,%s) quit\n",client_hostname, client_port);
	     exit(0);
	}
	Close(connfd);
    }
    exit(0);
}   
