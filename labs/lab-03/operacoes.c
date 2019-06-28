#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void numhandler(int signo);

int main (int argc, char *argv[]) {
	int nums[2], soma, subt, prod, divs;
	sscanf(argv[1], "%d", &nums[0]);
	sscanf(argv[2], "%d", &nums[1]);
	
	signal(SIGFPE, numhandler);
	
	soma = nums[0] + nums[1];
	subt = nums[0] - nums[1];
    prod = nums[0] * nums[1]; 
	divs = nums[0] / nums[1];
          
	printf("X = %d; Y = %d\n%d\n%d\n%d\n%d\n", nums[0], nums[1], soma, subt, prod, divs);
	
	return 0;
}

void numhandler(int signo) {
	printf("divisão por zero!\n");
	exit(0);
}
