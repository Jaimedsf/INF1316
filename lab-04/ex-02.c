#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#define NUM_POSITIONS 16000
#define NUM_THREADS 1600

int index = 1, max, min;
int * vetor = NULL;

void * encontraNum (int * vetor) {
	int ini = index;
	index += 9;
	if (index >= NUM_POSITIONS) {
		index = NUM_POSITIONS - 1;
	}

	for (int i = ini; i <= index; ++i) {
		if (vetor[i] > max) {
			max = vetor[i];
			printf("POSIÇÃO %d - NOVA MAX: %d\n", i, max);
		}
		if (vetor[i] < min) {
			min = vetor[i];
			printf("POSIÇÃO %d - NOVA MIN: %d\n", i, min);
		}
	}

	pthread_exit(NULL);
}

int main() {
	vetor = (int *) malloc(NUM_POSITIONS * sizeof(int));
	pthread_t threads[NUM_THREADS];
	srand(time(NULL)); 
	
	for(int i = 0; i < NUM_POSITIONS; i++) {
		vetor[i] = rand();
	}
	
	max = vetor[0];
	min = vetor[0];
	
	for(int i = 0; i < NUM_THREADS; i++) {
		pthread_create(&threads[i], NULL, encontraNum, vetor);
	}
	for(int i = 0; i < NUM_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	printf("Maior número encontrado no vetor foi: %d\n", max);
	printf("Menor número encontrado no vetor foi: %d\n", min);
}
