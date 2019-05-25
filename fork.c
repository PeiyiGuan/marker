/*  Author: Peiyi Guan */
/*  Student ID : 215328917  */

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
	was_alarm = 1;
	printf("Alarm triggered with code %d\n",i);
}

/* Prints string s using perror and exits. 
Also checks errno to make sure that it is not zero 
(if it is just prints s followed by newline 
using fprintf to the standard error) */
void f_error(char *s)
{
	if (errno != 0)
	{
		perror(s);
		exit(1);
	}
	else
	{
		fprintf(stderr, "%s: Error: %s\n", s, strerror(errno));
		exit(1);
	}
}

/* Creates a child process using fork and a function from the exec family 
 The standard input output and error are replaced by the last three 
 arguments to implement redirection and piping */

pid_t start_child(const char *path, char *const argv[],
				  int fdin, int fdout, int fderr)
{

	pid_t pid = fork();
	if (pid < 0)
	{
		fprintf(stderr, "%s: fork failed: %s\n", path, strerror(errno));
		exit(1);
	}

	if (pid == 0)
	{
		/*Testing alarm function*/
		/*
		while (1)
		{
			printf("%s waiting.....\n",path);
			sleep(1);
		}
		*/

		/* piping test.in to stdin */
		if (dup2(fdin, 0) < 0)
		{
			// error occurs
			fprintf(stderr, "%s: dup2 failed: %s\n", argv[0], strerror(errno));
			exit(1);
		}
		/* piping stderr to file test.err1 */
		if (dup2(fderr, 2) < 0)
		{
			// error
			fprintf(stderr, "%s: dup2 failed: %s\n", argv[0], strerror(errno));
			exit(1);
		}

		/* piping fd1 stdout to file */

		if (dup2(fdout, 1) < 0)
		{
			// error
			fprintf(stderr, "%s: dup2 failed: %s\n", argv[0], strerror(errno));
			exit(1);
		}

		close(fdin);  // close stdin
		close(fdout); // close stdout for pid 1
		close(fderr);
		execvp(path, argv);
	}

	return pid;
}
