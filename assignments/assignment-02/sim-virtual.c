#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "algoritmo.h"
#include "fila-acessos.h"

int getNextAcesso(FILE* arq, pFila f, int* virtualIndex, int unusedBits, char* rw);

void fillQueue(FILE* arq, pFila f, int unusedBits, int nAcessos);

int main(int argc, char* argv[]) {

   char fileName[20];
   char algType[5];
   
   int pageSize;
   int memSize;
   int maxFrames;
   int loadedFrames = 0;
   int numPV;
   int s;
   
   FILE* arq;
   Frame* listFrames;
   int* tabPags;
   
   pFila fil;
   int* proximosAcessos;
   int tamFila;
   
   int(*PageFault)(Frame*, int, int*);
   
   int virtualIndex;
   char rw;
   
   int time = 0;
   int pageFaults = 0;
   int pageWrites = 0;

   if(argc < 5){
      printf("USO DO PROGRAMA: 'sim-virtual [algoritmo] arquivo.log [tamPag] [tamMem]'");
      exit(1);
   }

   strcpy(algType, argv[1]);
   strcpy(fileName, argv[2]);
   pageSize = atoi(argv[3]);
   memSize = atoi(argv[4]); 

   s = log2(pageSize) + 10;
   maxFrames = memSize/pageSize;
   numPV = pow(2,(32-s));
   
   if (strcmp("LRU", algType) == 0)
   	  PageFault = &LRU;
   else if (strcmp("NRU", algType) == 0)
   	  PageFault = &NRU;
   else if (strcmp("NOVO", algType) == 0)
   	  PageFault = &NOVO;
   else {
   	  printf("SELECIONE UM DOS ALGORITMOS: LRU | NRU | NOVO\n");
   	  exit(1);
   }
   
   printf("Executando o simulador...\n");
   printf("Arquivo de entrada: %s\n", fileName);
   printf("Tamanho de memória física: %d\n", memSize);
   printf("Tamanho das páginas %d\n", pageSize);
   printf("Algoritmo de substituicao: %s\n", algType);
   
   tamFila = 50;
   fil=criaFila(tamFila);
   proximosAcessos = (int*) malloc(tamFila * sizeof(int));
   
   arq = fopen(fileName, "r");
   	  
   listFrames = malloc(maxFrames * sizeof(Frame));
   tabPags = malloc(numPV * sizeof(int));
   
   for(int i=0;i<numPV;i++){
      tabPags[i]=-1;
   }
   
   
   
   while (getNextAcesso(arq, fil, &virtualIndex, s, &rw) > -1) {
      int realIndex = tabPags[virtualIndex];
      if(time % 10 == 0){
         for(int i = 0; i < loadedFrames;i++) {
            listFrames[i].flagR = '0';
         }
      }
      
      
      if((loadedFrames < maxFrames) && realIndex == -1){
         realIndex = loadedFrames;
         loadedFrames++;
         listFrames[realIndex].flagW = '0';
         pageFaults++;
         tabPags[virtualIndex]=realIndex;
      }
      else {
         if(realIndex == -1){
            int newIndex;
            int membros;
            
            fillQueue(arq, fil, s, tamFila);
            getAddrVet(fil, proximosAcessos);
            membros = getMembros(fil);
          
            for(int i=0;i<membros;i++){
            
               int IR = tabPags[proximosAcessos[i]];
               if(IR<0)
                  proximosAcessos[i]=-1;
               else
                  proximosAcessos[i]=IR;
            }
               
            for(int i=membros;i<tamFila;i++)
               proximosAcessos[i]=-1;
               
            
            newIndex = (*PageFault)(listFrames, maxFrames, proximosAcessos);
            
            pageFaults++;
            if(listFrames[newIndex].flagW == '1')
               pageWrites++;
            
            tabPags[virtualIndex]=newIndex;
            tabPags[listFrames[newIndex].virtualIndex]=-1;
            realIndex = newIndex;
            
         }
      }
      
      if((rw == 'W')){
         listFrames[realIndex].flagW = '1';
      }
      
      listFrames[realIndex].flagR='1';
      listFrames[realIndex].time=time;
      listFrames[realIndex].virtualIndex = virtualIndex;
      
      time++;
   }
   
   printf("Numero de Faltas de Paginas: %d\n", pageFaults);
   printf("Numero de Paginas Escritas: %d\n", pageWrites);
      
   fclose(arq);
   destroiFila(fil);
   free(proximosAcessos);
   
   return 0;
}


int getNextAcesso(FILE* arq, pFila f, int* virtualIndex, int unusedBits, char* rw){

   if(getMembros(f) >0){
   
      Acesso a = dequeue(f);
      *virtualIndex = a.address;
      *rw = a.rw;
      return 1;
   }
   unsigned int addr;
   int ret = fscanf(arq, "%x %c", &addr, rw);
   *virtualIndex = addr >> unusedBits;
   return ret;
}

void fillQueue(FILE* arq, pFila f, int unusedBits, int nAcessos){

   int unread = nAcessos - getMembros(f);
   
   unsigned int addr;
   char rw;
   for(int i=0; i<unread ; i++){
   
      if (fscanf(arq, "%x %c", &addr, &rw) <= -1 ){
         return;
      }
      
      addr = addr >> unusedBits;
      queue(f,(int)addr,rw);
   }

}

