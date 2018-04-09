#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <wait.h>
#include <sys/time.h>
#include <sys/select.h>
#define BUFSIZE 1024

void error_handler(int line){
  printf("%d line error\n", line);
  exit(0);
}

void sig_handler(int sig){
  int status;
  pid_t pid;
  if((pid = waitpid(-1, &status, WNOHANG)) != -1){
    printf("프로세스(%d)가 종료되었습니다.\n", pid);
    printf("자식프로세스의 전달값은 %d입니다\n", status);
  }
}

int main(void){
  int serv_sock, clnt_sock;
  struct sockaddr_in serv_addr, clnt_addr;
  int clnt_addr_size = sizeof(clnt_addr);
  char message[BUFSIZE];
  fd_set reads, copy_reads;
  struct timeval timeout;
  struct sigaction act;
  int fd_max, str_len, fd_num, i;

  //===================================== for signal =======================================

  act.sa_handler = sig_handler;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;

  sigaction(SIGCHLD, &act, 0);

  //========================================================================================

  if((serv_sock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    error_handler(__LINE__);

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(9190);

  //===================================== for select =======================================

  FD_ZERO(&reads);
  FD_SET(serv_sock, &reads);
  fd_max = serv_sock;

  //========================================================================================

  if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
    error_handler(__LINE__);

  if((listen(serv_sock, 5)) == -1)
    error_handler(__LINE__);


  while(1){

    copy_reads = reads;

    timeout.tv_sec = 5;
    timeout.tv_usec = 5000;

    if((fd_num = select(fd_max+1, &copy_reads, 0, 0, &timeout)) == -1)
      error_handler(__LINE__);

    if(fd_num == 0)
      continue;

    for(i = 0 ; i < fd_max+1 ; i++){
      if(FD_ISSET(i, &copy_reads)){
        if(i == serv_sock){
          clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
          FD_SET(clnt_sock, &reads);
          if(fd_max < clnt_sock)
            fd_max = clnt_sock;
          printf("connected client : %d\n", clnt_sock);
          printf("fd_max : %d\n", fd_max);
        }
        else{
          memset(message, '\0', BUFSIZE);
          str_len = read(i, message, BUFSIZE);
          if(str_len == 0){
            FD_CLR(i, &reads);
            close(i);
            printf("closed client : %d\n", i);
          }
          else
            write(i, message, str_len);
        }
      }
    }
  }
  close(serv_sock);
  return 0;
}
