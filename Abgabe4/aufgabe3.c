/**
 * UE Betriebssysteme
 *
 * Author : Christian Timmerer <christian.timmerer@itec.uni-klu.ac.at>
 * Date   : 2017-03-24
 * Version: 0.0.1
 * 
 * Description:
 *   Exercise 4.3 of SS2017
 *   Fork demonstration; command line intepreter using exec
 *
 * Student information:
 *   gid      : none -- enter your group number here
 *   uid      : 1561281 -- enter your student id here
 *   lastname : Jensch -- enter your last name here
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
 *  main program
 */
int main(int argc, char **argv) {

	/* ----------------------------- */
	/* step 0: check input arguments */
	/* TODO -- check for any input arguments, especially for -h and --help
	 *         print usage information to dispay if required
	 */
	if (argc > 1) {

		// check if -h or --help was entered
		if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
			printf("Usage: %s command\n", argv[0]);
			return EXIT_SUCCESS;
		}
	}


	/* ----------------------------- */
	/* step 1: some variables ...    */
	pid_t pid;

	/* ------------------------------ */
	/* step 3: do the work ...        */
	fprintf(stdout, "[P]\tcurrent process id = %i\n", (int)getpid());

	/* step 3.1: create new process via fork */
	/* TODO fork() */
	pid = fork();
	/* step 3.2: provide code for the parent process */
	/* TODO pid > (pid_t)0 */
	/*      just wait for child */
	if (pid > (pid_t)0) {
		waitpid(-1, NULL, 0);
	}
	/* step 3.3: provide code for the child process */
	/* TODO pid == (pid_t)0 */
	/*      execute programm provided through command line */
	if (pid == (pid_t)0) {
		// leave the first element of argv out
		// and use first element of childArgs as first argument for execvp
		char** childArgs = argv + 1;
		execvp(*childArgs, childArgs);
	}

	/* step 3.4: code for the error case, i.e., fork failed */
	/* TODO pid < (pid_t)0 */
	if (pid < (pid_t)0) {
		perror("Fail while forking");
	}

	return EXIT_SUCCESS;
}

