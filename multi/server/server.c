#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#define BUFSIZE 30

void error_handler(int line){
  printf("%d line error", line);
  exit(1);
}

int main(void){
  int serv_sock, clnt_sock;
  struct sockaddr_in serv_addr, clnt_addr;
  int clnt_addr_size;
  pid_t pid;
  char message[BUFSIZE];

  printf("프로그램이 실행되었습니다.\n");

  serv_sock = socket(PF_INET, SOCK_STREAM, 0);
  if(serv_sock == -1)
    error_handler(__LINE__);

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(9190);

  if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
    error_handler(__LINE__);

  if(listen(serv_sock, 1) == -1)
    error_handler(__LINE__);
  printf("대기상태에 진입하였습니다.\n");

  clnt_addr_size = sizeof(clnt_addr);
  while(1){

    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    if(clnt_sock == -1)
      error_handler(__LINE__);
    else
      printf("연결 요청이 수락되었습니다.\n");
    pid = fork();

    if(pid == 0){ // 자식프로세스의 경우
      close(serv_sock);
      while(1){

        if(read(clnt_sock, message, BUFSIZE) != 0)
          write(clnt_sock, message, sizeof(message));

      }
    }
    else
      close(clnt_sock);
  }
  return pid;
}
