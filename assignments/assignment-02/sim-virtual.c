#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "algoritmo.h"

typedef struct frame {
   char flagR;
   char flagW;
   int time;
   int virtualIndex;

} Frame;

int main(int argc, char* argv[]){

   char fileName[20];
   char algType[5];
   
   int pageSize;
   int memSize;
   int maxFrames;
   int loadedFrames = 0;
   int numPV;
   int s;
   
   FILE* arq;
   Frame* listFrames  ;
   int* tabPags;
   
   unsigned int address;
   char rw;
   
   int time = 0;
   int pageFaults = 0;
   int pageWrites = 0;

   if(argc < 5){
      printf("USO DO PROGRAMA: 'sim-virtual [algoritmo] arquivo.log [tamPag] [tamMem] [-D?]'");
      exit(1);
   }

   strcpy(argv[1],algType);
   strcpy(argv[2],fileName);
   pageSize = atoi(argv[3]);
   memSize = atoi(argv[4]);

   s = log2(pageSize) + 10;
   maxFrames = memSize/pageSize;
   numPV = pow((32-s),2);
   
   arq = fopen(fileName,"r");
   listFrames = malloc(pageFrames * sizeof(Frame));
   tabPags = malloc(numPV * sizeof(int));
   
   for(int i=0;i<numPV;i++){
      tabPags[i]=-1;
   }
   
   while (fscanf(file, "%x %c", &address, &rw)) {
   
      int virtualIndex = address >> s;
      int realIndex = tabPags[virtualIndex];
      
      
      if(time%5 == 0){
         for(int i=0;i<loadedFrames;i++)
            listFrames[i].flagR='0';
      }
      
      
      if(loadedFrames < maxFrames){
         realIndex = loadedFrames;
         loadedFrames++;
      }
      else {
         if(realIndex == -1){
            int newIndex;
            newIndex = pageFault();
            
            pageFaults++;
            if(listFrames[newIndex].flagW == '1')
               pageWrites++;
            
            tabPags[virtualIndex]=newIndex;
            tabPags[listFrames[newIndex].virtualIndex]=-1;
            realIndex = newIndex;
            
         }
      }
      
      if(rw == 'W'){
         listFrames[realIndex].flagW='1';
      }
      listFrames[realIndex].flagR='1';
      listFrames[realIndex].time=time;
      listFrames[realIndex].virtualIndex = virtualIndex;
      
      time++;
   }
      
   fclose(arq);
   
   return 0;
}
