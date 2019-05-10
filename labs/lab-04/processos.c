#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#define NUM_POSITIONS 1000
#define NUM_PROCESSES 1000


int index = 1, max, min, status;
int * vetor = NULL;

void * encontraNum (int * vetor) {
	int ini = index, end = index + 9;

	if (end >= NUM_POSITIONS)
		end = NUM_POSITIONS - 1;

	for (int i = ini; i <= end; ++i) {
		if (vetor[i] > max) {
			max = vetor[i];
			printf("POSIÇÃO %d - NOVA MAX: %d\n", i, max);
		}
		if (vetor[i] < min) {
			min = vetor[i];
			printf("POSIÇÃO %d - NOVA MIN: %d\n", i, min);
		}
	}
}

int main () {
	clock_t start, stop;
	start = clock();

	vetor = (int *) malloc(NUM_POSITIONS * sizeof(int));	
	srand(time(NULL));
	pid_t pid; 

	for(int i = 0; i < NUM_POSITIONS; ++i)
		vetor[i] = rand();
	max = vetor[0]; min = vetor[0];

	for (int i = 0; i < NUM_PROCESSES; ++i)
        if(fork() == 0) 
            exit(0);
    
    for (int i = 0; i < NUM_PROCESSES; ++i) {
		encontraNum(vetor);
		index += 10;
		waitpid (-1, &status, 0);
	}

	stop = clock();

	printf("Maior número encontrado no vetor foi: %d\n", max);
	printf("Menor número encontrado no vetor foi: %d\n", min);
	printf("Tempo gasto com processos: %f\n", (double) (stop - start) / CLOCKS_PER_SEC);
}
