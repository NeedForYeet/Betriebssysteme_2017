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
	int i, shmID, *shared_mem, count = 0, shmSpinID;
	int pid[NUM_CHILDREN]; // array of child PIDs
	pthread_spinlock_t* spinlock;

	// create shm segment for spinlock, since all processes need access
	shmSpinID = shmget(IPC_PRIVATE, sizeof(pthread_spinlock_t), IPC_CREAT | 0644);
	spinlock = (pthread_spinlock_t*)shmat(shmSpinID, 0, 0); // spinlock now stores address of shm segment where the semaphore will be stored

	shmID = shmget(IPC_PRIVATE, SHMSEGSIZE, IPC_CREAT | 0644); // create a new shm segment with size SHMSEGSIZE and permissions 644 (rw,r,r)
	shared_mem = (int *)shmat(shmID, 0, 0); // attach the segment to the program's virtual memory. shared_mem stores the address where the new segment is located.
	*shared_mem = 0; // set value at shm segment to 0

	// initialize the spinlock. PTHREAD_PROCESS_SHARED indicates that all processes will have access to the segment
	if (pthread_spin_init(spinlock, PTHREAD_PROCESS_SHARED) != 0) {
		perror("Error during pthread_spin_init\n");
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

			// lock the spinlock for the first process that comes here
			// all others will be blocked until the spinlock is released
			if (pthread_spin_lock(spinlock) != 0) {
				fprintf(stderr, "Child %i failed to lock spinlock.\n", i);
			}
			// loop until value at shm segment is smaller than 100 million
			while (*shared_mem < MAXCOUNT) {
				*shared_mem += 1; // increment the value at shm segment
			
				// unlock the spinlock for other processes. 
				// other threads can now acquire the lock.
				pthread_spin_unlock(spinlock);

				count++; // count how often the child incremented
			
				// wait for unlock.
				pthread_spin_lock(spinlock);
			}

			// unlock spinlock after the loop is done as destroying
			// a locked spinlock may result in undefined behaviour.
			pthread_spin_unlock(spinlock);
			shmdt((void*)spinlock); // also detach the shm segment from child. cast to void* because pthread_spinlock_t is volatile and GCC will complain.

			printf("%ites Kind erhoehte Wert um %i!\n", i, count);
			shmdt(shared_mem); // detach shm segment from child process
			return EXIT_SUCCESS; // exit child

		}
	}

	// parent waits for all children to exit
	for (i=0; i < NUM_CHILDREN; i++) {
		waitpid(pid[i], NULL, 0);

	}

	// destroy spinlock and shm segment
	pthread_spin_destroy(spinlock);
	shmdt((void*)spinlock);
	shmctl(shmID, IPC_RMID, 0);

	printf("Shared Memory = %i - MAXCOUNT = %i\n", *shared_mem, MAXCOUNT); // print the final result stored in shm segment and MAXCOUNT
	shmdt(shared_mem); // detach shm segment from parent
	shmctl(shmID, IPC_RMID, 0); // and flag for destruction (destroyed after last process detaches the segment) 
	return EXIT_SUCCESS;
}
