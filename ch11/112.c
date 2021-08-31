#include <arpa/inet.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
  if (argc !=2 ) {
    fprintf(stderr, "Usage: ./hex2dd <hex>\n");
    return 0;
  }

  char dd[16];  /* dotted-decimal IP addr */
  uint32_t hex = htonl(strtol(argv[1], NULL, 16));

  if (inet_ntop(AF_INET, &hex, dd, 16) != NULL)
    printf("%s\n", dd);
  else
    printf("Invalid hex addres: %s\n", argv[1]);
  
  return 0;
}
