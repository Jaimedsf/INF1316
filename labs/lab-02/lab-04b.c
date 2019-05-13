#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>

int main (int argc, char * argv[]) {
	char * p;
	int id;

	if (argc != 2) {
		printf("Falha no engano.\n");
		exit(1);
	}

	id = *argv[1];
	p = shmat(id, 0, 0);
	printf("string - %s\n", p);
	
	return 0;
}
