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
  int serv_sock, clnt_sock;
  struct sockaddr_in serv_addr, clnt_addr;
  char message[BUFSIZE];
  int send_fsize, clnt_addr_size;


  serv_sock = socket(PF_INET, SOCK_STREAM, 0);

  if(serv_sock == -1)
    error_handler("socket생성을 실패하였습니다.");

  printf("File server를 위한 socket이 생성되었습니다.\n");

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(9190);
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
    error_handler("bind() error");

  if(listen(serv_sock, 5) == -1)
    error_handler("listen() error");

  printf("대기상태에 진입하였습니다.\n");

  clnt_addr_size = sizeof(clnt_addr);

  for(int i = 0 ; i < 5 ; i++){

    //=============== FILE Part ===================

      fp = fopen("/home/pandog/Socket/file/server/picture.jpeg", "rb");

      if(fp == NULL){
        error_handler("file open error");
      }

      fseek(fp, 0, SEEK_END); // file의 크기를 알기위해 스트림 위치를 파일의 끝으로 이동시킨다.
      send_fsize = ftell(fp);
      fseek(fp, 0, SEEK_SET);

    //================================================

    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);

    if(clnt_sock == -1)
      error_handler("accept() error");

    printf("연결 요청 수락\n");
    sprintf(message, "%10d", send_fsize); // client에게 파일의 크기를 알려주어 적절한 SYNC를 설정하도록 하기 위해

    if(write(clnt_sock, message, strlen(message)) == -1)
      error_handler("FILE SIZE SYNC ERROR");

    int len = read(clnt_sock, message, sizeof(message));
    message[len] = '\0';
    printf("FILE SIZE SYNC ================> %d bytes       [OK]\n", atoi(message));


    if(fread(message, sizeof(char), send_fsize, fp) == -1)
      error_handler("read error");

    printf("%s\n", message);

    if(write(clnt_sock, message, send_fsize) == -1)
      error_handler("write error");

    printf("Data sending to client\n");

    fclose(fp);
    printf("write stream shutdown\n");
    shutdown(clnt_sock, SHUT_WR);

    if(read(clnt_sock, message, sizeof(message)) == -1)
      error_handler("read error");

    printf("%s\n", message);
    close(clnt_sock);
  }
  close(serv_sock);
  return 0;
}

void error_handler(char* message){
  printf("%s\n", message);
  exit(1);
}
