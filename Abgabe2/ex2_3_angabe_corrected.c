#include <stdio.h>
#include <stdlib.h>

static int cnt = 0;

/* Read <cnt> Integers into given array.
   Return number of entered zeros */
int* readInts(int *arr) {
	int i=0;
	int *zeros = (int*)malloc(sizeof(int));
	*zeros = 0; // zeros was uninitialized when used with ++;

	while(i < cnt) { // loop runs too often. use i < cnt
		scanf("%d", &arr[i]); // changed arr++ to &arr[i]
		if (arr[i++] == 0) (*zeros)++; //increases the pointer, not the value. use (*zeros)++
	}
	return zeros;
}

/* Create "double-sized" copy of array (duplicate each value)*/
void extend(int *arr, int *newarr) {
	int i,j;
	for (i=0,j=0; i <= cnt; i++) {
		newarr[j++] = arr[i];
		newarr[j++] = arr[i];
	}
}

int main(void) 
{
	int arr[4], *zeros, i;
	printf("How many integers (3 or 4)?\n");
	scanf("%d", &cnt);

	zeros = readInts(arr); 
	printf("You entered %d zero(s)!\n", *zeros);

	int *newarr;
	newarr = (int*) malloc(2 * cnt * sizeof(int*)); // use malloc here to avoid usage of uninitialised values"
	extend(arr, newarr);

	for (i=0; i < cnt*2; i++) printf("%d ", newarr[i]);
	printf("\n");


	// print where some of the variables are located
	printf("%p, %p\n", zeros, &zeros);
	printf("%p\n", &arr);
	printf("%p\n", &cnt);
	printf("%p\n", &newarr);

	free(newarr); // no free for newarr
	free(zeros); // no free for newarr
	return 0;
}

