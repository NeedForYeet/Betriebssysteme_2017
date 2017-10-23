#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define MAX_SEQ 10
#define SHMSEGSIZE sizeof(shared_data_t)

typedef struct shared_data_s {
	long fib_seq[MAX_SEQ];
	int seq_size;
} shared_data_t;

long fibo(long n);

int main(int argc, char** argv) {
	int i, size, shmID = 0;
	shared_data_t *shared_mem;
	pid_t pid;

	/* check argument */
	if (argc != 2) {
		printf("Usage: %s number\nNumber has to be between 0 and 11\n", argv[0]);
		return EXIT_FAILURE;
	}
	// check if input was an integer and 
	sscanf(argv[1], "%d", &size);
	if (size <= 0 || size > 11) {
		printf("Input number out of range.\n");
		return EXIT_FAILURE;
	} 

	/* create shared mem and attach to local address space */
	shmID = shmget(IPC_PRIVATE, SHMSEGSIZE, IPC_CREAT | 0644);
	if (shmID == -1) {
		perror("shmget failed.");
		return EXIT_FAILURE;
	}

	shared_mem = (shared_data_t*) shmat(shmID, 0, 0);
	if (shared_mem == (shared_data_t *) -1) {
		perror("shmat failed.");
		return EXIT_FAILURE;
	}

	/* set sequence size */
	shared_mem->seq_size = size;

	/* fork stuff */
	pid = fork();
	if (pid < (pid_t)0) {
		fprintf(stderr, "Fork failed.\n");
		return EXIT_FAILURE;
	}
	if (pid == (pid_t)0) {
		for (int i = 0; i < shared_mem->seq_size; i++) {
			shared_mem->fib_seq[i] = fibo(i);
		}

		printf("Exiting child...\n");
		// detatch from child and terminate
		if (shmdt(shared_mem) == -1) {
			perror("Child shmdt failed.");
			return EXIT_FAILURE;
		}
		return EXIT_SUCCESS;
	}

	/* code for parent */
	/* wait for child, i.e., our sync mechanism ;-) */
	waitpid(pid, NULL, 0);

	/* iterate over shared mem and display fib sequence */
	for (int i = 0; i < shared_mem->seq_size; i++) {
		printf("Parent: %ld\n", shared_mem->fib_seq[i]);
	}

	/* detach shared mem from local address space and remove shamred mem from system */
	if(shmdt(shared_mem) == -1) {
		perror("Parent shmdt failed.");
		return EXIT_FAILURE;
	}

	if(shmctl(shmID, IPC_RMID, 0) == -1) {
		perror("shmctl failed.");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

/* Calculates the Fibonacci number of a given number n */
long fibo(long n) {
	if (n == 0) {
		return 0;
	} else if (n == 1) {
		return 1;
	} else {
		return (fibo(n-1) + fibo(n-2));
	}

}
