#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define BUFSIZE 32

void error_handling(int line, char *message);

int main(void)
{
  int acpt_sock, recv_sock;
  struct sockaddr_in recv_adr, serv_adr;
  int str_len;
  socklen_t serv_adr_sz;
  struct sigaction act;
  char buf[BUFSIZE];

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

  while(1)
  {
    str_len = recv(recv_sock, buf, sizeof(buf), MSG_PEEK|MSG_DONTWAIT);
    if(str_len > 0)
      break;
  }

  buf[str_len] = 0;
  printf("Buffering %d bytes : %s \n", str_len, buf);

  str_len = recv(recv_sock, buf, sizeof(buf), MSG_PEEK|MSG_DONTWAIT);
  buf[str_len] = 0;
  printf("Read again: %s \n", buf);
  close(recv_sock);
  close(acpt_sock);
}

void error_handling(int line, char *message)
{
  printf("%d line %s", line, message);
  exit(1);
}
