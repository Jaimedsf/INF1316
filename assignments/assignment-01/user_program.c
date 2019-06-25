//USER_PROGRAM.C

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

int main (int argc, char * argv[]) {

   int n_rajadas = 0;
   int i, j;
   pid_t pid_pai;
   char * string;
   int rajadas[50];
   
   if(argc != 2) {
      printf("Uso errado do programa!");
      fflush(stdout);
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
   raise(SIGSTOP);
   for(i = 0; i < n_rajadas; i++) {
      for(j = 0; j < rajadas[i]; j++) { 
         printf("pid: %d| i: %d| j:%d\n", getpid(),i,j);
         sleep(1);
      }
      if(i != n_rajadas - 1){
         kill(pid_pai, SIGUSR1);
         raise(SIGSTOP);
      }
   }
   kill(pid_pai, SIGUSR2);
   return 0;

}
