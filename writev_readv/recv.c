#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/uio.h>
#define BUFSIZE 32

void error_handling(int line, char *message);

int main(void)
{
  struct iovec vec[2];
  char buf1[BUFSIZE] = {0, };
  char buf2[BUFSIZE] = {0, };
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

  vec[0].iov_base = buf1;
  vec[0].iov_len = 6;
  vec[1].iov_base = buf2;
  vec[1].iov_len = BUFSIZE;

  while(1)
  {
    str_len = readv(recv_sock, vec, 2);
    if(str_len > 0)
      break;
  }

  printf("Read bytes : %d \n", str_len);
  printf("First message : %s \n", buf1);
  printf("Second message : %s \n", buf2);
  close(recv_sock);
  close(acpt_sock);
}

void error_handling(int line, char *message)
{
  printf("%d line %s", line, message);
  exit(1);
}
