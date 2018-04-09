#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#define BUFSIZE 2024

void error_handler(char*);

int main(void){

  FILE *fp;
  int sock;
  struct sockaddr_in clnt_addr;
  char message[BUFSIZE];
  int recv_size, clnt_addr_size;
  int nbyte;

  fp = fopen("picture.jpeg", "wb");

  sock = socket(AF_INET, SOCK_STREAM, 0);

  memset(&clnt_addr, 0 , sizeof(clnt_addr));
  clnt_addr.sin_family = AF_INET;
  clnt_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  clnt_addr.sin_port = htons(9190);
  clnt_addr_size = sizeof(clnt_addr);

  if(connect(sock, (struct sockaddr*)&clnt_addr, clnt_addr_size) == -1)
    error_handler("connect error");

  if(read(sock, message, sizeof(message)) == -1)
    error_handler(strcat(__LINE__, "line read error"));

  printf("file size = [%d]\n", atoi(message));
  recv_size = atoi(message);

  write(sock, message, strlen(message));

  while(recv_size > 0){

    if(recv_size < BUFSIZE){
      nbyte = read(sock, message, recv_size);
      recv_size = recv_size - nbyte;
    }

    else{
      nbyte = read(sock, message, BUFSIZE);
      recv_size = recv_size - nbyte;
    }

    fwrite(message, sizeof(char), nbyte, fp);

    strcpy(message, "thank you");
    write(sock, message, strlen(message));
    nbyte = 0;

  }

  close(sock);
  fclose(fp);
  return 0;

}

void error_handler(char *message){
  printf("%s\n", message);
  exit(1);
}
