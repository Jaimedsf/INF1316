#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#define MAX_PROCS 50
#define MAX_STR 200

typedef struct Processo {

   pid_t pid;
   int quant;
   char nome[MAX_STR];
   char args[MAX_STR];

} processo;

int main (int argc, char* argv[]) {

   int ret;
   int n_procs;
   char nome[MAX_STR];
   char args[MAX_STR];
   processo * nao_criados[MAX_PROCS];
   processo * terminados[MAX_PROCS];
   processo * prioridade1[MAX_PROCS];
   processo * prioridade2[MAX_PROCS];
   processo * prioridade3[MAX_PROCS];
   processo * esperando[MAX_PROCS];
   
   FILE * arq;
   
   arq = fopen(argv[1],"r");
   
   ret = fscanf(arq, "exec %s %s\n", nome, args);
   
   while (ret >= 0) {
   
      nao_criados[n_procs] = (processo *) malloc(sizeof(processo));
      strcpy(nao_criados[n_procs]->nome, nome);
      strcpy(nao_criados[n_procs]->args, args);
      nao_criados[n_procs]->quant = 0;
      n_procs++;
	  
      ret = fscanf(arq, "exec %s %s\n", nome, args);
   
   }

}
