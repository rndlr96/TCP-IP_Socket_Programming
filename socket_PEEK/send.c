#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define BUFSIZE 32

void error_handling(int line, char * message);

int main(void){
  int sock;
  struct sockaddr_in send_adr;

  sock = socket(PF_INET, SOCK_STREAM, 0);
  memset(&send_adr, 0, sizeof(send_adr));
  send_adr.sin_family = AF_INET;
  send_adr.sin_addr.s_addr = inet_addr("127.0.0.1");
  send_adr.sin_port = htons(9090);

  if(connect(sock, (struct sockaddr*)&send_adr, sizeof(send_adr)) == -1)
    error_handling(__LINE__, "connect() error!");

  write(sock, "123", strlen("123"));
  close(sock);
  return 0;

}

void error_handling(int line, char *message)
{
  printf("%d line %s", line, message);
  exit(1);
}
