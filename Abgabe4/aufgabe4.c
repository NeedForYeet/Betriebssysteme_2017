/**
 * UE Betriebssysteme
 *
 * Author : Christian Timmerer <christian.timmerer@itec.uni-klu.ac.at>
 * Date   : 2017-03-24
 * Version: 0.0.1
 * 
 * Description:
 *    Exercise 4.4 of SS2017
 *    parent provides int values through pipe
 *    a child performs calculation and 
 *    displays the result on the screen.
 * 
 * Student information:
 *  gid      : none -- enter your group number here
 *  uid      : 1561281 -- enter your student id here
 *  lastname : Jensch -- enter your last name here
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

/* define preprocessor macros */
// 0 is read end of pipe, 1 is write end
#define READ 0
#define WRITE 1

static int count;

/**
 *  main program
 */
int main(int argc, char **argv) {

	/* ----------------------------- */
	/* step 0: check input arguments */
	/* TODO -- check for any input arguments, especially for -h and --help
	 *         print usage information to display if required
	 */
	if (argc > 2){
		if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
			printf("Usage: %s command\n", argv[0]);
			return EXIT_SUCCESS;
		}
	}

	// how many variables?
	printf("Enter the amount of values to calculate: ");
	scanf("%d", &count);

	/* ----------------------------- */
	/* step 1: some variables ...    */
	time_t t1;
	(void) time(&t1);
//	srand48((long) t1); /* use time in seconds to set seed */

	/* ---------------------------- */
	/* step 3: create pipe          */
	int thePipe[2];
	pid_t pid;

	// pipe() returns -1 on failure
	if (pipe(thePipe) == -1) {
		fprintf(stderr, "Pipe creation failed.\n");
		return EXIT_FAILURE;
	}

	/* ----------------------------- */
	/* step 4: fork stuff & co       */
	pid = (pid_t)fork();
	if (pid < (pid_t)0) {
		/* step 4.1: fork failed */
		fprintf(stderr, "Fork failed.\n");
		return EXIT_FAILURE;
	}

	// child reads from pipe
	if (pid == (pid_t)0) {
		float buffer[count];
		/* step 4.2: code for child */
		// close unused write end, as child will only read
		close(thePipe[WRITE]);

		int index = 0;
		// read until parent stops sending data
		while (read(thePipe[READ], &buffer[index], sizeof(float)) > 0) {
			index++;
		}

		printf("\n");

		float sum = 0;
		// perform the calculations
		for (int i = 0; i < count; i++) {
			printf("%f ", buffer[i]);
			sum += buffer[i];
		}
		
		printf("\nSumme: %f\n", sum);


		/*close pipes and exit */
		close(thePipe[READ]);
		return EXIT_SUCCESS;
	}


	/* step 4.3: code for parent */
	/* TODO read data from user or file */
	//int count;
	float* input;

	// allocate space for input array
	input = (float*)malloc(count * sizeof(float));

	
	close(thePipe[READ]);

	//read values from stdin
	for (int i = 0; i < count; i++) {
		scanf("%f", &input[i]);
		write(thePipe[WRITE], &input[i], sizeof(float));
	}

	/* TODO close unnecessary pipes */

	/* TODO write actual int values to child */
/*	for (int i = 0; i < count; i++) {
		write(thePipe[WRITE], &input[i], 1);
	}
*/
	//write(thePipe[WRITE], &input, count);
	/* TODO close pipes */
	close(thePipe[WRITE]);


	/* TODO wait for child and exit*/
	waitpid(-1, NULL, 0);
	free(input);
	return EXIT_SUCCESS;
}
