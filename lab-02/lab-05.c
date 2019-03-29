#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>

int main (int argc, char *argv[])
{
   int segmento, *p, id, status;
   int i,j, min, max;
   int procurados[]={4,16,7,361,144};
   
   // aloca a memória compartilhada
   segmento = shmget (IPC_PRIVATE, 20*sizeof (int), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
   // associa a memória compartilhada ao processo
   
   p = (int *) shmat (segmento, 0, 0); // comparar o retorno com -1
   
   for (i = 0; i < 20; ++i) {
      p[i] = i * i;
   }
     
   for (i = 0; i < 4; ++i) {
      if ((id = fork()) < 0) {
         puts ("Erro na criação do novo processo");
         exit (-2);
      }
      else if (id == 0) {
         min = i * 5;
         max = i * 5 + 5;
         break;
 
      }
   }
   
   if (id != 0) {
         wait (&status);
         shmdt (p);
         shmctl (segmento, IPC_RMID, 0);
         return 0;
   }
   else {
      for (i = min; i < max; ++i){
         for (j = 0; j < 5; ++j){
            if (procurados[j] == p[i])
               printf("%d encontrado na posição %d\n", procurados[j], i);
         }
      }  
         
      // libera a memória compartilhada do processo
      shmdt (p);
      // libera a memória compartilhada
     
      return 0;
   }
}
