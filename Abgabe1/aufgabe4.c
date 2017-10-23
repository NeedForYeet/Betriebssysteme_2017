#include <stdio.h>
#include <stdlib.h>
#include <time.h> // seed for rand()

// calculate the length of an array by dividing its size by its first element.
// this only works for static arrays, not dynamically allocated ones.
#define ARRAY_LENGTH(x) (int)(sizeof(x)/sizeof(x[0]))

void printArray(int arraySize, unsigned char *array) {
	for(int i = 0; i < arraySize; i++) {
		printf("%d ", array[i]);
	}
	printf("\n");

	printf("%d\n", ARRAY_LENGTH(array));
}

int main(void) {
	// smallest data type to store 0-255 is unsigned char
	unsigned char myArray[10];

	// set the system time as seed for rand()
	srand(time(NULL));

	for(int i = 0; i < ARRAY_LENGTH(myArray); i++) {
		myArray[i] = rand() % 256; // %256 starts at 0. Therefore 0-255 = 256
	}

	printf("Array size: %ld bytes. \n", sizeof(myArray));
	printArray(ARRAY_LENGTH(myArray), myArray);

	return 0;
}
