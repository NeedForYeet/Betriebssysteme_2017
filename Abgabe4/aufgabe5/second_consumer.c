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

int runConsumer() {
	double sum = 0;
	int bytesRead;
	float value;

	// set file descriptor to named pipe and open as RO
	int fd = open(FIFO, O_RDONLY);
	if (fd < 0)	{
		perror("open failed");
		return EXIT_FAILURE;
	}

	// read data until EOF, when producer stops
	while((bytesRead = read(fd, &value, sizeof(float))) > 0) {
		// check if read data was actually a float
		if (bytesRead != sizeof(float)) {
			fprintf(stderr, "Failed to read whole float from pipe.");
			return EXIT_FAILURE;
		}
		printf("%f ", value);
		sum += value;
	}

	printf("\n%lf\n", sum);
	close(fd);
	return EXIT_SUCCESS;
}

int main(void) {
	mode_t mode = S_IWUSR | S_IRUSR | S_IFIFO; // RW for user, FIFO mode
	dev_t dev = 0; // used to specify major and minor dev numbers. not needed for S_IFIFO

	unlink(FIFO); // unlink the file to avoid File exists error

	// create named pipe
	if (mknod(FIFO, mode, dev) < 0) {
		perror("mknod");
	}

	runConsumer();

	return EXIT_SUCCESS;
}
