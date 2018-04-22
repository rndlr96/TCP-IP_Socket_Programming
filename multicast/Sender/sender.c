#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUFSIZE 30
#define TTL 64

void error_handler(int line, char *message){
  printf("%d line %s", line, message);
  exit(1);
}

int main(int argc, char *argv[]){
  int send_sock;
  struct sockaddr_in mul_adr;
  int time_live = TTL;
  FILE *fp;
  char buf[BUFSIZE];

  printf("프로그램이 실행되었습니다.\n");

  send_sock = socket(PF_INET, SOCK_DGRAM, 0);
  if(send_sock == -1)
    error_handler(__LINE__, "socket() error");

  memset(&mul_adr, 0, sizeof(mul_adr));
  mul_adr.sin_family = AF_INET;
  mul_adr.sin_addr.s_addr = inet_addr(argv[1]);
  mul_adr.sin_port = htons(atoi(argv[2]));

  setsockopt(send_sock, IPPROTO_IP, IP_MULTICAST_TTL, (void*)&time_live, sizeof(time_live));

  if((fp=fopen("news.txt", "r")) == NULL)
    error_handler(__LINE__, "fopen error");

  while(!feof(fp)) { // Broadcasting
    fgets(buf, BUFSIZE, fp);
    sendto(send_sock, buf, strlen(buf), 0, (struct sockaddr*)&mul_adr, sizeof(mul_adr));
    sleep(2);
  }
  close(fp);
  close(send_sock);
  return 0;
}
