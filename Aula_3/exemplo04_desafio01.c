/*

Exemplo 04-01: implementa desafio 01 do exemplo 03: testar 2 threads calculando fatorial.


===========
Experimento
===========

Digite o codigo em C a seguir em um editor de texto e salve o arquivo com o seguinte
nome:

exemplo04.c

Depois, compile o programa em linha de comando da seguinte forma:

cc -lpthread exemplo04.c -o exemplo4  {ou use -pthread ao inves de -lpthread}

Corrija os erros de compilacao. Quando a compilacao for bem sucedida, o arquivo
executavel "exemplo04" serah criado.

Entao, execute o executavel com a seguinte linha de comando:

./exemplo04

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define MAX_THREADS 100

void *fatorial(void *ptr);

int NUM_THREADS;

struct tipoFatData {
   char nome[10];
   int n, fat;
};

typedef struct tipoFatData tipoFatData;

int main(int argc, char *argv[])
{
     if(argc != 2 || atoi(argv[1]) == 0 || atoi(argv[1]) > MAX_THREADS){
          return 0;
     }
     NUM_THREADS = atoi(argv[1]);

     pthread_t threads[NUM_THREADS];

     tipoFatData fatData[NUM_THREADS];


     for(int i =0;i<NUM_THREADS;i++){
          strcpy(fatData[i].nome,"Thread ");
     }

     for(int i=0;i<NUM_THREADS;i++){
          printf("\n%s: Digite n > ", fatData[i].nome);
          scanf("%d",&(fatData[i].n));
          fatData[i].fat=1;
     }

     for(int i=0;i<NUM_THREADS;i++){
          pthread_create(&threads[i], NULL, fatorial, (void*) &fatData[i]);
     }

     for(int i=0;i<NUM_THREADS;i++){
          pthread_join(threads[i], NULL);
     }

     for(int i=0;i<NUM_THREADS;i++){
          printf("\nThread %d: Fat(%d) = %d\n",i, fatData[i].n, fatData[i].fat);
     }

     exit(0);
}

void *fatorial(void *ptr)
{    int i;

     tipoFatData *fatData;

     fatData = (tipoFatData *) ptr;

     printf("\n%s: vou calcular uma fatorial", fatData->nome);

     for (i=fatData->n; i>1; i--)
	(fatData->fat) = (fatData->fat) * i;

}

/*

Desafio: Ao inves de criar 2 threads, crie uma quantidade de threads definida
pelo usuário, ateh um maximo permitido. Ao inves de usar duas variaveis thread1
e thread2, use um vetor de threads. Compile, execute e analise.

*/

