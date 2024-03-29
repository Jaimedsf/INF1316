//ESCALONADOR.C:

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <time.h>

#define MAX_PROCS 50
#define MAX_STR 200

typedef struct Processo {

   pid_t pid;           /*pid do processo*/
   int prioridade;      /*a fila para a qual o processo deve voltar apos E/S*/
   time_t timestamp;	  /*tempo que o processo perdura em dado momento*/

} processo;

typedef struct Fila {

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

processo* getNext(fila* f, int remove);
/*retorna uma referencia para o proximo da fila, o remove se remove for diferente de 0*/

void handler1(int s);
/*tratador de SIGUSR1, suposto w4IO de um processo filho*/

void handler2(int s);
/*tratador de SIGCHLD, recebido quando processo filho termina*/

void handler3(int s);
/*tradador de SIGALRM, representante de quando o quantum do processo esgota*/

void manageFila(int quantum);
/*tratador das filas de processo*/

void checkIO(void);
/*verifica se algum processo em espera pode voltar a sua fila*/

void exibeFila(fila* f);
/*exibe uma unica fila de processos*/

void exibeFilas(void);
/*exibe todas as filas de processos*/


fila * terminados;
fila * prioridade1;
fila * prioridade2;
fila * prioridade3;
fila * esperando;
processo *proc;
int nProcs;
int quantum;
fila* inferior;
int nSuperior;
int nInferior;


int main (int argc, char* argv[]) {
  
   int ret;

   pid_t pid;
   char nome[MAX_STR];
   char args[MAX_STR];
   char pb[MAX_STR+2] = "./";
   char *parms[3];
   processo* proc;
   
   FILE * arq;
   
   terminados=criaFila();
   prioridade1=criaFila();
   prioridade2=criaFila();
   prioridade3=criaFila();
   esperando=criaFila();
   
   signal(SIGUSR1,handler1);
   signal(SIGUSR2,handler2);
   signal(SIGALRM,handler3);
   parms[2] = (char*)0;
   if(argc != 2){
     
     printf("Argumentos errados!\n");
     exit(1);
   }
  
   arq = fopen(argv[1],"r");
   if (arq == NULL){
     
     printf("Erro ao abrir arquivo : %s\n",argv[1]);
     exit(1);
   }
   
   ret = fscanf(arq, "exec %s %s\n", nome, args);
   nProcs = 0;
   while (ret >= 0) {
   
      proc = (processo *) malloc(sizeof(processo));
      parms[0]=nome;
      parms[1]=args;
      strcat(pb,nome);
      pid=fork();
      
      if(pid == 0){
         execv(pb,parms);
         perror("execv");
         return 0;
      }
      proc->prioridade = 1;
      proc->pid = pid;
      insProc(proc,prioridade1);
      nProcs++;
	  
      ret = fscanf(arq, "exec %s %s\n", nome, args);
      pb[2]='\0';
   
   }
   
   sleep(3);
   int i=0;
   while(nProcs>0) {
      checkIO();
      
      i=getMembros(prioridade1);
      if(i>0){
         manageFila(1);
      }
      else{
         i=getMembros(prioridade2);
         if(i>0){
            manageFila(2);
         }
         else{
            i=getMembros(prioridade3);
            if(i>0){
               manageFila(3);
            }
            else{
               sleep(1);
            }
         }
      }
 
   }
   
   free(prioridade1);
   free(prioridade2);
   free(prioridade3);
   free(esperando);
   proc=getNext(terminados, 1);
   
   while(proc != NULL){
     free(proc);
     proc=getNext(terminados, 1);
   }
      
      
   free(terminados);
   return 0;
}
              
void manageFila(int prioridade) {
   int ret;
   fila* corrente;
   pid_t pid;
  
   switch (prioridade) {
     case 1:
       quantum = 2;
       corrente = prioridade1;
       inferior = prioridade2;
       nSuperior = 1;
       nInferior = 2;
       break;
       
     case 2:
       quantum = 4;
       corrente = prioridade2;
       inferior = prioridade3;
       nSuperior = 1;
       nInferior = 3;
       break;
       
     case 3:
       quantum = 8;
       corrente = prioridade3;
       inferior = prioridade3;
       nSuperior = 2;
       nInferior = 3;
       break;
   }
  
   ret=getMembros(corrente);
    for(int i=0;i<ret;i++){
       exibeFilas();
       proc = getNext(corrente, 1);
       pid=proc->pid;

       alarm(quantum);
       printf("\n> Executando %d na fila %d\n",pid,prioridade);
       kill(pid,SIGCONT);
       pause();

      checkIO();
  }  

  checkIO();

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

processo* getNext(fila* f, int remove){
   
   processo* ret;
   if(remove == 0){
      return f->vet[f->proximo];
   }
   if(f->membros == 0)
      return NULL;

   ret = f->vet[f->proximo];
   f->vet[f->proximo] = NULL;
   f->proximo=((f->proximo)+1)%MAX_PROCS;
   f->membros = f->membros - 1;
   return ret;
}

void handler1(int s){

   alarm(0);
   printf("\n> %d pediu I/O\n",proc->pid);
   
   proc->prioridade = nSuperior;
   insProc(proc,esperando);
   time(&(proc->timestamp));
}

void handler2(int s){
   kill(proc->pid,SIGKILL);
   alarm(0);
   printf("\n> %d terminou\n",proc->pid);
  
   insProc(proc,terminados);
   nProcs--;
}

void handler3(int s){
  kill(proc->pid,SIGSTOP);
  printf("\n> %d esgotou seu quantum\n",proc->pid);
  proc->prioridade = nInferior;
  insProc(proc,inferior);
  
}

void checkIO(void){
   time_t segundos;
   if(getMembros(esperando)>0){
        proc = getNext(esperando, 0);
        time(&segundos);

        while( (difftime(segundos,proc->timestamp))>= 3){
           switch(proc->prioridade){
             case 1:
               insProc(proc,prioridade1);
               break;
             case 2:
               insProc(proc,prioridade2);
               break;
             case 3:
               insProc(proc,prioridade3);
               break;
           }
           getNext(esperando, 1);
           proc = getNext(esperando, 0);
           if (proc == NULL)
               break;
      }
   }
}

void exibeFilas(void){

   printf(">FILA 1: ");
   exibeFila(prioridade1);
   printf("fim\n");
   
   printf(">FILA 2: ");
   exibeFila(prioridade2);
   printf("fim\n");

   printf(">FILA 3: ");
   exibeFila(prioridade3);
   printf("fim\n");
   
   printf(">ESPERANDO IO: ");
   exibeFila(esperando);
   printf("fim\n");

}

void exibeFila(fila* f){
   
   int membros=f->membros;   
   
   for(int i=0;i<membros;i++){
      printf("%d -> ",f->vet[((f->proximo)+i)%MAX_PROCS]->pid);
   }   
   
}


























