/**
 * UE Betriebssysteme
 * 
 * Author : Christian Timmerer <christian.timmerer@itec.uni-klu.ac.at>
 * Date   : 2017-03-24
 * Version: 0.0.1
 * 
 * Description:
 *   Exercise 4.2 of SS2017
 *   Fork demonstration; parent and child iterates over loop
 * 
 * Student information:
 *   gid      : none -- enter your group number here
 *   uid      : 1561281 -- enter your student id here
 *   lastname : Jensch -- enter your last name here
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

void parentLoop() {
	pid_t pid = getpid();
	int t;
	time_t start;
	time_t now;

	for (int i = 0; i < 0xABCD; i++){
		printf("--P--%d--%d\n", pid, (i*2)+1);

		// busy waiting
		t = rand() % 3;
		start = time(NULL);
		do {
			now = time(NULL);

		} while (now-start <t);
	} 
}

void childLoop() {
	pid_t pid = getpid();
	int t;
	for (int i = 0; i < 0xABCD; i++){
		printf("--C--%d--%d\n", pid, i*2);

		// sleep and wakeup
		t = rand() % 3;
		sleep(t);
	}
}

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
		printf("Program takes no parameters.\nUsage: %s\n", argv[0]);
	}

	/* ----------------------------- */
	/* step 1: some variables ...    */
	pid_t pid;

	/* ------------------------------ */
	/* step 3: do the work ...        */
	/* step 3.1: create new process via fork */
	/* TODO fork() */
	pid = fork();

	/* step 3.2: provide code for the parent process */
	/* TODO pid > (pid_t)0 */
	if (pid > (pid_t)0) {
		parentLoop();
	} 
	/* step 3.3: provide code for the child process */
	/* TODO pid == (pid_t)0 */
	if (pid == (pid_t)0) {
		childLoop();
	}

	/* step 3.4: code for the error case, i.e., fork failed */
	/* TODO pid < (pid_t)0 */
	if (pid < (pid_t)0) {
		perror("Fail during fork().\n");
		return EXIT_FAILURE;
	}

	// and wait for all processes to finish
	// waitpid returns -1 on failure
	if (waitpid(-1, NULL, 0) < 0) {
		perror("Error while waiting for child processes.\n");
	}


	return EXIT_SUCCESS;
};
