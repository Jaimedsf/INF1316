
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

int main (int argc, char * argv[]) {

   int n_rajadas = 0;
   int i, j;
   pid_t pid_pai;
   char * string;
   int rajadas[50];
   
   if(argc != 2) {
      printf("Uso errado do programa!");
      exit(1);
   }
   
   pid_pai = getppid();
   
   string = &(argv[1][1]);
   i = 1;

   while(string[0] != '\0') {
      while(argv[1][i] != ',' && argv[1][i] != ')')
         i++;
         
      argv[1][i]='\0';
      
      rajadas[n_rajadas] = atoi(string);  
      n_rajadas++;
      string = &(argv[1][i+1]);
   }
   
   for(i = 0; i < n_rajadas; i++) {
      for(j = 0; j < rajadas[i]; j++) {
         printf("%d\n", getpid());
         sleep(1);
      }
      kill(pid_pai, SIGUSR1);
   }

   return 0;

}
