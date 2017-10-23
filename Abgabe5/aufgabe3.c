#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

#define READ 0
#define WRITE 1

typedef struct data_s {
	int val;
	char c;
} data_t;

void init(data_t* data);
void calc(data_t* data);
int check(data_t* data);
void runParent(int childPipe[2], int parentPipe[2]);
void runChild(int childPipe[2], int parentPipe[2]);
void closePipe();

int main(void) {

	pid_t pid;
	int parentPipe[2];
	int childPipe[2];

	// create the pipes
	if(pipe(parentPipe) == -1) {
		perror("Parent pipe failed.");
		return EXIT_FAILURE;
	}
	if(pipe(childPipe) == -1) {
		perror("Child pipe failed.");
		return EXIT_FAILURE;
	}

	pid = fork();

	if (pid < 0) {
		perror("Fork failed.");
		return EXIT_FAILURE;
	}

	if (pid == 0) {
		runChild(childPipe, parentPipe);
	} 

	runParent(childPipe, parentPipe);

	// now terminate the program
	printf("Killing child...\n");
	if (kill(pid, SIGKILL) == -1) {
		perror("Failed to kill child.\n");
		return EXIT_FAILURE;
	}

	if(waitpid(pid, NULL, 0) == -1) {
		perror("Failed to wait for child.\n");
		return EXIT_FAILURE;
	}

	closePipe(childPipe);
	closePipe(parentPipe);

	return EXIT_SUCCESS;
}

void init(data_t* data) {
	data->val = 10;
	data->c = 'a';
}

void calc(data_t* data) {
	data->val--;
	data->c++;
}

int check(data_t* data) {
	// return 0 to end program if val == 0
	printf("Checking... Value = %d, Char = %c\n", data->val, data->c);
	if (data->val == 0) {
		return 0;
	}
	return 1;
}

void runParent(int childPipe[2], int parentPipe[2]) {
	// close unused ends of pipe
	close(childPipe[WRITE]);
	close(parentPipe[READ]);

	data_t parentData;
	init(&parentData);

	printf("Initial Value = %d, Char = %c\n", parentData.val, parentData.c);

	while (1) {
		sleep(1); // wait 1 second between sending
		// write to child
		if(write(parentPipe[WRITE], &parentData, sizeof(data_t)) <= 0) {
			perror("Error while sending data to child.\n");
			exit(EXIT_FAILURE);
		}
		
		//read from child
		if(read(childPipe[READ], &parentData, sizeof(data_t)) <= 0) {
			perror("Error while reading data from child.\n");
			exit(EXIT_FAILURE);
		}

		// abort if check returns 0 
		if (check(&parentData) == 0) {
			break;
		}
	}
}

void runChild(int childPipe[2], int parentPipe[2]) {
	// close unused ends of pipe
	close(parentPipe[WRITE]);
	close(childPipe[READ]);
	
	data_t childData;
	
	while (1) {
		// read from parent
		if(read(parentPipe[READ], &childData, sizeof(data_t)) <= 0) {
			perror("Error while reading data from parent.\n");
			exit(EXIT_FAILURE);
		}

		calc(&childData);
	
		// write to parent
		if(write(childPipe[WRITE], &childData, sizeof(data_t)) <= 0) {
			perror("Error while sending data to parent.\n");
			exit(EXIT_FAILURE);
		}
	}
}

void closePipe(int pipe[2]) {
	close(pipe[READ]);
	close(pipe[WRITE]);
}
