#include <arpa/inet.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc !=2 ) {
    fprintf(stderr, "Usage: ./dd2hex <dd>\n");
    return 0;
  }

  int addr, rc;
  if ((rc = inet_pton(AF_INET, argv[1], &addr)) == 1)
    printf("0x%x\n", ntohl(addr));
  else if (rc == 0)
    printf("Invalid IP addr: %s\n", argv[1]);

  return 0;
}
