
typedef struct acesso{

   int address;
   char rw;

}Acesso;

typedef struct fila* pFila;

pFila criaFila(int tam);

void destroiFila(pFila f);

void queue(pFila f, int address, char rw);

Acesso dequeue(pFila f);

void getAddrVet(pFila f, int vet[]);

int getMembros(pFila f);
