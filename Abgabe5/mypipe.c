#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define FULLBASH "/bin/bash"
#define BASH "bash"
#define BASHOPTION "-c"

#define READ 0
#define WRITE 1

// functions for child processes
void childA(const char* command, int thePipe[2]);
void childB(const char* command, int thePipe[2]);

/**
 *  main program
 */
int main(int argc, char **argv) {
	
	/* ----------------------------- */
	/* step 0: check input arguments */
	if (argc < 2 || strcmp(argv[1], "-h")==0 || strcmp(argv[1], "--help")==0) {
		printf("Simple Unix pipe-like program. Redirects output from command1 to command2.\nUsage: %s \"command1\" \"command2\"\n", argv[0]);
	}
	
	/* ----------------------------- */
	/* step 1: pipes                 */
	int thePipe[2];    /* pipe:  */
	pid_t pid_c1, pid_c2;
	
	// setup the pipe and check for failures
	if (pipe(thePipe) == -1) {
		fprintf(stderr, "Pipe creation failed.\n");
		return EXIT_FAILURE;
	}
		
	/* ----------------------------- */
	/* step 2: fork stuff & co       */
	pid_c1 = fork();
	
	// check for failures during fork
	if (pid_c1 < (pid_t)0) {
		fprintf(stderr, "Fork of 1st child failed.\n");
		return EXIT_FAILURE;
	}
	
	if (pid_c1 == (pid_t)0) {
		/* code for first child */
		// function takes the entered command and the pipe
		childA(argv[1], thePipe);
		return EXIT_SUCCESS;
	}
	
	pid_c2 = fork();
	// check for failures during second fork
	if (pid_c2 < (pid_t)0) {
		fprintf(stderr, "Fork of 2nd child failed.\n");
		return EXIT_FAILURE;
	}
	
	if (pid_c2 == (pid_t)0) {
		/* code for second child */
		childB(argv[2], thePipe);
		return EXIT_SUCCESS;
	}
	
	/* code for parent */
	// wait for all children to exit and close the pipe
	waitpid(pid_c1, NULL, 0);
	close(thePipe[WRITE]);
	
	waitpid(pid_c2, NULL, 0);
	close(thePipe[READ]);
	
	return EXIT_SUCCESS;
}

void childA(const char* command, int thePipe[2]) {
	
	// duplicate the FD and abort if an error occurred.
	if (dup2(thePipe[WRITE], WRITE) == -1) {
		fprintf(stderr, "Failure during dup2 in childA.\nError: %d\n", errno);
		return;
	}

	// close read end of pipe
	close(thePipe[READ]);

	system(command);
	//execvp(command, command);
}

void childB(const char* command, int thePipe[2]) {
	if (dup2(thePipe[READ], READ) == -1) {
		fprintf(stderr, "Failure during dup2 in childB.\nError: %d\n", errno);
		return;
	}

	// close write end of pipe
	close(thePipe[WRITE]);
	
	system(command);
	//execvp(command, command);
}
