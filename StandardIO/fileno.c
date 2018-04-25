#include <stdio.h>
#include <fcntl.h>

int main(void)
{
  FILE *fp;
  int fd = open("data.dat", O_WRONLY|O_CREAT|O_TRUNC);
  //open function return file descriptor
  // write only OR if file is not exist then create the file OR Delete all existing content
  if( fd == -1 )
  {
    fputs("file open error\n", stdout);
    return -1;
  }

  printf("First file descriptor : %d \n", fd);
  fp = fdopen(fd, "w"); // fd file descriptor to FILE pointer
  fputs("TCP/IP SOCKET PROGRAMMING \n", fp);
  printf("Second file descriptor : %d \n", fileno(fp));
  fclose(fp);
  return 0;
}
