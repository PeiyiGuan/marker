#include<stdio.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int args,char* argv[]){

    int fd,fd2,fd1[3];
    // fd = open("test1.out", O_CREAT | O_APPEND | O_WRONLY);
    // fd2 = open("test.in", O_RDONLY);




  fd1[0] = open("test.in",O_RDONLY);
  fd1[1] = open("test1.out", O_CREAT|O_APPEND|O_RDWR,S_IRUSR|S_IWUSR|S_IXUSR);
  fd1[2] = open("test.err1",O_CREAT|O_APPEND| O_RDWR,S_IRUSR|S_IWUSR|S_IXUSR);

  printf("fd0 is %d\n",fd1[0]);
  printf("fd1 is %d\n",fd1[1]);
  printf("fd2 is %d\n",fd1[2]);

  //    if (dup2(fd, 1) < 0)
  //   {
  //     // error
  //     fprintf(stderr, "%s: dup2 failed in fd1: %s\n", argv[0], strerror(errno));
  //     exit(1);
  //   }

  //   if(dup2(fd2, 0) < 0)
  //   {
  //    fprintf(stderr, "%s: dup2 failed in fd2: %s\n", argv[0], strerror(errno));
  //     exit(1);
  //   }
  
  //   execlp("cat","cat", (char *)NULL);
  // //  execlp("ls","ls", (char *)NULL);
  //   close(fd); 
  //   close(fd2);
    return 0;
}