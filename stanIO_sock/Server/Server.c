#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#define BUFSIZE 30

void error_handling(int line, char *message);

int main(void) {
  int serv_sock, clnt_sock;
  struct sockaddr_in serv_addr;
  struct sockaddr_in clnt_addr;
  char message[BUFSIZE];
  int str_len, i;
  socklen_t clnt_addr_size;
  FILE *readfp;
  FILE *writefp;

  printf("TCP Server를 위한 Socket이 생성됩니다.\n");
  serv_sock = socket(PF_INET, SOCK_STREAM, 0);

  if (serv_sock == -1)
    error_handling(__LINE__, "socket() error");

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(9190);

  if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    error_handling(__LINE__, "bind() error");

  if (listen(serv_sock, 5) == -1)
    printf("listen() error\n");
  printf("대기상태 진입\n");

  clnt_addr_size = sizeof(clnt_addr);

  for (i = 0; i < 5; i++) {

    clnt_sock =
        accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);

    if (clnt_sock == -1)
      error_handling(__LINE__, "accept() error");

    printf("Connection Accept\n");
    printf("Connected client %d \n", i + 1);
    readfp = fdopen(clnt_sock, "r");
    writefp = fdopen(serv_sock, "w");

    while (!feof(readfp)) {

      fgets(message, BUFSIZE, readfp);
      fputs(message, writefp);
      fflush(writefp);
    }

    fclose(readfp);
    fclose(writefp);
  }
  close(serv_sock);
  return 0;
}

void error_handling(int line, char *message) {
  printf("%d line %s\n", line, message);
  exit(1);
}
