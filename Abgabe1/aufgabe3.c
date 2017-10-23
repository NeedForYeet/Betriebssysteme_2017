#include <stdio.h>
#include <string.h>

// flushes stdin buffer. if fgets is used for a string after a scanf
// it will just read the newline entered after the scanf and nothing else.
// stdin also needs to be flushed if the entered string is longer than the maximum
// allowed characters.
// this should also be possible with __fpurge(stream)
void flushStdin(void) {
	char c;
	while((c = getchar()) != '\n' && c != EOF);
}

// remove terminating newline in string
void removeTerminatingNewline(char* str) {
	if(str[strlen(str) - 1] == '\n') {
		str[strlen(str) - 1] = 0; 
	}
}

int main (void) {
	
	char readChar;
	float readFloat;
	double readDouble;
	short readShort;
	long long readLongLong;
	char readString[21];

	printf("Enter char: ");
	scanf("%c", &readChar);
	printf("Enter float: ");
	scanf("%f", &readFloat);
	printf("Enter double: ");
	scanf("%lf", &readDouble);
	printf("Enter short: ");
	scanf("%hi", &readShort);
	printf("Enter long long: ");
	scanf("%lld", &readLongLong);


	// flush stdin, so it doesn't just read the previous newline
	flushStdin();
	printf("Enter string: ");
	fgets(readString, sizeof(readString), stdin);

	removeTerminatingNewline(readString);

	printf("Read char: %c, has size %lu byte.\n", readChar, sizeof(readChar));
	printf("Read float: %f, has size %lu byte.\n", readFloat, sizeof(readFloat));
	printf("Read double: %lf, has size %lu byte.\n", readDouble, sizeof(readDouble));
	printf("Read short: %hi, has size %lu byte.\n", readShort, sizeof(readShort));
	printf("Read long long: %lld, has size %lu byte.\n", readLongLong, sizeof(readLongLong));
	printf("Read string: %s, has size %lu byte and is %ld characters long.\n", readString, sizeof(readString), strlen(readString));

	return 0;
}
