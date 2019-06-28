#include <stdio.h>
#include <unistd.h>

int main (int argc, char *argv[]) {
	int status;
	char prog1[] = "alomundo", prog2[] = "echo";

	if (fork() != 0) {
		waitpid(-1, &status, 0);
		execv(prog1, argv);
		return 0;
	}
	else {
		execv(prog2, argv);
		return 0;
	}
}
