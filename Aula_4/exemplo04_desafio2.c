/*

Exemplo 04-02: implementa desafio 02 do exemplo 03: testar 2 threads calculando o
n-ésimo termo da sequencia de Fibonacci em conjunto (cooperada).


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

#define maxElem 50

int NUM_THREADS;

void *fibonacci(void *ptr);

int fib[maxElem], n;

int main(int argc, char *argv[])
{
   if(argc != 2 || atoi(argv[1]) == 0){
      return 0;
   }
   NUM_THREADS = atoi(argv[1]);

   pthread_t threads[NUM_THREADS];

   int id[NUM_THREADS];

   for(int i=0;i<NUM_THREADS;i++){
      id[i] = i;
   }

   int  i;

   printf("\nDigite n > 0 = ");
   scanf("%d",&n);
   while (n<1)
   {  printf("\nDigite n > 0 = ");
      scanf("%d",&n);
   }

   for (i=0; i<n; i++)
      fib[i]=0;

   fib[0]=1;
   fib[1]=1;

   if (n>2)
   {
     for(int i=0;i<NUM_THREADS;i++){

         pthread_create(&threads[i], NULL, fibonacci, (void*) &id[i]);
     }
   }

   if (n>2)
   {
     for(int i=0;i<NUM_THREADS;i++){
          pthread_join(threads[i], NULL);
     }
   }

   printf("\nSequencia de Fibonacci: \n");
   for (i=0; i<n; i++)
      printf("%d, ", fib[i]);
   printf("\n%d-esimo elemento de Fibonacci: %d\n",n, fib[n-1]);
   exit(0);
}

void *fibonacci(void *ptr)
{    int *id, pos;

     id = (int *) ptr;

     printf("\nThread %d iniciada!\n", *id);

     pos=(*id)-1;

     while (pos<n-1)
     {
        pos=pos+2;
        while (fib[pos-1]==0);
        fib[pos]=fib[pos-1]+fib[pos-2];
     }

}

/*

Desafio: Ao inves de criar 2 threads, crie uma quantidade de threads definida
pelo usuario, ateh um maximo permitido. Ajuste o codigo. Compile, execute e analise.

*/
