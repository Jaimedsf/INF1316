
typedef struct frame {

   char flagR;
   char flagW;
   int time;
   int virtualIndex;

} Frame;

int LRU(Frame* listFrames, int lenList, int* nextIndexes);

int NRU(Frame* listFrames, int lenList, int* nextIndexes);

int NOVO(Frame* listFrames, int lenList, int* nextIndexes);

