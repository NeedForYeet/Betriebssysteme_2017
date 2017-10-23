#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <stdio_ext.h> // for __fpurge()

#define MAXLENGTH 81 // maximum string length including null terminator

void flushStdin(void) {
	char c;
	while ((c = getchar()) != '\n' && c != EOF);
}

int main(int argc, char** argv) {
	argc = 0; // shut up unused code warning
	
	int stringAmount;
	char buffer[MAXLENGTH];
	char **stringArray;

	if (argv[1] == NULL) {
		fprintf(stderr, "No value entered.\n");
		return 1;
	}
	sscanf(argv[1], "%d", &stringAmount);

	// allocate memory for the amount of strings
	// char pointer = 8 Byte
	stringArray = malloc(8*stringAmount);
	
	for (int i = 0; i < stringAmount; i++) {
		printf("Please enter a String: ");
		fgets(buffer, sizeof(buffer), stdin);

		// todo: fixme?
		//__fpurge(stdin);
		// call only if input exceeded maximum length and last char is not '\n'
		// to prevent having to press enter again
		if (strlen(buffer) >= MAXLENGTH - 1 && buffer[strlen(buffer) - 1] != '\n') {
			flushStdin();
		}

		// also cut the trailing newline if present
		if (buffer[strlen(buffer) - 1] == '\n') {
			buffer[strlen(buffer) - 1] = 0;
		}

		// allocate memory for a single string in the array. +1 for null-term
		stringArray[i] = malloc(strlen(buffer) + 1);

		// now copy the read string into the allocated memory
		strcpy(stringArray[i], buffer);
	}

	// string output & free
	for (int i = 0; i < stringAmount; i++) {
		printf("%s\n", stringArray[i]);
		free(stringArray[i]);
	}

	// also free the pointer array itself
	free (stringArray);
}
