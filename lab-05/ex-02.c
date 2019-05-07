#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
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

int main (void) {
	int semId, segmento;
	char * string;
	
	segmento = shmget (IPC_PRIVATE, 16 * sizeof(char), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	string = (char *) shmat (segmento, 0, 0);

	semId = semget (8752, 1, 0666 | IPC_CREAT);
	setSemValue(semId);
		

	if (fork() != 0) { /* produtor */
		for (int i = 0; i < LOOP; ++i) {
			semaforoP(semId);
			fgets(string, 16, stdin);
			semaforoV(semId);
		}
		
		printf ("\nProcesso pai terminou\n");
		delSemValue(semId);
	}
	else { /* consumidor */
		for (int j = 0; j < LOOP; ++j) {
			semaforoP(semId);
			printf("%s", string); fflush(stdout);
			semaforoV(semId);
		}
		
		printf ("\nProcesso filho terminou\n");
	}

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
	semop(semId, &semB, 1);
	return 0;
}

int semaforoV(int semId) {
	struct sembuf semB;
	semB.sem_num = 0;
	semB.sem_op = 1;
	semB.sem_flg = SEM_UNDO;
	semop(semId, &semB, 1);
	return 0;
}
