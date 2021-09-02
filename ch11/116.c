#include "csapp.h"

void echo(int fd);
int read_request(rio_t *rp, char *buf);


int main(int argc, char **argv) 
{
    int listenfd, connfd;
    char hostname[MAXLINE], port[MAXLINE];
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;

    /* Check command line args */
    if (argc != 2) {
	fprintf(stderr, "usage: %s <port>\n", argv[0]);
	exit(1);
    }

    listenfd = Open_listenfd(argv[1]);
    while (1) {
	clientlen = sizeof(clientaddr);
	connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen); //line:netp:tiny:accept
        Getnameinfo((SA *) &clientaddr, clientlen, hostname, MAXLINE, 
                    port, MAXLINE, 0);
        printf("Accepted connection from (%s, %s)\n", hostname, port);
	echo(connfd);                                             
	Close(connfd);                                            //line:netp:tiny:close
    }
}

void echo(int fd) {
  rio_t rio;
  int len = 0, c;
  char buf[MAXBUF], line[MAXLINE];

  /* Read request line */
  Rio_readinitb(&rio, fd);
  if (!(c = Rio_readlineb(&rio, line, MAXLINE))) 
    return; // Ignore empty request
  printf("0. %s", line);
  sprintf(buf, "%s", line);
  len += c;
  
  /* Read request headers */
  len += Rio_readlineb(&rio, line, MAXLINE);
  strcat(buf, line);
  printf("1. %s", line);
  while(strcmp(line, "\r\n")) {          
    c = Rio_readlineb(&rio, line, MAXLINE);
    printf("%s", line);
    strcat(buf, line);
    len += c;
  }

  /* Send response headers */
  sprintf(line, "HTTP/1.0 200 OK\r\n");
  Rio_writen(fd, line, strlen(line));
  sprintf(line, "Server: Tiny Web Server\r\n");
  Rio_writen(fd, line, strlen(line));
  sprintf(line, "Content-type: text/plain; charset=UTF-8\r\n");
  Rio_writen(fd, line, strlen(line));
  sprintf(line, "Content-length: %d\r\n\r\n", len);
  Rio_writen(fd, line, strlen(line));
  /* Send response body */
  Rio_writen(fd, buf, strlen(buf));
}
