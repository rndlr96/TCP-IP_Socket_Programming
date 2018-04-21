#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/uio.h>
#define BUFSIZE 32

void error_handling(int line, char * message);

int main(void){
  int sock;
  struct sockaddr_in send_adr;
  struct iovec vec[2];
  char buf1[] = "ABCDEFG";
  char buf2[] = "1234567";

  vec[0].iov_base = buf1;
  vec[0].iov_len = strlen(buf1);
  vec[1].iov_base = buf2;
  vec[1].iov_len = strlen(buf2);

  sock = socket(PF_INET, SOCK_STREAM, 0);
  memset(&send_adr, 0, sizeof(send_adr));
  send_adr.sin_family = AF_INET;
  send_adr.sin_addr.s_addr = inet_addr("127.0.0.1");
  send_adr.sin_port = htons(9090);

  if(connect(sock, (struct sockaddr*)&send_adr, sizeof(send_adr)) == -1)
    error_handling(__LINE__, "connect() error!");

  writev(sock, vec, 2);
  close(sock);
  return 0;

}

void error_handling(int line, char *message)
{
  printf("%d line %s", line, message);
  exit(1);
}
