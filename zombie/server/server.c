#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <wait.h>
#define BUFSIZE 1024

void error_handler(int line){
  printf("%d line error\n", line);
  exit(1);
}

void sig_handler(int sig){
  int status;
  pid_t pid;
  while((pid = waitpid(-1, &status, WNOHANG)) > 0 ){
    if(WIFEXITED(status)){
      printf("pid(%d) 프로세스가 종료되었습니다.\n", pid);
      printf("자식 프로세스의 신호 : %d\n", WEXITSTATUS(status));
    }
  }
}

int main(void){

  int serv_sock, clnt_sock;
  struct sockaddr_in serv_addr, clnt_addr;
  char message[BUFSIZE];
  pid_t pid;
  int clnt_addr_size, recv_len;

  struct sigaction act;
  act.sa_handler = sig_handler;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;

  serv_sock = socket(PF_INET, SOCK_STREAM, 0);

  if(serv_sock == -1)
    error_handler(__LINE__);

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(9190);

  if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
    error_handler(__LINE__);

  if(listen(serv_sock, 5) == -1)
    error_handler(__LINE__);


  sigaction(SIGCHLD, &act, 0);

  printf("대기 상태 진입\n");

  clnt_addr_size = sizeof(clnt_addr);

  while(1){

    if((clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size)) > 0)
      pid = fork();

    if(pid == -1){
      close(clnt_sock);
      continue;
    }

    if(pid == 0){ // Child process
      printf("new process\n");
      close(serv_sock);

      while((recv_len = read(clnt_sock, message, BUFSIZE)) != 0){
        write(clnt_sock, message, recv_len);
      }
      close(clnt_sock);
      return 0;
    }
    else
      close(clnt_sock);
  }
  close(serv_sock);
  return 0;
}
