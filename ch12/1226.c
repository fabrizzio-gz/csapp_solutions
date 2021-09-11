#include "csapp.h"

struct hostent *gethostbyname_ts(const char *name);
void init_gethostbyname(void);
void duphostdata(struct hostent *hostdata);
void free_hostdata(struct hostent *hostdata);
void print_hostdata(struct hostent *hostdata); /* To check results */ 

int main(int argc, char *argv[]) {
  struct hostent *hostdata;

  if (argc != 2) {
    fprintf(stderr, "Usage: ./main <hostname>\n");
    return 0;
  }
  
  hostdata = gethostbyname(argv[1]);

  printf("Results using gethostbyname:\n");
  if (hostdata != NULL)
    print_hostdata(hostdata);
  else
    printf("Invalid hostname: %s\n", argv[1]);

  hostdata = gethostbyname_ts(argv[1]);

  printf("Results using gethostbyname_ts:\n");
  if (hostdata != NULL)
    print_hostdata(hostdata);
  else
   printf("Invalid hostname: %s\n", argv[1]);

  free_hostdata(hostdata);
  return 0; 
}

static sem_t mutex;

struct hostent *gethostbyname_ts(const char *name) {
  static pthread_once_t once = PTHREAD_ONCE_INIT;
  struct hostent *hostdata;
  Pthread_once(&once, init_gethostbyname);
  P(&mutex);
  hostdata = gethostbyname(name);
  if (hostdata)
    duphostdata(hostdata);
  V(&mutex);

  return hostdata;
}

void init_gethostbyname(void) {
  sem_init(&mutex, 0, 1);
}

void duphostdata(struct hostent *hd) {
  char *h_name_copy;
  h_name_copy = (char *)Malloc(sizeof(char) * MAXBUF);
  hd->h_name = strncpy(h_name_copy, hd->h_name, MAXBUF);

  for (int i = 0; hd->h_aliases[i] != NULL; i++) {
    char *h_alias_copy = (char *)Malloc(sizeof(char) * MAXBUF);
    hd->h_aliases[i] = strncpy(h_alias_copy, hd->h_aliases[i], MAXBUF);
  }

  for (int i = 0; hd->h_addr_list[i] != NULL; i++) {
    char *h_addr_copy = (char *)Malloc(hd->h_length + 1);
    hd->h_addr_list[i] = strncpy(h_addr_copy, hd->h_addr_list[i], hd->h_length + 1);
  }

  hd->h_addr = hd->h_addr_list[0];
}

/* gethostbyname_ts requires explicit free of data */
void free_hostdata(struct hostent *hd) {
  Free(hd->h_name);
  
  for (int i = 0; hd->h_aliases[i] != NULL; i++) 
    Free(hd->h_aliases[i]);

  for (int i = 0; hd->h_addr_list[i] != NULL; i++) 
    Free(hd->h_addr_list[i]);
}

void print_hostdata(struct hostent *hd) {
  printf("Host name: %s\n", hd->h_name);
  
  for (int i= 0; hd->h_aliases[i] != NULL; i++)
    printf("Host alias: %s\n", hd->h_aliases[i]);
  
  printf("Addr type: %d\n", hd->h_addrtype);
  printf("Addr length: %d\n", hd->h_length);
  
  for (int i = 0; hd->h_addr_list[i] != NULL; i++)
    printf("Host Addr: %s\n", inet_ntoa(*((struct in_addr*)hd->h_addr_list[i])));
}
