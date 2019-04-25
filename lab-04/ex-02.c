#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#define NUM_THREADS 50



int vetor[40] = { 23, 37, 85, 34, 70, 12, 56, 234, 578, 23, 89, 34, 45,
	       		123, 12398, 23, 47, 456, 3, 234, 7856, 6789, 875, 1,
	       		894, 26, 96, 78234, 2783, 73, -1, 34, 234, 45, 7823,
	       		12, 456, 356, 78, 1 };
int index = 0, max = 0;

void * encontraMaior (int * vetor) {
	int ini = index;
	index += 11;
	
	for (int i = ini; i <= ini + 10; ++i) {
		if (vetor[i] > max)
			max = vetor[i];
	}

	pthread_exit(NULL);
}

int main() {
	pthread_t threads[NUM_THREADS];
	int t;
	for(t = 0; t < NUM_THREADS; t++) {
		printf("Criando thread %d\n", t + 1);
		pthread_create(&threads[t], NULL, encontraMaior, vetor);
	}
	for(t = 0; t < NUM_THREADS; t++) {
		pthread_join(threads[t], NULL);
		printf("---------- %d ----------\n", t);
	}

	printf("Maior número encontrado no vetor foi: %d", max);
}
