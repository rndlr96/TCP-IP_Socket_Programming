#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#define BUFSIZE 1024

int main(void){
  int sock;
  struct sockaddr_in serv_addr;
  struct sockaddr_in from_addr;
  char message[BUFSIZE];
  int str_len, addr_size;

  sock = socket(PF_INET, SOCK_DGRAM, 0);
  if(sock == -1)
    printf("socket() error");

  memset(&serv_addr, 0 , sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  serv_addr.sin_port = htons(9190);

  while(1){
    fputs("send message : ", stdout);
    fgets(message, sizeof(message), stdin);

    if(!strcmp(message, "q\n"))
      break;

    if(sendto(sock, message, strlen(message), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
      printf("here\n");
    addr_size = sizeof(from_addr);
    str_len = recvfrom(sock,message,BUFSIZE, 0, (struct sockaddr*)&serv_addr, &addr_size);

    message[str_len] = 0;
    printf("message from server : %s", message);
  }

  close(sock);

  return 0;

}
