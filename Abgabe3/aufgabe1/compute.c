#include <stdio.h>
#include <stdlib.h>

#define NUM 11

int compute(int *arr, int len)
{
	int sum = 0, squares = 0;
		int i;
	for (i=0; i < len; i++) {
		sum += arr[i];
		squares += arr[i] * arr[i];
	}
		return squares/sum; 	// es kommt hier zu einer floating point exception. Division durch 0.
								// Durch die Schleife in main() wird die summe der Arrayelemente genau 0.
	}

int main(int argc, char **argv)
{
	int i;
    int arr[NUM];

	for (i=0; i < NUM; i++)
		arr[i] = NUM/2 - i;

	printf("%d \n", compute(arr, NUM));

	return 0;
}
