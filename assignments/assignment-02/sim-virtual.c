#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "algoritmo.h"

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
   
   int(*PageFault)(Frame*, int, int*);
   
   unsigned int address;
   char rw;
   
   int time = 0;
   int pageFaults = 0;
   int pageWrites = 0;

   if(argc < 5){
      printf("USO DO PROGRAMA: 'sim-virtual [algoritmo] arquivo.log [tamPag] [tamMem] [-D?]'");
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
   
   arq = fopen(fileName, "r");
   	  
   listFrames = malloc(maxFrames * sizeof(Frame));
   tabPags = malloc(numPV * sizeof(int));
   
   for(int i=0;i<numPV;i++){
      tabPags[i]=-1;
   }
   
   while (fscanf(arq, "%x %c", &address, &rw) > -1) {
      int virtualIndex = address >> s;
      int realIndex = tabPags[virtualIndex];
      
      if(time % 10 == 0){
         for(int i = 0; i < loadedFrames;i++) {
            listFrames[i].flagR = '0';
         }
      }
      
      
      if(loadedFrames < maxFrames){
         realIndex = loadedFrames;
         loadedFrames++;
         listFrames[realIndex].flagW = '0';
      }
      else {
         if(realIndex == -1){
            int newIndex;
            newIndex = (*PageFault)(listFrames, maxFrames, NULL);
            
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
   
   return 0;
}
