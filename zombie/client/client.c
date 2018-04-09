#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#define BUFSIZE 1024

void error_handler(int line){
  printf("%d line error\n", line);
  exit(1);
}

int main(void){
  int sock;
  struct sockaddr_in clnt_addr;
  char message[BUFSIZE];
  int str_len;

  sock = socket(PF_INET, SOCK_STREAM, 0);
  if(sock == -1)
    error_handler(__LINE__);

  memset(&clnt_addr, 0, sizeof(clnt_addr));
  clnt_addr.sin_family = AF_INET;
  clnt_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  clnt_addr.sin_port = htons(9190);

  if(connect(sock, (struct sockaddr*)&clnt_addr, sizeof(clnt_addr)) == -1)
    error_handler(__LINE__);

  while(1){
    fputs("send message ('q' or 'Q' = turn off) : ", stdout);
    fgets(message, sizeof(message), stdin);

    if(strcmp("q\n", message) == 0 || strcmp("Q\n", message) == 0){
      printf("Program OFF\n");
      break;
    }

    str_len = write(sock, message, strlen(message));

    int recv_len, total_len = 0;

    while(total_len < str_len){

      recv_len = read(sock, message, sizeof(message));

      if(recv_len == -1){
        error_handler(__LINE__);
      }

      total_len += recv_len;

    }
    message[total_len] = 0;
    printf("message form server : %s\n", message);
  }
  close(sock);

  return 0;
}
