#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define BUFSIZE 30

int calculator(char num[]){
  long result=0;
  char operator = *(num+num[0]+1-48);
  int count = (int)(num[0]-48);
  result = (*(num+1)-48);
  printf("%d\n", count);
  switch(operator){
    case '+':
      for(int i = 1 ; i < count ; i++)
        result += (*(num+i+1)-48);
      break;

    case '-':
      for(int i = 1 ; i < count ; i++)
        result -= (*(num+i+1)-48);
      break;

    case '*':
      for(int i = 1 ; i < count ; i++)
        result *= (*(num+i+1)-48);
      break;
  }
  printf("%ld\n", result);
  return result;
}

int main(void){
  int serv_sock, clnt_sock;
  struct sockaddr_in serv_addr, clnt_addr;
  char message[BUFSIZE];
  int operand[BUFSIZE];
  int str_len;

  serv_sock = socket(PF_INET, SOCK_STREAM, 0);

  if(serv_sock == -1){
    printf("socket() error");
    return 0;
  }

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);;
  serv_addr.sin_port = htons(9200);

  if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
    printf("bind() error");
    return 0;
  }

  int num_cnt = 0, char_cnt = 0;
  if(listen(serv_sock, 5) == -1){
    printf("listen() error");
    return 0;
  }

  int clnt_addr_size = sizeof(clnt_addr);
  for(int i = 0 ; i < 5 ; i++){
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);

    if(clnt_sock == -1){
      printf("accept() error");
      return 0;
    }

    printf("연결 요청 수락\n");

    read(clnt_sock, message, sizeof(message));
    int recv_num_cnt = 0, num_cnt = 0, char_cnt = 0;

    int result = calculator(message);

    sprintf(message, "%010d", result);
    write(clnt_sock, message, strlen(message));
  }
  close(clnt_sock);
  close(serv_sock);
  return 0;
}
