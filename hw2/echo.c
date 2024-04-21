#include "csapp.h"
void echo(int connfd,char* client_hostname,char* client_port)
{
    size_t n;
    char buf[MAXLINE];
    rio_t rio;

    Rio_readinitb(&rio, connfd);
    while ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
	printf("(%s,%s) connects to the server\n",client_hostname, client_port);
        printf("server received %d bytes\n",(int) n);
        Rio_writen(connfd, buf, n);
	printf("client:");
	Fputs(buf, stdout);
    }
}
