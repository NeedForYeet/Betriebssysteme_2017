#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void wait_for_all_children() {
	waitpid(-1, NULL, 0);
}
int main(void) {
	int n=0; 
	int pid=0;
	pid = fork(); // child receives 0 as pid, parent receives pid of child
	n++;
	if (pid==0 && n){ //only child will branch here
		fork();
		sleep(10);
	}
		
	else {
		sleep(10);
		exit(0);
	}
	// this should only be reached by the 3rd fork, which does nothing for it.
	wait_for_all_children();
	exit(0);
}
