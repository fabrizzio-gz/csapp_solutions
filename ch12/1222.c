#include "csapp.h"
int echo_line(int connfd);
void command(void);

int main(int argc, char **argv) 
{
    int listenfd, connfd = 0;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    fd_set read_set, ready_set;

    if (argc != 2) {
	fprintf(stderr, "usage: %s <port>\n", argv[0]);
	exit(0);
    }
    listenfd = Open_listenfd(argv[1]);  

    FD_ZERO(&read_set);              /* Clear read set */ 
    FD_SET(STDIN_FILENO, &read_set); /* Add stdin to read set */ 
    FD_SET(listenfd, &read_set);     /* Add listenfd to read set */

    while (1) {
	ready_set = read_set;
        if (connfd)
          FD_SET(connfd, &read_set);
	Select(listenfd > connfd
               ? listenfd+1
               : connfd+1
               , &ready_set, NULL, NULL, NULL); 
	if (FD_ISSET(STDIN_FILENO, &ready_set))
	    command(); /* Read command line from stdin */
        else if (connfd && FD_ISSET(connfd, &ready_set)) {
          connfd = echo_line(connfd);
          if (!connfd)
              Close(connfd);
        }
	else if (FD_ISSET(listenfd, &ready_set)) { 
            clientlen = sizeof(struct sockaddr_storage); 
	    connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
	    connfd = echo_line(connfd);
            if (!connfd)
              Close(connfd);
	}
    }
}

void command(void) {
    char buf[MAXLINE];
    if (!Fgets(buf, MAXLINE, stdin))
	exit(0); /* EOF */
    printf("%s", buf); /* Process the input command */
}

int echo_line(int connfd) {
    size_t n; 
    char buf[MAXLINE]; 
    rio_t rio;

    Rio_readinitb(&rio, connfd);
    n = Rio_readlineb(&rio, buf, MAXLINE);
    printf("server received %d bytes\n", (int)n);
    Rio_writen(connfd, buf, n);

    if (!n) /* EOF */
      return 0; 
    return connfd;
}

