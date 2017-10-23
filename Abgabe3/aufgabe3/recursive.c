#include <stdio.h>

static int cnt = 0;
void foo() {
	char a[1000];
	if (++cnt == 5000) {
		printf("%d %c", cnt, a[0]);
		//printf("%ld", sizeof(a)); 
	}
	else
		foo();

}

int main(void) {

	foo();

	return 0;
}
