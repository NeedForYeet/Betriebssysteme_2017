#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/unistd.h>
#include <semaphore.h>
#include <pthread.h>

#define MAXCOUNT 100000000
#define NUM_CHILDREN 4
#define SHMSEGSIZE sizeof(int)

int main() {
	int i, shmID, *shared_mem, count = 0, shmSemID;
	int pid[NUM_CHILDREN]; // array of child PIDs
	sem_t* semaphore;

	// create shm segment for semaphore, since all processes need access
	shmSemID = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 0644);
	semaphore = (sem_t*)shmat(shmSemID, 0, 0); // semaphore now stores address of shm segment where the semaphore will be stored

	shmID = shmget(IPC_PRIVATE, SHMSEGSIZE, IPC_CREAT | 0644); // create a new shm segment with size SHMSEGSIZE and permissions 644 (rw,r,r)
	shared_mem = (int *)shmat(shmID, 0, 0); // attach the segment to the program's virtual memory. shared_mem stores the address where the new segment is located.
	*shared_mem = 0; // set value at shm segment to 0

	if (sem_init(semaphore, 1, 1) == -1) {
		perror("Failure during sem_init.\n");
		return EXIT_FAILURE;
	}

	// fork 4 children
	for (i=0; i < NUM_CHILDREN; i++) {
		pid[i] = fork();
		if (pid[i] == -1) {
			printf("%iter Kind-Prozess nicht erzeugbar!\n", i);
			return EXIT_FAILURE;

		}
		// all children will branch here
		if (pid[i] == 0) {

			// wait for unlock here and check for error
			// if semaphore is >0, it will be decremented and a process can proceed.
			// if it's 0, other processes will be blocked
			if (sem_wait(semaphore) == -1) {
				fprintf(stderr, "Child %i failed to wait for semaphore.\n", i);
			}
			// loop until value at shm segment is smaller than 100 million
			while (*shared_mem < MAXCOUNT) {
				*shared_mem += 1; // increment the value at shm segment
			
				// unlock the semaphore for other processes. 
				// the Value will be increased by 1.
				sem_post(semaphore);
				
				count++; // count how often the child incremented
			
				// wait for unlock (>0) again.
				sem_wait(semaphore);
			}

			// unlock semaphore after the loop is done as destroying
			// a locked semaphore may result in undefined behaviour.
			sem_post(semaphore);
			shmdt(semaphore); // also detach the shm segment from child

			printf("%ites Kind erhoehte Wert um %i!\n", i, count);
			shmdt(shared_mem); // detach shm segment from child process
			return EXIT_SUCCESS; // exit child

		}
	}

	// parent waits for all children to exit
	for (i=0; i < NUM_CHILDREN; i++) {
		waitpid(pid[i], NULL, 0);

	}

	// destroy semaphore and shm segment
	sem_destroy(semaphore);
	shmdt(semaphore);
	shmctl(shmID, IPC_RMID, 0);

	printf("Shared Memory = %i - MAXCOUNT = %i\n", *shared_mem, MAXCOUNT); // print the final result stored in shm segment and MAXCOUNT
	shmdt(shared_mem); // detach shm segment from parent
	shmctl(shmID, IPC_RMID, 0); // and flag for destruction (destroyed after last process detaches the segment) 
	return EXIT_SUCCESS;
}
