#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/unistd.h>

#define MAXCOUNT 100000000
#define NUM_CHILDREN 4
#define SHMSEGSIZE sizeof(int)

int main() {
	int i, shmID, *shared_mem, count = 0;
	int pid[NUM_CHILDREN]; // array of child PIDs
	shmID = shmget(IPC_PRIVATE, SHMSEGSIZE, IPC_CREAT | 0644); // create a new shm segment with size SHMSEGSIZE and permissions 644 (rw,r,r)
	shared_mem = (int *)shmat(shmID, 0, 0); // attach the segment to the program's virtual memory. shared_mem stores the address where the new segment is located. shmID = ID of the segment to attach, 2nd parameter = memory address it will be mapped to (0 means OS will find a suitable address), 3rd parameter = default flags (not SHM_EXEC, SHM_RDONLY, SHM_REMAP).
	*shared_mem = 0; // set value at shm segment to 0
	// fork 4 children
	for (i=0; i < NUM_CHILDREN; i++) {
		pid[i] = fork();
		if (pid[i] == -1) {
			printf("%iter Kind-Prozess nicht erzeugbar!\n", i);
			return EXIT_FAILURE;

		}
		// all children will branch here
		if (pid[i] == 0) {
/* 
 * Problem: Alle child-Prozesse greifen hier gleichzeitig auf das shm Segment zu. Dadurch kann (wird) es passieren, dass ein child einen bereits veralteten Wert liest, bearbeitet und danach zurückschreibt. 
 * Z.B.: c1 liest den Wert 10 vom Segment während c2 schon 11 berechnet hat. Kurz nachdem c1 den Wert 10 gelesen hat, schreibt c2 den Wert 11 ins shm Segment. c1 führt danach die berechnungen mit dem gelesenen Wert 10 durch, erhöht ihn auf 11 und schreibt ebenfalls 11 zurück. Der selbe Wert wurde also mehrmals berechnet und geschrieben.
 * Abhilfe schaffen kann hier die Synchronisierung der einzelnen child-Prozesse untereinander.
 * */

			// loop until value at shm segment is smaller than 100 million
			while (*shared_mem < MAXCOUNT) {
				*shared_mem += 1; // increment the value at shm segment
				count++; // count how often the child incremented

			}
			printf("%ites Kind erhoehte Wert um %i!\n", i, count);
			shmdt(shared_mem); // detach shm segment from child process
			return EXIT_SUCCESS; // exit child

		}
	}

	// parent waits for all children to exit
	for (i=0; i < NUM_CHILDREN; i++) {
		waitpid(pid[i], NULL, 0);

	}
	
	printf("Shared Memory = %i - MAXCOUNT = %i\n", *shared_mem, MAXCOUNT); // print the final result stored in shm segment and MAXCOUNT
	shmdt(shared_mem); // detach shm segment from parent
	shmctl(shmID, IPC_RMID, 0); // and flag for destruction (destroyed after last process detaches the segment) 
	return EXIT_SUCCESS;
}
