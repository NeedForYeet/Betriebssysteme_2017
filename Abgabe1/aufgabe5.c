#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXCHARS 100 // maximum characters in a line
#define DELIM ":" // delimiter used by strtok. all fields in passwd are separated by ':'

int main(void) {
	
	char readbuf[MAXCHARS];

	FILE *fp = fopen("/etc/passwd", "r"); // flags: r read, w write, b binary file

	if (fp == NULL) {
		fprintf(stderr, "Error while opening file.");
	}

	// while not at end of file, read a new line
	// fgets returns null on failure (when end is reached). 
	// we can also use !feof(fp), but fgets should suffice as it checks for EOF itself.
	while(fgets(readbuf, sizeof(readbuf), fp)) {
		// and strip the read line to only username and id
		// strtok requires the pointer to be NULL if you continue on the same string
		printf("%s, ", strtok(readbuf, DELIM));
		strtok(NULL, DELIM); // consume the second column and do nothing with it
		printf("%s\n", strtok(NULL, DELIM));
	}

	fclose(fp);
	return 0;
}
