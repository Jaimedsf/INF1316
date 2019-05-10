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

int main (int argc, char** argv) {
	int semId, segmento, * pos;
	char * string, char_aux;
	
	segmento = shmget (IPC_PRIVATE, 16 * sizeof(char), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	string = (char *) shmat (segmento, 0, 0);
	
	segmento = shmget (IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	pos = (int *) shmat (segmento, 0, 0);

	*pos = -1;
	semId = semget (8752, 1, 0666 | IPC_CREAT);
	setSemValue(semId);
		

	if (fork() != 0) { /* produtor */
		int j=-1;
		for (int i = 0; i < LOOP; ++i) {
			semaforoP(semId);
			
			if (*pos == 15) {
				semaforoV(semId);
				continue;
			}
			
			j++;
			char_aux = argv[1][j];
			printf ("\nchar = %c\n",char_aux);
			
			if (char_aux == '\0') {
				semaforoV(semId);				
				break;	
			}
			
			*pos++;
			string[*pos] = char_aux;
			semaforoV(semId);
		}
		
		printf ("\nProcesso pai terminou\n");
		delSemValue(semId);
	}
	else { /* consumidor */
		printf ("\nProcesso filho comecou\n");
		printf("%s", string);
		for (int j = 0; j < LOOP; ++j) {
			printf ("\nJ = %d\n",j);
			semaforoP(semId);
			printf ("\nProcesso filho POS = %d\n",*pos);
			if (*pos == -1) {
				semaforoV(semId);
				continue;
			}

			printf("\n%c CONSUMIDO\n", string[*pos]);// fflush(stdout);
			*pos--;			
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
