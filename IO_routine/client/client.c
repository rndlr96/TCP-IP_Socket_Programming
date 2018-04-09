#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <signal.h>
#include <wait.h>
#define BUFSIZE 1024

void error_handler(int line){
  printf("%d line error\n", line);
  exit(0);
}

int main(void){

  int sock;
  struct sockaddr_in clnt_addr;
  int clnt_addr_size;
  pid_t pid;
  char message[BUFSIZE];

  if((sock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    error_handler(__LINE__);

  memset(&clnt_addr, 0, sizeof(clnt_addr));
  clnt_addr.sin_family = AF_INET;
  clnt_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  clnt_addr.sin_port = htons(9190);

  clnt_addr_size = sizeof(clnt_addr);

  if(connect(sock, (struct sockaddr*)&clnt_addr, clnt_addr_size) == -1)
    error_handler(__LINE__);

  while(1){
    fputs("send message ('q' or 'Q' = process out) : ", stdout);
    fgets(message, sizeof(message), stdin);

    if(strcmp("q\n", message) == 0 || strcmp("Q\n", message) == 0){
      printf("Program out : Good Bye\n");
      break;
    }

    write(sock, message, strlen(message));
    memset(message, '\0', BUFSIZE);
    read(sock, message, BUFSIZE-1);

    printf("message form server : %s\n", message);

  }
  close(sock);
  return 0;
}
