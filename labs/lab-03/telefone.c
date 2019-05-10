#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#define EVER ; ;

void call(int signo);
void endcall(int signo);
clock_t tempo;

int main (void) {
	signal(SIGUSR1, call);
	signal(SIGUSR2, endcall);
	
	for(EVER);
	
	return 0;
}

void call(int signo) {
	printf("LIGANDO...\n");
	tempo = clock();
}

void endcall(int signo) {
	int secs;
	float preco;
	
	printf("DESLIGANDO...\n");
	
	tempo = clock() - tempo;
	secs = (int) tempo / CLOCKS_PER_SEC;
	
	if (secs <= 60)
		preco = (secs * 2) / 100.0; 
	else
		preco = 1.20 + (secs / 100.0);
		
	printf("TEMPO DA CHAMADA: %d segundos - PREÇO: %.02f\n", secs, preco);
	exit(1);
}
