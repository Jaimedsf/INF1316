#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

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

int main (int argc, char * argv[]) {
	float *Saldo, aux;
	int semId, shmId;
	char c = 's';
	key_t key;
	
	if (argc == 1) {
		printf("Uso do programa: contaBancaria [path para arquivo existente]\n");
	   exit(1);
	}
	
   key = ftok(argv[1],'a');
   
   shmId = shmget (key, sizeof(float), 0644);

   if (errno == ENOENT) {
      c = 'n';
      shmId = shmget (key, sizeof(float), 0644 | IPC_CREAT);
      semId = semget (key, 1, 0666 | IPC_CREAT);
   }
	else {
	   if(shmId != -1) {
	      semId = semget (key, 1, 0666);
      }
	   else {
	      printf("ERRO\n");
	      exit(1);
	   }
	}

	setSemValue(semId);


	Saldo = shmat(shmId, 0,0);
	if (c == 'n') {
	   *Saldo = 0.0;
   }
	   
   do {
      printf("O que deseja fazer?\n");
      printf("1- Consultar Saldo\n");
      printf("2- Fazer Deposito\n");
      printf("3- Fazer Retirada\n");
      printf("0- Encerrar\n");
      
      scanf(" %c",&c);

      switch(c) {
      
         case '0':
            break;
         
         case '1':
            printf("SALDO: %f\n",*Saldo);
            break;
         
         case '2':
            printf("Qual o valor a ser depositado?\n");
            scanf(" %f",&aux);
            printf("Espere um instante ...\n");
            semaforoP(semId);
            printf("O seu novo saldo sera %f. Deseja prosseguir? (s/n)\n",*Saldo + aux);
            scanf(" %c",&c);
            if (c == 's') {
               *Saldo+=aux;
               printf("Operacao bem sucedida\n");
            }
            else
               printf("Operacao cancelada\n");
               
            semaforoV(semId);
            
            break;
      
         case '3':
            printf("Qual o valor a ser retirado?\n");
            fflush(stdin);
            scanf(" %f",&aux);
            printf("Espere um instante ...\n");
            semaforoP(semId);
            aux = *Saldo - aux;
            if (aux <= 0.0) {
               printf("Saldo insuficiente\n");
               semaforoV(semId);
               break;
            }
            printf("O seu novo saldo sera %f. Deseja prosseguir? (s/n)\n",aux);
            fflush(stdin);
            scanf(" %c",&c);
            if (c == 's'){
               *Saldo=aux;
               printf("Operacao bem sucedida\n");
            }
            else {
               printf("Operacao cancelada\n");
            }
               
            semaforoV(semId);
            break;
         
         default:
            break;
      }
      
      
      
   } while(c != '0');
   
   shmdt(Saldo);
   shmctl(shmId,IPC_RMID,0);

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