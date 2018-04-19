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

void sig_handler(int sig){
  int status;
  pid_t pid;
  while((pid = waitpid(-1, &status, WNOHANG)) > 0){
    if(WIFEXITED(status)){
      printf("pid(%d) 프로세스가 종료되었습니다.\n", pid);
      printf("자식 프로세스의 신호 : %d\n", WEXITSTATUS(status));
    }
  }
}

int main(void){

  int serv_sock, clnt_sock;
  struct sockaddr_in serv_addr, clnt_addr;
  int clnt_addr_size;
  char message[BUFSIZE];
  char message2[BUFSIZE];
  pid_t pid;
  struct sigaction act;
  int buf[2]; // pipe file distritor
  int recv_len;

  act.sa_handler = sig_handler;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;

  sigaction(SIGCHLD, &act, 0);

  if((serv_sock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    error_handler(__LINE__);

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(9190);

  if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
    error_handler(__LINE__);

  if(listen(serv_sock, 5) == -1)
    error_handler(__LINE__);

  clnt_addr_size = sizeof(clnt_addr);

  while(1){

    if((clnt_sock = (accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size))) > 0){
      printf("클라이언트가 연결되었습니다.\n");
      pipe(buf);
      pid = fork(); // buf[0] == 출구    buf[1] == 입구
    }

    if(pid == -1)
      error_handler(__LINE__);

/* 자식 프로세스에서 소켓을 통해 클라이언트와 통신하고 파이프를 통해 부모 프로세스에게 전달하는 코드
    else if(pid == 0){ // 자식 프로세스의 경우
      close(serv_sock);
      while((recv_len = read(clnt_sock, message, BUFSIZE)) != 0){
        printf("child process get message form client : %s\n", message);
        write(clnt_sock, message, recv_len);
        write(buf[1], message, recv_len);
        memset(message, '\0', BUFSIZE);
      }
      close(clnt_sock);
      return 0;
    }

    else{ // 부모 프로세스의 경우
      close(clnt_sock);
      while((recv_len = read(buf[0], message2, BUFSIZE)) != 0){
        printf("Parent process get message : %s\n", message2);
        memset(message2, '\0', BUFSIZE);
      }
    }
*/

/* 부모 프로세스에서 수신하고 자식프로세스에서 전송하는 프로그램 코드
    else if(pid == 0){
      close(serv_sock);
      while((recv_len = read(buf[0], message, BUFSIZE)) != 0){
        write(clnt_sock, message, recv_len);
        memset(message, '\0', BUFSIZE);
      }
      close(clnt_sock);
    }

    else{
      while((recv_len = read(clnt_sock, message, BUFSIZE)) != 0){
        write(buf[1], message, recv_len);
        memset(message, '\0', BUFSIZE);
      }
      close(clnt_sock);
    }
  }
*/

  close(serv_sock);
  return 0;
}
