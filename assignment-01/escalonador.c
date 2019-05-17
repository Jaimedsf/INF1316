#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#define MAX_PROCS 50
#define MAX_STR 200

typedef struct Processo {

   pid_t pid;           /*pid do processo*/
   int prioridade;      /*a fila para a qual o processo deve voltar apos E/S*/
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

void handler1(int s);
/*tratador de SIGUSR1, suposto w4IO de um processo filho*/

void handler2(int s);
/*tratador de SIGCHLD, recebido quando processo filho termina*/

enum interrupcao{
   negativo,
   IO,
   terminou,
};

enum interrupcao interrupt = negativo;

int main (int argc, char* argv[]) {

   int ret;
   int nProcs;
   int i;
   pid_t pid;
   char nome[MAX_STR];
   char args[MAX_STR];
   processo* proc;

   fila * terminados;
   fila * prioridade1;
   fila * prioridade2;
   fila * prioridade3;
   fila * esperando;
   
   FILE * arq;
   
   
   terminados=criaFila();
   prioridade1=criaFila();
   prioridade2=criaFila();
   prioridade3=criaFila();
   esperando=criaFila();
   
   signal(SIGUSR1,handler1);
   signal(SIGCHLD,handler2);
   
   arq = fopen(argv[1],"r");
   
   ret = fscanf(arq, "exec %s %s\n", nome, args);
   nProcs = 0;
   while (ret >= 0) {
   
      proc = (processo *) malloc(sizeof(processo));
      strcpy(proc->nome, nome);
      strcpy(proc->args, args);
      pid=fork();
      
      if(pid == 0){
         raise(SIGSTOP);
         execv(nome,&args);
         return 0;
      }
      proc->pid = pid;
      proc->quant = 0;
      insProc(proc,prioridade1);
      nProcs++;
	  
      ret = fscanf(arq, "exec %s %s\n", nome, args);
   
   }
   
   
   
   while(nProcs>0){
   
   
      ret=getMembros(prioridade1);/*tratamento da primeira fila, pode ser uma funcao(?)*/
      for(i=0;i<ret;i++){
         proc = getNext(prioridade1);
         pid=proc->pid;
         kill(pid,SIGCONT);
         sleep(1);
         kill(pid,SIGSTOP);
         
         proc->quant+=1;
         if(interrupt == terminou){
            insproc(proc,terminados);
            nProcs--;
         }
         else{
            if(interrupt == IO){
               insproc(proc,esperando);
               /*aumentaria prioridade se essa n fosse maxima*/
               /*falta fazer a quarentena de 3 segundos para voltar para a fila*/
            }
            else{
               insproc(proc,prioridade2);
            }
         }
      }
      
   
   
   
   
   }
   
   free(prioridade1);
   free(prioridade2);
   free(prioridade3);
   free(esperando);
   proc=getNext(terminados);
   
   while(proc != NULL)
      free(proc);
      
   free(terminados);
   return 0;
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

void handler1(int s){
   interrupt = IO;
}

void handler2(int s){
   interrupt = terminou;
}

