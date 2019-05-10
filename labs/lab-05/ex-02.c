#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <wait.h>
#define LOOP 64

union semun {
	int val;
	struct semid_ds *buf;
	ushort *array;
};

// inicializa o valor do semáforo
int setSemValue(int semId);
// remove o semáforo
void delSemValue(int semId);
// operação P
int semaforoP(int semId);
//operação V
int semaforoV(int semId);

int main(void) {
	int segmento, status, semId;
	char * string;

	semId = semget (IPC_PRIVATE, 1, 0666 | IPC_CREAT);
	setSemValue(semId);

	segmento = shmget (IPC_PRIVATE, 16 * sizeof(char), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    string = shmat (segmento, 0, 0);

	if (fork() != 0) {	
		printf("\n=== Processo produtor iniciado ===\n");

		for (int i = 0; i < LOOP; i++) {
			semaforoP(semId);
			printf("\nCONSOME: ");
			scanf("%s", string);
			semaforoV(semId);
		}

		printf("\n=== Processo produtor encerrado ===\n");
	} 
	else {	
		printf("\n=== Processo consumidor iniciado ===\n");

		for (int i = 0; i < LOOP; i++) {
			semaforoP(semId);
			printf("\nPRODUZ: %s\n", string);
			semaforoV(semId);
		}

		printf("\n=== Processo consumidor encerrado ===\n");
	}

	shmdt (string);
	shmctl (segmento, IPC_RMID, 0);
	
	delSemValue(semId);
	return 0;
}

int setSemValue(int semId) {
	union semun semUnion;
	semUnion.val = 1;
	return semctl(semId, 0, SETVAL, semUnion);
}

void delSemValue(int semId) {
	union semun semUnion;
	semctl(semId, 0, IPC_RMID, semUnion);
}

int semaforoP(int semId) {
	struct sembuf semB;
	semB.sem_num = 0;
	semB.sem_op = -1;
	semB.sem_flg = SEM_UNDO;
	while(semop(semId, &semB, 1) != 0);
	return 0;
}

int semaforoV(int semId) {
	struct sembuf semB;
	semB.sem_num = 0;
	semB.sem_op = 1;
	semB.sem_flg = SEM_UNDO;
	while(semop(semId, &semB, 1) != 0);
	return 0;
}