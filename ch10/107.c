#include "csapp.h"

int main(int argc, char **argv) 
{
    int n;
    rio_t rio;
    char *buf;

    buf = (char *)Malloc(MAXBUF);

    Rio_readinitb(&rio, STDIN_FILENO);
    while((n = Rio_readnb(&rio, buf, MAXBUF)) != 0) 
	Rio_writen(STDOUT_FILENO, buf, n);
    exit(0);
}
