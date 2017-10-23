#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#define WATCH_FILE "/etc/bash.bashrc"
void signalHandler(int signalNumber);
void printFile();
int main(void) {

	// fork the process
	pid_t pid, sid;

	pid = fork();
	if(pid < (pid_t)0) {
		fprintf(stderr, "Fork failed.\n");
		return EXIT_FAILURE;
	}
	// kill the parent if the fork was successful
	if (pid > (pid_t)0) {
		printf("Fork successful. Exiting parent.\n\n------------\n\n");
		return EXIT_SUCCESS;
	}

	// change file mode mask for access to the file system
	umask(0);

	// TODO: log files?
/*	FILE *fpLog = fopen("daemon.log", "w");
	fwrite("Hello.", 6, 1, fpLog);
*/
	// create session ID
	sid = setsid(); 
	if(sid < (pid_t)0) {
		perror("setsid() failed.\n");
		return EXIT_FAILURE;
	}

	// change cwd to root
	if (chdir("/") < 0) {
		perror("Changing directory failed.\n");
		return EXIT_FAILURE;
	}

	// close standard file descriptors
	// NOTE: STDOUT_FILENO remains open, because we have to write to it later.
	// Alternatively we could write the daemon's output into a file.
	// Also keep STDERR open for debugging purposes
	close(STDIN_FILENO);
	//close(STDERR_FILENO);
	//close(STDOUT_FILENO);

	// register signal handler
	if(signal(SIGUSR1, signalHandler) == SIG_ERR) {
		perror("Error while registering signal handler.\n");
		exit(EXIT_FAILURE);
	}


	printFile();

	// main daemon loop
	// pause() returns only when a signal was caught
	// errno is then set to EINTR
	while (pause() == -1 && errno == EINTR);
}

void signalHandler(int signalNumber) {
	if (signalNumber == SIGUSR1) {
		printFile();

		// register signal handler again, because otherwise
		// it will use a default one
		if(signal(SIGUSR1, signalHandler) == SIG_ERR) {
			perror("Error while registering signal handler.\n");
			exit(EXIT_FAILURE);
		}
	}
}

void printFile() {
	FILE* fp = fopen(WATCH_FILE, "r");
	if (fp == NULL) {
		perror("Error while opening file.\n");
		exit(EXIT_FAILURE);
	}

	for (char c = fgetc(fp); c != EOF; c = fgetc(fp)) {
		// printf works only because we didn't close STDIN above.
		printf("%c", c);
	}

	fflush(stdout);

	fclose(fp);
}
