#include "csapp.h"

int main(int argc, char **argv) 
{
  struct addrinfo *p, *listp, hints;
  char buf[MAXLINE];
  int rc, flags;

  if (argc != 2) {
    fprintf(stderr, "usage: %s <domain name>\n", argv[0]);
    exit(0);
  }

  /* Get a list of addrinfo records */
  memset(&hints, 0, sizeof(struct addrinfo));                         
  hints.ai_family = AF_INET;       /* IPv4 only */        //line:netp:hostinfo:family
  hints.ai_socktype = SOCK_STREAM; /* Connections only */ //line:netp:hostinfo:socktype
  if ((rc = getaddrinfo(argv[1], NULL, &hints, &listp)) != 0) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(rc));
    exit(1);
  }


  flags = NI_NUMERICHOST; /* Display address string instead of domain name */
  for (p = listp; p; p = p->ai_next) {
    /*
     * Cast to sockaddr_in works only for that specific socket address
     * structure (Only IPv4?) 
     */
    inet_ntop(p->ai_family, &((struct sockaddr_in *)(p->ai_addr))->sin_addr.s_addr, buf, MAXLINE);
    printf("%s\n", buf);
  } 

  /* Clean up */
  Freeaddrinfo(listp);

  exit(0);
}
/* $end hostinfo */
