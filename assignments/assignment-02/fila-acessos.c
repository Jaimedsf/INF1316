
#include "fila-acessos.h"
#include <stdio.h>
#include <stdlib.h>

struct fila{

   Acesso* vet;
   int membros;
   int maxMembros;
   int nxt;

}Fila;

pFila criaFila(int tam){

   pFila f = (pFila)malloc(sizeof(Fila));
   
   if(f == NULL){
      printf("Erro de alocacao\n");
      exit(1);
   }
   
   f->maxMembros = tam;
   f->membros = 0;
   f->nxt = -1;
   
   f->vet = (Acesso*)malloc(tam*sizeof(Acesso));
   
   if(f->vet == NULL){
      printf("Erro de alocacao\n");
      exit(1);
   }
   
   return f;   

}

void destroiFila(pFila f){

   if(f != NULL){
      free(f->vet);
      free(f);
   }
}

void queue(pFila f, int address, char rw){

   if(f->membros == 0){
      
      f->membros = 1;
      f->nxt = 0;
      f->vet[0].address = address;
      f->vet[0].rw = rw;
   
   }
   else{
      Acesso* a = &((f->vet)[(f->nxt + f->membros)%f->maxMembros]);
      a->address = address;
      a->rw = rw;
      f->membros = f->membros + 1;
   }

}

Acesso dequeue(pFila f){
   
   Acesso ret;
   if(f->membros == 0){
      printf("Fila Vazia!\n");
      exit(1);
   }
      
   ret=(f->vet)[f->nxt];
   f->membros -= 1;
   f->nxt += 1;
   f->nxt = ((f->nxt) + 1)%f->maxMembros;
   return ret;
}

void getAddrVet(pFila f, int vet[]){

   int tam = f->membros;
   int nxt = f->nxt;
   Acesso* q = f->vet;
   for(int i=0; i<tam; i++)
      vet[i] = q[(nxt + i)%tam].address;
   

}

int getMembros(pFila f){

   return f->membros;

}

