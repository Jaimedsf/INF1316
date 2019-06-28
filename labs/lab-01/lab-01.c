#include <stdio.h>

int main () {
	int status = 0, pid_pai = 0, pid_filho= 0, pid_neto = 0;
	
	pid_pai = getpid();
	if (fork() != 0) {
		for (int i = 0; i < 100; ++i) {
			printf("PAI - PID: %d / i = %d\n", getpid(), i);
			sleep(1);
		}
		printf("Processo pai vai finalizar\n");
		waitpid(-1, &status, 0);
	}
	else {
		pid_filho = getpid();
		if (fork() != 0) {
			printf("Filho foi criado\n");
			for (int i = 100; i < 200; ++i) {
				printf("FILHO - PID: %d / PID Pai: %d / i = %d\n", getpid(), pid_pai, i);
				sleep(2);
			}
			printf("Processo filho vai finalizar\n");
			waitpid(-1, &status, 0);
		}
		else {
			printf("Neto foi criado\n");
			for (int i = 200; i < 300; ++i) {
				printf("NETO - PID: %d / PID Pai: %d / i = %d\n", getpid(), pid_filho, i);
				sleep(3);
			}
			printf("Processo neto vai finalizar\n");
		}
	}
	
	return 0;
}
