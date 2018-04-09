#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#define BUFSIZE 30

int main(void){
  int sock;
  struct sockaddr_in clnt_addr;
  char message[BUFSIZE];
  int str_len;
  int recv_cnt = 0;
  int input_num, input;
  char input_char;

  sock = socket(PF_INET, SOCK_STREAM, 0);
  if(sock == -1)
    printf("socket() error");

  memset(&clnt_addr, 0 , sizeof(clnt_addr));
  clnt_addr.sin_family = AF_INET;
  clnt_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  clnt_addr.sin_port = htons(9200);

  if(connect(sock, (struct sockaddr*)&clnt_addr, sizeof(clnt_addr)) == -1){
    printf("connect() error");
    return 0;
  }

  fputs("input how many operand : ", stdout);
  fgets(message, sizeof(message), stdin);
  input_num = (int)message[0]-48;
  printf("%d\n", input_num);
  for( ; recv_cnt < input_num ; recv_cnt++){

    printf("input %d operand : ", recv_cnt+1);
    scanf(" %c", message+recv_cnt+1);
  }

  printf("input operator : ");
  scanf(" %c", message+message[0]+1-48);
  write(sock, message, strlen(message));
  read(sock, message, sizeof(message));
  printf("result : %d\n", atoi(message));
  close(sock);

  return 0;

}
