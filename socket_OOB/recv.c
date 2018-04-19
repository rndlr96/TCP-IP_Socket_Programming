#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#define BUFSIZE 32

void error_handling(int line, char *message);
void urg_handler(int signo);

int acpt_sock;
int recv_sock;

int main(void)
{
  struct sockaddr_in recv_adr, serv_adr;
  int str_len, state;
  socklen_t serv_adr_sz;
  struct sigaction act;
  char buf[BUFSIZE];

  act.sa_handler = urg_handler;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;

  acpt_sock = socket(PF_INET, SOCK_STREAM, 0);
  memset(&recv_adr, 0, sizeof(recv_adr));
  recv_adr.sin_family = AF_INET;
  recv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
  recv_adr.sin_port = htons(9090);

  if(bind(acpt_sock, (struct sockaddr *)&recv_adr, sizeof(recv_adr)) == -1)
    error_handling(__LINE__, "bind error!");
  listen(acpt_sock, 5);

  serv_adr_sz = sizeof(serv_adr);
  recv_sock = accept(acpt_sock, (struct sockaddr*)&serv_adr, &serv_adr_sz);

  fcntl(recv_sock, F_SETOWN, getpid());
  state = sigaction(SIGURG, &act, 0);

  while((str_len = recv(recv_sock, buf, sizeof(buf), 0)) != 0)
  {
    if(str_len == -1)
      continue;
      buf[str_len] = 0;
      puts(buf);
  }

  close(recv_sock);
  close(acpt_sock);
}

void urg_handler(int signo)
{
  int str_len;
  char buf[BUFSIZE];
  str_len = recv(recv_sock, buf, sizeof(buf)-1, MSG_OOB);
  buf[str_len] = 0;
  printf("Urgent message : %s \n", buf);
}

void error_handling(int line, char *message)
{
  printf("%d line %s", line, message);
  exit(1);
}
