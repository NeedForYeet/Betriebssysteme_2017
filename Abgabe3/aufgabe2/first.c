#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUM 100000

int main(int argc, char **argv)
{
	int vals[NUM],i;

	FILE *fpIn = fopen("/dev/urandom","rb");
	for (i=0; i<NUM; i++) {
		fread(vals+i, sizeof(int), 1, fpIn);
	}	
	fclose(fpIn);

	FILE *fpOut = fopen("firstout", "w");
	for (i=0; i<NUM; i++) {
		vals[i] = fabsf(vals[i]);
		fwrite(vals+i, sizeof(int), 1, fpOut);
	}
	fclose(fpOut);

	return EXIT_SUCCESS;
}
