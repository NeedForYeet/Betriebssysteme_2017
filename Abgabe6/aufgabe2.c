#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>

typedef struct msgbuf_s {
	long mtype;
	int mresult;
} msgbuf_t;
int collatz(int n);

int main (int argc, char** argv) {
	
	int input, queueID;
	pid_t pid;
	msgbuf_t *msgbuf;

	// print usage help
	if (argc != 2 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
		printf("Usage: %s number\n", argv[0]);
		return EXIT_FAILURE;
	}

	input = atoi(argv[1]);
	if (input < 1) {
		printf("Number has to be >0");
		return EXIT_FAILURE;
	}

	// create message queue and msgbuf
	queueID = msgget(IPC_PRIVATE, IPC_CREAT | 0644);
	if(queueID == -1) {
		perror("msgget failed.");
		return EXIT_FAILURE;
	}
	msgbuf = malloc(sizeof(msgbuf_t));
	msgbuf->mtype = 1;

	// fork child
	pid = fork();

	if(pid < 0) {
		perror("Fork failed.");
		return EXIT_FAILURE;
	} 

	// code for child
	if(pid == 0) {
		printf("Child: %d\n", input);
		msgbuf->mresult = input;
		msgsnd(queueID, msgbuf, sizeof(msgbuf_t), 0);

		while(msgbuf->mresult != 1) {
			msgbuf->mresult = collatz(msgbuf->mresult);
			printf("Child: %d\n", msgbuf->mresult);
			msgsnd(queueID, msgbuf, sizeof(msgbuf_t), 0);
		}

		// free and exit
		free(msgbuf);		
		return EXIT_SUCCESS;
	}

	// read from message queue until 1 is read
	do {
		msgrcv(queueID, msgbuf, sizeof(msgbuf_t), 1, 0);
		printf("Parent: %d\n", msgbuf->mresult);
	} while (msgbuf->mresult != 1);

	waitpid(pid, NULL, 0);

	// remove queue and free struct
	msgctl(queueID, IPC_RMID, NULL);
	free(msgbuf);

	return EXIT_SUCCESS;
}

int collatz(int n) {
	if (n == 1) {
		return 1;
	} else if ((n%2) == 0) {
		return n/2;
	} else {
		return 3*n+1;
	}
}
