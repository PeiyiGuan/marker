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
#include <signal.h>
#include "fork.h"

int was_alarm = 0;

int main(int argc, char *argv[])
{

  int sep_pos;
  int fd[2];
  int fd1_test_in;
  int fd1_err;
  int fd2_err;
  int fd2_out;
  
  pid_t pid1, pid2, wid;

  fd1_test_in = open("test.in", O_RDONLY); //stdin
  fd1_err = open("test.err1", O_CREAT | O_APPEND | O_WRONLY, S_IRUSR | S_IWUSR | S_IXUSR);
  fd2_err = open("test.err2", O_CREAT | O_APPEND | O_WRONLY, S_IRUSR | S_IWUSR | S_IXUSR);
  fd2_out = open("test.out", O_CREAT | O_APPEND | O_WRONLY, S_IRUSR | S_IWUSR | S_IXUSR);

  if (argc < 3)
  {
    printf("Invaliad number of arguments, must be greater than 3\n");
    exit(1);
  }
  
  /* look for -p- seperator and replace it with NULL pointers s*/
  for (int i = 0; i < argc; i++)
  {

    if (strcmp(argv[i], "-p-") == 0) // strcmp equal 0 if -p- found
    {
      sep_pos = i;
      argv[i] = NULL;
    }
  }

  /*--------- formatting argv ---------- */

  /* First Process argv formatting */
  char *p1_path = argv[1];
  int p1_argv_size = sep_pos;
  char *p1_argv[p1_argv_size];

  for (int i = 0; i < p1_argv_size; i++)
  {
    p1_argv[i] = argv[i + 1];
    printf("p1 argv %s\n", p1_argv[i]);
  }
  /* Second Process argv formatting */
  int p2_argv_size = (argc - sep_pos);
  printf("p2 size is %d\n", p2_argv_size);
  char *p2_path = argv[sep_pos + 1];
  char *p2_argv[p2_argv_size];

  for (int i = 0; i < p2_argv_size; i++)
  {
    p2_argv[i] = argv[sep_pos + i + 1];
    // printf("p2 argv %s\n", p2_argv[i]);
  }

  /* ---------- creating pipe ------------------ */
  if (pipe(fd) < 0)
  {
    f_error(argv[0]);
  }

  /* First child process forking */
  pid1 = fork();
  if (pid1 < 0)
  {
    fprintf(stderr, "%s: fork failed: %s\n", p1_path, strerror(errno));
    exit(1);
  }

  if (pid1 == 0)
  {
    start_child(p1_path, p1_argv, fd1_test_in, fd[1], fd1_err);
    exit(1);
  }

  close(fd[1]);

  /* --- Second child process --- */
  pid2 = fork();
  if (pid2 < 0)
  {
    fprintf(stderr, "%s: fork failed: %s\n", p2_path, strerror(errno));
    exit(1);
  }
  if (pid2 == 0)
  {
    start_child(p2_path, p2_argv, fd[0], fd2_out, fd2_err);
  }

  close(fd[1]);
  close(fd[0]);

  int status1, status2;

  waitpid(pid1, &status1, 0);

  if (WIFEXITED(status1))
  {
    int exit_status = WEXITSTATUS(status1);
    printf("Process %d (%s) finished with status %d\n",
           wid, p1_path, exit_status);
  }

  waitpid(pid2, &status2, 0);

  if (WIFEXITED(status2))
  {
    int exit_status = WEXITSTATUS(status2);
    printf("Process %d (%s) finished with status %d\n",
           wid, p2_path, exit_status);
  }


  /*
  kill child after 3 seconds
  */
  return 0;
}
