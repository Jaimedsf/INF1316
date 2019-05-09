/* Exemplo de uso de semáforo*/

#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

union semun
{
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

int main (int argc, char * argv[])
{
	int i;
	int incremento = 1;
	int *inteiroCompartilhado = 0;
	int semId;
	int shmId;
	if (argc > 1)
	{

		semId = semget (8752, 1, 0666 | IPC_CREAT);
		shmId = shmget (1324, sizeof(int), 0644 | IPC_CREAT);
		setSemValue(semId);
		incremento = 5;

	}
	else
	{

		while ((semId = semget (8752, 1, 0666)) < 0)
		{

		}
		while((shmId = shmget(1324, sizeof(int), 0644)) < 0)
		{


		}

	}
	inteiroCompartilhado = shmat(shmId, 0,0);
	for (i=0; i<100000; i++)
	{

		semaforoP(semId);
		*inteiroCompartilhado+=incremento;
		printf("SOMEI %d, TOTAL: %d\n",incremento,*inteiroCompartilhado);
		semaforoV(semId);

	}
	printf ("\nProcesso %d terminou\n", getpid());
	shmdt(inteiroCompartilhado);
	if (argc > 1)
	{

		sleep(2);
		delSemValue(semId);
		shmctl(shmId, IPC_RMID, 0);

	}

	return 0;
}

int setSemValue(int semId)
{
	union semun semUnion;
	semUnion.val = 1;
	return semctl(semId, 0, SETVAL, semUnion);
}
void delSemValue(int semId)
{
	union semun semUnion;
	semctl(semId, 0, IPC_RMID, semUnion);
}
int semaforoP(int semId)
{
	struct sembuf semB;
	semB.sem_num = 0;
	semB.sem_op = -1;
	semB.sem_flg = SEM_UNDO;
	semop(semId, &semB, 1);
	return 0;
}
int semaforoV(int semId)
{
	struct sembuf semB;
	semB.sem_num = 0;
	semB.sem_op = 1;
	semB.sem_flg = SEM_UNDO;
	semop(semId, &semB, 1);
	return 0;
}
