/* 
 * echoservers.c - A concurrent echo server based on select
 */
#include "csapp.h"

typedef struct { /* Represents a pool of connected descriptors */ 
  int maxfd;        /* Largest descriptor in read_set */   
  fd_set read_set;  /* Set of all active descriptors */
  fd_set ready_set; /* Subset of descriptors ready for reading  */
  int nready;       /* Number of ready descriptors from select */   
  int maxi;         /* Highwater index into client array */
  int clientfd[FD_SETSIZE];    /* Set of active descriptors */
  rio_t clientrio[FD_SETSIZE]; /* Set of active read buffers */
} pool; 

void init_pool(int listenfd, pool *p);
void add_client(int connfd, pool *p);
void check_clients(pool *p);


int byte_cnt = 0; /* Counts total bytes received by server */

int main(int argc, char **argv)
{
  int listenfd, connfd;
  socklen_t clientlen;
  struct sockaddr_storage clientaddr;
  static pool pool; 

  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(0);
  }
  listenfd = Open_listenfd(argv[1]);
  init_pool(listenfd, &pool); 

  while (1) {
    /* Wait for listening/connected descriptor(s) to become ready */
    pool.ready_set = pool.read_set;
    pool.nready = Select(pool.maxfd+1, &pool.ready_set, NULL, NULL, NULL);

    /* If listening descriptor ready, add new client to pool */
    if (FD_ISSET(listenfd, &pool.ready_set)) { 
      clientlen = sizeof(struct sockaddr_storage);
      connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen); 
      add_client(connfd, &pool); 
    }
	
    /* Echo a text line from each ready connected descriptor */ 
    check_clients(&pool); 
  }
}

void init_pool(int listenfd, pool *p) 
{
  /* Initially, there are no connected descriptors */
  int i;
  p->maxi = -1;                   
  for (i=0; i< FD_SETSIZE; i++)  
    p->clientfd[i] = -1;        

  /* Initially, listenfd is only member of select read set */
  p->maxfd = listenfd;           
  FD_ZERO(&p->read_set);
  FD_SET(listenfd, &p->read_set); 
}


void add_client(int connfd, pool *p) 
{
  int i;
  p->nready--;
  for (i = 0; i < FD_SETSIZE; i++)  /* Find an available slot */
    if (p->clientfd[i] < 0) { 
      /* Add connected descriptor to the pool */
      p->clientfd[i] = connfd;                 
      Rio_readinitb(&p->clientrio[i], connfd); 

      /* Add the descriptor to descriptor set */
      FD_SET(connfd, &p->read_set); 

      /* Update max descriptor and pool highwater mark */
      if (connfd > p->maxfd) 
        p->maxfd = connfd; 
      if (i > p->maxi)       
        p->maxi = i;       
      break;
    }
  if (i == FD_SETSIZE) /* Couldn't find an empty slot */
    app_error("add_client error: Too many clients");
}

void check_clients(pool *p) 
{
  int i, connfd, n, c, conn_bytes;
  char buf[MAXLINE], *bufp;
  fd_set connset;
  rio_t rio;
  struct timeval timeout;

  /* Set timeout so that select returns immediately */
  timeout.tv_sec = 0;
  timeout.tv_usec = 0;

  for (i = 0; (i <= p->maxi) && (p->nready > 0); i++) {
    connfd = p->clientfd[i];
    rio = p->clientrio[i];

    /* If the descriptor is ready, echo from it */
    if ((connfd > 0) && (FD_ISSET(connfd, &p->ready_set))) { 
      p->nready--;
      c = conn_bytes = 0;
      /* Read one char at a time as long as connfd is not blocking */
      while (c < MAXLINE - 1) {
        FD_ZERO(&connset);
        FD_SET(connfd, &connset);
        /* Check if conn fd is ready for read */
        Select(connfd+1, &connset, NULL, NULL, &timeout);
        if (FD_ISSET(connfd, &connset)) {
          n = Rio_readn(connfd, buf + c, 1);
          conn_bytes += n;
          c++;
          if ((*(buf + c - 1) == '\n') || (n == 0))
            break; /* new line */
        } else
          break; /* connector is blocking */
      }
     
      byte_cnt += conn_bytes; 
      printf("Server received %d (%d total) bytes on fd %d\n", 
             conn_bytes, byte_cnt, connfd);
      Rio_writen(connfd, buf, conn_bytes);
      /* EOF detected, remove descriptor from pool */
      if (n == 0)  { 
        Close(connfd); 
        FD_CLR(connfd, &p->read_set); 
        p->clientfd[i] = -1;          
      }
    }
  }
}

