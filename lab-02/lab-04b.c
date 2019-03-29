#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>

int main (void) {
	char *p;
	int id;
	
	scanf("%d",&id);
	p = shmat(id, 0, 0);
	printf("string - %s\n", p);
	
	return 0;
}
