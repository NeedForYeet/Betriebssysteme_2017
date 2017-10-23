#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FIFO "./FILE_FIFO"

int main(void) {

	float input;
	int count;
	
	// how many variables?
	printf("Enter the amount of values to calculate: ");
	scanf("%d", &count);
	
	mkfifo(FIFO, 0666); // create a FIFO file

	// and open it via file descriptor as write only
	int fifo = open(FIFO, O_WRONLY);

	// get user input and write to named pipe
	for (int i = 0; i < count; i++) {
		scanf("%f", &input);
		write(fifo, &input, sizeof(float));

	}

	// close fd and exit
	close(fifo);	
	return EXIT_SUCCESS;
}
