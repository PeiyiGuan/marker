/*  Author: Peiyi Guan */

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
  pid_t p[2];

  struct sigaction psa;
  psa.sa_handler = alrm_handler; 

  sigaction(SIGALRM,&psa,0);
  alarm(3);

  fd1_test_in = open("test.in", O_RDONLY); //stdin
  fd1_err = open("test.err1", O_CREAT | O_APPEND | O_WRONLY, S_IRUSR | S_IWUSR );
  fd2_err = open("test.err2", O_CREAT | O_APPEND | O_WRONLY, S_IRUSR | S_IWUSR );
  fd2_out = open("test.out", O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR );

  if (argc < 3)
  {
    printf("Invaliad number of arguments, must be greater than 3\n");
    exit(1);
  }

  /* look for -p- seperator and replace it with NULL pointers s*/
  int k;
  for ( k = 0; k < argc; k++)
  {

    if (strcmp(argv[k], "-p-") == 0) // strcmp equal 0 if -p- found
    {
      sep_pos = k;
      argv[k] = NULL;
    }
  }

  /*--------- formatting argv ---------- */

  /* First Process argv formatting */
  char *p1_path = argv[1];
  int p1_argv_size = sep_pos;
  char *p1_argv[p1_argv_size];
  int i;
  for ( i = 0; i < p1_argv_size; i++)
  {
    p1_argv[i] = argv[i + 1];
    //  printf("p1 argv %s\n", p1_argv[i]);
  }
  /* Second Process argv formatting */
  int p2_argv_size = (argc - sep_pos);
  //printf("p2 size is %d\n", p2_argv_size);
  char *p2_path = argv[sep_pos + 1];
  char *p2_argv[p2_argv_size];

  int j;
  for ( j = 0; j < p2_argv_size; j++)
  {
    p2_argv[j] = argv[sep_pos + j + 1];
    // printf("p2 argv %s\n", p2_argv[i]);
  }

  /* ---------- creating pipe ------------------ */
  if (pipe(fd) < 0)
  {
    f_error(argv[0]);
  }

  /* First child process forking */
  p[0] = start_child(p1_path, p1_argv, fd1_test_in, fd[1], fd1_err);
  close(fd[1]);
  /*-------------------------------*/

  /* --- Second child process --- */
  p[1] = start_child(p2_path, p2_argv, fd[0], fd2_out, fd2_err);

  close(fd[1]);
  close(fd[0]);
  /*-------------------------------*/



  /* Print finished status */
  int p1_finish_status, p2_finish_status;
  int p1_wait_pid = waitpid(p[0], &p1_finish_status, 0);


  if(p1_wait_pid== -1 && was_alarm ==1 ){
      kill(p[0],9); 
      kill(p[1],9);
      fprintf(stderr, "marker: At least one process did not finish\n");
      exit(1);
  }

  if (WIFEXITED(p1_finish_status))
  {
    int exit_status = WEXITSTATUS(p1_finish_status);
    printf("Process %s finished with status %d\n",
            p1_path, exit_status);
  }

 
  int p2_wait_pid = waitpid(p[1], &p2_finish_status, 0);

  if(p2_wait_pid== -1 && was_alarm ==1 ){
    kill(p[1],9);
    fprintf(stderr, "marker: At least one process did not finish\n");
    exit(1);
  }

  if (WIFEXITED(p2_finish_status))
  {
    int exit_status = WEXITSTATUS(p2_finish_status);
    printf("Process %s finished with status %d\n",
           p2_path, exit_status);
  }

  return 0;
}
