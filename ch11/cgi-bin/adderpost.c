#include "csapp.h"

int main(void) {
  char *buf, *postdata, *arg1, *arg2;
  char content[MAXLINE];
  int n1=0, n2=0, len;


  if ((buf = getenv("CONTENT_LENGTH")) == NULL) 
    /* No Content-length header was sent*/
    return 0;

  len = atoi(buf);

  postdata = Malloc(len + 1);
  Fgets(postdata, len + 1, stdin);

  /* Parse args */
  arg1 = postdata;
  arg2 = strstr(postdata, "&");
  *arg2 = '\0';
  arg2++;
  n1 = atoi(arg1);
  n2 = atoi(arg2);
    
  /* Make the response body */
  sprintf(content, "Welcome to (POST) add.com: ");
  sprintf(content, "%sTHE Internet addition portal.\r\n<p>", content);
  sprintf(content, "%sThe answer is: %d + %d = %d\r\n<p>", 
          content, n1, n2, n1 + n2);
  sprintf(content, "%sThanks for visiting!\r\n", content);
  
  /* Generate the HTTP response */
  printf("Connection: close\r\n");
  printf("Content-length: %d\r\n", (int)strlen(content));
  printf("Content-type: text/html\r\n\r\n");
  printf("%s", content);
  fflush(stdout);

  exit(0);
}
