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
   int quant;           /*numero de quantuns consumidos pelo processo*/
   char nome[MAX_STR];  /*nome do programa*/
   char args[MAX_STR];  /*argumentos passados para o programa*/

} processo;

typedef struct Fila{

   int membros;                /*numero de processos na fila*/
   int proximo;                /*indice do proximo processo nessa fila*/
   processo* vet[MAX_PROCS]; /*referencias para processos*/

} fila;

fila* criaFila();
/*retorna referencia para uma fila vazia*/

int getMembros(fila* f);
/*retorna o numero de processos na fila f*/

int insProc(processo* p, fila* f);
/*insere o processo p no final da fila f, retornando o numero de processos
na fila apos a insercao*/

processo* getNext(fila* f);
/*remove o proximo processo da fila f, retornando uma referencia para o mesmo*/

int main (int argc, char* argv[]) {

   int ret;
   int nProcs;
   char nome[MAX_STR];
   char args[MAX_STR];
   processo* proc;
   fila * naoCriados;
   fila * terminados;
   fila * prioridade1;
   fila * prioridade2;
   fila * prioridade3;
   fila * esperando;
   
   FILE * arq;
   
   
   naoCriados=criaFila();
   terminados=criaFila();
   prioridade1=criaFila();
   prioridade2=criaFila();
   prioridade3=criaFila();
   esperando=criaFila();
   
   
   arq = fopen(argv[1],"r");
   
   ret = fscanf(arq, "exec %s %s\n", nome, args);
   nProcs = 0;
   while (ret >= 0) {
   
      proc = (processo *) malloc(sizeof(processo));
      strcpy(proc->nome, nome);
      strcpy(proc->args, args);
      insProc(proc,naoCriados);
      nProcs++;
	  
      ret = fscanf(arq, "exec %s %s\n", nome, args);
   
   }

}


fila* criaFila(){
   fila* f;
   f=(fila*)malloc(sizeof(fila));
   f->membros=0;
   return f;
}

int getMembros(fila* f){
   return f->membros;
}

int insProc(processo* p, fila* f){

   if(f->membros == 0){
      
      f->membros = 1;
      f->proximo = 0;
      f->vet[0] = p;
   
   }
   else{
      f->vet[(f->proximo + f->membros)%MAX_PROCS]=p;
      f->membros = f->membros + 1;
   }
   return f->membros;
}

processo* getNext(fila* f){
   
   processo* ret;
   
   if(f->membros == 0)
      return NULL;
   if(f->membros == 1){
      f->membros = 0;
      return f->vet[f->proximo];
   }
   ret = f->vet[f->proximo];
   f->proximo=f->proximo-1;
   if(f->proximo < 0)
      f->proximo = MAX_PROCS-1;
   return ret;
}

