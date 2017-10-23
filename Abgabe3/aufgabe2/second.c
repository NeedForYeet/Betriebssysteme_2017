#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>

#define NUM 100000

int main(int argc, char **argv)
{
	int vals[NUM],i;

	int fdIn = open("/dev/urandom",O_RDONLY);
	for (i=0; i<NUM; i++) {
		read(fdIn, vals+i, sizeof(int));
	}	
	close(fdIn);

	int fdOut = open("secondout", O_CREAT | O_WRONLY);
	for (i=0; i<NUM; i++) {
		vals[i] = fabsf(vals[i]);
		write(fdOut, vals+i, sizeof(int));
	}
	close(fdOut);

	return EXIT_SUCCESS;
}
