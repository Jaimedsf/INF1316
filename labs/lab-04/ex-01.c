#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#define NUM_THREADS 2

int global = 0;

void * contagem (void * threadid) {
	if (threadid == 0)
		for (int i = 1; i <= 20; ++i) {
			printf("CC: %d\n", i);
			global++;
			printf("%d - GLOBAL: %d\n", threadid, global);
			sleep(1);
		}
	else {
		for (int i = 30; i >= 1; --i) {
			printf("CD: %d\n", i);
			global++;
			printf("%d - GLOBAL: %d\n", threadid, global);
			sleep(2);
		}
	}
	
	printf("Concluindo thread %d\n", threadid + 1);
	pthread_exit(NULL);
}

int main() {
	pthread_t threads[NUM_THREADS];
	int t;
	for(t = 0; t < NUM_THREADS; t++) {
		printf("Criando thread %d\n", t + 1);
		pthread_create(&threads[t], NULL, contagem, (void *) t);
	}
	for(t = 0; t < NUM_THREADS; t++) {
		pthread_join(threads[t], NULL);
		printf("---------- %d ----------\n", t);
	}
}
