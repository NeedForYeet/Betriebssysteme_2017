#include <stdio.h>
#include <stdlib.h>

#define ARRAYSIZE 20

void sumReturn(const float *a, int size, float (*func) (float)) {
	float sum = 0;

	for (int i = 0; i < size; i++) {
		// call the specified function with each array element and add its return value to sum
		sum += func(a[i]);
	}

	printf("%f\n", sum);
}

float squareNumbers(float number) {
	return number * number;
}

float addNumbers(float number) {
	return number + number;
}

int main(void) {

	float fArray[ARRAYSIZE];

	for (int i = 0; i < ARRAYSIZE; i++) {
		fArray[i] = i;
	}
	
	// pointer to a function returning and taking a float
	float (*funcPtr) (float);
	
	// point to squareNumbers
	funcPtr = squareNumbers;
	sumReturn(fArray, ARRAYSIZE, funcPtr);

	// point to addNumbers
	funcPtr = addNumbers;
	sumReturn(fArray, ARRAYSIZE, funcPtr);
	
	return EXIT_SUCCESS;
}
