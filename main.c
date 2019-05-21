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
#include "fork.h"


int was_alarm = 0;
/*
kill(2), sigaction(2), wait(2), pipe(2), fork(2), read(2), write(2), open(2), close(2),
signal(7), errno(3), dup(2), dup2(2)



/* The main program does much of the work. parses the command line arguments */
/* sets up the alarm and the alarm signal handler opens the files and pipes */
/* for redirection etc., invoke start_child, closes files that should be  */
/* closed, waits for the children to finish and reports their status */
/* or exits printing a message and kill its children if they do not die */
/* in time (just the parent exiting may not be enough to kill the children) */
int main(int argc, char *argv[])
{

  int sep_pos;
  int fd1[2];
  int fd2[2];
pid_t pid1, pid2, wid;

//  fd1[0] = open("test.in",O_RDONLY);
//  fd1[1] = open("test1.out",O_CREAT | O_APPEND| O_RDWR);
 // fd1[2] = open("test.err1",O_CREAT | O_APPEND| O_RDWR );
  
 // fd2[1] = open("test.out",O_CREAT| O_APPEND| O_RDWR );
 // fd2[2] = open("test.err2",O_CREAT | O_APPEND| O_RDWR );

  

  if (argc < 3)
  {
    printf("Invaliad number of arguments, must be greater than 3\n");
    exit(1);
  }

  for (int i = 0; i < argc; i++)
  {

    if (strcmp(argv[i], "-p-") == 0)
    { // equal 0 if -p- found
      sep_pos = i;
      argv[i] = NULL;
    }
  }

  char *p1_path = argv[1];
  int p1_argv_size = sep_pos - 1;
  char *p1_argv[p1_argv_size];

  for (int i = 0; i < p1_argv_size; i++)
  {
    p1_argv[i] = argv[i + 2];
    printf("p1 argv %s\n",p1_argv[i]);
  }

  // printf("sep_pos is %d\n", sep_pos);
  // printf("argc is is %d\n", argc);

  int p2_argv_size = (argc - sep_pos);
   printf("p2 size is %d\n", p2_argv_size);
  char *p2_path = argv[sep_pos];
  char *p2_argv[p2_argv_size];

  for (int i = 0; i < p2_argv_size-1; i++)
  {
    p2_argv[i] = argv[sep_pos + i + 1];
    printf("i is %d\n", i);
    printf("%s  \n", p2_argv[i]);
  }

/*
create pipes 
*/
  // if (pipe(fd1) < 0)
  // {
  //   fprintf(stderr, "%s: Error creating pipe: %s\n", argv[0], strerror(errno));
  //   exit(1);
  // }



// pid1 = start_child(p1_path, p1_argv, fd[0], fd[1], fd[2]);  // stdin stdout stderr

//   printf("pid returned from first child %d\n",pid1);

//   start_child(p2_path, p2_argv, fd[0], fd[1], fd[2]);  // stdin stdout stderr

  //pid2 = start_child(p1_path,p1_argv,fd1[0],fd1[1],fd1[2]);

pid1 = fork();
	if (pid1 < 0)
		{
			fprintf(stderr, "%s: fork failed: %s\n",p1_path, strerror(errno));
			exit(1);
		}

		if (pid1 == 0)
		{
   //   start_child(p1_path, p1_argv, fd1[0], fd1[1], fd1[2]);
      // if (dup2(fd1[2], 2) < 0)
			// {
			// 	fprintf(stderr, "%s: stderr failed: %s\n", p1_path, strerror(errno));
			// 	exit(1);
			// }
		
			// close(fd1[0]);
			// close(fd1[1]);
			// close(fd1[2]);
			execvp(p1_path, p1_argv);
			fprintf(stderr, "%s: execv failed: %s\n", p1_path, strerror(errno));
			exit(1);


		}

    // pid2 = fork();

    // if(pid2 < 0){
    //     fprintf(stderr, "%s: fork failed: %s\n",p2_path, strerror(errno));
		// 	  exit(1);
    //   }
    //   if(pid2==0){
    //  //   start_child(p2_path,p2_argv,fd2[0],fd2[1],fd2[2]);
    //   execvp(p2_path, p2_argv);
    // }



  wait(NULL);

  /*
  kill child after 3 seconds
  */
  return 0;
}
