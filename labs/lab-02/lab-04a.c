#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>

int main (void) {
	char *p;
	int segmento, id, pid, status;
	// aloca a memória compartilhada
	segmento = shmget (8765, 10 * sizeof(char), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	printf("%d\n",segmento);
	p = (char *) shmat(segmento, 0, 0); // comparar o retorno com -1
	
	printf("Insira uma mensagem do dia\n");
	scanf("%[^\n]", p);
	printf("\n%s",p);
	
	sleep(30);
	// libera a memória compartilhada do processo
	shmdt(p);
	// libera a memória compartilhada
	shmctl(segmento, IPC_RMID, 0);
	
	return 0;
}
