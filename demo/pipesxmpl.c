/* Example code of a process firing of two children connected by a pipe */
/* By: Minas Spetsakis */
/* Date: Sep. 21 2016 */

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  pid_t pid1, pid2, wid;
  int fd[2];

  if (pipe(fd) < 0)
  {
    fprintf(stderr, "%s: Error creating pipe: %s\n", argv[0], strerror(errno));
    exit(1);
  }
  pid1 = fork();
  if (pid1 < 0)
  {
    fprintf(stderr, "%s: fork failed: %s\n", argv[0], strerror(errno));
    exit(1);
  }

  if (pid1 == 0)
  {
    if (dup2(fd[1], 1) < 0) // stdout of program 1
    {
      fprintf(stderr, "%s: dup2 failed: %s\n", argv[0], strerror(errno));
      exit(1);
    }
    close(fd[0]); // close stdin
    close(fd[1]); // close stdout for pid 1
    /*fprintf(stderr,"Starting %s\n",argv[1]);*/
    execlp(argv[1], argv[1], (char *)NULL);
    fprintf(stderr, "%s: execlp failed: %s\n", argv[0], strerror(errno));
    exit(1);
  }

  pid2 = fork();
  if (pid2 < 0)
  {
    fprintf(stderr, "%s: fork failed: %s\n", argv[0], strerror(errno));
    exit(1);
  }
  if (pid2 == 0)
  {
    if (dup2(fd[0], 0) < 0) // connect stdin of program2 to stdin
    {
      fprintf(stderr, "%s: dup2 failed: %s\n", argv[0], strerror(errno));
      exit(1);
    }
    close(fd[0]);
    close(fd[1]);
    /*fprintf(stderr,"Starting %s\n",argv[2]);*/
    execlp(argv[2], argv[2], (char *)NULL);
    fprintf(stderr, "%s: execlp failed: %s\n", argv[0], strerror(errno));
    exit(1);
  }

  close(fd[1]);
  close(fd[0]);

  wid = wait(NULL);
  if (wid == pid1)
    printf("Process %d (%s) finished\n", wid, argv[1]);
  if (wid == pid2)
    printf("Process %d (%s) finished\n", wid, argv[2]);
  wid = wait(NULL);

  if (wid == pid1)
    printf("Process %d (%s) finished\n", wid, argv[1]);
  if (wid == pid2)
    printf("Process %d (%s) finished\n", wid, argv[2]);
}