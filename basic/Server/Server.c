#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#define BUFSIZE 30

int main(void){
  int serv_sock;
  int clnt_sock;
  struct sockaddr_in serv_addr;
  struct sockaddr_in clnt_addr;
  int clnt_addr_size, str_len;
  char message[BUFSIZE];
  char ch1;
  int i;

  printf("실행시킬 서버의 종류를 선택하세요. \n");
  printf("1. TCP Server   2. UDP Server \n");
  printf("입력 : ");
  ch1 = getchar();
  fflush(stdin);
  // PF = Protocol Family , AF = Address Family
  switch(ch1){
    case '1': // TCP Server

      printf("TCP Server를 위한 Socket이 생성됩니다.\n");
      serv_sock = socket(PF_INET, SOCK_STREAM, 0); // PF_INET : IPv4   SOCK_STREAM : TCP 통신
      if(serv_sock == -1)
        printf("socket() error\n");

      memset(&serv_addr, 0, sizeof(serv_addr)); // bind를 위한 주소 할당
      serv_addr.sin_family = AF_INET; // IPv4 쭈소체계를 사용하므로 항상 AF_INET으로 설정한다.
      serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
      serv_addr.sin_port = htons(9190);
        // htonl() = host to network long : long int 데이터를 네트워크 byte order로 변경 <-> ntohl = network to host long
        // port는 2byte로 충분하기 때문에 short에 해당하는 htons를 사용하고 ip주소의 경우 4byte가 필요하기 때문에 long에 해당하는 htonl을 사용한다.
        // 호스트에서 네트워크로 넘어갈 때 바이트의 반전이 발생한다. 예를 들어 1234를 전송한다면 network는 3412로 인식한다.
        // INADDR_ANY : 서버의 IP주소를 자동으로 찾아서 대입해주는 함수 | 장점 : 멀티 네트워크 카드 동시 지원, 이식성

      if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        printf("bind() error\n");

      if(listen(serv_sock, 5) == -1) // listen(sockfd, backlog) : backlog = 접속가능한 클라이언트 최대수
        printf("listen() error\n");
      printf("대기상태 진입\n");


      clnt_addr_size = sizeof(clnt_addr);

      for(i=0 ; i <5 ; i++){
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        if(clnt_sock == -1)
          printf("accept() error \n");
        printf("연결 요청 수락\n");

        while(1){
         if((str_len = read(clnt_sock, message, BUFSIZE)) != 0){
//           printf("message form client : %s\n", message);
           write(clnt_sock, message, sizeof(message));
         }
         else
            break;
        }
        close(clnt_sock);
      }
      close(serv_sock);
      return 0;

    case '2': // UDP Server

      // PF_INET : IPv4   SOCK_DGRAM : UDP 통신
      serv_sock = socket(PF_INET, SOCK_DGRAM, 0);

      printf("socket 생성완료\n");
      if(serv_sock == -1)
        printf("socket() error\n");


      memset(&serv_addr, 0, sizeof(serv_addr));
      serv_addr.sin_family = AF_INET;
      serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
      serv_addr.sin_port = htons(9190);

      if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        printf("bind() error\n");



      while(1){

        clnt_addr_size = sizeof(clnt_addr);
        str_len = recvfrom(serv_sock, message, BUFSIZE, 0, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        sendto(serv_sock, message, strlen(message), 0, (struct sockaddr*)&clnt_addr, sizeof(clnt_addr));

        printf("message from client : %s\n", message);

        close(clnt_sock);
      }

      break;
  }


  return 0;
}
