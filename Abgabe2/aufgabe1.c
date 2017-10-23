#include <stdio.h>
#include <stdlib.h>

int main(void) {
	int a = 4, i=0; // initialize int variables
	int *b, *c, *pi; // declare int pointers
	printf("0: a=%d &a=%p *(&a)=%d\n", a, &a, *(&a)); // print value of a, address of a, value of a
	printf("0: b=%p &b=%p\n\n", b, &b); // print address b points to (uninitialized?), address of b
	c = b = (int*) malloc(sizeof(int) * 4); // allocate 4*sizeof(int) bytes to b and c
	*b = 10; // write 10 at address b points to
	printf("1: b=%p *b=%d &b=%p\n\n", b, *b, &b); // print address b points to, value at that address, address of b itself
	b[2] = 30; // write 30 to 3rd position of the allocated space "array"
	b++; // increment the address b points to by 4 byte (sizeof(int), because it's an int-pointer)
	b[2] = 40; // write 40 to 4th position of the allocated space
	*b = 20; // write 20 to the 2nd position, because b address points to was incremented from 1st to 2nd "array element"
	for (pi=c; i < 4; i++, pi++) { // pi now points to the same address as c, iterate over the allocated "array space", increment the address pi points to by 4 (sizeof(int)) in each iteration
		printf("2: b[%d]=%d (addr=%p)\n", pi-c, *pi, pi); // difference between pi and c is the offset(index) of the allocated memory space, *pi is the value at the address pi points to, pi is the address it points to

	}
	free(--b); // free the memory block starting at --b (original address of b), which is the block allocated at the beginning
	b = &a; // b now points to the address of a
	printf("\n3: a=%d &a=%p *(&a)=%d\n", a, &a, *(&a)); // print value of a, address of a, value of a
	printf("3: b=%p *b=%d &b=%p\n", b, *b, &b); // print address b points to, value at that address (==a), address of b
}
