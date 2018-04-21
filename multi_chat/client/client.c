#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#define BUFSIZE 1024

int main(void){
  int sock;
  struct sockaddr_in serv_addr;
  char message[BUFSIZE];
  int str_len;

  sock = socket(PF_INET, SOCK_STREAM, 0);
  if(sock == -1)
    printf("socket() error");

  memset(&serv_addr, 0 , sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  serv_addr.sin_port = htons(9190);

  if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
    printf("connect() error");

  while(1){

    fputs("send message ('q' or 'Q' = turn off) : ", stdout);
    fgets(message, sizeof(message), stdin);

    if(strcmp("q\n", message) == 0 || strcmp("Q\n", message) == 0){
      printf("Program OFF\n");
      break;
    }

    str_len = write(sock, message, strlen(message));

    int recv_len = 0, recv_cnt;

    while(recv_len < str_len){

      recv_cnt = read(sock, message, BUFSIZE-1);
      if(str_len == -1){
        printf("read() error\n");
        break;
      }
      recv_len += str_len;
    }
    message[recv_len] = 0;
    printf("message from server : %s \n", message);

  }
  close(sock);

  return 0;

}
