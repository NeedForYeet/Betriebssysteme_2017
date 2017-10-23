#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

static void *countdown(void *arg);
int main(int argc, char** argv) {

	int numThreads, *inputValues;
	pthread_t* threads;
	
	if (argc < 2 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
		printf("Usage: %s num1 num2 num3 ...", argv[0]);
		return EXIT_FAILURE;
	}

	// number of threads determined by amount of input values
	numThreads = argc - 1;

	// parse input arguments
	inputValues = (int*) malloc(4*sizeof(int));
	if (inputValues == NULL) {
		perror("malloc inputValues failed.\n");
		return EXIT_FAILURE;
	}
	for (int i = 0; i < numThreads; i++) {
		inputValues[i] = atoi(argv[i+1]);
		//printf("%d\n", inputValues[i]);
	}

	// allocate memory for the thread ID buffer
	threads = malloc(sizeof(pthread_t) * numThreads);
	if (threads == NULL) {
		perror("malloc threads failed.\n");
		free(inputValues);
		return EXIT_FAILURE;
	}
	for (int i = 0; i < numThreads; i++) {
		if(pthread_create(&threads[i], NULL, countdown, inputValues+i) != 0) {
			perror("pthread_create failed.");
			free(inputValues);
			free(threads);
			return EXIT_FAILURE;
		}
	}

	// join threads
	for (int i = 0; i < numThreads; i++) {
		if(pthread_join(threads[i], 0) != 0) {
			perror("pthread_join failed.\n");
			free(inputValues);
			free(threads);
			return EXIT_FAILURE;
		}
	}

	free(inputValues);
	free(threads);
	return EXIT_SUCCESS;
}

static void *countdown(void *arg) {
	int n = *((int*)arg);
	pthread_t threadID = pthread_self();
	printf("Thread %lu: will count from %d to 0\n", threadID, n);

	while (n >= 0) {
		printf("Thread %lu: %d\n", threadID, n);
		n--;
		sleep(1);
	}
	printf("Thread %lu: Finished.\n", threadID);
	return NULL;
}
