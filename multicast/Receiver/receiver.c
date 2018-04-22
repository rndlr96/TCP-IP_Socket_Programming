#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUFSIZE 1024

void error_handling(int line, char *message);

int main(int argc, char *argv[])
{
  int recv_sock;
  int str_len;
  struct sockaddr_in addr;
  char buf[BUFSIZE];
  struct ip_mreq join_adr;

  recv_sock = socket(PF_INET, SOCK_DGRAM, 0);

  if(recv_sock == -1)
    error_handling(__LINE__, "socket() error");

  memset(&addr, 0 , sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(atoi(argv[2]));

  if(bind(recv_sock, (struct sockaddr*)&addr, sizeof(addr)) == -1)
    error_handling(__LINE__, "bind() error");

  join_adr.imr_multiaddr.s_addr = inet_addr(argv[1]);
  join_adr.imr_interface.s_addr = htonl(INADDR_ANY);

  setsockopt(recv_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*)&join_adr, sizeof(join_adr));

  while(1){
    str_len = recvfrom(recv_sock, buf, BUFSIZE-1, 0, NULL, 0);
    if(str_len < 0)
      break;
    buf[str_len] = 0;
    fputs(buf, stdout);
  }

  close(recv_sock);
  return 0;
}

void error_handling(int line, char *message){
  printf("%d line %s \n", line, message);
  exit(1);
}
