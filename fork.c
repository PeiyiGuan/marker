#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "fork.h"

/* This is the handler of the alarm signal. It just updates was_alarm */
void alrm_handler(int i)
{
	printf("inside alarm handler");
}

/* Prints string s using perror and exits. Also checks errno to make */
/* sure that it is not zero (if it is just prints s followed by newline */
/* using fprintf to the standard error */
void f_error(char *s)
{
	printf("inside f_error");
}

/* Creates a child process using fork and a function from the exec family 
 The standard input output and error are replaced by the last three 
 arguments to implement redirection and piping */
 
// int processCheck(char *const argv[])
// {
// 	if (argv[0] != NULL)
// 	{
// 		return 1;
// 	}
// 	else
// 	{
// 		return 2;
// 	}
// }

pid_t start_child(const char *path, char *const argv[],
				  int fdin, int fdout, int fderr)
{


		/* In side child process */		
		
		if (dup2(fdout, 1) < 0)
			{
				fprintf(stdout, "%s: dup2 failed: %s\n", path, strerror(errno));
				exit(1);
			}
		if (dup2(fderr, 2) < 0)
			{
				fprintf(stderr, "%s: stderr failed: %s\n", path, strerror(errno));
				exit(1);
			}
		
			close(fdin);
			close(fderr);
			close(fdout);
			execvp(path, argv);
			fprintf(stderr, "%s: execv failed: %s\n", path, strerror(errno));
			exit(1);


}
