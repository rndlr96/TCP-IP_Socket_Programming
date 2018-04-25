#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#define BUFSIZE 1024

void error_handling(int line, char *message);

int main(void) {
  int sock;
  struct sockaddr_in serv_addr;
  char message[BUFSIZE];
  int str_len;
  FILE *readfp;
  FILE *writefp;

  sock = socket(PF_INET, SOCK_STREAM, 0);

  if (sock == -1)
    error_handling(__LINE__, "socket() error");

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  serv_addr.sin_port = htons(9190);

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    error_handling(__LINE__, "connect() error");

  else
    puts("Connected....................");

  readfp = fdopen(sock, "r");
  writefp = fdopen(sock, "w");

  while (1) {

    fputs("send message ('q' or 'Q' = turn off) : ", stdout);
    fgets(message, sizeof(message), stdin);

    if (strcmp("q\n", message) == 0 || strcmp("Q\n", message) == 0) {
      printf("Program OFF\n");
      break;
    }

    fputs(message, writefp);
    fflush(writefp);
    fgets(message, BUFSIZE, readfp);
    printf("Message from server : %s \n", message);

  }
  fclose(writefp);
  fclose(readfp);
  close(sock);

  return 0;
}

void error_handling(int line, char *message)
{
  printf("%d line %s\n", line, message);
  exit(1);
}
